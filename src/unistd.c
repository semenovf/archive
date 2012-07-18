/*
 * unistd.c
 *
 *  Created on: 08.07.2012
 *      Author: wladt
 */


#include <cwt/unistd.h>

static void __cwt_sleep(UINT sec);
static void __cwt_msleep(UINT msec);
static void __cwt_usleep(UINT usec);

static CwtUnistdNS __cwtUnistdNS = {
#ifdef CWT_UNICODE
	  CWT_ISO_CPP_NAME(wopen)
	, CWT_ISO_CPP_NAME(close)
	, CWT_ISO_CPP_NAME(read)
	, CWT_ISO_CPP_NAME(write)
	, CWT_ISO_CPP_NAME(lseek)
#ifdef __SUPPORT_SHARING__
	, CWT_ISO_CPP_NAME(wsopen)
#endif
#else
	  CWT_ISO_CPP_NAME(open)
	, CWT_ISO_CPP_NAME(close)
	, CWT_ISO_CPP_NAME(read)
	, CWT_ISO_CPP_NAME(write)
	, CWT_ISO_CPP_NAME(lseek)
#ifdef __SUPPORT_SHARING__
	, CWT_ISO_CPP_NAME(sopen)
#endif
#endif

	, __cwt_sleep
	, __cwt_msleep
	, __cwt_usleep

};

DLL_API_EXPORT CwtUnistdNS* cwtUnistdNS(void)
{
	return &__cwtUnistdNS;
}


#if defined(CWT_OS_DOS)

static void __cwt_sleep(UINT sec)  { sleep(sec); }
static void __cwt_msleep(UINT msec) {  delay(msec); }
static viod __cwt_usleep(UINT usec) { delay((usec)/1000); } /* TODO ? */

#elif defined(CWT_CC_MSC)

static void __cwt_sleep(UINT sec)   { Sleep((sec)*1000); }
static void __cwt_msleep(UINT msec) { Sleep(msec); }

/* Source: http://stackoverflow.com/questions/5801813/c-usleep-is-obsolete-workarounds-for-windows-mingw */
static void __cwt_usleep(UINT usec)
{
    __int64 time1 = 0, time2 = 0, freq = 0;

    QueryPerformanceCounter((LARGE_INTEGER *) &time1);
    QueryPerformanceFrequency((LARGE_INTEGER *)&freq);

    do {
        QueryPerformanceCounter((LARGE_INTEGER *) &time2);
    } while((time2-time1) < usec);
}


#elif defined(CWT_CC_GNUC)

static void __cwt_sleep(UINT sec)  { sleep(sec); }
static void __cwt_msleep(UINT msec) { usleep((msec)*1000); }
static void __cwt_usleep(UINT usec) { usleep(usec); }

#else

#	error "sleep family functions is not implemented"

#endif
