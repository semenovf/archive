#include <stdlib.h>
#include <dos.h>
#include <cwt/logger.h>
#include <cwt/io/serial/config.h>
#include <cwt/io/serial/serial.h>

#ifdef __WATCOMC__
#   ifndef inportb
#       include <conio.h>
#       define inportb inp
#       define outportb outp
#   endif
#   define getvect _dos_getvect
#   define setvect _dos_setvect
#   define enable  _enable
#   define disable _disable
#endif

#ifdef __WATCOMC__
#   define WORD_LOW_BYTE(WORD) ((char)((WORD)&0x00FF))
#   define WORD_HI_BYTE(WORD)  ((char)(((WORD)>>8)&0x00FF))

unsigned int outport(int port, unsigned int word)
{
    outp(port, WORD_LOW_BYTE(word));
    outp(port+1, WORD_HI_BYTE(word));
    return word;
}
#endif        



#define INVALID_SERIALPORT ((SERIALPORT_IMPL_PTR)0)

#define CHECK_SERIALPORT_OPERABLE(portnum)                         \
	(((portnum) > 0)                                               \
	&& (portnum) <= SERIALPORT_MAX                                 \
	&& __SP_CONTEXT[(portnum)-1].serialport != INVALID_SERIALPORT)

#define CARP_INVALID_SERIALPORT(portnum) \
    printf_error("%d: port number is out of bounds or serial port is not opened", portnum);

struct SERIALPORT_IMPL
{
	char    old_i8259_mask0; /* Copy as it was when we were called */
	char    old_i8259_mask1;
	char    old_ier;        /* Modem register contents saved for */
	char    old_mcr;        /*  restoring when we're done */


	/* FIXME: replace this queues with ringbuffer_t structure (memory overhead) */
	/*  Transmit queue.  Characters to be transmitted are held here until the  */
	/*  UART is ready to transmit them.  */
	struct {
		char    queue[SERIALPORT_TX_QUEUE_SIZE];
		int     in;                  /* Index of where to store next character */
		int     out;                 /* Index of where to retrieve next character */
		int     chars;               /* Count of characters in queue */
	} tx;

	/*  Receive queue.  Received characters are held here until retrieved by  */
	/*  com_rx()  */
	struct {
		char    queue[SERIALPORT_RX_QUEUE_SIZE];
		char    msr[SERIALPORT_RX_QUEUE_SIZE];
		int     in;                  /* Index of where to store next character */
		int     out;                 /* Index of where to retrieve next character */
		int     chars;               /* Count of characters in queue */
	} rx;
	
	void interrupt (*old_vector)(void); /* Place to save COM1 vector */
	int  err;
};

typedef struct SERIALPORT_IMPL  SERIALPORT_IMPL;
typedef struct SERIALPORT_IMPL* SERIALPORT_IMPL_PTR;

/*****************************************************************************
 *                             8250 Definitions                              *
 *****************************************************************************/

/*      Offsets to various 8250 registers.  Taken from IBM Technical         */
/*      Reference Manual, p. 1-225                                           */

#define TXBUFF  0                       /* Transmit buffer register */
#define RXBUFF  0                       /* Receive buffer register */
#define DLLSB   0                       /* Divisor latch LS byte */
#define DLMSB   1                       /* Divisor latch MS byte */
#define IER     1                       /* Interrupt enable register */
#define IIR     2                       /* Interrupt ID register */
#define LCR     3                       /* Line control register */
#define MCR     4                       /* Modem control register */
#define LSR     5                       /* Line status register */
#define MSR     6                       /* Modem status register */

/*      Modem control register bits                                         */

#define DTR     0x01                    /* Data terminal ready */
#define RTS     0x02                    /* Request to send */
#define OUT1    0x04                    /* Output #1 */
#define OUT2    0x08                    /* Output #2 */
#define LPBK    0x10                    /* Loopback mode bit */

/*      Modem status register bits                                           */
#define DCTS    0x01                    /* Delta clear to send */
#define DDSR    0x02                    /* Delta data set ready */
#define TERI    0x04                    /* Trailing edge ring indicator */
#define DRLSD   0x08                    /* Delta Rx line signal detect */
#define CTS     0x10                    /* Clear to send */
#define DSR     0x20                    /* Data set ready */
#define RI      0x40                    /* Ring indicator */
#define RLSD    0x80                    /* Receive line signal detect */

/*      Line control register bits                                           */
#define DATA5   0x00                    /* 5 Data bits */
#define DATA6   0x01                    /* 6 Data bits */
#define DATA7   0x02                    /* 7 Data bits */
#define DATA8   0x03                    /* 8 Data bits */

#define STOP1   0x00                    /* 1 Stop bit */
#define STOP2   0x04                    /* 2 Stop bits */

#define NOPAR   0x00                    /* No parity */
#define ODDPAR  0x08                    /* Odd parity */
#define EVNPAR  0x18                    /* Even parity */
#define STKPAR  0x28                    /* Stick parity */
#define ZROPAR  0x38                    /* Zero parity */

