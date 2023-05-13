/*====================================================================================
* File name: 	 isotp.cpp
* Description:   a class to encapsulate the ISO can-tp stack
*                The can-tp stack was orignated from https://github.com/lishen2/isotp-c.git
* Project:       EPT
* Note:
* Created on:    2023-05-09
* Author:        Fred.C
=======================================================================================
 History:
---------------------------------------------------------------------------------------
09-05-2023		Version 1.0.0       FC		Initialized version
-------------------------------------------------------------------------------------*/

#include <string.h>
#include "assert.h"
#include <QDebug>
#include "windows.h"
#include "isotp.h"
#include "canthread.h"
#include <QTime>

///////////////////////////////////////////////////////
///                 STATIC FUNCTIONS                ///
///////////////////////////////////////////////////////

/* st_min to microsecond */
uint8_t IsoTp::isotp_ms_to_st_min(uint8_t ms) {
    uint8_t st_min;

    st_min = ms;
    if (st_min > 0x7F) {
        st_min = 0x7F;
    }

    return st_min;
}

/* st_min to msec  */
uint8_t IsoTp::isotp_st_min_to_ms(uint8_t st_min) {
    uint8_t ms;
    
    if (st_min >= 0xF1 && st_min <= 0xF9) {
        ms = 1;
    } else if (st_min <= 0x7F) {
        ms = st_min;
    } else {
        ms = 0;
    }

    return ms;
}

int IsoTp::isotp_send_flow_control(uint8_t flow_status, uint8_t block_size, uint8_t st_min_ms) {

    IsoTpCanMessage message;
    int ret;

    /* setup message  */
    message.as.flow_control.type = ISOTP_PCI_TYPE_FLOW_CONTROL_FRAME;
    message.as.flow_control.FS = flow_status;
    message.as.flow_control.BS = block_size;
    message.as.flow_control.STmin = isotp_ms_to_st_min(st_min_ms);

    /* send message */
#ifdef ISO_TP_FRAME_PADDING
    (void) memset(message.as.flow_control.reserve, 0, sizeof(message.as.flow_control.reserve));
    ret = isotp_user_send_can(m_link.send_arbitration_id, message.as.data_array.ptr, sizeof(message));
#else    
    ret = isotp_user_send_can(m_link.send_arbitration_id,
            message.as.data_array.ptr,
            3);
#endif

    return ret;
}

int IsoTp::isotp_send_single_frame(uint32_t id) {

    IsoTpCanMessage message;
    int ret;

    /* multi frame message length must greater than 7  */
    assert(m_link.send_size <= 7);

    /* setup message  */
    message.as.single_frame.type = ISOTP_PCI_TYPE_SINGLE;
    message.as.single_frame.SF_DL = (uint8_t) m_link.send_size;
    (void) memcpy(message.as.single_frame.data, m_link.send_buffer, m_link.send_size);

    /* send message */
#ifdef ISO_TP_FRAME_PADDING
    (void) memset(message.as.single_frame.data + m_link.send_size, 0, sizeof(message.as.single_frame.data) - m_link.send_size);
    ret = isotp_user_send_can(id, message.as.data_array.ptr, sizeof(message));
#else
    ret = isotp_user_send_can(id,
            message.as.data_array.ptr,
            m_link.send_size + 1);
#endif
    if(ISOTP_RET_OK == ret){
        m_sts.txPayloadBytes += m_link.send_size;
    }

    return ret;
}

