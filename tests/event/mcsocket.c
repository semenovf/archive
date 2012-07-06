#include <stdio.h>
#include <cwt/logger.h>
#include <cwt/types.h>
#include <cwt/test.h>
#include <cwt/string.h>
#include <cwt/unistd.h>
#include <cwt/event/event.h>
#include <cwt/event/channel.h>
#include <cwt/io/sockdev.h>


const CWT_CHAR *text = "This is a test message";
const CWT_CHAR *cmdQuit = "quit";

static BOOL on_multicast_receiver(CwtEventPtr pevt)
{
	CwtChannelPtr chan;
	ssize_t br;
	BYTE buf[256];

	cwtEventPeekChannel(pevt, &chan);

	cwtChannelReadBegin(chan);

	br = cwtChannelRead(chan, buf, 255);
	buf[(size_t)br] = '\x0';

	if( cwtStrStr((CWT_CHAR*)buf, cmdQuit) ) {
		printf_trace("finishing event by command, received from channel");
		cwtChannelReadCommit(chan);
		cwtEventQuit();
		return TRUE;
	}
	cwtChannelReadCommit(chan);
	printf_trace("received: \"%s\"", buf);

	return TRUE;
}


int main(int argc, char *argv[])
{
	CwtChannelPtr chan_reader;
	CwtChannelPtr chan_writer;
	CwtEventSourcePtr chan_src;

	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	CWT_BEGIN_TESTS(5);

	chan_writer = cwtNewChannel(cwtMulticastSocketDeviceOpen(
			  _T("192.168.0.198")
			, 4321
			, _T("226.1.1.1")
			, CWT_MCSOCKET_SENDER));

	chan_reader = cwtNewChannel(cwtMulticastSocketDeviceOpen(
			  _T("192.168.0.198")
			, 4321
			, _T("226.1.1.1")
			, CWT_MCSOCKET_RECEIVER));


	CWT_TEST_FAIL(chan_reader);
#ifdef __COMMENT__
	{
		char databuf[1024];
		int datalen = sizeof(databuf);
		/*CwtIODevicePtr sockdev = cwtChannelDevice(chan_reader);*/

		while( TRUE ) {
			if( cwtChannelRead(chan_reader, (BYTE*)databuf, datalen) < 0 ) {
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

	CWT_TEST_OK(chan_writer)
	CWT_TEST_OK(chan_src)
	CWT_TEST_OK(cwtEventRegisterSource(chan_src));
	CWT_TEST_OK(cwtEventIsRegisteredSource(chan_src));

	cwtEventChannelAddListener(chan_reader, on_multicast_receiver);

	cwtChannelWrite(chan_writer, (BYTE*)text, cwtStrLen(text));
	cwtChannelWrite(chan_writer, (BYTE*)cmdQuit, cwtStrLen(cmdQuit));

	cwtEventLoop();

	cwtEventUnregisterAllSources();

	cwtEventChannelRemoveListener(chan_reader);
	cwtDeleteChannel(chan_reader);
	cwtDeleteChannel(chan_writer);

	CWT_END_TESTS;
}


