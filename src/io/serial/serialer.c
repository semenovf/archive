/*
 * serialer.c
 *
 *  Created on: 13.04.2012
 *      Author: user
 */


#include <cwt/io/serial/serial.h>
#include <cwt/stdio.h>

static const CWT_CHAR* __serialport_error_strings[E_SP_COUNT] = {
	  _Tr("no error")
	, _Tr("serial port environment is not initialized properly")
	, _Tr("serial port is not opened")
	, _Tr("out of bounds")
	, _Tr("already opened")
	, _Tr("bad attributes")
	, _Tr("not enough memory")
	, _Tr("can not access the UART")
	, _Tr("failed to install interrupt vector")
	, _Tr("overflow of I/O buffer")
};

static const CWT_CHAR* __parity_strings[3] = {
	  _Tr("none")
	, _Tr("even")
	, _Tr("odd")
};


const CWT_CHAR* serialport_strerror(CwtSerialRC rc)
{
	CWT_ASSERT( rc >= 0 && rc < E_SP_COUNT );
	return __serialport_error_strings[rc];
}

const CWT_CHAR* serialport_parity_str(CwtSerialParity parity)
{
	return __parity_strings[parity];
}


void serialport_dump(int portnum)
{
	char irq;
	UINT16 uart_base;
	long speed;
	CwtSerialParity parity;
	CwtSerialDB db;
	CwtSerialSB sb;
	BOOL hispeed;

/*
	static const char* __chipset_names[] = {
		  "<unknown>"
		, "CHIP8250"
		, "CHIP16450"
		, "CHIP16550"
		, "CHIP16550A"
		, "NOCHIP"
	};
*/

	serialport_settings(portnum, &uart_base, &irq);
	serialport_parms(portnum, &speed, &hispeed, &db, &parity, &sb);

	cwtStdioNS()->printf(_Tr("COM%d: irq=%d; uart_base=0x%04X; speed=%ld; hispeed=%c; db=%d; sb=%d; parity=%s\n")
		, portnum
		, irq
		, uart_base
		, speed
		, hispeed ? _T('Y') : _T('N')
		, db
		, sb
		, serialport_parity_str(parity));
}
