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

static BOOL on_multicast_receiver(CwtEventPtr pevt)
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
		printf("finishing event by command, received from channel");
		cns->readCommit(pchan);
		cwtEventQuit();
		return TRUE;
	}
	cns->readCommit(pchan);
	printf("Received: \"%s\"", buf);

	return TRUE;
}


int main(int argc, char *argv[])
{
	CwtChannelNS *cns = cwtChannelNS();

	CwtChannel *pchan_reader;
	CwtChannel *pchan_writer;
	CwtEventSourcePtr chan_src;

	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	CWT_BEGIN_TESTS(5);

	pchan_writer = cns->create(cwtMulticastSocketDeviceOpen(
			  _T("192.168.0.198")
			, 4321
			, _T("226.1.1.1")
			, CWT_MCSOCKET_SENDER));

	pchan_reader = cns->create(cwtMulticastSocketDeviceOpen(
			  _T("192.168.0.198")
			, 4321
			, _T("226.1.1.1")
			, CWT_MCSOCKET_RECEIVER));


	CWT_TEST_FAIL(pchan_reader);
#ifdef __COMMENT__
	{
		char databuf[1024];
		int datalen = sizeof(databuf);
		/*CwtIODevicePtr sockdev = cwtChannelDevice(chan_reader);*/

		while( TRUE ) {
			if( cwtChannelRead(pchan_reader, (BYTE*)databuf, datalen) < 0 ) {
				print_error("Reading datagram message error");
				return;
			} else {
				print_trace("Reading datagram message...OK.\n");
				printf_debug("The message from multicast server is: \"%s\"\n", databuf);
			}
			cwtMsleep(500);
		}
	}
#endif

	chan_src = cwtEventChannelSource();

	CWT_TEST_OK(pchan_writer)
	CWT_TEST_OK(chan_src)
	CWT_TEST_OK(cwtEventRegisterSource(chan_src));
	CWT_TEST_OK(cwtEventIsRegisteredSource(chan_src));

	cwtEventChannelAddListener(pchan_reader, on_multicast_receiver);

	cns->write(pchan_writer, (BYTE*)text, strlen(text));
	cns->write(pchan_writer, (BYTE*)cmdQuit, strlen(cmdQuit));

	cwtEventLoop();

	cwtEventUnregisterAllSources();

	cwtEventChannelRemoveListener(pchan_reader);
	cns->free(pchan_reader);
	cns->free(pchan_writer);

	CWT_END_TESTS;
}


