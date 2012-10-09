/*
 * signal.h
 *
 *  Created on: Oct 5, 2012
 *      Author: wladt
 */

#ifndef __CWT_EVENT_SIGNAL_H__
#define __CWT_EVENT_SIGNAL_H__

#include <signal.h>

typedef enum _CwtSignal {
	  Cwt_Sig_Hup       /* Hangup detected on controlling terminal or death of controlling process */
	, Cwt_Sig_Int       /* Interrupt from keyboard */
	, Cwt_Sig_Quit      /* Quit from keyboard */
	, Cwt_Sig_Ill       /* Illegal Instruction */
	, Cwt_Sig_Abrt      /* Abort signal from abort(3) */
	, Cwt_Sig_Fpe       /* Floating point exception */
	, Cwt_Sig_Last
} CwtSignal;

typedef struct _CwtEventSignal
{
	CwtEvent __base;
	CwtSignal signum;
} CwtEventSignal;

typedef struct _CwtEventSignalNS {
	CwtEventSource* (*source)  (void);
	CwtList*        (*handlers)(void);
	BOOL            (*accept)  (CwtSignal signum);
	BOOL            (*raise)   (CwtSignal signum);
	int             (*signum)  (CwtEvent *pevt);
} CwtEventSignalNS;


EXTERN_C_BEGIN
DLL_API_EXPORT CwtEventSignalNS* cwtEventSignalNS(void);
EXTERN_C_END


#endif /* __CWT_EVENT_SIGNAL_H__ */
