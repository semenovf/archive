/*
 * unistd.h
 *
 *  Created on: 26.06.2012
 *      Author: user
 */

#ifndef __CWT_UNISTD_H__
#define __CWT_UNISTD_H__

#include <cwt/global.h>

#ifdef __GNUC__
#	include <unistd.h>
#else
#	include <io.h>
#endif

EXTERN_C_BEGIN

#define cwtOpen   CWT_ISO_CPP_NAME(open)
#define cwtClose  CWT_ISO_CPP_NAME(close)
#define cwtRead(fd,buf,sz)   CWT_ISO_CPP_NAME(read)(fd,buf,(unsigned int)(sz)) /* TODO fix unsigned int cast */
#define cwtWrite  CWT_ISO_CPP_NAME(write)
#define cwtLseek  CWT_ISO_CPP_NAME(lseek)
#define cwtSopen  CWT_ISO_CPP_NAME(sopen)

#ifndef CWT_CC_MSC
#	define cwtCloseSocket CWT_ISO_CPP_NAME(close)
#else
#	define cwtCloseSocket closesocket
#endif


#if defined(CWT_OS_DOS)
#	include <dos.h>
#	define cwtSleep(sec)   sleep(sec)
#	define cwtMsleep(msec) delay(msec)
#	define cwtUsleep(usec) delay((usec)/1000) /* TODO ? */
#elif defined(CWT_CC_MSC)
#	define cwtSleep(sec)   Sleep((sec)*1000)
#	define cwtMsleep(msec) Sleep(msec)
DLL_API_EXPORT void cwtUsleep(UINT32 usec);
#elif defined(CWT_OS_UNIX)
#	define cwtSleep(sec)   sleep(sec)
#	define cwtMsleep(msec) usleep((msec)*1000)
#	define cwtUsleep(usec) usleep(usec)
#else
#	error "sleep family functions is not implemented"
#endif

EXTERN_C_END

#endif /* __CWT_UNISTD_H__ */
