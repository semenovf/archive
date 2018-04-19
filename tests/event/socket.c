#include <stdio.h>
#include <cwt/logger.h>
#include <cwt/types.h>
#include <cwt/test.h>
#include <cwt/string.h>
#include <cwt/unistd.h>
#include <cwt/event/event.h>
#include <cwt/event/channel.h>
#include <cwt/io/sockdev.h>
#include <string.h>

#ifdef __COMMENT__

const char *text = "This is a test message";
const char *cmdQuit = "quit";

static CwtChannelNS      *__channelNS = NULL;
static CwtEventNS        *__eventNS = NULL;
static CwtEventChannelNS *__eventChannelNS = NULL;


static BOOL on_multicast_receiver(CwtEvent *pevt)
{
	CwtChannelNS *cns = cwt_channel_ns();

	CwtChannel *pchan;
	ssize_t br;
	BYTE buf[256];

	__eventChannelNS->peekChannel(pevt, &pchan);

	br = cns->read(pchan, buf, 255);
	buf[(size_t)br] = '\x0';

	if( strstr((char*)buf, cmdQuit) ) {
		printf("finishing event by command, received from channel");
		__eventNS->quit();
		return TRUE;
	}
	printf("Received: \"%s\"", buf);

	return TRUE;
}
#endif

int main(int argc, char *argv[])
{
#ifdef __COMMENT__
	CwtChannel     *pchan_reader;
	CwtChannel     *pchan_writer;
	CwtEventSource *chan_src;

	__channelNS = cwt_channel_ns();
	__eventNS = cwt_event_ns();
	__eventChannelNS = cwt_event_channel_ns();
#endif


	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	CWT_BEGIN_TESTS(4);

#ifdef __COMMENT__
	pchan_writer = __channelNS->create(cwtMSocketDeviceOpen(
			  _T("192.168.0.198")
			, 4321
			, _T("226.1.1.1")
			, CWT_MCSOCKET_SENDER));

	pchan_reader = __channelNS->create(cwtMSocketDeviceOpen(
			  _T("192.168.0.198")
			, 4321
			, _T("226.1.1.1")
			, CWT_MCSOCKET_RECEIVER));


	CWT_TEST_FAIL(pchan_reader);

	chan_src = __eventChannelNS->source();

	CWT_TEST_OK(pchan_writer)
	CWT_TEST_OK(chan_src)
	__eventNS->registerSource(chan_src);
	CWT_TEST_OK(__eventNS->isRegisteredSource(chan_src));

	__eventChannelNS->addListener(pchan_reader, on_multicast_receiver);

	__channelNS->write(pchan_writer, (BYTE*)text, strlen(text));
	__channelNS->write(pchan_writer, (BYTE*)cmdQuit, strlen(cmdQuit));

	__eventNS->loop();

	__eventNS->unregisterAllSources();

	__eventChannelNS->removeListener(pchan_reader);
	__channelNS->free(pchan_reader);
	__channelNS->free(pchan_writer);
#endif
	CWT_END_TESTS;
}


