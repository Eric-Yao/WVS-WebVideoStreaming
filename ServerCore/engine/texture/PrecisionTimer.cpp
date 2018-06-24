/*****************************************************************************
*
* File: PrecisionTimer.cpp
* Project: OpenGL test: Torus.exe
*
* Description: This class handle accurate timer measure, using rdtsc pentium
* instruction. This code compile with gcc, under Linux, and windows. See in
* PrecisionTimer.h for more information about Sleep(millis)/sleep(sec) issues.
*
* Author: Vedder Bruno
* Date: 19/04/2004
* URL: http://bcz.emu-france.com/
*****************************************************************************/
#include "PrecisionTimer.h"

PrecisionTimer::PrecisionTimer() {
  T0[0] = 0;
  T0[1] = 0;
  T1[0] = 0;
  T1[1] = 0;
  _duration = 0;
}

PrecisionTimer::~PrecisionTimer() {
}

void PrecisionTimer::start() {
 if(0==gettimeofday(&_tv,NULL)){
    T0[0] = _tv.tv_sec;
    T0[1] = _tv.tv_usec;
         // printf("§§§§§§§§§§§§ bGetDetailTime Sec:%d; MicSec:%d\n",_tv.tv_sec,_tv.tv_usec);
    }
}

void PrecisionTimer::stop() {
 if(0==gettimeofday(&_tv,NULL)){
    T1[0] = _tv.tv_sec;
    T1[1] = _tv.tv_usec;
        // printf("§§§§§§§§§§§§ bGetDetailTime Sec:%d; MicSec:%d\n",_tv.tv_sec,_tv.tv_usec);
    }
}

float PrecisionTimer::getDuration() {
  if (T0[0] == T1[0]) {
    _duration = T1[1] - T0[1];
 //   TRACE("§§§§§§§§§§§§ bGetDetailTime Sec:%d; MicSec:%d\n",tv.tv_sec,tv.tv_usec);
  } else {
    _duration = 1000000 - T0[1] + T1[1];
  }
  return (_duration/1000000);
}

