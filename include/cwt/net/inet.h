/*
 * inet.h
 *
 *  Created on: 04.07.2012
 *      Author: wladt
 */

#ifndef __CWT_NET_INET_H__
#define __CWT_NET_INET_H__

#include <cwt/types.h>

typedef struct CwtInetAddrIterator {
	int nothing;
} CwtInetAddrIterator;

typedef struct _CwtInetNS {
	void (*addrBegin)   (CwtInetAddrIterator *it);
	BOOL (*addrHasMore) (CwtInetAddrIterator *it);
	BOOL (*addrNext)    (CwtInetAddrIterator *it);
} CwtInetNS;


EXTERN_C_BEGIN
DLL_API_EXPORT CwtInetNS* cwtInetNS(void);
EXTERN_C_END


#endif /* __CWT_NET_INET_H__ */