/*      Line status register bits                                            */
#define RDR     0x01                    /* Receive data ready */
#define ERRS    0x1E                    /* All the error bits */
#define TXR     0x20                    /* Transmitter ready */

/*      Interrupt enable register bits                                       */
#define DR      0x01                    /* Data ready */
#define THRE    0x02                    /* Tx buffer empty */
#define RLS     0x04                    /* Receive line status */

/* array to store opened serial ports */
static struct _serialport_context {
        SERIALPORT_IMPL_PTR serialport;
        UINT uart_base;
        char irq;
    	BOOL hispeed;
/*        char i8259level0;*/
        void interrupt (*interrupt_driver)(void);
} __SP_CONTEXT[SERIALPORT_MAX];

static BOOL __serialport_init = FALSE;

static void serialport_interrupt_driver_common(int portindex);

void interrupt com1_interrupt_driver(void)
{ serialport_interrupt_driver_common(0); }

void interrupt com2_interrupt_driver(void)
{ serialport_interrupt_driver_common(1); }

#if SERIALPORT_MAX > 2
void interrupt com3_interrupt_driver(void)
{ serialport_interrupt_driver_common(2); }

void interrupt com4_interrupt_driver(void)
{ serialport_interrupt_driver_common(3); }
#endif

#if SERIALPORT_MAX > 4
void interrupt com5_interrupt_driver(void)
{ serialport_interrupt_driver_common(4); }
#endif

#if SERIALPORT_MAX > 5
void interrupt com6_interrupt_driver(void)
{ serialport_interrupt_driver_common(5); }
#endif

#if SERIALPORT_MAX > 6
void interrupt com7_interrupt_driver(void)
{ serialport_interrupt_driver_common(6); }
#endif

#if SERIALPORT_MAX > 7
void interrupt com8_interrupt_driver(void)
{ serialport_interrupt_driver_common(7); }
#endif

#if SERIALPORT_MAX > 8
void interrupt com9_interrupt_driver(void)
{ serialport_interrupt_driver_common(8); }
#endif

#if SERIALPORT_MAX > 9
void interrupt com10_interrupt_driver(void)
{ serialport_interrupt_driver_common(9); }
#endif

#if SERIALPORT_MAX > 10
void interrupt com11_interrupt_driver(void)
{ serialport_interrupt_driver_common(10); }
#endif

typedef void interrupt (*interrupt_driver_t)(void);
static interrupt_driver_t __interrupt_drivers[] = {
	  com1_interrupt_driver
	, com2_interrupt_driver
#if SERIALPORT_MAX > 2
	, com3_interrupt_driver
	, com4_interrupt_driver
#endif
#if SERIALPORT_MAX > 4
	, com5_interrupt_driver
#endif

#if SERIALPORT_MAX > 5
	, com6_interrupt_driver
#endif

#if SERIALPORT_MAX > 6
	, com7_interrupt_driver
#endif

#if SERIALPORT_MAX > 7
	, com8_interrupt_driver
#endif

#if SERIALPORT_MAX > 8
	, com9_interrupt_driver
#endif

#if SERIALPORT_MAX > 9
	, com10_interrupt_driver
#endif

#if SERIALPORT_MAX > 10
	, com11_interrupt_driver
#endif

};

#if 0
static int  __uart_base[SERIALPORT_MAX]; /* = { 0x3F8, 0x2F8, 0x3E8, 0x2E8 };*/
static char __irq_nums[]; /* =     { 0x0C,  0x0B,  0x0C,  0x0B }; */
static char __i8259levels[]; /* = { 4,     3,     4,     3 }; */
#endif


BOOL serialport_has_valid_settings(int portnum)
{
	int portindex = portnum-1;
	
	/* error */
	if( portnum < 1 || portnum > SERIALPORT_MAX ) { 
			return FALSE;
	}
	
	if( __SP_CONTEXT[portindex].uart_base > 0
			&& __SP_CONTEXT[portindex].irq > 0
			/*&& __SP_CONTEXT[portindex].i8259level0 >= 0 */) {
			
			return TRUE;
	}
	
	return FALSE;
}

CwtSerialRC serialport_set_settings(
    int portnum
  , UINT16 uart_base
  , char irq )
{
	int portindex = portnum-1;

	if( !__serialport_init ) {
		serialport_init();
	}

	/* error */
	if( portnum < 1 || portnum > SERIALPORT_MAX ) { 
			return E_SP_BOUNDS;
	}
	
	/* error: already opened */
	if( __SP_CONTEXT[portindex].serialport != INVALID_SERIALPORT ) {
			return E_SP_OPENED;
	}

	__SP_CONTEXT[portindex].uart_base  = uart_base;
	__SP_CONTEXT[portindex].irq        = irq;
/*	__SP_CONTEXT[portindex].i8259level = i8259level;*/
	__SP_CONTEXT[portindex].interrupt_driver = __interrupt_drivers[portindex];

	return E_SP_NOERROR;
}