int IsoTp::isotp_send_first_frame(uint32_t id) {
    
    IsoTpCanMessage message;
    int ret;

    /* multi frame message length must greater than 7  */
    assert(m_link.send_size > 7);

    /* setup message  */
    message.as.first_frame.type = ISOTP_PCI_TYPE_FIRST_FRAME;
    message.as.first_frame.FF_DL_low = (uint8_t) m_link.send_size;
    message.as.first_frame.FF_DL_high = (uint8_t) (0x0F & (m_link.send_size >> 8));
    (void) memcpy(message.as.first_frame.data, m_link.send_buffer, sizeof(message.as.first_frame.data));

    /* send message */
    ret = isotp_user_send_can(id, message.as.data_array.ptr, sizeof(message));
    if (ISOTP_RET_OK == ret) {
        m_link.send_offset += sizeof(message.as.first_frame.data);
        m_sts.txPayloadBytes += sizeof(message.as.first_frame.data);
        m_link.send_sn = 1;
    }

    return ret;
}

int IsoTp:: isotp_send_consecutive_frame() {
    
    IsoTpCanMessage message;
    uint16_t data_length;
    int ret;

    /* multi frame message length must greater than 7  */
    assert(m_link.send_size > 7);

    /* setup message  */
    message.as.consecutive_frame.type = TSOTP_PCI_TYPE_CONSECUTIVE_FRAME;
    message.as.consecutive_frame.SN = m_link.send_sn;
    data_length = m_link.send_size - m_link.send_offset;
    if (data_length > sizeof(message.as.consecutive_frame.data)) {
        data_length = sizeof(message.as.consecutive_frame.data);
    }
    (void) memcpy(message.as.consecutive_frame.data, m_link.send_buffer + m_link.send_offset, data_length);

    /* send message */
#ifdef ISO_TP_FRAME_PADDING
    (void) memset(message.as.consecutive_frame.data + data_length, 0, sizeof(message.as.consecutive_frame.data) - data_length);
    ret = isotp_user_send_can(m_link.send_arbitration_id, message.as.data_array.ptr, sizeof(message));
#else
    ret = isotp_user_send_can(m_link.send_arbitration_id,
            message.as.data_array.ptr,
            data_length + 1);
#endif
    if (ISOTP_RET_OK == ret) {
        m_link.send_offset += data_length;
        m_sts.txPayloadBytes += data_length;
        if (++(m_link.send_sn) > 0x0F) {
            m_link.send_sn = 0;
        }
    }
    
    return ret;
}

int IsoTp::isotp_receive_single_frame(IsoTpCanMessage *message, uint8_t len) {
    /* check data length */
    if ((0 == message->as.single_frame.SF_DL) || (message->as.single_frame.SF_DL > (len - 1))) {
        isotp_user_debug("Single-frame length too small.");
        return ISOTP_RET_LENGTH;
    }

    /* copying data */
    (void) memcpy(m_link.receive_buffer, message->as.single_frame.data, message->as.single_frame.SF_DL);
    m_link.receive_size = message->as.single_frame.SF_DL;
    m_sts.rxPayloadBytes += message->as.single_frame.SF_DL;

    return ISOTP_RET_OK;
}

int IsoTp::isotp_receive_first_frame(IsoTpCanMessage *message, uint8_t len) {
    uint16_t payload_length;

    if (8 != len) {
        isotp_user_debug("First frame should be 8 bytes in length.");
        return ISOTP_RET_LENGTH;
    }

    /* check data length */
    payload_length = message->as.first_frame.FF_DL_high;
    payload_length = (payload_length << 8) + message->as.first_frame.FF_DL_low;

    /* should not use multiple frame transmition */
    if (payload_length <= 7) {
        isotp_user_debug("Should not use multiple frame transmission.");
        return ISOTP_RET_LENGTH;
    }
    
    if (payload_length > m_link.receive_buf_size) {
        isotp_user_debug("Multi-frame response too large for receiving buffer.");
        return ISOTP_RET_OVERFLOW;
    }
    
    /* copying data */
    (void) memcpy(m_link.receive_buffer, message->as.first_frame.data, sizeof(message->as.first_frame.data));
    m_link.receive_size = payload_length;
    m_link.receive_offset = sizeof(message->as.first_frame.data);
    m_sts.rxPayloadBytes += sizeof(message->as.first_frame.data);
    m_link.receive_sn = 1;
    
    return ISOTP_RET_OK;
}

