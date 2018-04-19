/*
 * unistd.c
 *
 *  Created on: 08.07.2012
 *      Author: wladt
 */


#include <cwt/unistd.h>

static ssize_t  cwt_read   (int fd, void *buffer, size_t count );
static ssize_t  cwt_write  (int fd, const void *buffer, size_t count );
extern BOOL     cwt_sleep  (ULONG sec);
extern BOOL     cwt_msleep (ULONG msec);
extern BOOL     cwt_usleep (ULONG usec);


static CwtUnistdNS __cwtUnistdNS = {
#ifdef CWT_UNICODE
	  CWT_ISO_CPP_NAME(wopen)
	, CWT_ISO_CPP_NAME(close)
	, cwt_read
	, cwt_write
	, CWT_ISO_CPP_NAME(lseek)
#ifdef CWT_CC_MSC
	, CWT_ISO_CPP_NAME(wsopen)
#endif
#else
	  CWT_ISO_CPP_NAME(open)
	, CWT_ISO_CPP_NAME(close)
	, cwt_read
	, cwt_write
	, CWT_ISO_CPP_NAME(lseek)
#ifdef CWT_CC_MSC
	, CWT_ISO_CPP_NAME(sopen)
#endif
#endif

	, cwt_sleep
	, cwt_msleep
	, cwt_usleep

};

DLL_API_EXPORT CwtUnistdNS* cwt_unistd_ns(void)
{
	return &__cwtUnistdNS;
}

static ssize_t  cwt_read (int fd, void *buffer, size_t count )
{
	CWT_ASSERT(sizeof(size_t) <= sizeof(UINT) && count <= UINT_MAX);
	return (ssize_t)CWT_ISO_CPP_NAME(read)(fd, buffer, (UINT)count);
}

static ssize_t  cwt_write(int fd, const void *buffer, size_t count )
{
	CWT_ASSERT(sizeof(size_t) <= sizeof(UINT) && count <= UINT_MAX);
	return (ssize_t)CWT_ISO_CPP_NAME(write)(fd, buffer, (UINT)count);

}
