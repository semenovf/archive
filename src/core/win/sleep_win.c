/*
 * unistd.c
 *
 *  Created on: 08.07.2012
 *      Author: wladt
 */


#include <cwt/unistd.h>

void cwt_sleep(ULONG sec)
{
	Sleep((sec)*1000);
}

void cwt_msleep(ULONG msec)
{
	Sleep(msec);
}

/* Source: http://stackoverflow.com/questions/5801813/c-usleep-is-obsolete-workarounds-for-windows-mingw */
void cwt_usleep(ULONG usec)
{
    __int64 time1 = 0, time2 = 0, freq = 0;

    QueryPerformanceCounter((LARGE_INTEGER *) &time1);
    QueryPerformanceFrequency((LARGE_INTEGER *)&freq);

    do {
        QueryPerformanceCounter((LARGE_INTEGER *) &time2);
    } while((time2-time1) < usec);
}