int IsoTp::isotp_receive_consecutive_frame(IsoTpCanMessage *message, uint8_t len) {
    uint16_t remaining_bytes;
    
    /* check sn */
    if (m_link.receive_sn != message->as.consecutive_frame.SN) {
        return ISOTP_RET_WRONG_SN;
    }

    /* check data length */
    remaining_bytes = m_link.receive_size - m_link.receive_offset;
    if (remaining_bytes > sizeof(message->as.consecutive_frame.data)) {
        remaining_bytes = sizeof(message->as.consecutive_frame.data);
    }
    if (remaining_bytes > len - 1) {
        isotp_user_debug("Consecutive frame too short.");
        return ISOTP_RET_LENGTH;
    }

    /* copying data */
    (void) memcpy(m_link.receive_buffer + m_link.receive_offset, message->as.consecutive_frame.data, remaining_bytes);

    m_link.receive_offset += remaining_bytes;
    m_sts.rxPayloadBytes += remaining_bytes;
    if (++(m_link.receive_sn) > 0x0F) {
        m_link.receive_sn = 0;
    }

    return ISOTP_RET_OK;
}

int IsoTp::isotp_receive_flow_control_frame(IsoTpCanMessage *message, uint8_t len) {
    Q_UNUSED(message);
    /* check message length */
    if (len < 3) {
        isotp_user_debug("Flow control frame too short.");
        return ISOTP_RET_LENGTH;
    }

    return ISOTP_RET_OK;
}

///////////////////////////////////////////////////////
///                 PUBLIC FUNCTIONS                ///
///////////////////////////////////////////////////////

IsoTp::IsoTp(void * canChannel, uint32_t sendId, uint16_t sendBufSize, uint32_t recvId, uint16_t recvBufSize)
{
    m_canChannel = canChannel;

    memset(&m_link, 0, sizeof(m_link));
    
    //init send Direction
    m_link.send_status = ISOTP_SEND_STATUS_IDLE;
    if(sendBufSize == 0 || sendBufSize > ISO_TP_MAX_PAYLOAD_SIZE)
    {
        sendBufSize = ISO_TP_MAX_PAYLOAD_SIZE;
    }
    m_link.send_arbitration_id = sendId;
    m_link.send_buffer = new uint8_t[sendBufSize];
    assert(m_link.send_buffer != NULL);
    m_link.send_buf_size = sendBufSize;

    //Init recv Direction
    m_link.receive_status = ISOTP_RECEIVE_STATUS_IDLE;
    if(recvBufSize == 0 || recvBufSize > ISO_TP_MAX_PAYLOAD_SIZE)
    {
        recvBufSize = ISO_TP_MAX_PAYLOAD_SIZE;
    }
    m_link.receive_arbitration_id = recvId;
    m_link.receive_buffer = new uint8_t[recvBufSize];
    assert(m_link.receive_buffer != NULL);
    m_link.receive_buf_size = recvBufSize;

    //statistics
    memset(&m_sts, 0, sizeof(m_sts));
}

IsoTp::~IsoTp()
{
    delete [] m_link.send_buffer;
    delete [] m_link.receive_buffer;

    m_link.send_buffer = NULL;
    m_link.receive_buffer = NULL;

    isotp_user_debug(__FUNCTION__);
}

int IsoTp::isotp_send(const uint8_t payload[], uint16_t size) {
    return isotp_send_with_id(m_link.send_arbitration_id, payload, size);
}

