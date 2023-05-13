/*====================================================================================
* File name: 	 hrtimer_x.c
* Description:   a high-resoluiton timer implemented by CPP-11 or -higher version directly, 
*                intead of window multi-media timer which does not support <15ms or (1ms)
*                resoluiton timer anymore since win10
* Project:       EPT
* Note:
* Created on:    2023-5-11
* Author:        Fred.C
=======================================================================================
 History:
---------------------------------------------------------------------------------------
11-05-2023		Version 1.0.0       FC		Initialized version
-------------------------------------------------------------------------------------*/
#include <chrono>
#include <QDebug>
#include <QTime>
#include "hrtimer_x.h"

HRTimer_X::HRTimer_X(uint32_t interval_us)
{
    m_interval_us = interval_us;
    m_thd = nullptr;
    m_listener = nullptr;
    qDebug("HRTimer_X::HRTimer_X with %d us\n", interval_us);
}

HRTimer_X::~HRTimer_X()
{
    stop();
    qDebug("HRTimer_X::~HRTimer_X with %d us\n", m_interval_us);
}

int HRTimer_X::run(uint32_t intervalu_us, HRTimer_X_Listener *listener)
{
    std::chrono::steady_clock::time_point tp = std::chrono::steady_clock::now();
    if(intervalu_us == 0) intervalu_us = 1000;
    auto dt_micri = std::chrono::microseconds(intervalu_us/2);
	while (true) 
    {
		tp += dt_micri;
        std::this_thread::sleep_until(tp);
        if(listener)
        {
//            QString strDateTime = QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
//            qDebug() << strDateTime << "HRTimer_X::" <<__FUNCTION__ << "timer_CB";
            listener->timer_CB();
        }
        else
        {
            QString strDateTime = QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
            qDebug() << strDateTime << "HRTimer_X::" <<__FUNCTION__ << "m_listener is null !";
        }
	}
 
	return(0);
} 

bool HRTimer_X::start(HRTimer_X_Listener *listener, uint32_t _delay_ms )
{
    if(m_thd == nullptr)
    {
        m_interval_us = _delay_ms * 1000;   //ms -> micoseconds
        m_listener = listener;
        m_thd = new std::thread(HRTimer_X::run, m_interval_us, m_listener);
        m_thd->detach();
        return true;
    }
    
    //qDebug("HRTimer_X::start is running !");
    return false;
   
}
 
void HRTimer_X::stop()
{
    if(m_thd)
    {
        delete m_thd;
    }
    m_thd = nullptr;
    m_listener = nullptr;
}
 


