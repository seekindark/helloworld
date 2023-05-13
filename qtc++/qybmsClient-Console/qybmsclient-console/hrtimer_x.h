/*====================================================================================
* File name: 	 hrtimer_x.h
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
#ifndef _hrtimer_x_h_
#define _hrtimer_x_h_
#include <stdint.h>
#include <map>
#include <thread>

class HRTimer_X_Listener
{
public:
    virtual void timer_CB() = 0;
};
 
class HRTimer_X
{
public:
    HRTimer_X(uint32_t interval_us = 1000);
    ~HRTimer_X();
    bool start(HRTimer_X_Listener *listener, uint32_t _delay_ms = 1);
    void stop();
    static int run(uint32_t intervalu_us, HRTimer_X_Listener *listener);
private:
    uint32_t            m_interval_us;
    std::thread *       m_thd;
    HRTimer_X_Listener *m_listener;

};



#endif