int IsoTp::isotp_send_with_id(uint32_t id, const uint8_t payload[], uint16_t size) {
    int ret;

    if (size > m_link.send_buf_size) {
        isotp_user_debug("Message size too large. Increase ISO_TP_MAX_MESSAGE_SIZE to set a larger buffer\n");
        char message[128];
        sprintf(&message[0], "Attempted to send %d bytes; max size is %d!\n", size, m_link.send_buf_size);
        isotp_user_debug(message);
        return ISOTP_RET_OVERFLOW;
    }

    if (ISOTP_SEND_STATUS_INPROGRESS == m_link.send_status) {
        isotp_user_debug("Abort previous message, transmission in progress.\n");
        return ISOTP_RET_INPROGRESS;
    }

    m_sts.txPayloads += 1;

    /* copy into local buffer */
    m_link.send_size = size;
    m_link.send_offset = 0;
    (void) memcpy(m_link.send_buffer, payload, size);

    if (m_link.send_size < 8) {
        /* send single frame */
        ret = isotp_send_single_frame(id);
    } else {
        /* send multi-frame */
        ret = isotp_send_first_frame(id);

        /* init multi-frame control flags */
        if (ISOTP_RET_OK == ret) {
            m_link.send_bs_remain = 0;
            m_link.send_st_min = 0;
            m_link.send_wtf_count = 0;
            m_link.send_timer_st = isotp_user_get_ms();
            m_link.send_timer_bs = isotp_user_get_ms() + ISO_TP_DEFAULT_RESPONSE_TIMEOUT;
            m_link.send_protocol_result = ISOTP_PROTOCOL_RESULT_OK;
            m_link.send_status = ISOTP_SEND_STATUS_INPROGRESS;
        }
    }

    return ret;
}

