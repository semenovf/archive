/*
 * sockdev.c
 *
 *  Created on: 18.11.2011
 *      Author: user
 */

#include <cwt/io/sockdev.h>
#include <cwt/net/socket.h>

static void    __dev_close(CwtIODevice *dev);
static size_t  __dev_bytesAvailable(CwtIODevice *dev);
static ssize_t __dev_read(CwtIODevice *dev, BYTE* buf, size_t sz);
static ssize_t __dev_write(CwtIODevice *dev, const BYTE* buf, size_t sz);

typedef struct CwtSocketDevice {
	CwtIODevice   __base;
	struct _CwtSocket *sockfd;
} CwtSocketDevice;



static CwtIODevice* __dev_createSocketDevice (struct _CwtSocket *sd)
{
	CwtSocketNS *socketNS = cwtSocketNS();
	CwtSocketDevice *sdev = NULL;

	if( !sd )
		return NULL;

	sdev = CWT_MALLOC(CwtSocketDevice);

	sdev->__base.close          = __dev_close;
	sdev->__base.bytesAvailable = __dev_bytesAvailable;
	sdev->__base.read           = __dev_read;
	sdev->__base.write          = __dev_write;
	sdev->sockfd                = sd;

	return (CwtIODevice*)sdev;
}


static CwtIODevice* __dev_UdpTcpSocketDeviceOpen(struct _CwtSocket *sd, const CWT_CHAR *inetAddr, UINT16 port, BOOL is_listener)
{
	BOOL ok;

	if (!sd)
		return NULL;

	if( is_listener )
		ok = cwtSocketNS()->listen(sd, inetAddr, port);
	else
		ok = cwtSocketNS()->connect(sd, inetAddr, port);

	if (ok)
		return __dev_createSocketDevice(sd);

	return NULL;
}

/**
 * @fn CwtIODevice* cwtUdpSocketDeviceOpen(const CWT_CHAR *inetAddr, UINT16 port, BOOL is_listener)
 *
 * @brief Opens UDP socket device.
 *
 * @details Opens UDP socket device in non-blocking mode.
 *
 * @param inetAddr
 * @param port
 * @param is_listener
 * @return
 */
DLL_API_EXPORT CwtIODevice* cwtUdpSocketDeviceOpen(const CWT_CHAR *inetAddr, UINT16 port, BOOL is_listener)
{
	return __dev_UdpTcpSocketDeviceOpen(cwtSocketNS()->openUdpSocket(TRUE)
			, inetAddr
			, port
			, is_listener);
}

/**
 * @fn CwtIODevice* cwtTcpSocketDeviceOpen(const CWT_CHAR *inetAddr, UINT16 port, BOOL is_listener)
 *
 * @brief Opens TCP socket device.
 *
 * @details Opens TCP socket device in non-blocking mode.
 *
 * @param inetAddr
 * @param port
 * @param is_listener
 * @return
 */
DLL_API_EXPORT CwtIODevice* cwtTcpSocketDeviceOpen(const CWT_CHAR *inetAddr, UINT16 port, BOOL is_listener)
{
	return __dev_UdpTcpSocketDeviceOpen(cwtSocketNS()->openTcpSocket(TRUE)
			, inetAddr
			, port
			, is_listener);
}


/**
 * @fn CwtIODevice* cwtMSocketDeviceOpen(const CWT_CHAR *inetAddr, UINT16 port, const CWT_CHAR *inetMCastAddr, BOOL is_listener)
 *
 * @brief Opens Multicast (UDP) socket device.
 *
 * @details Opens Multicast (UDP) socket device in non-blocking mode.
 *
 * @param inetAddr
 * @param port
 * @param inetMCastAddr
 * @param is_listener
 * @return
 */
DLL_API_EXPORT CwtIODevice* cwtMSocketDeviceOpen(const CWT_CHAR *inetAddr, UINT16 port, const CWT_CHAR *inetMCastAddr, BOOL is_listener)
{
	CwtSocketNS *socketNS = cwtSocketNS();
	struct _CwtSocket *sd;
	BOOL ok;

	sd = cwtSocketNS()->openMSocket(TRUE);

	if (!sd)
		return NULL;

	if( is_listener )
		ok = socketNS->listenMSocket(sd, inetAddr, port, inetMCastAddr);
	else
		ok = socketNS->connectMSocket(sd, inetAddr, port, inetMCastAddr);

	if (ok)
		return __dev_createSocketDevice(sd);

	return NULL;
}


static void __dev_close(CwtIODevice *dev)
{
	CwtSocketDevice *sd = (CwtSocketDevice*)dev;
	if (sd) {
		cwtSocketNS()->close(sd->sockfd);
		CWT_FREE(sd);
	}
}

static size_t __dev_bytesAvailable(CwtIODevice *dev)
{
	CwtSocketDevice *sd = (CwtSocketDevice*)dev;
	CWT_ASSERT(sd);
	return cwtSocketNS()->bytesAvailable(sd->sockfd);
}

static ssize_t __dev_read(CwtIODevice *dev, BYTE *buf, size_t sz)
{
	CwtSocketDevice *sd = (CwtSocketDevice*)dev;
	CWT_ASSERT(sd);
	return cwtSocketNS()->read(sd->sockfd, buf, sz);
}

static ssize_t __dev_write(CwtIODevice *dev, const BYTE *buf, size_t sz)
{
	CwtSocketDevice *sd = (CwtSocketDevice*)dev;
	CWT_ASSERT(sd);
	return cwtSocketNS()->write(sd->sockfd, buf, sz);
}
