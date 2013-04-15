/**
 * @file   system.h
 * @author wladt
 * @date   Apr 11, 2013 11:35:21 AM
 *
 * @brief
 */

#ifndef __CWT_SYSTEM_H__
#define __CWT_SYSTEM_H__

#include <cwt/cwt.h>

EXTERN_C_BEGIN

DLL_API char* cwt_strerror(int errn, char *buf, size_t buflen);

EXTERN_C_END

#endif /* __CWT_SYSTEM_H__ */