void IsoTp::isotp_on_can_message(uint8_t *data, uint8_t len) {
    IsoTpCanMessage message;
    int ret;
    
    if (len < 2 || len > 8) {
        return;
    }

    m_sts.rxBytes += len;
    memcpy(message.as.data_array.ptr, data, len);
    memset(message.as.data_array.ptr + len, 0, sizeof(message.as.data_array.ptr) - len);

    switch (message.as.common.type) {
        case ISOTP_PCI_TYPE_SINGLE: {
            /* update protocol result */
            if (ISOTP_RECEIVE_STATUS_INPROGRESS == m_link.receive_status) {
                m_link.receive_protocol_result = ISOTP_PROTOCOL_RESULT_UNEXP_PDU;
                m_sts.rxResult_UNEXP_PDU += 1;
            } else {
                m_link.receive_protocol_result = ISOTP_PROTOCOL_RESULT_OK;
            }

            /* handle message */
            ret = isotp_receive_single_frame(&message, len);
            
            if (ISOTP_RET_OK == ret) {
                /* change status */
                m_link.receive_status = ISOTP_RECEIVE_STATUS_FULL;
            }
            break;
        }
        case ISOTP_PCI_TYPE_FIRST_FRAME: {
            /* update protocol result */
            if (ISOTP_RECEIVE_STATUS_INPROGRESS == m_link.receive_status) {
                m_link.receive_protocol_result = ISOTP_PROTOCOL_RESULT_UNEXP_PDU;
                m_sts.rxResult_UNEXP_PDU += 1;
            } else {
                m_link.receive_protocol_result = ISOTP_PROTOCOL_RESULT_OK;
            }

            /* handle message */
            ret = isotp_receive_first_frame(&message, len);

            /* if overflow happened */
            if (ISOTP_RET_OVERFLOW == ret) {
                /* update protocol result */
                m_link.receive_protocol_result = ISOTP_PROTOCOL_RESULT_BUFFER_OVFLW;
                m_sts.rxResult_BUFFER_OVFLW += 1;
                /* change status */
                m_link.receive_status = ISOTP_RECEIVE_STATUS_IDLE;
                /* send error message */
                isotp_send_flow_control(PCI_FLOW_STATUS_OVERFLOW, 0, 0);
                break;
            }

            /* if receive successful */
            if (ISOTP_RET_OK == ret) {
                /* change status */
                m_link.receive_status = ISOTP_RECEIVE_STATUS_INPROGRESS;
                /* send fc frame */
                m_link.receive_bs_count = ISO_TP_DEFAULT_BLOCK_SIZE;
                isotp_send_flow_control(PCI_FLOW_STATUS_CONTINUE, m_link.receive_bs_count, ISO_TP_DEFAULT_ST_MIN);
                /* refresh timer cs */
                m_link.receive_timer_cr = isotp_user_get_ms() + ISO_TP_DEFAULT_RESPONSE_TIMEOUT;
            }
            
            break;
        }
        case TSOTP_PCI_TYPE_CONSECUTIVE_FRAME: {
            /* check if in receiving status */
            if (ISOTP_RECEIVE_STATUS_INPROGRESS != m_link.receive_status) {
                m_link.receive_protocol_result = ISOTP_PROTOCOL_RESULT_UNEXP_PDU;
                m_sts.rxResult_UNEXP_PDU += 1;
                break;
            }

            /* handle message */
            ret = isotp_receive_consecutive_frame(&message, len);
            /* if wrong sn */
            if (ISOTP_RET_WRONG_SN == ret) {
                m_link.receive_protocol_result = ISOTP_PROTOCOL_RESULT_WRONG_SN;
                m_link.receive_status = ISOTP_RECEIVE_STATUS_IDLE;
                m_sts.rxResult_WRONG_SN += 1;
                break;
            }

            /* if success */
            if (ISOTP_RET_OK == ret) {
                /* refresh timer cs */
                m_link.receive_timer_cr = isotp_user_get_ms() + ISO_TP_DEFAULT_RESPONSE_TIMEOUT;
                
                /* receive finished */
                if (m_link.receive_offset >= m_link.receive_size) {
                    qDebug() << QDateTime::currentDateTime().toString("hh:mm:ss.zzz") << "-F";
                    m_link.receive_status = ISOTP_RECEIVE_STATUS_FULL;
                } else {
                    /* send fc when bs reaches limit */
                    if (0 == --m_link.receive_bs_count) {
                        m_link.receive_bs_count = ISO_TP_DEFAULT_BLOCK_SIZE;
                        isotp_send_flow_control(PCI_FLOW_STATUS_CONTINUE, m_link.receive_bs_count, ISO_TP_DEFAULT_ST_MIN);
                    }
                }
            }
            
            break;
        }
        case ISOTP_PCI_TYPE_FLOW_CONTROL_FRAME:
            /* handle fc frame only when sending in progress  */
            if (ISOTP_SEND_STATUS_INPROGRESS != m_link.send_status) {
                break;
            }

            /* handle message */
            ret = isotp_receive_flow_control_frame(&message, len);
            
            if (ISOTP_RET_OK == ret) {
                /* refresh bs timer */
                m_link.send_timer_bs = isotp_user_get_ms() + ISO_TP_DEFAULT_RESPONSE_TIMEOUT;

                /* overflow */
                if (PCI_FLOW_STATUS_OVERFLOW == message.as.flow_control.FS) {
                    m_link.send_protocol_result = ISOTP_PROTOCOL_RESULT_BUFFER_OVFLW;
                    m_link.send_status = ISOTP_SEND_STATUS_ERROR;
                    m_sts.txStatusError += 1;
                    m_sts.txResult_BUFFER_OVFLW += 1;
                }

                /* wait */
                else if (PCI_FLOW_STATUS_WAIT == message.as.flow_control.FS) {
                    m_link.send_wtf_count += 1;
                    /* wait exceed allowed count */
                    if (m_link.send_wtf_count > ISO_TP_MAX_WFT_NUMBER) {
                        m_link.send_protocol_result = ISOTP_PROTOCOL_RESULT_WFT_OVRN;
                        m_link.send_status = ISOTP_SEND_STATUS_ERROR;
                        m_sts.txStatusError += 1;
                        m_sts.txResult_WFT_OVRN += 1;
                    }
                }

                /* permit send */
                else if (PCI_FLOW_STATUS_CONTINUE == message.as.flow_control.FS) {
                    if (0 == message.as.flow_control.BS) {
                        m_link.send_bs_remain = ISOTP_INVALID_BS;
                    } else {
                        m_link.send_bs_remain = message.as.flow_control.BS;
                    }
                    m_link.send_st_min = isotp_st_min_to_ms(message.as.flow_control.STmin);
                    m_link.send_wtf_count = 0;
                }
            }
            break;
        default:
            break;
    };
    
    return;
}

