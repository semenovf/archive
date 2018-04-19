/*
 * esignal.c
 *
 *  Created on: Oct 5, 2012
 *      Author: wladt
 */

#include <errno.h>
#include <cwt/unistd.h>
#include <cwt/str.h>
#include <cwt/logger.h>
#include <cwt/event/event.h>
#include <cwt/event/signal.h>

#ifdef CWT_HAVE_SIGACTION
#	include <signal.h>
#else
#	error Need to support sigaction
#endif


static void __poll(void);
static void __finish(CwtEventSource *source);

static CwtList *__signal_handlers = NULL;

static int  __pipefd[2];
static void __sigaction(int signum) { write(__pipefd[1], &signum, sizeof(int)); }

static BOOL __accept (CwtSignal signum);
static BOOL __raise  (CwtSignal signum);

static CwtListNS    *__listNS = NULL;
static CwtEventNS   *__eventNS = NULL;

static int __mapping_CwtSig_SIG[] = {
	  SIGHUP
	, SIGINT
	, SIGQUIT
	, SIGILL
	, SIGABRT
	, SIGFPE
};

#define _SIGMAP(sn) __mapping_CwtSig_SIG[sn]

static BOOL __accept (CwtSignal signum)
{
	struct sigaction sa;

	sa.sa_handler = __sigaction;
	sa.sa_flags = 0;
	sa.sa_restorer = 0;
	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask, _SIGMAP(signum));

	if (sigaction(_SIGMAP(signum), &sa, NULL) != 0) {
		cwt_logger_ns()->error(_Tr("Failed to set handler for signal: %d"), signum);
		return FALSE;
	} else {
		cwt_logger_ns()->debug(_Tr("Set handler for signal: %d"), signum);
	}
	return TRUE;
}


static BOOL __raise(CwtSignal signum)
{
	return raise(_SIGMAP(signum)) == 0 ? TRUE : FALSE;
}


static void __poll(void)
{
	int signum;
	if( read(__pipefd[0], &signum, sizeof(int)) > 0 ) {
		CwtEventSignal *pevt = CWT_MALLOC(CwtEventSignal);
		pevt->signum = signum;
		__eventNS->initEvent((CwtEvent*)pevt, __signal_handlers, __eventNS->defaultDestructor);
		__eventNS->post((CwtEvent*)pevt);
	}
}


static void __finish(CwtEventSource *source)
{
	CWT_UNUSED(source);

	if( !__signal_handlers ) {
		int i;

		 __listNS->free(__signal_handlers);
		 __signal_handlers = NULL;
		 close(__pipefd[0]);
		 close(__pipefd[1]);

		 for( i = 0; i < Cwt_Sig_Last; i++ ) {
			 signal(i, SIG_DFL);
		 }
	}
}


BOOL __ns_init__(const CWT_CHAR *params)
{
	CWT_UNUSED(params);
	if( !__eventNS ) {
		CwtEventSignalNS *eNS;

		__eventNS = cwt_event_ns();
		__listNS = cwt_list_ns();
		eNS = cwt_event_signal_ns();

		if( pipe(__pipefd) != 0 ) {
			cwt_logger_ns()->error(cwt_str_ns()->strError(errno));
			return FALSE;
		}
		fcntl(__pipefd[0], F_SETFL, fcntl(__pipefd[0], F_GETFL) | O_NONBLOCK);
		fcntl(__pipefd[1], F_SETFL, fcntl(__pipefd[1], F_GETFL) | O_NONBLOCK);

		eNS->accept  = __accept;
		eNS->raise   = __raise;
		eNS->source()->poll = __poll;
		eNS->source()->finish = __finish;
		__signal_handlers = eNS->handlers();
	}

	return TRUE;
}
