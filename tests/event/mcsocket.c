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

const char *text = "This is a test message";
const char *cmdQuit = "quit";

static CwtChannelNS      *__channelNS = NULL;
static CwtEventNS        *__eventNS = NULL;
static CwtEventChannelNS *__eventChannelNS = NULL;


static BOOL on_multicast_receiver(CwtEvent *pevt)
{
	CwtChannelNS *cns = cwtChannelNS();

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


int main(int argc, char *argv[])
{
	CwtChannel     *pchan_reader;
	CwtChannel     *pchan_writer;
	CwtEventSource *chan_src;

	__channelNS = cwtChannelNS();
	__eventNS = cwtEventNS();
	__eventChannelNS = cwtEventChannelNS();

	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	CWT_BEGIN_TESTS(4);

	pchan_writer = __channelNS->create(cwtMcastSocketDeviceOpen(
			  _T("192.168.0.198")
			, 4321
			, _T("226.1.1.1")
			, Cwt_SocketDevice_MSender));

	pchan_reader = __channelNS->create(cwtMcastSocketDeviceOpen(
			  _T("192.168.0.198")
			, 4321
			, _T("226.1.1.1")
			, Cwt_SocketDevice_MReceiver));


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

	CWT_END_TESTS;
}


