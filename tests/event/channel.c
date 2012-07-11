#include <stdio.h>
#include <cwt/logger.h>
#include <cwt/types.h>
#include <cwt/test.h>
#include <cwt/str.h>
#include <cwt/event/event.h>
#include <cwt/event/channel.h>
#include <cwt/io/bufdev.h>

const char *text    = "This is a test message";
const char *cmdQuit = "quit";

static BOOL quit_event(CwtEventPtr pevt)
{
	CwtChannelPtr chan;
	ssize_t br;
	BYTE buf[256];

	cwtEventPeekChannel(pevt, &chan);

	cwtChannelReadBegin(chan);

	br = cwtChannelRead(chan, buf, 255);
	buf[(size_t)br] = '\x0';

	if( strstr((char*)buf, cmdQuit) ) {
		printf_trace(_T("finishing event by command, received from channel"));
		cwtChannelReadCommit(chan);
		cwtEventQuit();
		return TRUE;
	}

	cwtChannelReadRollback(chan);

	return FALSE;
}


static BOOL echo(CwtEventPtr pevt)
{
	CwtChannelPtr chan;
	ssize_t br;
	BYTE buf[256];

	cwtEventPeekChannel(pevt, &chan);

	while( (br = cwtChannelRead(chan, buf, 255)) > 0 ) {
		buf[(size_t)br] = '\x0';
		printf("Received: %s\n", buf);

		CWT_TEST_OK(strcmp((char*)buf, text) == 0);

		cwtChannelWrite(chan, buf, (size_t)br);
	}
	cwtChannelWrite(chan, (BYTE*)cmdQuit, strlen(cmdQuit));

	return FALSE;
}

static void cwt_evt_test_0(void)
{
	CwtChannelPtr chan_reader;
	CwtChannelPtr chan_writer;
	CwtEventSourcePtr event_src;

	chan_reader = cwtNewChannel(cwtBufferDeviceOpen());
	chan_writer = cwtNewChannel(cwtBufferDeviceOpenPeer(cwtChannelDevice(chan_reader)));
	event_src   = cwtEventChannelSource();

	CWT_TEST_OK(cwtEventRegisterSource(event_src));

	cwtEventChannelAddListener(chan_reader, echo);
	cwtEventChannelAddListener(chan_reader, quit_event);
	cwtEventChannelAddListener(chan_writer, echo);
	cwtEventChannelAddListener(chan_writer, quit_event);

	cwtChannelWrite(chan_writer, (BYTE*)text, strlen(text));

	cwtEventLoop();

	cwtEventUnregisterAllSources();

	cwtEventChannelRemoveListener(chan_reader);
	cwtDeleteChannel(chan_reader);
	cwtDeleteChannel(chan_writer);
}

int main(int argc, char *argv[])
{
	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	CWT_BEGIN_TESTS(2);

	cwt_evt_test_0();

	CWT_END_TESTS;
}


