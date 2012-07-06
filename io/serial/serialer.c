/*
 * serialer.c
 *
 *  Created on: 13.04.2012
 *      Author: user
 */


#include <cwt/io/serial/serial.h>

static const char* __serialport_error_strings[E_SP_COUNT] = {
	  "no error"
	, "serial port environment is not initialized properly"
	, "serial port is not opened"
	, "out of bounds"
	, "already opened"
	, "bad attributes"
	, "not enough memory"
	, "can not access the UART"
	, "failed to install interrupt vector"
	, "overflow of I/O buffer"
};

static const char* __parity_strings[3] = {
	  "none"
	, "even"
	, "odd"
};


const char* serialport_strerror(CwtSerialRC rc)
{
	CWT_ASSERT( rc >= 0 && rc < E_SP_COUNT );
	return __serialport_error_strings[rc];
}

const char* serialport_parity_str(CwtSerialParity parity)
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

	printf("COM%d: irq=%d; uart_base=0x%04X; speed=%ld; hispeed=%c; db=%d; sb=%d; parity=%s\n"
		, portnum
		, irq
		, uart_base
		, speed
		, hispeed ? 'Y' : 'N'
		, db
		, sb
		, serialport_parity_str(parity));
}
