/*
 * fs.h
 *
 *  Created on: 22.08.2012
 *      Author: user
 */

#ifndef __CWT_FS_H__
#define __CWT_FS_H__

#include <cwt/types.h>

typedef struct _CwtFileSystemNS {
	CWT_CHAR       (*separator)(void);
	CWT_CHAR*      (*buildPath)(const CWT_CHAR *arg0, ...);
	CWT_CHAR*      (*buildPathArgv)(const CWT_CHAR *argv[], size_t n);
	BOOL           (*unlink)(const CWT_CHAR *pathname);
} CwtFileSystemNS;


EXTERN_C_BEGIN
DLL_API_EXPORT CwtFileSystemNS* cwtFileSystemNS(void);
EXTERN_C_END


#endif /* __CWT_FS_H__ */