void serialport_settings(int portnum, UINT16 *uart_base, char *irq)
{
	int portindex = portnum-1;

	if( uart_base )
		*uart_base = __SP_CONTEXT[portindex].uart_base;
	if( irq )
		*irq = __SP_CONTEXT[portindex].irq;
}


/**
 * Initializes subsystem of communications over serial ports
 * Not need to call explicitly, it launched with first call of serialport_open function
*/
CwtSerialRC serialport_init(void)
{
	int i = SERIALPORT_MAX;
	while( i-- > 0 ) {
		__SP_CONTEXT[i].serialport = INVALID_SERIALPORT;
		__SP_CONTEXT[i].uart_base  = 0;
		__SP_CONTEXT[i].irq        = 0;
/*		__SP_CONTEXT[i].i8259level = 0;*/
		__SP_CONTEXT[i].interrupt_driver = NULL;
		__SP_CONTEXT[i].hispeed    = FALSE;
	}

#if SERIALPORT_MAX > 0
	__SP_CONTEXT[0].uart_base = 0x3F8; /* UART base address for COM1 */
	__SP_CONTEXT[0].irq = 4/*0x0C*/; /* IRQ number for COM1 */
/*	__SP_CONTEXT[0].i8259level = 4;*/ /* TODO need to know what this is mean */
	__SP_CONTEXT[0].interrupt_driver = com1_interrupt_driver;
#endif

#if SERIALPORT_MAX > 1
	__SP_CONTEXT[1].uart_base = 0x2F8;
	__SP_CONTEXT[1].irq = 3/*0x0B*/;
/*	__SP_CONTEXT[1].i8259level = 3;*/
	__SP_CONTEXT[1].interrupt_driver = com2_interrupt_driver;
#endif


#if SERIALPORT_MAX > 2
	__SP_CONTEXT[2].uart_base = 0x3E8;
	__SP_CONTEXT[2].irq = 4/*0x0C*/;
/*	__SP_CONTEXT[2].i8259level = 4;*/
	__SP_CONTEXT[2].interrupt_driver = com3_interrupt_driver;
#endif

#if SERIALPORT_MAX > 3
	__SP_CONTEXT[3].uart_base = 0x2E8;
	__SP_CONTEXT[3].irq = 3/*0x0B*/;
/*	__SP_CONTEXT[3].i8259level = 3;*/
	__SP_CONTEXT[3].interrupt_driver = com4_interrupt_driver;
#endif
	__serialport_init = TRUE;
	return E_SP_NOERROR;
}

void serialport_release(void)
{
	int i = SERIALPORT_MAX;

	if( __serialport_init ) {
		while( i-- > 0 ) {
			if( __SP_CONTEXT[i].serialport != INVALID_SERIALPORT )
				serialport_close(i+1);
		}
		__serialport_init = FALSE;
	}
}

