/*====================================================================================
* File name: 	 hrtimer.c
* Description:   a high-resoluiton timer implemented by windows multi-media timer
*                which can support 1ms resoution timer. But unfortunately such feature 
*                was removed by Microsoft since Win-10.
* Project:       EPT
* Note:
* Created on:    2023-5-11
* Author:        Fred.C
=======================================================================================
 History:
---------------------------------------------------------------------------------------
11-05-2023		Version 1.0.0       FC		Initialized version
-------------------------------------------------------------------------------------*/
#include "hrtimer.h"
#include <map>
#include <Windows.h>
//#pragma comment(lib, "Winmm.lib")
#include "hrtimer.h"
#include <QDebug>
 
HRTimer *g_instance = nullptr;
std::map<uint32_t, HRTimerListener *> g_listeners;
 
static void TimeProc(UINT uID, UINT UMsg, DWORD_PTR dwUser,
                     DWORD_PTR dw1, DWORD_PTR dw2)
{
    (void) UMsg;
    (void) dwUser;
    (void) dw1;
    (void) dw2;
    g_listeners[uID]->timer_CB();
}
 
HRTimer::HRTimer()
{
    m_PeriodMax_ms = 1;        //resolution ms

    TIMECAPS TimeCap;
    ::timeGetDevCaps(&TimeCap, sizeof(TIMECAPS));

    qDebug("HRTImer timerCap = [%d, %d]",TimeCap.wPeriodMin, TimeCap.wPeriodMax);
}
 
HRTimer *HRTimer::get()
{
    if (g_instance == nullptr)
    {
        g_instance = new HRTimer;
    }
    return g_instance;
}
 
void HRTimer::release()
{
    if (g_instance != nullptr)
    {
        delete g_instance;
        g_instance = nullptr;
    }
    if (!g_listeners.empty())
    {
        g_listeners.clear();
    }
}
 
bool HRTimer::start(HRTimerListener *listener, uint32_t _delay_ms )
{
    bool Result = true;
    uint32_t m_Id;

    //CF: Note (DWORD)(0) replace (DWORD)(this) !
    /*
         uDelay：以毫秒指定事件的周期。
         Uresolution：以毫秒指定延时的精度，数值越小定时器事件分辨率越高。缺省值为1ms。
         LpTimeProc：指向一个回调函数。
         DwUser：存放用户提供的回调数据。
         FuEvent：指定定时器事件类型：
         TIME_ONESHOT：uDelay毫秒后只产生一次事件
         TIME_PERIODIC ：每隔uDelay毫秒周期性地产生事件。
       */
    if ((m_Id = ::timeSetEvent(_delay_ms, m_PeriodMax_ms, (LPTIMECALLBACK)TimeProc,
                               (DWORD_PTR)(this), TIME_PERIODIC)) != NULL)
    {
        g_listeners.insert(std::map<uint32_t, HRTimerListener *>::value_type(m_Id, listener));
    }
    else
    {
        Result = false;
    }
    return Result;
}
 
void HRTimer::stop(HRTimerListener *listener)
{
    for (const auto &p : g_listeners)
    {
        if (p.second == listener)
        {
            ::timeKillEvent(p.first);
        }
    }
}
 
HRTimer::~HRTimer()
{
    for (const auto &p : g_listeners)
    {
        ::timeKillEvent(p.first);
    }
}

