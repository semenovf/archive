/*
 * serial.c
 *
 *  Created on: 13.04.2012
 *      Author: user
 */


#include <cwt/io/serial/serial.h>
#include <cwt/string.h>
#include <cwt/ringbuf.h>
#include <cwt/logger.h>
#include <cwt/io/serial/dos/pmcom/com.h>
#include <pmcom/irq.h>



/* As 'portnum' do not use values defined by COMxx macros in pmcom.h:
 * for COM1 use 1 , for COM2 - 2 and etc
 *
 * */

static BOOL __serialport_init = FALSE;


static struct _serialport_context {
	RingBufferPtr in;
	RingBufferPtr msr;
	RingBufferPtr out;
	void (*event_handler)(int nevent);
	int  err;
} __SP_CONTEXT[COMMAX];


#define _OUTBUF_SZ 256
static void _serialport_write_bytes(int portnum)
{
	int portindex = portnum-1;
	BYTE buf[_OUTBUF_SZ+1];
	int sz = 0;

	JQ_ASSERT(portnum > 0);

	if( rb_size(__SP_CONTEXT[portnum-1].out) == 0 )
		return;
/*
	if( COMTXBufferUsed(portnum-1) != 0 )
		return;
*/

	/* TODO REMOVE THIS */
	memset(buf, 0, _OUTBUF_SZ+1);

	sz = COMTXBufferFree(portindex); /* free space in output queue */

	if( sz == 0 )
		return;

	sz = JQ_MIN(sz, _OUTBUF_SZ);     /* how many characters we can send at once */
	sz = rb_read(__SP_CONTEXT[portnum-1].out, buf, sz); /* how many characters we can send at once actually */

	if( sz > 0) {
		int count = 0;
		rb_pop_front(__SP_CONTEXT[portindex].out, sz);
		/*printf("COM%d: %d chars written: %s\n", portnum, sz, buf);*/

		if( COMERR_TXOVERFLOW == COMWriteBuffer(portindex, (char*)buf, NULL, sz, &count) ) {
			__SP_CONTEXT[portindex].err = E_SP_OVERFLOW;
			return;
		}
	}
}
END_OF_FUNCTION(_serialport_write_bytes)

static void _serialport_read_byte(int portnum)
{
	int portindex = portnum-1;
	BYTE ch;
	int rc;

	JQ_ASSERT(portnum > 0);

/*	printf_debug("COM%d: read char: %c", portnum, ch);*/
	rc = COMReadChar(portindex, (char*)&ch, NULL);
	JQ_ASSERT(rc <= 0 && rc >= COMERR_GENERAL);

	if( rc == COMERR_RXOVERFLOW ) {
		__SP_CONTEXT[portindex].err = E_SP_OVERFLOW;
		return;
	}

	if( !rb_put(__SP_CONTEXT[portindex].in, ch) ) {
		__SP_CONTEXT[portindex].err = E_SP_OVERFLOW;
		return;
	}

	rb_put(__SP_CONTEXT[portindex].msr, (CWT_CHAR)COMGetModemStatus(portindex));
}
END_OF_FUNCTION(_serialport_read_byte)

