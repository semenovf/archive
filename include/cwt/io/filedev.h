/*
 * file.h
 *
 *  Created on: 18.11.2011
 *      Author: wladt
 */

#ifndef __CWT_IO_FILEDEV_H__
#define __CWT_IO_FILEDEV_H__

#include <cwt/io/channel.h>


typedef enum CwtOpenMode {
	  Cwt_OM_Read        = 0x01
	, Cwt_OM_Write       = 0x02
	, Cwt_OM_ReadWrite   = Cwt_OM_Read | Cwt_OM_Write
	, Cwt_OM_Create      = 0x04
	, Cwt_OM_NonBlocking = 0x80 /* only for CWT_CC_GNUC */
} CwtOpenMode;

EXTERN_C_BEGIN

DLL_API_EXPORT CwtIODevice* cwtFileDeviceOpen(const CWT_CHAR *path, CwtOpenMode mode);
#ifdef __SUPPORT_SHARING__
DLL_API_EXPORT CwtIODevice* cwtSharedFileDeviceOpen(const CWT_CHAR* infilename, const CWT_CHAR* outfilename, BOOL master);
#endif

EXTERN_C_END

#endif /* __CWT_IO_FILEDEV_H__ */
