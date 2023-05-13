#ifndef TESTCANTP_H
#define TESTCANTP_H
#include <QObject>
#include <QThread>
#include "canthread.h"
#include "can_tp.h"
#include "cantpappbase.h"
#include <QTime>
#include <hrtimer_x.h>


class testCanTp: public QThread, canTpAppBase, HRTimer_X_Listener
{
    Q_OBJECT

    typedef enum{
        e_tpIdle = 0,
        e_tpReq,
        e_tpReplied
    }E_tpState;

public:
    testCanTp(CANThread *canThr, CAN_TP *canTp);
    ~testCanTp();
    int receive_cantp_payload(uint8_t payload[], uint16_t size);


    void run();
    void start_run();
    void stop_run();


    void timer_CB();
    void start_sendMsgTask(int msgIdx, int txNum);
    void sendMsgTask();
private:

    E_tpState m_state;
    bool m_is_started;

    CANThread * m_canthread;
    CAN_TP *    m_canTpLink;



    QString delta_hhmmss(QTime beginTS);


    HRTimer_X *m_timerX;
    int m_smt_option;
    int m_smt_txNum;
    int m_smt_txIdx;
    bool m_smt_isRunning;
    QTime m_testBegin_ts;
    bool m_stm_timerCBRunning;
};

#endif // TESTCANTP_H