/* TODO need to implement settings with paramaters */
/** Opens serial port
*/
CwtSerialRC serialport_open(
	int portnum
  , CwtSerialBitrate speed
  , BOOL hispeed
  , CwtSerialDB db
  , CwtSerialParity parity
  , CwtSerialSB sb)
{
	int portindex = portnum - 1; /* index in __serialport_vector */
	SERIALPORT_IMPL_PTR sp;

	UINT uart_data
	  , uart_ier
	  , uart_lcr
	  , uart_mcr;

	char irq;
	char ivect;
	char i8259bit0 = 0;
	char i8259bit1 = 0;


	if( !__serialport_init ) {
		serialport_init();
	}

	/* error: port number is out of bounds */
	if( portnum < 1 || portnum > SERIALPORT_MAX ) {
		return E_SP_BOUNDS;
	}

	if( !serialport_has_valid_settings(portnum) ) {
		return E_SP_BADATTS;
	}

	/* error: serial port already is opened */
	if( __SP_CONTEXT[portindex].serialport != INVALID_SERIALPORT ) {
		return E_SP_OPENED;
	}

	sp = JQ_MALLOC(SERIALPORT_IMPL);
	/* error: no memory */
	if( !sp ) {
		return E_SP_NOMEM;
	}

	uart_data = __SP_CONTEXT[portindex].uart_base;    /* Set UART I/O addresses */
	uart_ier  = uart_data + IER;                /*  for the selected comm */
	uart_lcr  = uart_data + LCR;
	uart_mcr  = uart_data + MCR;
    irq       = __SP_CONTEXT[portindex].irq;             /* Ditto for interrupt */

    if( irq < 8 ) {
    	ivect = irq + 8;
    	i8259bit0 = 1 << irq;
    	i8259bit1 = 0;
    } else {
    	ivect = (irq - 8) + 0x70; /* http://www.lammertbies.nl/comm/info/RS-232_io.html */
    	i8259bit0 = 1 << 2;       /* slave PIC i8259 on IRQ #2 */
    	i8259bit1 = 1 << (irq-8);
    }

    sp->old_ier = inportb(uart_ier);    /* Return an error if we */
    outportb(uart_ier, 0);              /*  can't access the UART */
        
    /* error: can't access the UART */
    if( inportb(uart_ier) != 0 ) {
    	JQ_FREE(sp);
		return E_SP_UART;
    }

	sp->err = E_SP_NOERROR;
    __SP_CONTEXT[portindex].serialport = sp;

    disable();                                  /* Save the original 8259 */
    sp->old_i8259_mask0 = inportb(0x21);        /*  mask, then disable the */
    sp->old_i8259_mask1 = inportb(0xA1);
    outportb(0x21, sp->old_i8259_mask0 | i8259bit0);  /*  8259 for this interrupt */
    outportb(0xA1, sp->old_i8259_mask1 | i8259bit1);  /*  8259 for this interrupt */
    enable();

    serialport_flush_tx(portnum);  /* Clear the transmit and */
    serialport_flush_rx(portnum);  /*  receive queues */
        
    sp->old_vector = getvect(ivect); /* Save old COMM vector, */
    setvect(ivect, __SP_CONTEXT[portindex].interrupt_driver);  /*  then install a new one, */
	if( getvect(ivect) == __SP_CONTEXT[portindex].interrupt_driver ) {
		printf_debug("new vector installed for irq #%d (vector #%d, COM%d)", (int)irq, (int)ivect, portnum);
	} else {
		printf_error("failed to install new vector for irq #%d (vector #%d, COM%d)", (int)irq, (int)ivect, portnum);
		JQ_FREE(sp);
		return E_SP_SETVECT;
	}
	
    outportb(uart_lcr, DATA8 + NOPAR + STOP1);  /* 8 data, no parity, 1 stop */

    disable();                                  /* Save MCR, then enable */
    sp->old_mcr = inportb(uart_mcr);            /*  interrupts onto the bus, */
    outportb(uart_mcr,                          /*  activate RTS and leave */
        (sp->old_mcr & DTR) | (OUT2 + RTS));    /*  DTR the way it was */
    enable();
    outportb(uart_ier, DR);                     /* Enable receive interrupts */
    disable();                                  /* Now enable the 8259 for */
    outportb(0x21, inportb(0x21) & ~i8259bit0);  /*  this interrupt */
    outportb(0xA1, inportb(0xA1) & ~i8259bit1);
    enable();

    /* raise DTR */
    disable();
    outportb(uart_mcr, inportb(uart_mcr) | DTR);
    enable();


	serialport_set_parms(portnum, speed, hispeed, db, parity, sb);
	return E_SP_NOERROR;
}

/**
 serialport_close()
 * DESCRIPTION: Deinstalls the communications drivers completely, without    *
 *              changing the baud rate or DTR.  It tries to leave the        *
 *              interrupt vectors and enables and everything else as they    *
 *              were when the driver was installed.                          *
 *                                                                           *
 * NOTE:        This function MUST be called before returning to DOS, so the *
 *              interrupt vector won't point to our driver anymore, since it *
 *              will surely get overwritten by some other transient program  *
 *              eventually.                                                  *
 *                                                                           *
 * REVISIONS:   18 OCT 89 - RAC - Translated from IBMCOM.PAS                 *
 *****************************************************************************/
void serialport_close(int portnum) 
{
    if( !CHECK_SERIALPORT_OPERABLE(portnum) ) {
        CARP_INVALID_SERIALPORT(portnum);
    } else {
        int portindex = portnum-1;
        UINT uart_ier
          , uart_mcr;
        char irq;
        char ivect;
        char i8259bit0;
        char i8259bit1;
        SERIALPORT_IMPL_PTR sp;
                
        sp        = __SP_CONTEXT[portindex].serialport;
        uart_ier  = __SP_CONTEXT[portindex].uart_base + IER;                /*  for the selected comm */
        uart_mcr  = __SP_CONTEXT[portindex].uart_base + MCR;
        irq       = __SP_CONTEXT[portindex].irq;

        if( irq < 8 ) {
        	ivect = irq + 8;
        	i8259bit0 = 1 << irq;
        	i8259bit1 = 0;
        } else {
        	ivect = (irq - 8) + 0x70;
        	i8259bit0 = 1 << 2;
        	i8259bit1 = 1 << (irq - 8);
        }

        /* lower DTR */
        disable();
        outportb(uart_mcr, inportb(uart_mcr) & ~DTR);
        enable();

        outportb(uart_mcr, sp->old_mcr);         /* Restore the UART */
        outportb(uart_ier, sp->old_ier);         /*  registers ... */
        disable();

        outportb(0x21,                           /*  ... the 8259 interrupt */
            (inportb(0x21)  & ~i8259bit0) |      /*  mask ... */
            (sp->old_i8259_mask0 &  i8259bit0));
        outportb(0xA1,                           /*  ... the 8259 interrupt */
            (inportb(0xA1)  & ~i8259bit1) |      /*  mask ... */
            (sp->old_i8259_mask1 &  i8259bit1));
        enable();
        setvect(ivect, sp->old_vector);          /*  ... and the comm */
        JQ_FREE(sp);
        __SP_CONTEXT[portindex].serialport = INVALID_SERIALPORT;
			
		if( getvect(ivect) == sp->old_vector ) {
			printf_debug("old vector restored for irq #%d (vector #%d, COM%d)", (int)irq, (int)ivect, portnum);
		} else {
			printf_error("failed to restore vector for irq #%d (vector #%d, COM%d)", (int)irq, (int)ivect, portnum);
		}

	}
}

