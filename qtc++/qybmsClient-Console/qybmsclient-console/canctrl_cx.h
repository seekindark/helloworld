/*====================================================================================
* File name: 	 CanCtrl_cx.h
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

#ifndef CANCTRL_CX_H
#define CANCTRL_CX_H
#include <QLibrary>

namespace CX{


#include "lib-cx/ControlCAN.h"

EXTERNC typedef DWORD __stdcall (*FPTR_VCI_OpenDevice)	(DWORD DeviceType,DWORD DeviceInd,DWORD Reserved);
EXTERNC typedef DWORD __stdcall (*FPTR_VCI_CloseDevice)	(DWORD DeviceType,DWORD DeviceInd);
EXTERNC typedef DWORD __stdcall (*FPTR_VCI_InitCAN)		(DWORD DeviceType, DWORD DeviceInd, DWORD CANInd, PVCI_INIT_CONFIG pInitConfig);
EXTERNC typedef DWORD __stdcall (*FPTR_VCI_ReadBoardInfo)(DWORD DeviceType,DWORD DeviceInd,PVCI_BOARD_INFO pInfo);
EXTERNC typedef DWORD __stdcall (*FPTR_VCI_SetReference)(DWORD DeviceType,DWORD DeviceInd,DWORD CANInd,DWORD RefType,PVOID pData);
EXTERNC typedef ULONG __stdcall (*FPTR_VCI_GetReceiveNum)(DWORD DeviceType,DWORD DeviceInd,DWORD CANInd);
EXTERNC typedef DWORD __stdcall (*FPTR_VCI_ClearBuffer)	(DWORD DeviceType,DWORD DeviceInd,DWORD CANInd);
EXTERNC typedef DWORD __stdcall (*FPTR_VCI_StartCAN)	(DWORD DeviceType,DWORD DeviceInd,DWORD CANInd);
EXTERNC typedef DWORD __stdcall (*FPTR_VCI_ResetCAN)	(DWORD DeviceType,DWORD DeviceInd,DWORD CANInd);
EXTERNC typedef ULONG __stdcall (*FPTR_VCI_Transmit)	(DWORD DeviceType,DWORD DeviceInd,DWORD CANInd,PVCI_CAN_OBJ pSend,ULONG Len);
EXTERNC typedef ULONG __stdcall (*FPTR_VCI_Receive)		(DWORD DeviceType,DWORD DeviceInd,DWORD CANInd,PVCI_CAN_OBJ pReceive,ULONG Len,INT WaitTime);
//Other Functons dedicated for CX
EXTERNC typedef DWORD __stdcall (*FPTR_VCI_UsbDeviceReset)(DWORD DevType,DWORD DevIndex,DWORD Reserved);
EXTERNC typedef DWORD __stdcall (*FPTR_VCI_FindUsbDevice2)(PVCI_BOARD_INFO pInfo);

class canctrl_cx:public QLibrary
{
    Q_OBJECT
public:
    canctrl_cx(const QString& fileName, QObject *parent = nullptr);
    ~canctrl_cx();

    FPTR_VCI_OpenDevice 	 m_VCI_OpenDevice 	 ;
    FPTR_VCI_CloseDevice     m_VCI_CloseDevice   ;
    FPTR_VCI_InitCAN         m_VCI_InitCAN       ;
    FPTR_VCI_ReadBoardInfo   m_VCI_ReadBoardInfo ;
    FPTR_VCI_SetReference    m_VCI_SetReference  ;
    FPTR_VCI_GetReceiveNum   m_VCI_GetReceiveNum ;
    FPTR_VCI_ClearBuffer     m_VCI_ClearBuffer   ;
    FPTR_VCI_StartCAN        m_VCI_StartCAN      ;
    FPTR_VCI_ResetCAN        m_VCI_ResetCAN      ;
    FPTR_VCI_Transmit        m_VCI_Transmit      ;
    FPTR_VCI_Receive         m_VCI_Receive       ;
    FPTR_VCI_UsbDeviceReset  m_VCI_UsbDeviceReset;
    FPTR_VCI_FindUsbDevice2  m_VCI_FindUsbDevice2;

    bool init_can();

};


}   //namespace CX

#endif // CANCTRL_CX_H
