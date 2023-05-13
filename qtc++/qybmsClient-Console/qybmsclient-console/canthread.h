/*====================================================================================
* File name: 	 m_canthread.h
* Description:   a class to operate USBCAN Device (open, close, init, reset, transmit, recieve etc.)
* Project:       EPT
* Note:
* Created on:    2022-12-19
* Author:        Fred.C
=======================================================================================
 History:
---------------------------------------------------------------------------------------
19-12-2022		Version 1.0.0       FC		Initialized version
-------------------------------------------------------------------------------------*/
#ifndef CANTHREAD_H
#define CANTHREAD_H

#include <QThread>
#include "ControlCAN.h"
#include <QDebug>
#include "canctrl_cx.h"
#include <QQueue>
#include <qmutex.h>
#include <QString>
#include "hrtimer_x.h"

#define USE_ZLG_CANLIB  1

class CANThread:public QThread, HRTimer_X_Listener
{
    Q_OBJECT

public:
    typedef enum
    {
        e_refType_0 = 0,
        e_refType_filterSet = 1,
        e_refType_filterStart = 2,
        e_refType_filterClear = 3,
        e_refType_transTimeoutSet = 4,
        e_refType_autoSendObj_set = 5,
        e_refType_autoSendObj_clear = 6
    }E_refType;

    typedef enum
    {
        e_filter_swup_only = 0, // filtered by SWUP CAN ID only, 0x600~0x6FF, standard CAN Frame
        e_filter_rcv_all = 1    //no filtering
    }E_FilterType;


public:
    CANThread();
    ~CANThread();
    void timer_CB();

    //quint16 crc16ForModbus(const QByteArray &data, int size);


    //1.打开设备
    bool openDevice();

    bool readBoardInfo();

    //2.初始化CAN
    bool initCAN();

    //3.启动CAN
    bool startCAN();

    quint32 getErrInfo();

    //4.发送数据
    bool sendData(UINT msgID,BYTE remoteFlag,BYTE externFlag,const unsigned char *data,BYTE len);

    //5.关闭设备
    void closeDevice();

    //0.复位设备，  复位后回到3
    bool reSetCAN();

    void start_run(Priority pri = InheritPriority);
    void stop_run();

    void set_params(UINT deviceType,UINT debicIndex, UINT channel ,UINT baudRate, E_FilterType filterSet);

    UINT canChannleID(){return m_canChannelId;}

    bool is_canDrvQueueEmpty(){return m_canDrvQueue.isEmpty();}
    VCI_CAN_OBJ readOut_CanMsg();
    void receiveIn_canMsges(VCI_CAN_OBJ *msgList, int msgNum);
    void register_canTP(void * cantp);

    void push_canTxQ(VCI_CAN_OBJ * canFrameObj);
    VCI_CAN_OBJ pop_canTxQ();


    UINT m_deviceType;
    UINT m_deviceIdx;
    UINT m_canChannelId;
    UINT m_baudRate;       //kbps

    uint8_t         m_mode;
    E_FilterType    m_filterSet;
    bool            m_stopped;

signals:
    void getProtocolData(unsigned int dwRel,unsigned int channel);
    void boardInfo(VCI_BOARD_INFO vbi);

private:



    void run();

    QString versionStr(short ver);
    const QString getDeviceTypeStr(int vci_deviceType);
    bool setBaudrate();
    bool setDeviceMode(uint8_t mode);
    void set_timing_baudrate_cx(VCI_INIT_CONFIG &vic);

    CX::canctrl_cx *    m_canCtrl_cx;
    QQueue<VCI_CAN_OBJ> m_canDrvQueue;
    QMutex              m_canDrvQMutex;
    QVector<void *>     m_canTPVec;

    QQueue<VCI_CAN_OBJ> m_canTxQueue;
    QMutex              m_canTxQMutex;

    HRTimer_X           *m_timerX;
};

#endif // CANTHREAD_H