CwtSerialRC serialport_errno(int portnum)
{
	if( portnum <= 0 && portnum > SERIALPORT_MAX )
		return E_SP_BOUNDS;
	if( __SP_CONTEXT[portnum-1].serialport == INVALID_SERIALPORT )
		return E_SP_INVALID;
		
	return __SP_CONTEXT[(portnum)-1].serialport->err;
}

/**
 * serialport_set_speed()
 
 * DESCRIPTION: Sets the baud rate.                                          *
 *                                                                           *
 * SYNOPSIS:    void com_set_speed(unsigned speed);                          *
 *              unsigned speed;                 Desired baud rate            *
 *                                                                           *
 * NOTES:       The input parameter can be anything between 2 and 65535.     *
 *              However, I (Wayne) am not sure that extremely high speeds    *
 *              (those above 19200) will always work, since the baud rate    *
 *              divisor will be six or less, where a difference of one can   *
 *              represent a difference in baud rate of 3840 bits per second  *
 *              or more.)                                                    *
 *                                                                           *
 * REVISIONS:   18 OCT 89 - RAC - Translated from IBMCOM.PAS                 *
 *****************************************************************************/
#ifdef __COMMENT__
void serialport_set_speed(int portnum, CwtSerialBitrate speed, BOOL hispeed)
{
	if( !CHECK_SERIALPORT_OPERABLE(portnum) ) {
		CARP_INVALID_SERIALPORT(portnum);
	} else {
		int portindex = portnum-1;
		UINT divisor;
		UINT uart_data     /* Set UART I/O addresses */
		  , uart_lcr;

		uart_data = __SP_CONTEXT[portindex].uart_base;    /* Set UART I/O addresses */
		uart_lcr  = __SP_CONTEXT[portindex].uart_base + LCR;

		if( speed < 2 )
			speed = 2;                 /* Force proper input */
		if( hispeed ) {
			divisor = (UINT)(921600L / speed); /* Recond baud rate divisor */
		} else {
			divisor = (UINT)(115200L / speed);
		}
		disable();
		outportb(uart_lcr, inportb(uart_lcr) | 0x80); /* Set up to load baud rate */
													  /*  divisor into UART */
		outport(uart_data, divisor);                  /* Do so */
		outportb(uart_lcr, inportb(uart_lcr) & ~0x80); /* Back to normal UART ops */
			
		enable();
	}
}
#endif


/*****************************************************************************
 *                             com_set_parity()                              *
 *****************************************************************************
 * DESCRIPTION: Sets the parity and stop bits.                               *
 *                                                                           *
 * SYNOPSIS:    void com_set_parity(enum par_code parity, int stop_bits);    *
 *              int     code;           COM_NONE = 8 data bits, no parity    *
 *                                      COM_EVEN = 7 data, even parity       *
 *                                      COM_ODD  = 7 data, odd parity        *
 *                                      COM_ZERO = 7 data, parity bit = zero *
 *                                      COM_ONE  = 7 data, parity bit = one  *
 *              int     stop_bits;      Must be 1 or 2                       *
 *                                                                           *
 * REVISIONS:   18 OCT 89 - RAC - Translated from the Pascal                 *
 *****************************************************************************/

void serialport_set_parms(int portnum
	, CwtSerialBitrate speed
	, BOOL hispeed
	, CwtSerialDB db
	, CwtSerialParity parity
	, CwtSerialSB sb)
