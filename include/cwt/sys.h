/*
 * sys.h
 *
 *  Created on: Sep 4, 2012
 *      Author: wladt
 */

#ifndef __CWT_SYS_H__
#define __CWT_SYS_H__

#include <cwt/dl.h>

typedef struct _CwtSysNS {
	void (*parseDSN)(const CWT_CHAR *dsn, CWT_CHAR **scheme, CWT_CHAR **driver, CWT_CHAR **driverSN);
	BOOL (*loadNS)(const CWT_CHAR *dsn);
} CwtSysNS;


EXTERN_C_BEGIN
DLL_API_EXPORT CwtSysNS* cwtSysNS(void);
EXTERN_C_END



#endif /* __CWT_SYS_H__ */