#define _SP_DEV_EVENT_HANDLER(x)                          \
	void _sp_event_handler_##x(int nevent) {              \
		switch(nevent) {                                  \
			case evCOMRx:                                 \
				_serialport_read_byte(x);                 \
				break;                                    \
			case evCOMTx:                                 \
				if( COMIsTXBufferSent(x-1) == 0           \
						&& COMTXBufferUsed(x-1) == 0 )    \
					_serialport_write_bytes(x);           \
			case evCOMStat:                               \
			case evCOMModem:                              \
			default:                                      \
				break;                                    \
		}                                                 \
	}                                                     \
	END_OF_FUNCTION(_sp_event_handler_##x)

_SP_DEV_EVENT_HANDLER(1);
_SP_DEV_EVENT_HANDLER(2);
_SP_DEV_EVENT_HANDLER(3);
_SP_DEV_EVENT_HANDLER(4);
_SP_DEV_EVENT_HANDLER(5);
_SP_DEV_EVENT_HANDLER(6);
_SP_DEV_EVENT_HANDLER(7);
_SP_DEV_EVENT_HANDLER(8);
_SP_DEV_EVENT_HANDLER(9);
_SP_DEV_EVENT_HANDLER(10);
_SP_DEV_EVENT_HANDLER(11);
_SP_DEV_EVENT_HANDLER(12);
_SP_DEV_EVENT_HANDLER(13);
_SP_DEV_EVENT_HANDLER(14);
_SP_DEV_EVENT_HANDLER(15);
_SP_DEV_EVENT_HANDLER(16);
_SP_DEV_EVENT_HANDLER(17);
_SP_DEV_EVENT_HANDLER(18);
_SP_DEV_EVENT_HANDLER(19);
_SP_DEV_EVENT_HANDLER(20);
_SP_DEV_EVENT_HANDLER(21);
_SP_DEV_EVENT_HANDLER(22);
_SP_DEV_EVENT_HANDLER(23);
_SP_DEV_EVENT_HANDLER(24);
_SP_DEV_EVENT_HANDLER(25);
_SP_DEV_EVENT_HANDLER(26);
_SP_DEV_EVENT_HANDLER(27);
_SP_DEV_EVENT_HANDLER(28);
_SP_DEV_EVENT_HANDLER(29);
_SP_DEV_EVENT_HANDLER(30);
_SP_DEV_EVENT_HANDLER(31);
_SP_DEV_EVENT_HANDLER(32);

#define _SP_LOCK_HANDLER(x)                                              \
	case x: __SP_CONTEXT[x-1].event_handler = _sp_event_handler_##x;     \
		return LOCK_FUNCTION(_sp_event_handler_##x) == -1 ? FALSE : TRUE

static BOOL _sp_lock_event_handler(int portnum)
{
	switch(portnum) {
		_SP_LOCK_HANDLER(1);
		_SP_LOCK_HANDLER(2);
		_SP_LOCK_HANDLER(3);
		_SP_LOCK_HANDLER(4);
		_SP_LOCK_HANDLER(5);
		_SP_LOCK_HANDLER(6);
		_SP_LOCK_HANDLER(7);
		_SP_LOCK_HANDLER(8);
		_SP_LOCK_HANDLER(9);
		_SP_LOCK_HANDLER(10);
		_SP_LOCK_HANDLER(11);
		_SP_LOCK_HANDLER(12);
		_SP_LOCK_HANDLER(13);
		_SP_LOCK_HANDLER(14);
		_SP_LOCK_HANDLER(15);
		_SP_LOCK_HANDLER(16);
		_SP_LOCK_HANDLER(17);
		_SP_LOCK_HANDLER(18);
		_SP_LOCK_HANDLER(19);
		_SP_LOCK_HANDLER(20);
		_SP_LOCK_HANDLER(21);
		_SP_LOCK_HANDLER(22);
		_SP_LOCK_HANDLER(23);
		_SP_LOCK_HANDLER(24);
		_SP_LOCK_HANDLER(25);
		_SP_LOCK_HANDLER(26);
		_SP_LOCK_HANDLER(27);
		_SP_LOCK_HANDLER(28);
		_SP_LOCK_HANDLER(29);
		_SP_LOCK_HANDLER(30);
		_SP_LOCK_HANDLER(31);
		_SP_LOCK_HANDLER(32);
		default: break;
	}
	return FALSE;
}


CwtSerialRC serialport_errno(int portnum)
{
	JQ_ASSERT(portnum > 0);
	return __SP_CONTEXT[portnum-1].err;
}


CwtSerialRC serialport_init(void)
{
	int rc;
	int i;

	/* already init */
	if( __serialport_init )
		return E_SP_NOERROR;

	rc = COMInit();

	if( !rc ) {
		return E_SP_GENERAL;
	}

	/* Initialize serial ports' contexts */
	for( i = 0; i < COMMAX; i++ ) {
		__SP_CONTEXT[i].in = NULL;
		__SP_CONTEXT[i].out = NULL;
		__SP_CONTEXT[i].msr = NULL;
		__SP_CONTEXT[i].err = E_SP_NOERROR;
	}

	if( LOCK_FUNCTION(_serialport_write_bytes) == -1 ) {
		print_error("unable to lock writer");
		return E_SP_GENERAL;
	}

	if( LOCK_FUNCTION(_serialport_read_byte) == -1 ) {
		print_error("unable to lock reader");
		return E_SP_GENERAL;
	}

	__serialport_init = TRUE;

	return E_SP_NOERROR;
}

void serialport_release(void)
{
	int i;

	if( !__serialport_init )
		return;


	for( i = 0; i < COMMAX; i++ ) {
		rb_delete(__SP_CONTEXT[i].in);
		rb_delete(__SP_CONTEXT[i].out);
		rb_delete(__SP_CONTEXT[i].msr);
		__SP_CONTEXT[i].in = NULL;
		__SP_CONTEXT[i].out = NULL;
		__SP_CONTEXT[i].msr = NULL;
	}

	COMShutDown();


	__serialport_init = FALSE;
}


CwtSerialRC serialport_set_settings(int portnum, UINT16 uart_base, char irq)
{
	JQ_ASSERT(__serialport_init);
	JQ_ASSERT(portnum > 0);
	COMSetHardwareParameters(portnum-1, (int)irq, (int)uart_base);

	return E_SP_NOERROR;
}

void serialport_settings(int portnum, UINT16 *uart_base, char *irq)
{
	int portindex = portnum-1;
	int irq_val, uart_base_val;

	JQ_ASSERT(__serialport_init);
	JQ_ASSERT(portnum > 0);

	COMGetHardwareParameters(portindex, &irq_val, &uart_base_val);

	if( uart_base )
		*uart_base = (UINT16)uart_base_val;
	if( irq )
		*irq = (char)irq_val;
}

BOOL serialport_has_valid_settings(int portnum)
{
	JQ_UNUSED(portnum);
	return TRUE;
}


CwtSerialRC serialport_open(int portnum
		  , CwtSerialBitrate speed
		  , BOOL hispeed
		  , CwtSerialDB db
		  , CwtSerialParity parity
		  , CwtSerialSB sb)
{
	int rc;
	int par = 'N';
	int pindex = portnum-1;

	JQ_ASSERT(portnum > 0);

	switch( parity ) {
		case SP_ParityNone: par = 'N'; break;
		case SP_ParityEven: par = 'E'; break;
		case SP_ParityOdd:  par = 'O'; break;
		default: break;
	}

	rc = COMPortOpen(pindex, speed, db, par, sb, 0, hispeed == TRUE ? 1 : 0, NULL);

	switch( rc ) {
		case COMERR_NOCHIP:   return E_SP_UART;
		case COMERR_NOMEMORY: return E_SP_NOMEM;
		case COMERR_GENERAL:  return E_SP_GENERAL;
		default: break;
	}

	JQ_ASSERT(rc == 0);

	__SP_CONTEXT[pindex].in  = rb_new_defaults();
	__SP_CONTEXT[pindex].out = rb_new_defaults();
	__SP_CONTEXT[pindex].msr = rb_new_defaults();

	if( !_sp_lock_event_handler(portnum) ) {
		printf_error("COM%d: unable to lock event handler", portnum);
		return E_SP_GENERAL;
	}

	if( LOCK_VARIABLE(__SP_CONTEXT[pindex]) < -1 ) {
		printf_error("COM%d: unable to lock serial port context internal data", portnum);
		return E_SP_GENERAL;
	}

	COMSetEventHandler(pindex, __SP_CONTEXT[pindex].event_handler);

	return E_SP_NOERROR;
}

void serialport_close(int portnum)
{
	JQ_ASSERT(portnum > 0);
	COMPortClose(portnum-1);
	if( __SP_CONTEXT[portnum-1].in ) {
		rb_delete(__SP_CONTEXT[portnum-1].in);
		rb_delete(__SP_CONTEXT[portnum-1].out);
		rb_delete(__SP_CONTEXT[portnum-1].msr);
		__SP_CONTEXT[portnum-1].in = NULL;
		__SP_CONTEXT[portnum-1].out = NULL;
		__SP_CONTEXT[portnum-1].msr = NULL;
	}
}


/*
void serialport_set_speed(int portnum, CwtSerialBitrate speed, BOOL hispeed)
{
	long old_speed;
	int db, par, sb, hs;

	JQ_ASSERT(__serialport_init);
	JQ_ASSERT(portnum > 0);

	COMGetTransmitParameters(portnum-1, &old_speed, &db, &par, &sb, &hs);
	COMSetTransmitParameters(portnum-1, speed, db, par, sb, hispeed == TRUE ? 1 : 0);
}
*/

void serialport_set_parms(int portnum
	, CwtSerialBitrate speed
	, BOOL hispeed
	, CwtSerialDB data_bits
	, CwtSerialParity parity
	, CwtSerialSB stop_bits)
{
	int par;

	JQ_ASSERT(__serialport_init);
	JQ_ASSERT(portnum > 0);

/*	COMGetTransmitParameters(portnum-1, &speed, &db, &par, &sb, &hs);*/

	switch( parity ) {
		case SP_ParityNone: par = 'N'; break;
		case SP_ParityEven: par = 'E'; break;
		case SP_ParityOdd:  par = 'O'; break;
		default: break;
	}

	COMSetTransmitParameters(portnum-1, speed, data_bits, par, stop_bits, hispeed == TRUE ? 1 : 0);
}


void serialport_parms(int portnum, CwtSerialBitrate *speed, BOOL *hispeed, CwtSerialDB *data_bits, CwtSerialParity *parity, CwtSerialSB *stop_bits)
{
	long sp;
	int db, par, sb, hs = 0;
	int portindex = portnum-1;

	JQ_ASSERT(__serialport_init);
	JQ_ASSERT(portnum > 0);

	COMGetTransmitParameters(portindex, &sp, &db, &par, &sb, &hs);

	if( speed )
		*speed = sp;

	if( hispeed )
		*hispeed = (hs != 0 ? TRUE : FALSE);

	if( data_bits )
		*data_bits = db;

	if( parity ) {
		switch( par ) {
			case 'N': *parity = SP_ParityNone; break;
			case 'E': *parity = SP_ParityEven; break;
			case 'O': *parity = SP_ParityOdd; break;
			default: break;
		}
	}

	if( stop_bits )
		*stop_bits = (CwtSerialSB)sb;
}


ssize_t serialport_write(int portnum, const BYTE *buf, size_t count)
{
	ssize_t bw;

	JQ_ASSERT(__serialport_init);
	JQ_ASSERT(portnum > 0);

	bw = rb_write(__SP_CONTEXT[portnum-1].out, buf, count);
	if( bw > 0 )
		_serialport_write_bytes(portnum);
	return bw;
}

ssize_t serialport_read(int portnum, BYTE *buf, size_t count)
{
	ssize_t br;

	JQ_ASSERT(__serialport_init);
	JQ_ASSERT(portnum > 0);
	br = rb_read(__SP_CONTEXT[portnum-1].in, buf, count);
	if( br > 0 ) {
		rb_pop_front(__SP_CONTEXT[portnum-1].in, (size_t)br);
		rb_pop_front(__SP_CONTEXT[portnum-1].msr, (size_t)br);
	}
	return br;
}

ssize_t serialport_read_msr(int portnum, BYTE *buf, BYTE *msr_buf, size_t count)
{
	ssize_t br;

	JQ_ASSERT(__serialport_init);
	JQ_ASSERT(portnum > 0);
	br = rb_read(__SP_CONTEXT[portnum-1].in, buf, count);
	br = rb_read(__SP_CONTEXT[portnum-1].msr, msr_buf, count);
	if( br > 0 ) {
		rb_pop_front(__SP_CONTEXT[portnum-1].in, (size_t)br);
		rb_pop_front(__SP_CONTEXT[portnum-1].msr, (size_t)br);
	}
	return br;
}

size_t  serialport_bytes_available(int portnum)
{
	JQ_ASSERT(__serialport_init);
	JQ_ASSERT(portnum > 0);
	return rb_size(__SP_CONTEXT[portnum-1].in);
}

BOOL serialport_tx_ready(int portnum)
{
	JQ_ASSERT(__serialport_init);
	JQ_ASSERT(portnum > 0);
	return COMTXBufferFree(portnum-1) > 0 ? TRUE : FALSE;
}

BOOL serialport_tx_empty(int portnum)
{
	JQ_ASSERT(__serialport_init);
	JQ_ASSERT(portnum > 0);
	return COMTXBufferUsed(portnum-1) == 0
		? rb_size(__SP_CONTEXT[portnum-1].out) == 0 ? TRUE : FALSE
		: FALSE;
}

BOOL serialport_rx_empty(int portnum)
{
	JQ_ASSERT(__serialport_init);
	JQ_ASSERT(portnum > 0);
	return COMRXBufferUsed(portnum-1) == 0
		? rb_size(__SP_CONTEXT[portnum-1].in) == 0 ? TRUE : FALSE
		: FALSE;
}

void serialport_flush_tx(int portnum)
{
	JQ_ASSERT(__serialport_init);
	JQ_ASSERT(portnum > 0);
	COMClearTXBuffer(portnum-1);
	rb_clear(__SP_CONTEXT[portnum-1].out);
}

void serialport_flush_rx(int portnum)
{
	JQ_ASSERT(__serialport_init);
	JQ_ASSERT(portnum > 0);
	COMClearRXBuffer(portnum-1);
	rb_clear(__SP_CONTEXT[portnum-1].in);
	rb_clear(__SP_CONTEXT[portnum-1].msr);
}

BOOL serialport_carrier_detected(int portnum)
{
	JQ_ASSERT(__serialport_init);
	JQ_ASSERT(portnum > 0);
	return COMGetCD(portnum-1) ? TRUE : FALSE;
}