/*void serialport_set_attrs(int portnum, CwtSerialParity parity, CwtSerialSB stop_bits)*/
{
	if( !CHECK_SERIALPORT_OPERABLE(portnum) ) {
		CARP_INVALID_SERIALPORT(portnum);
	} else {
		int portindex = portnum-1;
		UINT divisor;
		UINT uart_data     /* Set UART I/O addresses */
		  , uart_lcr;
		BYTE lcr;

		uart_data = __SP_CONTEXT[portindex].uart_base;    /* Set UART I/O addresses */
		uart_lcr  = __SP_CONTEXT[portindex].uart_base + LCR;

		lcr = (db == SP_DB5)
			? DATA5
			: (db == SP_DB6)
			? DATA6
			: (db == SP_DB7)
			? DATA7
			: DATA8;
		lcr |= ((sb == SP_SB2) ? STOP2 : STOP1);
		lcr |= ((parity == SP_ParityOdd)
			? ODDPAR
			: (parity == SP_ParityEven)
			? EVNPAR : NOPAR);

		if( speed < 2 )
			speed = 2;                 /* Force proper input */
		if( hispeed ) {
			divisor = (UINT)(921600L / speed); /* Recond baud rate divisor */
		} else {
			divisor = (UINT)(115200L / speed);
		}
		disable();
		outportb(uart_lcr, inportb(uart_lcr) | 0x80); /* Set up to load baud rate */
													  /*  divisor into UART */
		outport(uart_data, divisor);                  /* Do so */
		outportb(uart_lcr, inportb(uart_lcr) & ~0x80); /* Back to normal UART ops */
		outportb(uart_lcr, lcr);
		enable();
		__SP_CONTEXT[portindex].hispeed = hispeed;
	}
}

void serialport_parms(int portnum, CwtSerialBitrate *speed, BOOL *hispeed, CwtSerialDB *db, CwtSerialParity *parity, CwtSerialSB *sb)
{
	if( !CHECK_SERIALPORT_OPERABLE(portnum) ) {
		CARP_INVALID_SERIALPORT(portnum);
	} else {
		int portindex = portnum-1;
		UINT divisor;
		UINT uart_data     /* Set UART I/O addresses */
		  , uart_lcr;
		BYTE lcr;

		uart_data = __SP_CONTEXT[portindex].uart_base;    /* Set UART I/O addresses */
		uart_lcr  = __SP_CONTEXT[portindex].uart_base + LCR;

		disable();
		outportb(uart_lcr, inportb(uart_lcr) | 0x80); /* Set up to load baud rate */
													  /*  divisor into UART */
		divisor = inport(uart_data);                  /* Do so */
		outportb(uart_lcr, inportb(uart_lcr) & ~0x80); /* Back to normal UART ops */
		lcr = inportb(uart_lcr);
		enable();

		if( speed ) {
			if(__SP_CONTEXT[portindex].hispeed)
				*speed = 921600L / divisor;
			else
				*speed = 115200L / divisor;
		}

		if( hispeed )
			*hispeed = __SP_CONTEXT[portindex].hispeed;

		if( parity ) {
			if( (lcr & 0x38) == ODDPAR )
				*parity = SP_ParityOdd;
			else if( (lcr & 0x38) == EVNPAR )
				*parity = SP_ParityEven;
			else if( (lcr & 0x38) == STKPAR )
				*parity = SP_ParityHigh;
			else if( (lcr & 0x38) == ZROPAR )
				*parity = SP_ParityLow;
			else
				*parity = SP_ParityNone;
		}


		if( sb )
			*sb = (lcr & STOP2) ? SP_SB2 : SP_SB1;

		if( db )
			*db = (lcr & 3) == 0
				? SP_DB5
				: (lcr & 3) == 1
				? SP_DB6
				: (lcr & 3) == 2
				? SP_DB7
				: SP_DB8;

	}
}

/**
 sends a single character by waiting until the transmit buffer isn't full, then putting
 the character into it.  The interrupt driver will then send
 the character once it is at the head of the transmit queue
 and a transmit interrupt occurs.
 */
ssize_t serialport_write(int portnum, const BYTE *buf, size_t count)
{
	if( CHECK_SERIALPORT_OPERABLE(portnum) ) {
		size_t i; /* bytes written */
		UINT uart_ier;
		SERIALPORT_IMPL_PTR sp;
		
		sp = __SP_CONTEXT[portnum-1].serialport;
		
		uart_ier  = __SP_CONTEXT[portnum-1].uart_base + IER;

		for( i = 0; i < count; i++ ) {
			/* Wait for non-full buffer. In fact it is equivalent to serialport_tx_ready() call */
			while( !((__SP_CONTEXT[portnum-1].serialport->tx.chars < SERIALPORT_TX_QUEUE_SIZE)) ) 
				;

			disable();
			sp->tx.queue[sp->tx.in++] = (char)buf[i]; /* Stuff character in queue */
					
			/* Wrap index if needed */
			if( sp->tx.in == SERIALPORT_TX_QUEUE_SIZE ) 
				sp->tx.in = 0;
							
			sp->tx.chars++;                              /* Number of char's in queue */
			outportb(uart_ier, inportb(uart_ier) | THRE); /* Enable UART tx interrupt */
					 
			enable();

			if( sp->tx.chars == SERIALPORT_TX_QUEUE_SIZE ) {
				sp->err = E_SP_OVERFLOW;
				break;
			}
		}
		
		if( !sp->err )
			return (ssize_t)i;
		else
			return (ssize_t)-1;
	}
	CARP_INVALID_SERIALPORT(portnum);
	return (ssize_t)-1;
}