int IsoTp::isotp_receive(uint8_t *payload, const uint16_t payload_size, uint16_t *out_size) {
    uint16_t copylen;
    
    if (ISOTP_RECEIVE_STATUS_FULL != m_link.receive_status) {
        return ISOTP_RET_NO_DATA;
    }

    qDebug() << QDateTime::currentDateTime().toString("hh:mm:ss.zzz") << "C";
    copylen = m_link.receive_size;
    if (copylen > payload_size) {
        copylen = payload_size;
    }

    memcpy(payload, m_link.receive_buffer, copylen);
    *out_size = copylen;
    m_sts.rxPayloads += 1;

    m_link.receive_status = ISOTP_RECEIVE_STATUS_IDLE;

    return ISOTP_RET_OK;
}

void IsoTp::isotp_init_link(uint32_t sendid, uint8_t *sendbuf, uint16_t sendbufsize, uint8_t *recvbuf, uint16_t recvbufsize) {
    memset(&m_link, 0, sizeof(m_link));
    m_link.receive_status = ISOTP_RECEIVE_STATUS_IDLE;
    m_link.send_status = ISOTP_SEND_STATUS_IDLE;
    m_link.send_arbitration_id = sendid;
    m_link.send_buffer = sendbuf;
    m_link.send_buf_size = sendbufsize;
    m_link.receive_buffer = recvbuf;
    m_link.receive_buf_size = recvbufsize;
    
    return;
}

void IsoTp::isotp_poll() {
    int ret;

    /* only polling when operation in progress */
    if (ISOTP_SEND_STATUS_INPROGRESS == m_link.send_status) {
        
//         qDebug() << QDateTime::currentDateTime().toString("hh:mm:ss.zzz")
//                  << "isotp_poll:" << m_link.send_bs_remain << m_link.send_st_min << isotp_user_get_ms() << m_link.send_timer_st;

        /* continue send data */
        if (/* send data if bs_remain is invalid or bs_remain large than zero */
        (ISOTP_INVALID_BS == m_link.send_bs_remain || m_link.send_bs_remain > 0) &&
        /* and if st_min is zero or go beyond interval time */
        (0 == m_link.send_st_min || (0 != m_link.send_st_min && IsoTpTimeAfter(isotp_user_get_ms(), m_link.send_timer_st)))) {

            //qDebug() << QDateTime::currentDateTime().toString("hh:mm:ss.zzz") << "Tx CF";

            ret = isotp_send_consecutive_frame();
            if (ISOTP_RET_OK == ret) {
                if (ISOTP_INVALID_BS != m_link.send_bs_remain) {
                    m_link.send_bs_remain -= 1;
                }
                m_link.send_timer_bs = isotp_user_get_ms() + ISO_TP_DEFAULT_RESPONSE_TIMEOUT;
                m_link.send_timer_st = isotp_user_get_ms() + m_link.send_st_min;

                /* check if send finish */
                if (m_link.send_offset >= m_link.send_size) {
                    m_link.send_status = ISOTP_SEND_STATUS_IDLE;
                }
            } else {
                m_link.send_status = ISOTP_SEND_STATUS_ERROR;
                m_sts.txStatusError += 1;
            }
        }

        /* check timeout */
        if (IsoTpTimeAfter(isotp_user_get_ms(), m_link.send_timer_bs)) {
            m_link.send_protocol_result = ISOTP_PROTOCOL_RESULT_TIMEOUT_BS;
            m_link.send_status = ISOTP_SEND_STATUS_ERROR;
            m_sts.txStatusError += 1;
            m_sts.txResult_TIMEOUT_BS += 1;
        }
    }

    /* only polling when operation in progress */
    if (ISOTP_RECEIVE_STATUS_INPROGRESS == m_link.receive_status) {
        
        /* check timeout */
        if (IsoTpTimeAfter(isotp_user_get_ms(), m_link.receive_timer_cr)) {
            m_link.receive_protocol_result = ISOTP_PROTOCOL_RESULT_TIMEOUT_CR;
            m_link.receive_status = ISOTP_RECEIVE_STATUS_IDLE;
            m_sts.rxResult_TIMEOUT_CR += 1;
        }
    }

    return;
}

