/*====================================================================================
* File name: 	 CAN_TP.c
* Description:   a class to encapsulate the ISO CAN-TP layer
* Project:       EPT
* Note:
* Created on:    2023-04-25
* Author:        Fred.C
=======================================================================================
 History:
---------------------------------------------------------------------------------------
25-04-2023		Version 1.0.0       FC		Initialized version
-------------------------------------------------------------------------------------*/
#include "can_tp.h"
#include <QDebug>
#include <QCoreApplication>
#include <QTime>

CAN_TP::CAN_TP(void *canChannel, uint32_t snd_canID, uint32_t rcv_canID, int32_t bufSize):
              m_isoTp(canChannel, snd_canID, bufSize, rcv_canID, bufSize)
{
    m_is_started = false;
    qDebug(__FUNCTION__);

    m_timerX = new HRTimer_X();
}

CAN_TP::~CAN_TP()
{
    if(m_is_started)
    {
        quit();
        wait();
    }

    if(m_timerX)
    {
        delete m_timerX;
    }

    qDebug(__FUNCTION__);
}



// Just buffer the canFrame that is received from can-driver layer into a FIFO queue
void CAN_TP::receive_canFrame(uint8_t *data, uint8_t len)
{
    T_canFrame cf;
    memcpy(cf.data, data, sizeof(cf.data));
    cf.len = len;

    m_rx_canFrameQ_Mutex.lock();
    m_rx_canFrameQ.enqueue(cf);
    m_rx_canFrameQ_Mutex.unlock();

//     qDebug("CAN_TP::%s: Data[%d]:%x %x %x %x %x %x %x %x",
//            __FUNCTION__, len,
//            data[0], data[1], data[2], data[3],
//            data[4], data[5], data[6], data[7]);
}


/*
 * send out a message via CAN-TP, 
 * and the length of message can be greater than ISO_TP_MAX_PAYLOAD_SIZE 4095
*/
int CAN_TP::send_a_message(uint8_t *msgData, uint16_t msgDataLen)
{
    if(msgData == NULL || msgDataLen == 0)
    {
        return ISOTP_RET_NO_DATA;
    }

    if(m_isoTp.tx_status() == ISOTP_SEND_STATUS_INPROGRESS ||
       ! m_tx_payloadQ.isEmpty())
    {
        return ISOTP_RET_INPROGRESS;
    }

    uint8_t *data = msgData;
    int size = 0;
    T_canTPPayload payload;

    m_tx_payloadQ_Mutex.lock();
    while(msgDataLen > 0)
    {
        if( msgDataLen > ISO_TP_MAX_PAYLOAD_SIZE)
        {
            data += size;
            size = ISO_TP_MAX_PAYLOAD_SIZE;
            msgDataLen -= size;
        }
        else
        {
            data += size;
            size = msgDataLen;
            msgDataLen -= size;
        }
        memcpy(payload.data, data, size);
        payload.len = size;
        m_tx_payloadQ.enqueue(payload);
    }
    m_tx_payloadQ_Mutex.unlock();

    return ISOTP_RET_OK;
}

void CAN_TP::start_run(Priority pri)
{
    if(m_timerX)     //C++ high-resolution Timer
    {
        m_timerX->start(this, 1);
    }
    else            //QT Thread
    {
        if(!m_is_started)
        {
            m_is_started = true;
            start(pri);
        }
    }

}
void CAN_TP::stop_run()
{
    if(m_timerX)
    {
        m_timerX->stop();
    }
    else
    {
        m_is_started = false;
    }
}


void CAN_TP::timer_CB()
{
    int i=5;
    while( i > 0)
    {
        run();
        i --;
    }
}


//can-tp main thread
void CAN_TP::run()
{
    int ret = ISOTP_RET_OK;
    uint16_t outSize = 0;
    static  int is_printed = false;



    //qDebug("CAN_TP::run() Started!");

    do {
        //1. handle the canFrame queue
        //Note: it must read the queue one by one !!
        m_rx_canFrameQ_Mutex.lock();
        if(!m_rx_canFrameQ.isEmpty())
        {
            T_canFrame frame = m_rx_canFrameQ.dequeue();
            m_isoTp.isotp_on_can_message(frame.data,frame.len);
            
            //debug only
            if(m_isoTp.rx_protocolResult() != ISOTP_PROTOCOL_RESULT_OK)
            {
                qDebug("CAN_TP::run, Failed to receive a CAN Frame[%d]:[%02x %02x %02x %02x %02x %02x %02x %02x]",
                    frame.len,
                   frame.data[0], frame.data[1], frame.data[2],frame.data[3],
                   frame.data[4], frame.data[5], frame.data[6],frame.data[7]);
                m_isoTp.dump();
            }
        }
        m_rx_canFrameQ_Mutex.unlock();

        /*2.  Poll link to handle multiple frame transmition */
        m_isoTp.isotp_poll();

        //debug only
        if(m_isoTp.rx_protocolResult() != ISOTP_PROTOCOL_RESULT_OK || 
           m_isoTp.tx_protocolResult() != ISOTP_PROTOCOL_RESULT_OK ||
           m_isoTp.tx_status() == ISOTP_SEND_STATUS_ERROR)
        {
            if(!is_printed)
            {
                is_printed = true;
                qDebug("CAN_TP::run, poll() found Error !");
                m_isoTp.dump();
            }
        }
        else
        {
            is_printed = false;
        }


        /* 3. receive a message from CAN-TP and forward to UDS or other Appliation SWC  */
        if(ISOTP_RECEIVE_STATUS_FULL == m_isoTp.rx_status())
        {
            ret = m_isoTp.isotp_receive(m_rx_payload, sizeof(m_rx_payload), &outSize);
            if (ISOTP_RET_OK == ret)
            {
                /* Handle received message , forward to App !!*/
                //
                m_tpApp->receive_cantp_payload(m_rx_payload, outSize);
            }
        }

        /* 4. And send message via isotp_send to can-driver layer*/
        if(ISOTP_SEND_STATUS_IDLE == m_isoTp.tx_status() ||
           ISOTP_SEND_STATUS_ERROR == m_isoTp.tx_status())
        {
            if(!m_tx_payloadQ.isEmpty())
            {
                m_tx_payloadQ_Mutex.lock();
                T_canTPPayload payload = m_tx_payloadQ.dequeue();
                m_tx_payloadQ_Mutex.unlock();

                ret = m_isoTp.isotp_send(payload.data, payload.len);

                //debug only
                //qDebug("##### ISO-TP send out a payload[%d], ret = %d", payload.len, ret);
                if(ret != ISOTP_RET_OK)
                {
                    m_isoTp.dump();
                }
            }
        }

        //take a break to handle GUI events
        //msleep(1);

    }while(m_is_started);

    m_is_started = false;
    //qDebug("CAN_TP::run Exit !");
}
