/*
 * inet.h
 *
 *  Created on: 04.07.2012
 *      Author: wladt
 *
 *
 * IPv4 internet protocol subsystem
 */

#ifndef __CWT_NET_INET_H__
#define __CWT_NET_INET_H__

#include <cwt/types.h>


struct _CwtInetAddrIterator;

typedef struct _CwtInetNS {
	void   (*addrBegin)   (struct _CwtInetAddrIterator *it);
	BOOL   (*addrHasMore) (struct _CwtInetAddrIterator *it);
	BOOL   (*addrNext)    (struct _CwtInetAddrIterator *it);
} CwtInetNS;


EXTERN_C_BEGIN
DLL_API_EXPORT CwtInetNS* cwtInetNS(void);
EXTERN_C_END


#endif /* __CWT_NET_INET_H__ */