///////////////////////////////////////////////////////
///             Private:  USER CONFIGURED FUNCTIONS  ///
///////////////////////////////////////////////////////

void IsoTp::isotp_user_debug(const char* message)
{
    qDebug("isotp: %s", message);
}

/*
    Note: 
        Extended Flag and Remote Flag should be input here or stored in ISOTp link !!
        TBD Later 
*/
int  IsoTp::isotp_user_send_can(const uint32_t arbitration_id,
                         const uint8_t* data, const uint8_t size)
{
    if(m_canChannel)
    {
        bool ret = static_cast<CANThread *>(m_canChannel)->sendData(arbitration_id, 0, 0, data, size);
        if(ret)
        {
            m_sts.txBytes += size;
            return ISOTP_RET_OK;
        }
    }
    
    return ISOTP_RET_ERROR;
}

/* user implemented, get millisecond */
uint32_t IsoTp::isotp_user_get_ms(void)
{
    //return (uint32_t)GetTickCount();
    return QTime::currentTime().msecsSinceStartOfDay();
}


const char *IsoTp::protocolResult2Str(int res)
{
    if (res == ISOTP_PROTOCOL_RESULT_OK           )      return "OK";
    else if (res == ISOTP_PROTOCOL_RESULT_TIMEOUT_A    ) return "TIMEOUT_A";
    else if (res == ISOTP_PROTOCOL_RESULT_TIMEOUT_BS   ) return "TIMEOUT_BS";
    else if (res == ISOTP_PROTOCOL_RESULT_TIMEOUT_CR   ) return "TIMEOUT_CR";
    else if (res == ISOTP_PROTOCOL_RESULT_WRONG_SN     ) return "WRONG_SN";
    else if (res == ISOTP_PROTOCOL_RESULT_INVALID_FS   ) return "INVALID_FS";
    else if (res == ISOTP_PROTOCOL_RESULT_UNEXP_PDU    ) return "UNEXP_PDU";
    else if (res == ISOTP_PROTOCOL_RESULT_WFT_OVRN     ) return "WFT_OVRN";
    else if (res == ISOTP_PROTOCOL_RESULT_BUFFER_OVFLW ) return "BUFFER_OVFLW";
    else if (res == ISOTP_PROTOCOL_RESULT_ERROR        ) return "ERROR";
    else return "Invalid!";
}

const char *IsoTp::sendStatus2Str(int st)
{
    if(st == ISOTP_SEND_STATUS_IDLE) return "Idle";
    else if(st == ISOTP_SEND_STATUS_INPROGRESS) return "inPrograss";
    else if(st == ISOTP_SEND_STATUS_ERROR) return "Error";
    else return "Invalid!";
}
const char *IsoTp::recvStatus2Str(int st)
{
    if(st == ISOTP_RECEIVE_STATUS_IDLE) return "Idle";
    else if(st == ISOTP_RECEIVE_STATUS_INPROGRESS) return "inPrograss";
    else if(st == ISOTP_RECEIVE_STATUS_FULL) return "Full";
    else return "Invalid!";
}

