#include <stdio.h>
#include <cwt/logger.h>
#include <cwt/types.h>
#include <cwt/test.h>
#include <cwt/str.h>
#include <cwt/event/event.h>
#include <cwt/event/channel.h>
#include <cwt/io/bufdev.h>
#include <string.h>

const char *text    = "This is a test message";
const char *cmdQuit = "quit";

static BOOL quit_event(CwtEventPtr pevt)
{
	CwtChannelNS *cns = cwtChannelNS();

	CwtChannel *pchan;
	ssize_t br;
	BYTE buf[256];

	cwtEventPeekChannel(pevt, &pchan);

	cns->readBegin(pchan);

	br = cns->read(pchan, buf, 255);
	buf[(size_t)br] = '\x0';

	if( strstr((char*)buf, cmdQuit) ) {
		printf_trace(_T("finishing event by command, received from channel"));
		cns->readCommit(pchan);
		cwtEventQuit();
		return TRUE;
	}

	cns->readRollback(pchan);

	return FALSE;
}


static BOOL echo(CwtEventPtr pevt)
{
	CwtChannelNS *cns = cwtChannelNS();

	CwtChannel *chan;
	ssize_t br;
	BYTE buf[256];

	cwtEventPeekChannel(pevt, &chan);

	while( (br = cns->read(chan, buf, 255)) > 0 ) {
		buf[(size_t)br] = '\x0';
		printf("Received: %s\n", buf);

		CWT_TEST_OK(strcmp((char*)buf, text) == 0);

		cns->write(chan, buf, (size_t)br);
	}
	cns->write(chan, (BYTE*)cmdQuit, strlen(cmdQuit));

	return FALSE;
}

static void cwt_evt_test_0(void)
{
	CwtChannelNS *cns = cwtChannelNS();

	CwtChannel  *pchan_reader;
	CwtChannel  *pchan_writer;
	CwtEventSourcePtr event_src;

	pchan_reader = cns->create(cwtBufferDeviceOpen());
	pchan_writer = cns->create(cwtBufferDeviceOpenPeer(cns->device(pchan_reader)));
	event_src   = cwtEventChannelSource();

	CWT_TEST_OK(cwtEventRegisterSource(event_src));

	cwtEventChannelAddListener(pchan_reader, echo);
	cwtEventChannelAddListener(pchan_reader, quit_event);
	cwtEventChannelAddListener(pchan_writer, echo);
	cwtEventChannelAddListener(pchan_writer, quit_event);

	cns->write(pchan_writer, (BYTE*)text, strlen(text));

	cwtEventLoop();

	cwtEventUnregisterAllSources();

	cwtEventChannelRemoveListener(pchan_reader);
	cns->free(pchan_reader);
	cns->free(pchan_writer);
}

int main(int argc, char *argv[])
{
	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	CWT_BEGIN_TESTS(2);

	cwt_evt_test_0();

	CWT_END_TESTS;
}


