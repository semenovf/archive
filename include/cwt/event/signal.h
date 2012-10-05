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
	  Cwt_Sig_Hup  = SIGHUP  /* Hangup detected on controlling terminal or death of controlling process */
	, Cwt_Sig_Int  = SIGINT  /* Interrupt from keyboard */
	, Cwt_Sig_Quit = SIGQUIT /* Quit from keyboard */
	, Cwt_Sig_Ill  = SIGILL  /* Illegal Instruction */
	, Cwt_Sig_Abrt = SIGABRT /* Abort signal from abort(3) */
	, Cwt_Sig_Fpe  = SIGFPE  /* Floating point exception */
	, Cwt_Sig_Kill = SIGKILL /* Kill signal */
} CwtSignal;

typedef struct _CwtEventSignalNS {
	CwtEventSource* (*source)        (void);
} CwtEventSignalNS;


EXTERN_C_BEGIN
DLL_API_EXPORT CwtEventSignalNS* cwtEventSignalNS(void);
EXTERN_C_END


#endif /* __CWT_EVENT_SIGNAL_H__ */
