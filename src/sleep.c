/*
 * sleep.c
 *
 *  Created on: 27.06.2012
 *      Author: user
 */


#ifdef CWT_CC_MSC
#include <cwt/global.h>
#include <cwt/types.h>

/* Source: http://stackoverflow.com/questions/5801813/c-usleep-is-obsolete-workarounds-for-windows-mingw */
void cwtUsleep(UINT32 usec)
{
    __int64 time1 = 0, time2 = 0, freq = 0;

    QueryPerformanceCounter((LARGE_INTEGER *) &time1);
    QueryPerformanceFrequency((LARGE_INTEGER *)&freq);

    do {
        QueryPerformanceCounter((LARGE_INTEGER *) &time2);
    } while((time2-time1) < waitTime);
}

#endif