/*****************************************************************************
 *                                 com_rx()                                  *
 *****************************************************************************
 * DESCRIPTION: Returns the next character from the receive buffer, or a     *
 *              NULL character ('\0') if the buffer is empty.                *
 *                                                                           *
 * SYNOPSIS:    c = com_rx();                                                *
 *              char    c;                      The returned character       *
 *                                                                           *
 * REVISIONS:   18 OCT 89 - RAC - Translated from the Pascal.                *
 *****************************************************************************/
ssize_t serialport_read(int portnum, BYTE *buf, size_t count)
{
	if( CHECK_SERIALPORT_OPERABLE(portnum) ) {
		size_t i = 0;
		SERIALPORT_IMPL_PTR sp;
		
		sp = __SP_CONTEXT[portnum-1].serialport;
		
		/* if buffer is empty */
		if( !sp->rx.chars )
			return (ssize_t)0;
					
		disable();
		while( i < count && sp->rx.chars ) {
			buf[i++] = (BYTE)sp->rx.queue[sp->rx.out++];      /* Grab char from queue */
			if(sp->rx.out == SERIALPORT_RX_QUEUE_SIZE)  /* Wrap index if needed */
				sp->rx.out = 0;
			sp->rx.chars--;                             /* One less char in queue */
		}
		enable();
		return (ssize_t)i;
	}
	CARP_INVALID_SERIALPORT(portnum);
	return (ssize_t)-1;
}

/**
 * Notice: if it is need to read MSR register data and transmitted
 * first need to call @c serialport_read_msr function
 *
 * @param portnum
 * @param buf
 * @param count
 * @return
 */
ssize_t serialport_read_msr(int portnum, BYTE *buf, BYTE *msr_buf, size_t count)
{
	if( CHECK_SERIALPORT_OPERABLE(portnum) ) {
		size_t i = 0;
		SERIALPORT_IMPL_PTR sp;

		sp = __SP_CONTEXT[portnum-1].serialport;

		/* if buffer is empty */
		if( !sp->rx.chars )
			return (ssize_t)0;

		disable();
		while( i < count && sp->rx.chars ) {
			msr_buf[i] = (BYTE)sp->rx.msr[sp->rx.out];
			buf[i++] = (BYTE)sp->rx.queue[sp->rx.out++];      /* Grab char from queue */
			if(sp->rx.out == SERIALPORT_RX_QUEUE_SIZE)  /* Wrap index if needed */
				sp->rx.out = 0;
			sp->rx.chars--;                             /* One less char in queue */
		}
		enable();
		return (ssize_t)i;
	}
	CARP_INVALID_SERIALPORT(portnum);
	return (ssize_t)-1;
}

size_t serialport_bytes_available(int portnum)
{
	if( !CHECK_SERIALPORT_OPERABLE(portnum) ) {
		CARP_INVALID_SERIALPORT(portnum);
		return 0;
	}
	return __SP_CONTEXT[portnum-1].serialport->rx.chars;
}

/** Return TRUE if the transmit queue can accept a character 
 */
BOOL serialport_tx_ready(int portnum)
{
	if( !CHECK_SERIALPORT_OPERABLE(portnum) ) {
		CARP_INVALID_SERIALPORT(portnum);
		return FALSE;
	}
	return (__SP_CONTEXT[portnum-1].serialport->tx.chars < SERIALPORT_TX_QUEUE_SIZE);
}


/** Return TRUE if the transmit queue is empty 
 */
BOOL serialport_tx_empty(int portnum)
{
	if( !CHECK_SERIALPORT_OPERABLE(portnum) ) {
		CARP_INVALID_SERIALPORT(portnum);
		return TRUE;
	}
	return !__SP_CONTEXT[portnum-1].serialport->tx.chars;
}

/** Return TRUE if the receive queue is empty
 */
BOOL serialport_rx_empty(int portnum)
{  
	if( !CHECK_SERIALPORT_OPERABLE(portnum) ) {
		CARP_INVALID_SERIALPORT(portnum);
		return TRUE;
	}
	return !__SP_CONTEXT[portnum-1].serialport->rx.chars;
}


/*****************************************************************************
 *                              com_flush_tx()                               *
 *                              com_flush_rx()                               *
 *****************************************************************************
 * DESCRIPTION: Buffer flushers!  These guys just initialize the transmit    *
 *              and receive queues (respectively) to their empty state.      *
 *                                                                           *
 * REVISIONS:   18 OCT 89 - RAC - Translated from the Pascal                 *
 *****************************************************************************/
void serialport_flush_tx( int portnum ) 
{ 
	if( !CHECK_SERIALPORT_OPERABLE(portnum) ) {
		CARP_INVALID_SERIALPORT(portnum);
	} else {
		SERIALPORT_IMPL_PTR sp = __SP_CONTEXT[portnum-1].serialport;
		disable();
		sp->tx.chars 
			= sp->tx.in 
			= sp->tx.out 
			= 0;
		enable();
		sp->err = E_SP_NOERROR;
	}
}

