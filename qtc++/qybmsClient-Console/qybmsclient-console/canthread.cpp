/*====================================================================================
* File name: 	 m_canthread.cpp
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
#include "canthread.h"
#include "ControlCAN.h"
#include <QTime>
#include <QCoreApplication>
#include <QMetaType>
#include <string.h>
#include "can_tp.h"


CANThread::CANThread()
{
    m_stopped = true;
    //qRegisterMetaType<VCI_CAN_OBJ>("VCI_CAN_OBJ");
    //qRegisterMetaType<unsigned int>("DWORD");
    m_mode = 0;         //0 表示正常模式， 1 表示只听模式
    m_filterSet = e_filter_swup_only;

    m_canCtrl_cx = new CX::canctrl_cx("lib-cx/ControlCAN.dll");

    m_deviceType = 0;
    m_deviceIdx = 0;
    m_canChannelId = 0;
    m_baudRate = 0;

    m_timerX = new HRTimer_X();
}

CANThread::~CANThread()
{
    m_stopped = true;
    terminate();
    closeDevice();
    delete m_canCtrl_cx;
    qDebug(__FUNCTION__);

}

const QString CANThread::getDeviceTypeStr(int vci_deviceType)
{
    switch(vci_deviceType)
    {
        case VCI_USBCAN1: return QString("%1-USBCAN1").arg(vci_deviceType);
        case VCI_USBCAN2: return QString("%1-USBCANalyst-II(CAN2 or CAN2A)").arg(vci_deviceType);
        case VCI_USBCAN_E_U: return QString("%1-USBCAN_E_U").arg(vci_deviceType);
        case VCI_USBCAN_2E_U: return QString("%1-USBCAN_2E_U").arg(vci_deviceType);
    default:
            return QString("%1-Wrong Type").arg(vci_deviceType);
    }
}

void CANThread::set_params(UINT deviceType,UINT debicIndex, UINT channel ,UINT baudRate, E_FilterType filterSet)
{
    m_deviceType    = deviceType;
    m_deviceIdx     = debicIndex;
    m_canChannelId  = channel;
    m_baudRate      = baudRate;
    m_filterSet     = filterSet;

    qDebug() << __FUNCTION__;
    qDebug() << "Device: " << getDeviceTypeStr(m_deviceType);
    qDebug() << "Index: " << m_deviceIdx;
    qDebug() << "Channel:" << m_canChannelId;
    qDebug() << "Baudrate:" << m_baudRate;
    qDebug() << "filterSet:" << (filterSet == e_filter_rcv_all?"Rcv All":"Swup only");
 }

//1.打开设备
bool CANThread::openDevice()
{
    unsigned int dwRel;

    if(m_deviceType == VCI_USBCAN2)
    {
        if(m_canCtrl_cx->m_VCI_OpenDevice(m_deviceType, m_deviceIdx, m_baudRate))
        {
            qDebug("Open device-type %s OK, m_deviceIdx=%d",
                   getDeviceTypeStr(m_deviceType).toStdString().data(),
                   m_deviceIdx);
            return true;
        }
        return false;
    }
    else
    {
        dwRel = VCI_OpenDevice(m_deviceType, m_deviceIdx, 0);
        if(dwRel == 1)
        {
            qDebug("Open device-type %s OK, m_deviceIdx=%d",
                   getDeviceTypeStr(m_deviceType).toStdString().data(),
                   m_deviceIdx);
            return true;
        }
        else
        {
            qDebug("Open device-type %s NOK, m_deviceIdx = %d!",
                   getDeviceTypeStr(m_deviceType).toStdString().data(),
                   m_deviceIdx);

            VCI_ERR_INFO erroInfo;
            memset(&erroInfo, 0, sizeof(erroInfo));
            if(VCI_ReadErrInfo(m_deviceType, m_deviceIdx, m_canChannelId, &erroInfo) == 1)
            {
                qDebug("errorInfo.errorCode = 0x%x", erroInfo.ErrCode);
            }
            else
            {
                qDebug("Faield to do VCI_ReadErrInfo !");
            }
            return false;
        }
    }
}

//set baudrate
bool CANThread::setBaudrate()
{
    uint8_t buffer[4];
    switch (m_baudRate)
    {
    case 5:
        buffer[0] = 0xC1;
        buffer[1] = 0x01;
        buffer[2] = 0x1C;
        buffer[3] = 0x00;
        break;
    case 10:
        buffer[0] = 0xE0;
        buffer[1] = 0x00;
        buffer[2] = 0x1C;
        buffer[3] = 0x00;
        break;
    case 20:
        buffer[0] = 0xB3;
        buffer[1] = 0x00;
        buffer[2] = 0x16;
        buffer[3] = 0x00;
        break;
    case 50:
        buffer[0] = 0x2C;
        buffer[1] = 0x00;
        buffer[2] = 0x1C;
        buffer[3] = 0x00;
        break;
    case 100:
        buffer[0] = 0x23;
        buffer[1] = 0x00;
        buffer[2] = 0x16;
        buffer[3] = 0x00;
        break;
    case 125:
        buffer[0] = 0x11;
        buffer[1] = 0x00;
        buffer[2] = 0x1C;
        buffer[3] = 0x00;
        break;
    case 250:
        buffer[0] = 0x08;
        buffer[1] = 0x00;
        buffer[2] = 0x1C;
        buffer[3] = 0x00;
        break;
    case 500:
        buffer[0] = 0x07;
        buffer[1] = 0x00;
        buffer[2] = 0x06;
        buffer[3] = 0x00;
        break;
    case 800:
        buffer[0] = 0x04;
        buffer[1] = 0x00;
        buffer[2] = 0x06;
        buffer[3] = 0x00;
        break;
    case 1000:
    default:
        buffer[0] = 0x03;
        buffer[1] = 0x00;
        buffer[2] = 0x06;
        buffer[3] = 0x00;
        break;
    }

    int ret = VCI_SetReference(m_deviceType, m_deviceIdx, m_canChannelId, 0, buffer);
    if (ret != 1)
    {
        qDebug("VCI_SetReference: set baudrate Failed !");
        return false;
    }
    else
    {
        qDebug("VCI_SetReference:set baudrate OK !");
    }
    return true;
}

bool CANThread::setDeviceMode(uint8_t mode)
{
    VCI_INIT_CONFIG vic;
    vic.AccCode = 0x0;
    vic.AccMask = 0xFFFFFFFF;
    vic.Filter=1;
    vic.Mode=mode;     //only mode is concerned !! 0 表示正常模式， 1 表示只听模式
    int ret = VCI_InitCAN(m_deviceType, m_deviceIdx, m_canChannelId, &vic);
    if(ret != 1)
    {
        qDebug("VCI_InitCAN:setDeviceMode Failed !");
        return false;
    }
    else
    {
         qDebug("VCI_InitCAN:setDeviceMode OK !");
    }

    return true;
}

void CANThread::set_timing_baudrate_cx(VCI_INIT_CONFIG &vic)
{
    switch (m_baudRate) {
    case 5:
        vic.Timing0=0xBF;
        vic.Timing1=0xFF;
        break;
    case 10:
        vic.Timing0=0x31;
        vic.Timing1=0x1c;
        break;
    case 20:
        vic.Timing0=0x18;
        vic.Timing1=0x1c;
        break;
    case 50:
        vic.Timing0=0x09;
        vic.Timing1=0x1c;
        break;
    case 100:
        vic.Timing0=0x04;
        vic.Timing1=0x1c;
        break;
    case 125:
        vic.Timing0=0x03;
        vic.Timing1=0x1c;
        break;
    case 250:
        vic.Timing0=0x01;
        vic.Timing1=0x1c;
        break;
    case 500:
        vic.Timing0=0x00;
        vic.Timing1=0x1c;
        break;
    case 800:
        vic.Timing0=0x00;
        vic.Timing1=0x16;
        break;
    case 1000:
    default:
        vic.Timing0=0x00;
        vic.Timing1=0x14;
        break;
    }
}
QString CANThread::versionStr(short ver)
{
    return "v" + QString::number((ver & 0x0FFF) /0x100,16).toUpper()
            + "." + QString("%1 ").arg((ver & 0x0FFF) % 0x100,2,16,QChar('0')).toUpper();
}

bool CANThread::readBoardInfo()
{
    unsigned int dwRel = 0;
     if(m_deviceType == VCI_USBCAN2)
     {
         //read board information
         VCI_BOARD_INFO vbi;
         dwRel = m_canCtrl_cx->m_VCI_ReadBoardInfo(m_deviceType, m_deviceIdx, &vbi);
         if(dwRel != 1)
         {
             qDebug("Failed to Read Board Info!");
             return false;
         }
         else
         {
             qDebug("Read Board Info OK !");

#if 0
             emit boardInfo(vbi);
#else
             QString boardInfo;
             boardInfo +=("Chnl: " + QString::number(vbi.can_Num) + "\n");
             boardInfo +=("HW  : " + versionStr(vbi.hw_Version) + "\n");
             boardInfo +=("SW  : " + versionStr(vbi.fw_Version) + "\n");
             boardInfo +=("Intf: " + versionStr(vbi.in_Version) + "\n");
             boardInfo +=("Drv : " + versionStr(vbi.dr_Version) + "\n");
             boardInfo +=("SN  : " + QString(vbi.str_Serial_Num) + "\n");
             boardInfo +=("Type: " + QString(vbi.str_hw_Type));
#endif
            qDebug() << boardInfo;
         }
     }
     else
     {
         //read board information
         VCI_BOARD_INFO vbi;
         dwRel = VCI_ReadBoardInfo(m_deviceType, m_deviceIdx, &vbi);
         if(dwRel != 1)
         {
             qDebug("Read Board Info NOK !");
             return false;
         }
         else
         {
             qDebug("Read Board Info OK !");
             emit boardInfo(vbi);
         }

     }

     return true;
}

//2.初始化CAN
bool CANThread::initCAN()
{
    unsigned int dwRel = 0;

    if(m_deviceType == VCI_USBCAN2)
    {
        //clear buffer no need

        VCI_INIT_CONFIG vic;

        if(m_filterSet == e_filter_rcv_all)
        {
            vic.AccCode=0x80000008; //default all
            vic.AccMask=0xFFFFFFFF; //default all
        }
        else if(m_filterSet == e_filter_swup_only)
        {
            vic.AccCode = 0xC0000000; //0X600 << 21;   //0X601, the beginning of ID range 0x601~0x6ff
            vic.AccMask = 0x1FFFFFFF; //0xFF << 21;
        }

        vic.Filter=1;   //=1 表示单滤波， ，=0 表示双滤波
        vic.Mode=m_mode;     //0 表示正常模式， 1 表示只听模式

        set_timing_baudrate_cx(vic);

        int ret =m_canCtrl_cx->m_VCI_InitCAN(m_deviceType, m_deviceIdx, m_canChannelId, &vic);
        if(ret == 1)
        {
            qDebug("init device-type %s OK, idx=%d,ch=%d, ret=%d, accCode = 0x%lx, accMask=0x%lx",
                   getDeviceTypeStr(m_deviceType).toStdString().data(),
                   m_deviceIdx, m_canChannelId,
                   ret,
                   vic.AccCode,
                   vic.AccMask);
        }
        else
        {
            qDebug("init device-type %s NOK, ret=%d",
                   getDeviceTypeStr(m_deviceType).toStdString().data(),
                   ret);
        }

    }
    else
    {
        qDebug(" ===USE_ZLG_CANLIB ===");
        if(m_deviceType == VCI_USBCAN_E_U ||
           m_deviceType == VCI_USBCAN_2E_U ||
           m_deviceType == VCI_USBCAN_4E_U)
        {

            if(setBaudrate())
            {
                qDebug("Set baudrate %dK OK", m_baudRate);
            }
            else
            {
                qDebug("Set baudrate %dK NOK", m_baudRate);
                return false;
            }

            if(setDeviceMode(m_mode))
            {
                qDebug("setDeviceMode mode=%d OK", m_mode);
            }
            else
            {
                qDebug("setDeviceMode mode=%d NOK", m_mode);
            }

            //set filtering
            if(m_filterSet == e_filter_swup_only)
            {
                //set filter
                VCI_FILTER_RECORD vfr;
                vfr.ExtFrame = 0;       //standard frame
                vfr.Start = 0x600;
                vfr.End = 0x6FF;

                uint refType = m_deviceType == VCI_USBCAN_4E_U? 7: 1;   //set type for filtering set
                int ret = VCI_SetReference(m_deviceType, m_deviceIdx, m_canChannelId, refType, &vfr);
                if (ret != 1)
                {
                    qDebug("VCI_SetReference: set filtering Failed !");
                    return false;
                }
                else
                {
                    qDebug("VCI_SetReference:set filtering OK !");
                    if(m_deviceType == VCI_USBCAN_E_U ||
                       m_deviceType == VCI_USBCAN_2E_U ||
                       m_deviceType == VCI_USBCAN_4E_U)
                    {
                        //type 2 - means to enable the  filtering set
                        char tm = 0;        //CF: !!! Though the last parameter pData is ignnored by VCI_SetReference,
                                            //    we still need fill a non-nullPtr here, e.g. &tm;
                                            //    Or the filtering cannot be enabled !!
                        int ret = VCI_SetReference(m_deviceType, m_deviceIdx, m_canChannelId, 2, &tm);
                        qDebug("StartCAN: VCI_SetReference: enable filtering set %s !", ret == 1? "OK": "NOK");
                    }
                }
            }

        }
        else  //Other USBCAN Devices:USBCAN1
        {
            VCI_INIT_CONFIG vic;
            if(m_filterSet == e_filter_rcv_all)
            {

                vic.AccCode=0x0;        //default all//0x80000008 if it's Chuang xin Tech.
                vic.AccMask=0xFFFFFFFF; //default all
            }
            else if(m_filterSet == e_filter_swup_only)
            {
                vic.AccCode = 0xC0000000; //0X600 << 21;   //0X601, the beginning of ID range 0x601~0x6ff
                vic.AccMask = 0x1FFFFFFF;            //0xFF << 21;
            }

            vic.Filter=1;   //=1 表示单滤波， ，=0 表示双滤波
            vic.Mode=m_mode;     //0 表示正常模式， 1 表示只听模式

            set_timing_baudrate_cx(vic);

            dwRel = 0;
            dwRel += VCI_InitCAN(m_deviceType, m_deviceIdx, m_canChannelId, &vic);
            if(dwRel == 0)
            {
                qDebug("init device-type %s NOK !", getDeviceTypeStr(m_deviceType).toStdString().data());
                return false;
            }
            else
            {
                qDebug("init device-type %s OK, dwRel=%d",
                       getDeviceTypeStr(m_deviceType).toStdString().data(),
                       dwRel);
            }
        }


    }

    return true;
}


quint32 CANThread::getErrInfo()
{
    VCI_ERR_INFO erroInfo;
    memset(&erroInfo, 0, sizeof(erroInfo));
    if(m_deviceType == VCI_USBCAN2)
    {
        return -1;
    }
    else
    {
        if(VCI_ReadErrInfo(m_deviceType, m_deviceIdx, m_canChannelId, &erroInfo) == 1)
        {
            qDebug("errorInfo.errorCode = 0x%x", erroInfo.ErrCode);
        }
        else
        {
            qDebug("Faield to do VCI_ReadErrInfo !");
        }
    }
    return erroInfo.ErrCode;

}


//3.启动CAN
bool CANThread::startCAN()
{

    if(m_deviceType == VCI_USBCAN2)
    {
        //reset can channel
        if(m_canCtrl_cx->m_VCI_ResetCAN(m_deviceType, m_deviceIdx, m_canChannelId) !=1)
        {

            qDebug("StartCAN: reset CAN channel %d NOK!", m_canChannelId);
            return false;
        }
        else
        {
            qDebug("StartCAN: reset CAN channel %d OK", m_canChannelId);
        }
        //start channel
        if(m_canCtrl_cx->m_VCI_StartCAN(m_deviceType, m_deviceIdx, m_canChannelId) !=1)
        {
            qDebug("StartCAN: start CAN channel %d NOK!", m_canChannelId);
            return false;
        }
        else
        {
            qDebug("StartCAN: start CAN channel %d OK", m_canChannelId);
        }
    }
    else
    {
        //reset can channel 0
        if(VCI_ResetCAN(m_deviceType, m_deviceIdx, m_canChannelId) !=1)
        {

            qDebug("StartCAN: reset CAN channel 0 NOK!");
            return false;
        }
        else
        {
            qDebug("StartCAN: reset CAN channel 0 OK");
        }

        //start channel 0
        if(VCI_StartCAN(m_deviceType, m_deviceIdx, m_canChannelId) !=1)
        {
            qDebug()<<"StartCAN  Channel 0 NOK !.";
            return false;
        }
        else
        {
           qDebug()<<"StartCAN  Channel 0 OK.";
        }
    }

    return true;
}

void CANThread:: push_canTxQ(VCI_CAN_OBJ * canFrameObj)
{
    m_canTxQMutex.lock();
    m_canTxQueue.enqueue(*canFrameObj);
    m_canTxQMutex.unlock();
}
VCI_CAN_OBJ CANThread::pop_canTxQ()
{
    VCI_CAN_OBJ obj;
    obj.DataLen = 0;

    m_canTxQMutex.lock();
    if(!m_canTxQueue.isEmpty())
    {
        obj= m_canTxQueue.dequeue();
    }
    m_canTxQMutex.unlock();

    return obj;
}



//4.发送数据
bool CANThread::sendData(UINT msgID,BYTE remoteFlag,BYTE externFlag,const unsigned char *data,BYTE len)
{
    unsigned int dwRel = 0;
    VCI_CAN_OBJ vco;
    vco.ID = msgID ;
    vco.RemoteFlag = remoteFlag;
    vco.ExternFlag = externFlag;
    vco.SendType = 0;   //正常发送
    vco.DataLen = len;

    for(UINT j = 0;j < len;j++)
        vco.Data[j] = data[j];

    if(m_deviceType == VCI_USBCAN2)
    {
        //dwRel =m_canCtrl_cx->m_VCI_Transmit(m_deviceType, m_deviceIdx, m_canChannelId, &vco,1);
        
        push_canTxQ(&vco);

        dwRel = 1;
    }
    else
    {
        //dwRel = VCI_Transmit(m_deviceType, m_deviceIdx, m_canChannelId,&vco,1);
        push_canTxQ(&vco);
        dwRel = 1;
    }

    if(dwRel>0)
        return true;
    else
        return false;

}

//5.关闭设备
void CANThread::closeDevice()
{
    qDebug("clear buffer & close device...");

    stop_run();
    if(m_deviceType == VCI_USBCAN2)
    {
        m_canCtrl_cx->m_VCI_ClearBuffer(m_deviceType, m_deviceIdx, m_canChannelId);
        m_canCtrl_cx->m_VCI_CloseDevice(m_deviceType, m_deviceIdx);
    }
    else
    {
        VCI_ClearBuffer(m_deviceType, m_deviceIdx, m_canChannelId);
        VCI_CloseDevice(m_deviceType, m_deviceIdx);
    }
    qDebug("close device...OK");
 }

//0.复位设备，  复位后回到3
bool CANThread::reSetCAN()
{
    int ret = 0;
    if(m_deviceType == VCI_USBCAN2)
    {
        //reset can channel
        if((ret=m_canCtrl_cx->m_VCI_ResetCAN(m_deviceType, m_deviceIdx, m_canChannelId)) !=1)
        {

            qDebug("reSetCAN: reset CAN channel 0 NOK!, ret = %d", ret);

            if(m_canCtrl_cx->m_VCI_UsbDeviceReset(m_deviceType, m_deviceIdx, m_canChannelId))
            {
                qDebug("%s: VCI_UsbDeviceReset CX CANBox OK", __FUNCTION__);
            }
            else
            {
                qDebug("%s: VCI_UsbDeviceReset CANalyst_II CANBox NOK !", __FUNCTION__);
            }

            return false;
        }
        else
        {
            qDebug("reSetCAN: reset CAN channel 0 OK");
        }

    }
    else
    {
        //reset can channel
        if(VCI_ResetCAN(m_deviceType, m_deviceIdx, m_canChannelId) !=1)
        {

            qDebug("reSetCAN: reset CAN channel 0 NOK!");
            return false;
        }
        else
        {
            qDebug("reSetCAN: reset CAN channel 0 OK");
        }
    }

    return true;
}

void CANThread::register_canTP(void * cantp)
{
    if(cantp != NULL)
    {
        m_canTPVec.append(cantp);
    }
}

void CANThread::start_run(Priority pri)
{
    if(m_timerX)
    {
        m_timerX->start(this, 1);
    }
    else
    {
        m_stopped = false;
        start(pri);
    }
}

void CANThread::stop_run()
{
    if(m_timerX)
    {
        m_timerX->stop();
    }
    
    m_stopped = true;
}

void CANThread::timer_CB()
{
    run();
}

VCI_CAN_OBJ CANThread::readOut_CanMsg()
{
    VCI_CAN_OBJ msg;
    msg.DataLen = 0;

    m_canDrvQMutex.lock();
    if(!m_canDrvQueue.isEmpty())
    {
        msg = m_canDrvQueue.dequeue();
    }
    m_canDrvQMutex.unlock();

    return msg;
}

void CANThread::receiveIn_canMsges(VCI_CAN_OBJ *msgList, int msgNum)
{
    if(msgList == NULL || msgNum == 0)
    {
        return;
    }
    for(int i=0; i< msgNum; i++)
    {
        m_canDrvQMutex.lock();
        m_canDrvQueue.enqueue(msgList[i]);
        m_canDrvQMutex.unlock();
    }
}

void CANThread::run()
{
    int dwRel=0;
    static VCI_CAN_OBJ vco[2500];

    //qDebug("CANThread::run started !!");
    //qDebug() << QDateTime::currentDateTime().toString("hh:mm:ss.zzz") << "CANThread::run" ;


    do
    {
        //qDebug(">>> CANThread::Run");
        if(m_deviceType == VCI_USBCAN2)
        {
            //send can frame
#if 0
            while(m_canTxQueue.size() > 0)
            {
                VCI_CAN_OBJ canFrame = pop_canTxQ();

                if(canFrame.DataLen != 0)
                {
                    /*int ret = */m_canCtrl_cx->m_VCI_Transmit(m_deviceType, m_deviceIdx, m_canChannelId, &canFrame,1);
                    //qDebug() << QDateTime::currentDateTime().toString("hh:mm:ss.zzz") << "canThread, Tx Frame";

