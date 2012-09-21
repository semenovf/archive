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


#ifdef __OBSOLETE__
static CwtIODevice* __dev_createSocketDevice (struct _CwtSocket *sd)
{
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
#endif

/**
 * @fn CwtIODevice* cwtSocketDeviceOpen(CwtSocket *sockfd)
 *
 * @brief Opens socket device.
 *
 * @param sockfd Socket descriptor.
 * @return Socket device associated with socket descriptor.
 */
DLL_API_EXPORT CwtIODevice* cwtSocketDeviceOpen(CwtSocket *sockfd)
{
	CwtSocketDevice *sdev = NULL;

	if( !sockfd )
		return NULL;

	sdev = CWT_MALLOC(CwtSocketDevice);

	sdev->__base.close          = __dev_close;
	sdev->__base.bytesAvailable = __dev_bytesAvailable;
	sdev->__base.read           = __dev_read;
	sdev->__base.write          = __dev_write;
	sdev->sockfd                = sockfd;

	return (CwtIODevice*)sdev;
}

#ifdef __OBSOLETE__
DLL_API_EXPORT CwtIODevice* cwtLocalSocketDeviceOpen(const CWT_CHAR *path, UINT32 flags)
{
	CwtSocket *sd;
	if( CWT_CAST_BOOL(flags & Cwt_SocketDevice_Listener) ) {
		sd = cwtSocketNS()->openLocalServerSocket(
				path
				, CWT_CAST_BOOL(flags & Cwt_SocketDevice_NonBlocking));
	} else {
		sd = cwtSocketNS()->openLocalSocket(
				path
				, CWT_CAST_BOOL(flags & Cwt_SocketDevice_NonBlocking));
	}

	return sd ? __dev_createSocketDevice(sd) : NULL;
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
DLL_API_EXPORT CwtIODevice* cwtUdpSocketDeviceOpen(const CWT_CHAR *inetAddr, UINT16 port, UINT32 flags)
{
	CwtSocket *sd;
	if( CWT_CAST_BOOL(flags & Cwt_SocketDevice_Listener) ) {
		sd = cwtSocketNS()->openUdpServerSocket(
				inetAddr
				, port
				, CWT_CAST_BOOL(flags & Cwt_SocketDevice_NonBlocking));
	} else {
		sd = cwtSocketNS()->openUdpSocket(
				inetAddr
				, port
				, CWT_CAST_BOOL(flags & Cwt_SocketDevice_NonBlocking));
	}

	return sd ? __dev_createSocketDevice(sd) : NULL;
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
DLL_API_EXPORT CwtIODevice* cwtTcpSocketDeviceOpen (const CWT_CHAR *inetAddr, UINT16 port, UINT32 flags)
{
	CwtSocket *sd;
	if( CWT_CAST_BOOL(flags & Cwt_SocketDevice_Listener) ) {
		sd = cwtSocketNS()->openTcpServerSocket(
				inetAddr
				, port
				, CWT_CAST_BOOL(flags & Cwt_SocketDevice_NonBlocking));
	} else {
		sd = cwtSocketNS()->openTcpSocket(
				inetAddr
				, port
				, CWT_CAST_BOOL(flags & Cwt_SocketDevice_NonBlocking));
	}

	return sd ? __dev_createSocketDevice(sd) : NULL;
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
DLL_API_EXPORT CwtIODevice* cwtMcastSocketDeviceOpen(const CWT_CHAR *inetAddr, UINT16 port, const CWT_CHAR *inetMcastAddr, UINT32 flags)
{
	CwtSocket *sd;
	if( CWT_CAST_BOOL(flags & Cwt_SocketDevice_Listener) ) {
		sd = cwtSocketNS()->openMcastServerSocket(
				inetAddr
				, port
				, inetMcastAddr
				, CWT_CAST_BOOL(flags & Cwt_SocketDevice_NonBlocking));
	} else {
		sd = cwtSocketNS()->openMcastSocket(
				inetAddr
				, port
				, inetMcastAddr
				, CWT_CAST_BOOL(flags & Cwt_SocketDevice_NonBlocking));
	}

	return sd ? __dev_createSocketDevice(sd) : NULL;
}
#endif

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