void serialport_flush_rx( int portnum )
{ 
	if( !CHECK_SERIALPORT_OPERABLE(portnum) ) {
		CARP_INVALID_SERIALPORT(portnum);
	} else {
		SERIALPORT_IMPL_PTR sp = __SP_CONTEXT[portnum-1].serialport;
		disable(); 
		sp->rx.chars
			= sp->rx.in
			= sp->rx.out
			= 0;
		enable();
		sp->err = E_SP_NOERROR;
	}
}

/*****************************************************************************
 *                               com_carrier()                               *
 *****************************************************************************
 * DESCRIPTION: Returns TRUE if a carrier is present.                        *
 *                                                                           *
 * REVISIONS:   18 OCT 89 - RAC - Translated from the Pascal.                *
 *****************************************************************************/
BOOL serialport_carrier_detected(int portnum)
{
	if( CHECK_SERIALPORT_OPERABLE(portnum) ) {
		UINT uart_msr;
		uart_msr = __SP_CONTEXT[portnum-1].uart_base + MSR;
		return (inportb(uart_msr) & RLSD);
	}
	CARP_INVALID_SERIALPORT(portnum);
	return FALSE;
}



/*****************************************************************************
 *                   serialport_interrupt_driver_common()                    *
 *****************************************************************************
 * DESCRIPTION: Handles communications interrupts.  The UART will interrupt  *
 *              whenever a character has been received or when it is ready   *
 *              to transmit another character.  This routine responds by     *
 *              sticking received characters into the receive queue and      *
 *              yanking characters to be transmitted from the transmit queue *
 *                                                                           *
 * REVISIOSN:   18 OCT 89 - RAC - Translated from the Pascal.                *
 *****************************************************************************/
void serialport_interrupt_driver_common(int portindex)
{
	char iir;   /* Local copy if IIR */
	char c;     /* Local character variable */
	char cm;    /* MSR register snapshot */

	UINT uart_data
	  , uart_ier
	  , uart_iir
	  , uart_lsr
	  , uart_msr;

    SERIALPORT_IMPL_PTR sp = __SP_CONTEXT[portindex].serialport;

    /* TODO error: must be fatal error ? */
	if( sp == INVALID_SERIALPORT )
		return;

	uart_data = __SP_CONTEXT[portindex].uart_base;    /* Set UART I/O addresses */
	uart_ier  = uart_data + IER;                /*  for the selected comm */
	uart_iir  = uart_data + IIR;                /*  port */
	uart_lsr  = uart_data + LSR;
    uart_msr  = uart_data + MSR;
          
/*  While bit 0 of the IIR is 0, there remains an interrupt to process  */
    while (!((iir = inportb(uart_iir)) & 1)) {  /* While there is an int ... */
        switch (iir) {                          /* Branch on interrupt type */
            case 0:                             /* Modem status interrupt */
                /*inportb(uart_msr);*/          /* Just clear the interrupt */
                break;
            case 2:                             /* Transmit register empty */
/*****************************************************************************
 *  NOTE:  The test of the line status register is to see if the transmit    *
 *         holding register is truly empty.  Some UARTS seem to cause        *
 *         transmit interrupts when the holding register isn't empty,        *
 *         causing transmitted characters to be lost.                        *
 *****************************************************************************/
                if( sp->tx.chars <= 0 ) {          /* If tx buffer empty, turn */
                    outportb(uart_ier, inportb(uart_ier) & ~2); /*  off transmit interrupts */
                } else {                          /* Tx buffer not empty */
                    if (inportb(uart_lsr) & TXR) {
                        outportb(uart_data, sp->tx.queue[sp->tx.out++]);
                        if( sp->tx.out == SERIALPORT_TX_QUEUE_SIZE )
                            sp->tx.out = 0;
                        sp->tx.chars--;
                    }
                }                           /* End 'tx buffer not empty */
                break;
            case 4:                             /* Received data interrupt */
                c = inportb(uart_data);         /* Grab received character */
                cm = inportb(uart_msr);         /* Grab received character */
                if( sp->rx.chars < SERIALPORT_RX_QUEUE_SIZE) { /* If queue not full, save */
                	sp->rx.msr[sp->rx.in] = cm;      /*  the new character */
                    sp->rx.queue[sp->rx.in++] = c;      /*  the new character */
                    if( sp->rx.in == SERIALPORT_RX_QUEUE_SIZE ) /* Wrap index if needed */
                        sp->rx.in = 0;
                    sp->rx.chars++;         /* Count the new character */
                } else {
					sp->err = E_SP_OVERFLOW;
				}
                break;
            case 6:                             /* Line status interrupt */
                inportb(uart_lsr);              /* Just clear the interrupt */
                break;
        }                                   /* End switch */
    }                                       /* End 'is an interrupt' */
    outportb(0x20, 0x20);                       /* Send EOI to 8259 */
}

