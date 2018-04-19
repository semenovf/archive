/*
 * seriadev.h
 *
 *  Created on: 18.11.2011
 *      Author: wladt
 */

#ifndef __CWT_IO_SERIADEV_H__
#define __CWT_IO_SERIADEV_H__

#include <cwt/io/serial/serial.h>
#include <cwt/io/channel.h>

EXTERN_C_BEGIN

DLL_API_EXPORT CwtIODevice* cwtSerialDeviceOpen(CWT_CHAR *attrs[], int nattrs);
DLL_API_EXPORT int          cwtSerialDevicePort(CwtIODevice *pdev);

EXTERN_C_END

#endif /* __CWT_IO_SERIADEV_H__ */
