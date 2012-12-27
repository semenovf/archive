/*
 * win/serial.c
 *
 *  Created on: 27.06.2012
 *      Author: wladt
 */

#include <cwt/io/serial/serial.h>

CwtSerialRC serialport_errno(int portnum)
{
	CWT_UNUSED(portnum);
	return E_SP_GENERAL;
}


CwtSerialRC serialport_init(void)
{
	return E_SP_GENERAL;
}

void serialport_release(void)
{
}


CwtSerialRC serialport_set_settings(int portnum, UINT16 uart_base, char irq)
{
	CWT_UNUSED(portnum);
	CWT_UNUSED(uart_base);
	CWT_UNUSED(irq);
	return E_SP_GENERAL;
}

void serialport_settings(int portnum, UINT16 *uart_base, char *irq)
{
	CWT_UNUSED(portnum);
	CWT_UNUSED(uart_base);
	CWT_UNUSED(irq);
}

BOOL serialport_has_valid_settings(int portnum)
{
	CWT_UNUSED(portnum);
	return TRUE;
}


CwtSerialRC serialport_open(int portnum
		  , CwtSerialBitrate speed
		  , BOOL hispeed
		  , CwtSerialDB data_bits
		  , CwtSerialParity parity
		  , CwtSerialSB stop_bits)
{
	CWT_UNUSED(portnum);
	CWT_UNUSED(speed);
	CWT_UNUSED(hispeed);
	CWT_UNUSED(data_bits);
	CWT_UNUSED(parity);
	CWT_UNUSED(stop_bits);
	return E_SP_GENERAL;
}

void serialport_close(int portnum)
{
	CWT_UNUSED(portnum);
}


void serialport_set_parms(int portnum
	, CwtSerialBitrate speed
	, BOOL hispeed
	, CwtSerialDB data_bits
	, CwtSerialParity parity
	, CwtSerialSB stop_bits)
{
	CWT_UNUSED(portnum);
	CWT_UNUSED(speed);
	CWT_UNUSED(hispeed);
	CWT_UNUSED(data_bits);
	CWT_UNUSED(parity);
	CWT_UNUSED(stop_bits);
}


void serialport_parms(int portnum, CwtSerialBitrate *speed, BOOL *hispeed, CwtSerialDB *data_bits, CwtSerialParity *parity, CwtSerialSB *stop_bits)
{
	CWT_UNUSED(portnum);
	CWT_UNUSED(speed);
	CWT_UNUSED(hispeed);
	CWT_UNUSED(data_bits);
	CWT_UNUSED(parity);
	CWT_UNUSED(stop_bits);
}


ssize_t serialport_write(int portnum, const BYTE *buf, size_t count)
{
	CWT_UNUSED(portnum);
	CWT_UNUSED(buf);
	CWT_UNUSED(count);

	return (ssize_t)0;
}

ssize_t serialport_read(int portnum, CwtByteArray *ba, size_t count)
{
	CWT_UNUSED(portnum);
	CWT_UNUSED(ba);
	CWT_UNUSED(count);

	return (ssize_t)0;
}

ssize_t serialport_read_msr(int portnum, BYTE *buf, BYTE *msr_buf, size_t count)
{
	CWT_UNUSED(portnum);
	CWT_UNUSED(buf);
	CWT_UNUSED(msr_buf);
	CWT_UNUSED(count);
	return (ssize_t)0;
}

size_t  serialport_bytes_available(int portnum)
{
	CWT_UNUSED(portnum);
	return 0;
}

BOOL serialport_tx_ready(int portnum)
{
	CWT_UNUSED(portnum);
	return FALSE;
}

BOOL serialport_tx_empty(int portnum)
{
	CWT_UNUSED(portnum);
	return TRUE;
}

BOOL serialport_rx_empty(int portnum)
{
	CWT_UNUSED(portnum);
	return TRUE;
}

void serialport_flush_tx(int portnum)
{
	CWT_UNUSED(portnum);
}

void serialport_flush_rx(int portnum)
{
	CWT_UNUSED(portnum);
}

BOOL serialport_carrier_detected(int portnum)
{
	CWT_UNUSED(portnum);
	return FALSE;
}
