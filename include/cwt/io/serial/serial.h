#ifndef __CWT_IO_SERIAL_H__
#define __CWT_IO_SERIAL_H__

#include <stdio.h>
#include <cwt/global.h>
#include <cwt/types.h>

EXTERN_C_BEGIN

typedef enum CwtSerialRC {
	  E_SP_NOERROR
	, E_SP_GENERAL  /* serial port environment is not initialized properly */
	, E_SP_INVALID  /* serial port is not opened */
	, E_SP_BOUNDS   /* out of bounds (e.g., portnum) */
	, E_SP_OPENED   /* serial port already opened */
	, E_SP_BADATTS  /* serial port's attributes is bad */
	, E_SP_NOMEM    /* not enough memory */
	, E_SP_UART     /* can't access the UART */
	, E_SP_SETVECT  /* failed to install interrupt vector */
	, E_SP_OVERFLOW /* overflow of input/output buffer */
	, E_SP_COUNT
} CwtSerialRC;

typedef long CwtSerialBitrate;

typedef enum CwtSerialDB {
	  SP_DB5 = 5
	, SP_DB6
	, SP_DB7
	, SP_DB8
} CwtSerialDB;


typedef enum CwtSerialSB {
	  SP_SB1 = 1
	, SP_SB2
} CwtSerialSB;

typedef enum CwtSerialParity {
	  SP_ParityNone /* no parity */
	, SP_ParityEven /* even parity */
	, SP_ParityOdd   /* odd parity */
	, SP_ParityHigh
	, SP_ParityLow
} CwtSerialParity;

#define cwtSerialStrerror         serialport_strerror
#define cwtSerialParityStr        serialport_parity_str
#define cwtSerialDump             serialport_dump
#define cwtSerialInit             serialport_init
#define cwtSerialRelease          serialport_release
#define cwtSerialSetSettings      serialport_set_settings
#define cwtSerialSettings         serialport_settings
#define cwtSerialHasValidSettings serialport_has_valid_settings
#define cwtSerialOpen             serialport_open
#define cwtSerialClose            serialport_close
#define cwtSerialErrno            serialport_errno
#define cwtSerialSetParms         serialport_set_parms
#define cwtSerialParms            serialport_parms
#define cwtSerialWrite            serialport_write
#define cwtSerialRead             serialport_read
#define cwtSerialReadMsr          serialport_read_msr
#define cwtSerialBytesAvailable   serialport_bytes_available
#define cwtSerialTxReady          serialport_tx_ready
#define cwtSerialTxEmpty          serialport_tx_empty
#define cwtSerialRxEmpty          serialport_rx_empty
#define cwtSerialFlushTx          serialport_flush_tx
#define cwtSerialFlushRx          serialport_flush_rx
#define cwtSerialCarrierDetected  serialport_carrier_detected

DLL_API_EXPORT const CHAR* serialport_strerror(CwtSerialRC rc);
DLL_API_EXPORT const CHAR* serialport_parity_str(CwtSerialParity parity);
DLL_API_EXPORT void serialport_dump(int portnum);
DLL_API_EXPORT CwtSerialRC serialport_init(void);
DLL_API_EXPORT void serialport_release(void);
DLL_API_EXPORT CwtSerialRC serialport_set_settings(int portnum, UINT16 uart_base, char irq);
DLL_API_EXPORT void serialport_settings(int portnum, UINT16 *uart_base, char *irq);
DLL_API_EXPORT BOOL serialport_has_valid_settings(int portnum);

DLL_API_EXPORT CwtSerialRC serialport_open(
		int portnum
	  , CwtSerialBitrate speed
	  , BOOL hispeed
	  , CwtSerialDB db
	  , CwtSerialParity parity
	  , CwtSerialSB sb); /* TRUE if port support max speed 921600L, FALSE if max speed is 115200L */

DLL_API_EXPORT void serialport_close(int portnum);
DLL_API_EXPORT CwtSerialRC serialport_errno(int portnum);
DLL_API_EXPORT void serialport_set_parms(int portnum, CwtSerialBitrate speed, BOOL hispeed, CwtSerialDB db, CwtSerialParity parity, CwtSerialSB stop_bits);
DLL_API_EXPORT void serialport_parms(int portnum, CwtSerialBitrate *speed, BOOL *hispeed, CwtSerialDB *db, CwtSerialParity *parity, CwtSerialSB *stop_bits);

DLL_API_EXPORT ssize_t serialport_write(int portnum, const BYTE *buf, size_t count);
DLL_API_EXPORT ssize_t serialport_read(int portnum, BYTE *buf, size_t count);
DLL_API_EXPORT ssize_t serialport_read_msr(int portnum, BYTE *buf, BYTE *msr_buf, size_t count);
DLL_API_EXPORT size_t  serialport_bytes_available(int portnum);

DLL_API_EXPORT BOOL serialport_tx_ready(int portnum);
DLL_API_EXPORT BOOL serialport_tx_empty(int portnum);
DLL_API_EXPORT BOOL serialport_rx_empty(int portnum);
DLL_API_EXPORT void serialport_flush_tx(int portnum);
DLL_API_EXPORT void serialport_flush_rx(int portnum);
DLL_API_EXPORT BOOL serialport_carrier_detected(int portnum);

/**
serialport_init();        // optional

if(  serialport_has_valid_settings() ) { // optional
...
}
serialport_set_settings() // optional
...

serialport_open();        // mandatory

// serial port operations

serialport_close();       // optional, closed by serialport_release() if it wasn't closed before

serialport_release();     // mandatory
*/

EXTERN_C_END

#endif /* __CWT_IO_SERIAL_H__ */

