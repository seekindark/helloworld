/*====================================================================================
* File name: 	 CAN_TP.h
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
#ifndef CAN_TP_H
#define CAN_TP_H

#include <QObject>
#include <QThread>
#include <QQueue>
#include <QMutex>
#include <QTimer>
#include "isotp.h"
#include "cantpappbase.h"
#include "hrtimer_x.h"

class CAN_TP: public QThread, HRTimer_X_Listener
{
    Q_OBJECT

    typedef struct
    {
        uint8_t data[8];
        uint8_t len;
    }T_canFrame;

    typedef struct{
        uint8_t data[ISO_TP_MAX_PAYLOAD_SIZE];
        uint16_t len;
    }T_canTPPayload;

public:
    CAN_TP(void *canChannel, uint32_t snd_canID, uint32_t rcv_canID, int32_t bufSize = ISO_TP_MAX_PAYLOAD_SIZE);

    ~CAN_TP();

    void set_callbackApp(canTpAppBase *cb){m_tpApp = cb;}
    uint32_t rxCanId() {return m_isoTp.rxCanId();}
    uint32_t txCanId() {return m_isoTp.txCanId();}
    uint8_t  tx_status(){return m_isoTp.tx_status();}
    uint8_t  rx_status(){return m_isoTp.rx_status();}
    void receive_canFrame(uint8_t *data, uint8_t len);
    int send_a_message(uint8_t *msg, uint16_t msgLen);

    void run();
    void start_run(Priority pri=InheritPriority);
    void stop_run();

    void timer_CB();

    void dump() {return m_isoTp.dump();}
    void resetSts(){return m_isoTp.resetSts();}

private:
    bool                    m_is_started;
    IsoTp                   m_isoTp;
    HRTimer_X               *m_timerX;

    bool                    m_isHrTimer; //High-Resolution Timer


    uint8_t                 m_rx_payload[ISO_TP_MAX_PAYLOAD_SIZE];

    QQueue<T_canFrame>      m_rx_canFrameQ;
    QMutex                  m_rx_canFrameQ_Mutex;

    QQueue<T_canTPPayload>  m_tx_payloadQ;
    QMutex                  m_tx_payloadQ_Mutex;

    
    canTpAppBase *          m_tpApp;
};

#endif // CAN_TP_H
