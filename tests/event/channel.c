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

static CwtChannelNS      *__channelNS = NULL;
static CwtEventNS        *__eventNS = NULL;
static CwtEventChannelNS *__eventChannelNS = NULL;


static BOOL quit_event(CwtEvent *pevt)
{
	CwtChannel *pchan;
	ssize_t br;
	BYTE buf[256];

	__eventChannelNS->peekChannel(pevt, &pchan);

	__channelNS->readBegin(pchan);

	br = __channelNS->read(pchan, buf, 255);
	buf[(size_t)br] = '\x0';

	if( strstr((char*)buf, cmdQuit) ) {
		printf_trace(_T("finishing event by command, received from channel"));
		__channelNS->readCommit(pchan);
		__eventNS->quit();
		return TRUE;
	}

	__channelNS->readRollback(pchan);

	return FALSE;
}


static BOOL echo(CwtEvent *pevt)
{
	CwtChannel *chan;
	ssize_t br;
	BYTE buf[256];

	__eventChannelNS->peekChannel(pevt, &chan);

	while( (br = __channelNS->read(chan, buf, 255)) > 0 ) {
		buf[(size_t)br] = '\x0';
		printf("Received: %s\n", buf);

		CWT_TEST_OK(strcmp((char*)buf, text) == 0);

		__channelNS->write(chan, buf, (size_t)br);
	}
	__channelNS->write(chan, (BYTE*)cmdQuit, strlen(cmdQuit));

	return FALSE;
}

static void cwt_evt_test_0(void)
{
	CwtChannel  *pchan_reader;
	CwtChannel  *pchan_writer;
	CwtEventSource *event_src;

	pchan_reader = __channelNS->create(cwtBufferDeviceOpen());
	pchan_writer = __channelNS->create(cwtBufferDeviceOpenPeer(__channelNS->device(pchan_reader)));
	event_src    = __eventChannelNS->source();

	__eventNS->registerSource(event_src);

	__eventChannelNS->addListener(pchan_reader, echo);
	__eventChannelNS->addListener(pchan_reader, quit_event);
	__eventChannelNS->addListener(pchan_writer, echo);
	__eventChannelNS->addListener(pchan_writer, quit_event);

	__channelNS->write(pchan_writer, (BYTE*)text, strlen(text));

	__eventNS->loop();

	__eventNS->unregisterAllSources();

	__eventChannelNS->removeListener(pchan_reader);
	__channelNS->free(pchan_reader);
	__channelNS->free(pchan_writer);
}

int main(int argc, char *argv[])
{
	__channelNS = cwtChannelNS();
	__eventNS = cwtEventNS();
	__eventChannelNS = cwtEventChannelNS();

	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	CWT_BEGIN_TESTS(1);

	cwt_evt_test_0();

	CWT_END_TESTS;
}