//                    qDebug("CANThread::run: send out a can Frame[%02x %02x %02x %02x %02x %02x %02x %02x], ret = %d",
//                           canFrame.Data[0], canFrame.Data[1], canFrame.Data[2],canFrame.Data[3],
//                            canFrame.Data[4], canFrame.Data[5], canFrame.Data[6],canFrame.Data[7],
//                            ret);
                }

                break;  //just send one frame
            }
#else
        int txNum = 0;
        while(m_canTxQueue.size() > 0)
        {
            vco[txNum] = pop_canTxQ();
            if(vco[txNum].DataLen != 0)
            {
                txNum ++;
            }
        }
        m_canCtrl_cx->m_VCI_Transmit(m_deviceType, m_deviceIdx, m_canChannelId, vco, txNum);
        //if (txNum> 0)
            //qDebug() << QDateTime::currentDateTime().toString("hh:mm:ss.zzz") << "canThread, Tx= " << txNum;
 #endif


            //recive can frame
            dwRel = m_canCtrl_cx->m_VCI_Receive(m_deviceType, m_deviceIdx, m_canChannelId, vco,2500,0);
            if(dwRel > 0)
            {
                bool is_canTpMsg = false;
                for( int i=0; i< dwRel; i++)
                {
                    //qDebug("Rx[%d]:", i);
                    is_canTpMsg = false;
                    CAN_TP * canTp_p = NULL;
                    for(int j=0;j < m_canTPVec.size(); j++)
                    //for(CAN_TP * canTp_p : m_canTPVec)
                    {
                        canTp_p = static_cast<CAN_TP *>(m_canTPVec.at(j));
                        if(canTp_p != NULL &&
                           canTp_p->rxCanId() == vco[i].ID)
                        {
                            canTp_p->receive_canFrame(vco[i].Data, vco[i].DataLen);
                            is_canTpMsg = true;
                            break;
                        }
                    }

                    if(!is_canTpMsg)
                    {
                        receiveIn_canMsges(vco, 1);

                        emit getProtocolData(dwRel, m_canChannelId);
                        qDebug(" == emit canFrame to getProtocolData, i=%d", i);
                    }
                }

            }
            else if(dwRel < 0)
            {
                qDebug("VCI_Receive, dwRel = %d, break !", dwRel);
                break;
            }

        }
        else
        {
            dwRel = VCI_Receive(m_deviceType, m_deviceIdx, m_canChannelId, vco,2500,0);
            if(dwRel > 0)
            {
                receiveIn_canMsges(vco, dwRel);

                emit getProtocolData(dwRel,m_canChannelId);

                //msleep(10);
            }
        }

        //msleep(1);

    }while(!m_stopped);

    m_stopped = true;
    //qDebug("CANThread::run Exit !!");
}



