/*====================================================================================
* File name: 	 isotp.h
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
#ifndef ISOTP_H
#define ISOTP_H
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "isotp_defines.h"
#include "isotp_config.h"


/**
 * @brief Struct containing the data for linking an application to a CAN instance.
 * The data stored in this struct is used internally and may be used by software programs
 * using this library.
 */
typedef struct IsoTpLink {
    /* sender paramters */
    uint32_t                    send_arbitration_id; /* used to reply consecutive frame */
    /* message buffer */
    uint8_t*                    send_buffer;
    uint16_t                    send_buf_size;
    uint16_t                    send_size;
    uint16_t                    send_offset;
    /* multi-frame flags */
    uint8_t                     send_sn;
    uint16_t                    send_bs_remain; /* Remaining block size */
    uint8_t                     send_st_min;    /* Separation Time between consecutive frames, unit millis */
    uint8_t                     send_wtf_count; /* Maximum number of FC.Wait frame transmissions  */
    uint32_t                    send_timer_st;  /* Last time send consecutive frame */    
    uint32_t                    send_timer_bs;  /* Time until reception of the next FlowControl N_PDU
                                                   start at sending FF, CF, receive FC
                                                   end at receive FC */
    int                         send_protocol_result;
    uint8_t                     send_status;

    /* receiver paramters */
    uint32_t                    receive_arbitration_id;
    /* message buffer */
    uint8_t*                    receive_buffer;
    uint16_t                    receive_buf_size;
    uint16_t                    receive_size;
    uint16_t                    receive_offset;
    /* multi-frame control */
    uint8_t                     receive_sn;
    uint8_t                     receive_bs_count; /* Maximum number of FC.Wait frame transmissions  */
    uint32_t                    receive_timer_cr; /* Time until transmission of the next ConsecutiveFrame N_PDU
                                                     start at sending FC, receive CF 
                                                     end at receive FC */
    int                         receive_protocol_result;
    uint8_t                     receive_status;                                                     
} IsoTpLink;


//Statistics
typedef struct {
    uint32_t txBytes;
    uint32_t txPayloads;
    uint32_t txPayloadBytes;
    uint32_t txStatusError;
    uint32_t txResult_TIMEOUT_A    ;
    uint32_t txResult_TIMEOUT_BS   ;
    uint32_t txResult_TIMEOUT_CR   ;
    uint32_t txResult_WRONG_SN     ;
    uint32_t txResult_INVALID_FS   ;
    uint32_t txResult_UNEXP_PDU    ;
    uint32_t txResult_WFT_OVRN     ;
    uint32_t txResult_BUFFER_OVFLW ;
    uint32_t txResult_ERROR        ;

    uint32_t rxBytes;
    uint32_t rxPayloads;
    uint32_t rxPayloadBytes;
    uint32_t rxResult_TIMEOUT_A    ;
    uint32_t rxResult_TIMEOUT_BS   ;
    uint32_t rxResult_TIMEOUT_CR   ;
    uint32_t rxResult_WRONG_SN     ;
    uint32_t rxResult_INVALID_FS   ;
    uint32_t rxResult_UNEXP_PDU    ;
    uint32_t rxResult_WFT_OVRN     ;
    uint32_t rxResult_BUFFER_OVFLW ;
    uint32_t rxResult_ERROR        ;  
}ISOTpLinkStatistics;

/**
 * @brief A class that encapsulates the ISO-TP behaviors
 * 
 */
class IsoTp
{
public:
    IsoTp(void *canChannel, uint32_t sendId, uint16_t sendBufSize, uint32_t recvId, uint16_t recvBufSize);
    ~IsoTp();

    uint32_t rxCanId() {return m_link.receive_arbitration_id;}
    uint32_t txCanId() {return m_link.send_arbitration_id;}
    int rx_protocolResult() {return m_link.receive_protocol_result;}
    int tx_protocolResult() {return m_link.send_protocol_result;}
    uint8_t tx_status() {return m_link.send_status;}
    uint8_t rx_status() {return m_link.receive_status;}
    const char *protocolResult2Str(int res);
    const char *sendStatus2Str(int st);
    const char *recvStatus2Str(int st);
    void dump();
    void resetSts() {memset(&m_sts, 0, sizeof(m_sts));}

