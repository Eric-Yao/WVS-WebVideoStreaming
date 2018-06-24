/*****************************************************************************
*
* File: PrecisionTimer.cpp
* Project: OpenGL test: Torus.exe
*
* Description: This class handle accurate timer measure, using rdtsc pentium
* instruction. This code compile with gcc, under Linux, and windows. 
* Windows Sleep() methods works with millisecond, while Linux sleep works with
* seconds.
*
* Usage note:
* The Timer MUST be initialised with the method init(int s). The longest is s
* the more accurate is the mesaurment. Then Call start() to start measuring,
* and call stop() to stop it. Then call getDuration() to  get time duration in
* second.
*
* Author: Vedder Bruno
* Date: 19/04/2004
* URL: http://bcz.emu-france.com/
*****************************************************************************/
#ifndef _RDTSC_TIMER_H
#define _RDTSC_TIMER_H

#include <iostream>
#include <unistd.h>
#include <sys/time.h>


#define PORTABLE_SLEEP_FUNC  sleep( s );

class PrecisionTimer
{
    public:
        PrecisionTimer();
        ~PrecisionTimer();
        void start();
        void stop();
        float getDuration();
    private:
        struct timeval _tv;
        float _duration;
        unsigned long T0[2];
        unsigned long T1[1];
};
#endif
