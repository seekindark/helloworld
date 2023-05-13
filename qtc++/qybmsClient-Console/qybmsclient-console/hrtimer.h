/*====================================================================================
* File name: 	 hrtimer.h
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
#ifndef _hr_timer_h_
#define _hr_timer_h_
#include <stdint.h>

class HRTimerListener
{
public:
    virtual void timer_CB() = 0;
};
 
class HRTimer
{
public:
    HRTimer();
    ~HRTimer();
    bool start(HRTimerListener *listener, uint32_t _delay_ms);
    void stop(HRTimerListener *listener);
    static HRTimer *get();
    static void release();
private:
    uint32_t m_PeriodMax_ms;


};

#endif
