/*
 * sys.h
 *
 *  Created on: Sep 4, 2012
 *      Author: wladt
 */

#ifndef __CWT_SYS_H__
#define __CWT_SYS_H__

#include <cwt/types.h>

typedef struct _CwtSysNS {
	int unused;
} CwtSysNS;


EXTERN_C_BEGIN
DLL_API_EXPORT CwtSysNS* cwtSysNS(void);
EXTERN_C_END



#endif /* __CWT_SYS_H__ */
