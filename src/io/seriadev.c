/*
 * filedev.c
 *
 *  Created on: 18.11.2011
 *      Author: user
 */

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <cwt/string.h>
#include <cwt/str.h>
#include <cwt/logger.h>
#include <cwt/io/serial/config.h>
#include <cwt/io/seriadev.h>


static void    __dev_close(CwtIODevice*);
static size_t  __dev_bytesAvailable(CwtIODevice*);
static ssize_t __dev_read(CwtIODevice*, BYTE*, size_t);
static ssize_t __dev_write(CwtIODevice*, const BYTE*, size_t);

typedef struct CwtSerialDevice
{
	CwtIODevice __base;
	int portnum;
} CwtSerialDevice;


int cwtSerialDevicePort(CwtIODevice *dev)
{
	CwtSerialDevice *spd = (CwtSerialDevice*)dev;
	CWT_ASSERT(dev);

	return spd->portnum;
}

/**
 * Open serial device
 * port=5 speed=9600 parity=none stop=1 uart_base=0xd800 irq=0x0A
 */
CwtIODevice* cwtSerialDeviceOpen(CWT_CHAR *attrs[], int nattrs)
{
	CwtStrNS *strNS = cwtStrNS();
	CwtSerialDevice *spd;
	int i;
	BOOL is_error = FALSE;

	CwtSerialRC rc;
	int portnum = -1;
	CwtSerialBitrate speed = (CwtSerialBitrate)9600;
	CwtSerialParity parity = SP_ParityNone;
	CwtSerialDB db = SP_DB8;
	CwtSerialSB sb = SP_SB1;
	UINT uart_base = 0;
	UINT irq = 0;
	BOOL hispeed = FALSE;

	for( i = 0; i < nattrs; i++ ) {
		BOOL ok;
		if( strNS->strncmp(_T("port="), attrs[i], 5) == 0 ) {
			portnum = strNS->toUINT(attrs[i]+5, 0, &ok);
			if( !ok || portnum < 1 || portnum > SERIALPORT_MAX ) {
				printf_error(_T("bad serial port number or it is out of bounds (must be between 1 and %d)")
					, SERIALPORT_MAX);
				is_error = TRUE;
			}
		} else if( strNS->strncmp(_T("speed="), attrs[i], 6) == 0 ) {
			speed = strNS->toLONG(attrs[i]+6, 0, &ok);
			if( !ok ) {
				print_error(_Tr("bad serial port speed"));
				is_error = TRUE;
			}
		} else if( strNS->strncmp(_T("parity="), attrs[i], 7) == 0 ) {
			if( strNS->streq(_T("none"), attrs[i] + 7) ) {
				parity = SP_ParityNone;
			} else if( strNS->streq(_T("odd"), attrs[i] + 7) ) {
				parity = SP_ParityOdd;
			} else if( strNS->streq(_T("even"), attrs[i] + 7) ) {
				parity = SP_ParityEven;
			} else {
				print_error(_Tr("bad serial port parity"));
				is_error = TRUE;
			}
		} else if( strNS->strncmp(_T("data="), attrs[i], 5) == 0 ) {
			if( strNS->streq(_T("5"), attrs[i] + 5) ) {
				db = SP_DB5;
			} else if( strNS->streq(_T("6"), attrs[i] + 5) ) {
				db = SP_DB6;
			} else if( strNS->streq(_T("7"), attrs[i] + 5) ) {
				db = SP_DB7;
			} else if( strNS->streq(_T("8"), attrs[i] + 5) ) {
				db = SP_DB8;
			} else {
				print_error(_Tr("bad serial port stop bits"));
				is_error = TRUE;
			}
		} else if( strNS->strncmp(_T("stop="), attrs[i], 5) == 0 ) {
			if( strNS->streq(_T("1"), attrs[i] + 5) ) {
				sb = SP_SB1;
			} else if( strNS->streq(_T("2"), attrs[i] + 5) ) {
				sb = SP_SB2;
			} else {
				print_error(_Tr("bad serial port stop bits"));
				is_error = TRUE;
			}
		} else if( strNS->strncmp(_T("uart_base="), attrs[i], 10) == 0 ) {
			uart_base = strNS->toUINT(attrs[i] + 10, 0, &ok);
			if( !ok ) {
				print_error(_Tr("bad serial port uart base address"));
				is_error = TRUE;
			}
		} else if( strNS->strncmp(_T("irq="), attrs[i], 4) == 0 ) {
			irq = strNS->toUINT(attrs[i] + 4, 0, &ok);
			if( !ok || irq < 1 || irq > 15 ) {
				print_error(_Tr("bad serial port IRQ"));
				is_error = TRUE;
			}
		} else if( strNS->streq(_T("hispeed"), attrs[i]) ) {
			hispeed = TRUE;
		} else {
			printf_warn(_Tr("%s: invalid serial port attribute, ignored")
				, CWT_STRING_OR_NULLSTR(attrs[i]));
		}
	}

	if( is_error )
		return (CwtIODevice*)NULL;

	if( portnum < 0) {
		print_error(_Tr("serial port number must be specified"));
		return (CwtIODevice*)NULL;
	}

	if( !uart_base ) {
		switch(portnum) {
			case 1: uart_base = 0x3F8;
				break;
			case 2: uart_base = 0x2F8;
				break;
			case 3: uart_base = 0x3E8;
				break;
			case 4: uart_base = 0x2E8;
				break;
			default:
				print_error(_Tr("serial port uart base address must be specified"));
				return (CwtIODevice*)NULL;
		}
	}

	if( !irq ) {
		switch(portnum) {
			case 1:
			case 3: irq = 4/*0x0C*/; break;
			case 2:
			case 4: irq = 3/*0x0B*/; break;
			default:
				print_error(_Tr("serial port IRQ must be specified"));
				return (CwtIODevice*)NULL;
		}
	}

	rc = serialport_set_settings(portnum, uart_base, irq);

	if( rc != E_SP_NOERROR ) {
/*		serialport_close(portnum);*/
		printf_error(_Tr("serialport_set_settings: %s"), serialport_strerror(rc));
		return (CwtIODevice*)NULL;
	}

	rc = serialport_open(portnum, speed, hispeed, db, parity, sb);

	if( rc != E_SP_NOERROR ) {
		printf_error(_Tr("serialport_open: %s"), serialport_strerror(rc));
		return (CwtIODevice*)NULL;
	}

	serialport_dump(portnum);

/*
	printf_debug("serial device: port=%d; speed=%ld; parity=%s; stop=%d; uart_base=0x%04X; irq=%d"
		, portnum
		, speed
		, parity == SP_ParityNone ? "none" : parity == SP_ParityOdd ? "odd" : "even"
		, sb
		, uart_base
		, irq);
*/

	spd = CWT_MALLOC(CwtSerialDevice);

	spd->__base.close          = __dev_close;
	spd->__base.bytesAvailable = __dev_bytesAvailable;
	spd->__base.read           = __dev_read;
	spd->__base.write          = __dev_write;

	spd->portnum = portnum;
	return (CwtIODevice*)spd;
}


void __dev_close(CwtIODevice *dev)
{
	CwtSerialDevice *spd = (CwtSerialDevice*)dev;
	CWT_ASSERT(dev);
	cwtSerialClose(spd->portnum);
	CWT_FREE(spd);
}

size_t __dev_bytesAvailable(CwtIODevice *dev)
{
	CWT_ASSERT(dev);
	return cwtSerialBytesAvailable(((CwtSerialDevice*)dev)->portnum);
}

ssize_t __dev_read(CwtIODevice *dev, BYTE* buf, size_t sz)
{
	CWT_ASSERT(dev);
	return cwtSerialRead(((CwtSerialDevice*)dev)->portnum, buf, sz);
}

ssize_t __dev_write(CwtIODevice *dev, const BYTE* buf, size_t sz)
{
	CWT_ASSERT(dev);
	return cwtSerialWrite(((CwtSerialDevice*)dev)->portnum, buf, sz);
}
