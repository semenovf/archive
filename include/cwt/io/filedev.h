/*
 * file.h
 *
 *  Created on: 18.11.2011
 *      Author: wladt
 */

#ifndef __CWT_IO_FILEDEV_H__
#define __CWT_IO_FILEDEV_H__

#include <cwt/io/channel.h>


typedef enum CwtOpenFileMode {
	  Cwt_FileRead        = 0x01
	, Cwt_FileWrite       = 0x02
	, Cwt_FileReadWrite   = Cwt_FileRead | Cwt_FileWrite
	, Cwt_FileCreate      = 0x04
	, Cwt_FileNonBlocking = 0x80 /* only for CWT_CC_GNUC */
} CwtOpenMode;

EXTERN_C_BEGIN

DLL_API_EXPORT CwtIODevice* cwtFileDeviceOpen(const CWT_CHAR *path, CwtOpenMode mode);
#ifdef __SUPPORT_SHARING__
DLL_API_EXPORT CwtIODevice* cwtSharedFileDeviceOpen(const CWT_CHAR* infilename, const CWT_CHAR* outfilename, BOOL master);
#endif

EXTERN_C_END

#endif /* __CWT_IO_FILEDEV_H__ */
