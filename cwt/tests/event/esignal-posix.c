#include <stdio.h>
#include <cwt/logger.h>
#include <cwt/types.h>
#include <cwt/test.h>
#include <cwt/dl.h>
#include <cwt/event/event.h>
#include <cwt/event/signal.h>

static int ntimes = 0;

static CwtEventNS        *__eventNS = NULL;
static CwtEventSignalNS  *__eventSignalNS = NULL;


static void __idle_proc(void)
{
	__eventSignalNS->raise(Cwt_Sig_Hup);
	__eventSignalNS->raise(Cwt_Sig_Int);
	__eventSignalNS->raise(Cwt_Sig_Quit);
}


static BOOL on_signal(CwtEvent *pevt)
{
	int signum;

	signum = __eventSignalNS->signum(pevt);

	switch(__eventSignalNS->signum(pevt)) {
	case Cwt_Sig_Hup:
		CWT_TEST_OK(Cwt_Sig_Hup == signum);
		break;
	case Cwt_Sig_Int:
		CWT_TEST_OK(Cwt_Sig_Int == signum);
		break;
	case Cwt_Sig_Quit:
		ntimes++;
		CWT_TEST_OK(Cwt_Sig_Quit == signum);
		break;
	default:
		break;
	}

	if( ntimes >= 10 )
		__eventNS->quit();

	return FALSE;
}


int main(int argc, char *argv[])
{
	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	CWT_BEGIN_TESTS(35);

	__eventNS = cwt_event_ns();
	__eventSignalNS = cwt_event_signal_ns();

	CWT_TEST_FAIL(cwt_dl_ns()->loadNS(_T("event:signal-posix")));

	__eventNS->setIdleProccess(__idle_proc);
	__eventNS->registerSource(__eventSignalNS->source());
	CWT_TEST_OK(__eventNS->isRegisteredSource(__eventSignalNS->source()));

	__eventNS->pushHandler(__eventSignalNS->handlers(), on_signal);
	CWT_TEST_OK(__eventSignalNS->accept(Cwt_Sig_Quit));
	CWT_TEST_OK(__eventSignalNS->accept(Cwt_Sig_Hup));
	CWT_TEST_OK(__eventSignalNS->accept(Cwt_Sig_Int));

	__eventNS->loop();

	CWT_END_TESTS;
}


