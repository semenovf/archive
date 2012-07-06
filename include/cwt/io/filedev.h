/*
 * file.h
 *
 *  Created on: 18.11.2011
 *      Author: wladt
 */

#ifndef __CWT_IO_FILEDEV_H__
#define __CWT_IO_FILEDEV_H__

#include <cwt/io/channel.h>

EXTERN_C_BEGIN

DLL_API_EXPORT CwtIODevicePtr cwtFileDeviceOpen(const CHAR *path, int mode);
DLL_API_EXPORT CwtIODevicePtr cwtSharedFileDeviceOpen(const char* infilename, const char* outfilename, BOOL master);

EXTERN_C_END

#endif /* __CWT_IO_FILEDEV_H__ */
