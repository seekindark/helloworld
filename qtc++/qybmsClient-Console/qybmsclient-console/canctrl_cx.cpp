/*====================================================================================
* File name: 	 CanCtrl_cx.c
* Description:   wrap a class for CANControl.Dll 64bits from ChangXin(CX), in order to be easier usage
* Project:       EPT
* Note:
* Created on:    2022-12-19
* Author:        Fred.C
=======================================================================================
 History:
---------------------------------------------------------------------------------------
19-12-2022		Version 1.0.0       FC		Initialized version
-------------------------------------------------------------------------------------*/

#include <QDebug>
#include "canctrl_cx.h"

namespace CX{

canctrl_cx::canctrl_cx(const QString& fileName, QObject *parent):QLibrary(fileName, parent)
{
    qDebug(__FUNCTION__);
    if(!load())
    {
        qDebug() << "load " <<fileName <<"failed !!";
    }
    else
    {
        qDebug() << "load " <<fileName <<"OK !!";
    }

    m_VCI_OpenDevice 	= (FPTR_VCI_OpenDevice 	  )resolve("VCI_OpenDevice");
    m_VCI_CloseDevice   = (FPTR_VCI_CloseDevice   )resolve("VCI_CloseDevice");
    m_VCI_InitCAN       = (FPTR_VCI_InitCAN       )resolve("VCI_InitCAN");
    m_VCI_ReadBoardInfo = (FPTR_VCI_ReadBoardInfo )resolve("VCI_ReadBoardInfo");
    m_VCI_SetReference  = (FPTR_VCI_SetReference  )resolve("VCI_SetReference");
    m_VCI_GetReceiveNum = (FPTR_VCI_GetReceiveNum )resolve("VCI_GetReceiveNum");
    m_VCI_ClearBuffer   = (FPTR_VCI_ClearBuffer   )resolve("VCI_ClearBuffer");
    m_VCI_StartCAN      = (FPTR_VCI_StartCAN      )resolve("VCI_StartCAN");
    m_VCI_ResetCAN      = (FPTR_VCI_ResetCAN      )resolve("VCI_ResetCAN");
    m_VCI_Transmit      = (FPTR_VCI_Transmit      )resolve("VCI_Transmit");
    m_VCI_Receive       = (FPTR_VCI_Receive       )resolve("VCI_Receive");
    m_VCI_UsbDeviceReset= (FPTR_VCI_UsbDeviceReset)resolve("VCI_UsbDeviceReset");
    m_VCI_FindUsbDevice2= (FPTR_VCI_FindUsbDevice2)resolve("VCI_FindUsbDevice2");

    if(m_VCI_OpenDevice    == nullptr)qDebug() << "Failed to resolve VCI_OpenDevice    ";
    if(m_VCI_CloseDevice   == nullptr)qDebug() << "Failed to resolve VCI_CloseDevice   ";
    if(m_VCI_InitCAN       == nullptr)qDebug() << "Failed to resolve VCI_InitCAN       ";
    if(m_VCI_ReadBoardInfo == nullptr)qDebug() << "Failed to resolve VCI_ReadBoardInfo ";
    if(m_VCI_SetReference  == nullptr)qDebug() << "Failed to resolve VCI_SetReference  ";
    if(m_VCI_GetReceiveNum == nullptr)qDebug() << "Failed to resolve VCI_GetReceiveNum ";
    if(m_VCI_ClearBuffer   == nullptr)qDebug() << "Failed to resolve VCI_ClearBuffer   ";
    if(m_VCI_StartCAN      == nullptr)qDebug() << "Failed to resolve VCI_StartCAN      ";
    if(m_VCI_ResetCAN      == nullptr)qDebug() << "Failed to resolve VCI_ResetCAN      ";
    if(m_VCI_Transmit      == nullptr)qDebug() << "Failed to resolve VCI_Transmit      ";
    if(m_VCI_Receive       == nullptr)qDebug() << "Failed to resolve VCI_Receive       ";
    if(m_VCI_UsbDeviceReset== nullptr)qDebug() << "Failed to resolve VCI_UsbDeviceReset";
    if(m_VCI_FindUsbDevice2== nullptr)qDebug() << "Failed to resolve VCI_FindUsbDevice2";

}

canctrl_cx::~canctrl_cx()
{
    qDebug(__FUNCTION__);
}

bool canctrl_cx::init_can()
{
    VCI_INIT_CONFIG vic;
    vic.AccCode = 0x0;
    vic.AccMask = 0xFFFFFFFF;
    vic.Filter=1;
    vic.Mode = 0;
    vic.Timing0 = 0x01;
    vic.Timing1 = 0x1C;

   return m_VCI_InitCAN(4, 0, 1, &vic);
}


}   //namespce CX
