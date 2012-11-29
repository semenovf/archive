/*
 * unistd.c
 *
 *  Created on: 08.07.2012
 *      Author: wladt
 */


#include <cwt/unistd.h>


extern int cwt_usleep(unsigned int usec);

static ssize_t  __read (int fd, void *buffer, size_t count );
static ssize_t  __write(int fd, const void *buffer, size_t count );
static void     __cwt_sleep(UINT sec);
static void     __cwt_msleep(UINT msec);
static void     __cwt_usleep(UINT usec);


static CwtUnistdNS __cwtUnistdNS = {
#ifdef CWT_UNICODE
	  CWT_ISO_CPP_NAME(wopen)
	, CWT_ISO_CPP_NAME(close)
	, __read
	, __write
	, CWT_ISO_CPP_NAME(lseek)
#ifdef CWT_CC_MSC
	, CWT_ISO_CPP_NAME(wsopen)
#endif
#else
	  CWT_ISO_CPP_NAME(open)
	, CWT_ISO_CPP_NAME(close)
	, __read
	, __write
	, CWT_ISO_CPP_NAME(lseek)
#ifdef CWT_CC_MSC
	, CWT_ISO_CPP_NAME(sopen)
#endif
#endif

	, __cwt_sleep
	, __cwt_msleep
	, __cwt_usleep

};

DLL_API_EXPORT CwtUnistdNS* cwt_unistd_ns(void)
{
	return &__cwtUnistdNS;
}

static ssize_t  __read (int fd, void *buffer, size_t count )
{
	CWT_ASSERT(sizeof(size_t) <= sizeof(UINT) && count <= UINT_MAX);
	return (ssize_t)CWT_ISO_CPP_NAME(read)(fd, buffer, (UINT)count);
}

static ssize_t  __write(int fd, const void *buffer, size_t count )
{
	CWT_ASSERT(sizeof(size_t) <= sizeof(UINT) && count <= UINT_MAX);
	return (ssize_t)CWT_ISO_CPP_NAME(write)(fd, buffer, (UINT)count);

}


#if defined(CWT_OS_MSDOS)

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

static void __cwt_sleep(UINT sec)   { sleep(sec); }
static void __cwt_msleep(UINT msec) { cwt_usleep((msec)*1000); }
static void __cwt_usleep(UINT usec) { cwt_usleep(usec); }

#else

#	error "sleep family functions is not implemented"

#endif
