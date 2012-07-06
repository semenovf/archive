/*
 * inet.h
 *
 *  Created on: 04.07.2012
 *      Author: wladt
 */

#ifndef __CWT_NET_INET_H__
#define __CWT_NET_INET_H__

#include <cwt/types.h>

typedef struct CwtInetAddressIterator {
	int nothing;
} CwtInetAddressIterator;

EXTERN_C_BEGIN

DLL_API_EXPORT void cwtInetAddressBegin(CwtInetAddressIterator* it);
DLL_API_EXPORT BOOL cwtInetAddressHasMore(CwtInetAddressIterator* it);
DLL_API_EXPORT BOOL cwtInetAddressNext(CwtInetAddressIterator* it);

EXTERN_C_END


#endif /* __CWT_NET_INET_H__ */