void IsoTp::dump()
{
    qDebug("ISO-TP Stack Status:");
    qDebug("====================");
    qDebug("Send Can-ID: 0x%x\n"
           "    Buffer:   %5p, Size:           %d\n"
           "    SendSize: %5d, SendOffset:        %d\n"
           "    SN:       %5d, RemainedBlockSize: %d\n"
           "    St_min:   %5d, wtf_count:         %d\n"
           "    timer_st: %5d, timer_BS:          %d ms\n"
           "    ProtoResult:%d(%s), Status: %d(%s)",
           m_link.send_arbitration_id,
           m_link.send_buffer, m_link.send_buf_size,
           m_link.send_size, m_link.send_offset,
           m_link.send_sn, m_link.send_bs_remain,
           m_link.send_st_min, m_link.send_wtf_count,
           m_link.send_timer_st, m_link.send_timer_bs,
           m_link.send_protocol_result, protocolResult2Str(m_link.send_protocol_result),
           m_link.send_status, sendStatus2Str(m_link.send_status)
           );
    qDebug("   -------------------------------\n"
           "   Statistics:\n"
           "     Bytes:     %8d, Payloads/Bytes:       %d/%d\n"
           "     SendError:     %4d\n"
           "     res_TIMEOUT_A: %4d, res_TIMEOUT_BS:   %d\n"
           "     res_TIMEOUT_CR:%4d, res_WRONG_SN:     %d\n"
           "     res_INVALID_FS:%4d, res_UNEXP_PDU:    %d\n"
           "     res_WFT_OVRN:  %4d, res_BUFFER_OVFLW: %d\n"
           "     res_ERROR:     %4d\n",
           m_sts.txBytes,               m_sts.txPayloads,   m_sts.txPayloadBytes,
           m_sts.txStatusError,
           m_sts.txResult_TIMEOUT_A,    m_sts.txResult_TIMEOUT_BS,
           m_sts.txResult_TIMEOUT_CR,   m_sts.txResult_WRONG_SN,
           m_sts.txResult_INVALID_FS,   m_sts.txResult_UNEXP_PDU,
           m_sts.txResult_WFT_OVRN,     m_sts.txResult_BUFFER_OVFLW,
           m_sts.txResult_ERROR);

    qDebug("Recv CAN-ID: 0x%x\n"
           "    Buffer:      %5p,   Size:      %d\n"
           "    RecvSize:    %5d,   RecvOffset:  %d\n"
           "    SN:          %5d,   Recv_BS: %d, timer_CR: %d ms\n"
           "    ProtoResult: %d(%s), Status: %d(%s)",
           m_link.receive_arbitration_id,
           m_link.receive_buffer, m_link.receive_buf_size,
           m_link.receive_size, m_link.receive_offset,
           m_link.receive_sn, m_link.receive_bs_count, m_link.receive_timer_cr,
           m_link.receive_protocol_result, protocolResult2Str(m_link.receive_protocol_result),
           m_link.receive_status, recvStatus2Str(m_link.receive_status));
    qDebug("   -------------------------------\n"
           "   Statistics:\n"
           "     Bytes:     %8d, Payloads/Bytes:       %d/%d\n"
           "     res_TIMEOUT_A: %4d, res_TIMEOUT_BS:   %d\n"
           "     res_TIMEOUT_CR:%4d, res_WRONG_SN:     %d\n"
           "     res_INVALID_FS:%4d, res_UNEXP_PDU:    %d\n"
           "     res_WFT_OVRN:  %4d, res_BUFFER_OVFLW: %d\n"
           "     res_ERROR:     %4d\n",
           m_sts.rxBytes,            m_sts.rxPayloads,          m_sts.rxPayloadBytes,
           m_sts.rxResult_TIMEOUT_A, m_sts.rxResult_TIMEOUT_BS,
           m_sts.rxResult_TIMEOUT_CR,m_sts.rxResult_WRONG_SN,
           m_sts.rxResult_INVALID_FS,m_sts.rxResult_UNEXP_PDU,
           m_sts.rxResult_WFT_OVRN,  m_sts.rxResult_BUFFER_OVFLW,
           m_sts.rxResult_ERROR);
}