    /**
     * @brief Initialises the ISO-TP library.
     *
     * @param sendid The ID used to send data to other CAN nodes.
     * @param sendbuf A pointer to an area in memory which can be used as a buffer for data to be sent.
     * @param sendbufsize The size of the buffer area.
     * @param recvbuf A pointer to an area in memory which can be used as a buffer for data to be received.
     * @param recvbufsize The size of the buffer area.
     */
    void isotp_init_link(uint32_t sendid, 
                        uint8_t *sendbuf, uint16_t sendbufsize,
                        uint8_t *recvbuf, uint16_t recvbufsize);

    /**
     * @brief Polling function; call this function periodically to handle timeouts, send consecutive frames, etc.
     *
     * @param link The @code IsoTpLink @endcode instance used.
     */
    void isotp_poll();

    /**
     * @brief Handles incoming CAN messages.
     * Determines whether an incoming message is a valid ISO-TP frame or not and handles it accordingly.
     *
     * @param link The @code IsoTpLink @endcode instance used for transceiving data.
     * @param data The data received via CAN.
     * @param len The length of the data received.
     */
    void isotp_on_can_message( uint8_t *data, uint8_t len);

    /**
     * @brief Sends ISO-TP frames via CAN, using the ID set in the initialising function.
     *
     * Single-frame messages will be sent immediately when calling this function.
     * Multi-frame messages will be sent consecutively when calling isotp_poll.
     *
     * @param link The @code IsoTpLink @endcode instance used for transceiving data.
     * @param payload The payload to be sent. (Up to 4095 bytes).
     * @param size The size of the payload to be sent.
     *
     * @return Possible return values:
     *  - @code ISOTP_RET_OVERFLOW @endcode
     *  - @code ISOTP_RET_INPROGRESS @endcode
     *  - @code ISOTP_RET_OK @endcode
     *  - The return value of the user shim function isotp_user_send_can().
     */
    int isotp_send( const uint8_t payload[], uint16_t size);

    /**
     * @brief See @link isotp_send @endlink, with the exception that this function is used only for functional addressing.
     */
    int isotp_send_with_id( uint32_t id, const uint8_t payload[], uint16_t size);

    /**
     * @brief Receives and parses the received data and copies the parsed data in to the internal buffer.
     * @param link The @link IsoTpLink @endlink instance used to transceive data.
     * @param payload A pointer to an area in memory where the raw data is copied from.
     * @param payload_size The size of the received (raw) CAN data.
     * @param out_size A reference to a variable which will contain the size of the actual (parsed) data.
     *
     * @return Possible return values:
     *      - @link ISOTP_RET_OK @endlink
     *      - @link ISOTP_RET_NO_DATA @endlink
     */
    int isotp_receive( uint8_t *payload, const uint16_t payload_size, uint16_t *out_size);

private:
    IsoTpLink           m_link;
    void *              m_canChannel;
    ISOTpLinkStatistics m_sts;

    uint8_t isotp_ms_to_st_min(uint8_t ms);
    uint8_t isotp_st_min_to_ms(uint8_t st_min);
    int isotp_send_flow_control(uint8_t flow_status, uint8_t block_size, uint8_t st_min_ms);
    int isotp_send_single_frame(uint32_t id);
    int isotp_send_first_frame(uint32_t id);
    int isotp_send_consecutive_frame();
    int isotp_receive_single_frame( IsoTpCanMessage *message, uint8_t len);
    int isotp_receive_first_frame( IsoTpCanMessage *message, uint8_t len);
    int isotp_receive_consecutive_frame( IsoTpCanMessage *message, uint8_t len);
    int isotp_receive_flow_control_frame( IsoTpCanMessage *message, uint8_t len);


    /* user implemented, print debug message */
    void isotp_user_debug(const char* message);

    /* user implemented, send can message. should return ISOTP_RET_OK when success.
    */
    int  isotp_user_send_can(const uint32_t arbitration_id,
                            const uint8_t* data, const uint8_t size);

    /* user implemented, get millisecond */
    uint32_t isotp_user_get_ms(void);
};






#endif // ISOTP_H
