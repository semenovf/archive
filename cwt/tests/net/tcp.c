#include <stdio.h>
#include <string.h>
#include <cwt/types.h>
#include <cwt/test.h>
#include <cwt/str.h>
#include <cwt/logger.h>
#include <cwt/net/socket.h>

const char *loremipsum =
"Lorem ipsum dolor sit amet, consectetuer adipiscing elit,    \n\
sed diam nonummy nibh euismod tincidunt ut laoreet dolore     \n\r\
magna aliquam erat volutpat. Ut wisi enim ad minim veniam,    \n\
quis nostrud exerci tation ullamcorper suscipit lobortis      \r\n\
nisl ut aliquip ex ea commodo consequat. Duis autem vel eum   \n\
iriure dolor in hendrerit in vulputate velit esse molestie    \n\
consequat, vel illum dolore eu feugiat nulla facilisis at     \n\
vero eros et accumsan et iusto odio dignissim qui blandit     \n\
praesent luptatum zzril delenit augue duis dolore te feugait  \n\
nulla facilisi. Nam liber tempor cum soluta nobis eleifend    \n\
option congue nihil imperdiet doming id quod mazim placerat   \n\r\r\
facer possim assum. Typi non habent claritatem insitam; est   \n\
usus legentis in iis qui facit eorum claritatem.              \n\
Investigationes demonstraverunt lectores legere me lius quod  \n\
ii legunt saepius. Claritas est etiam processus dynamicus,    \n\
qui sequitur mutationem consuetudium lectorum. Mirum est      \n\
notare quam littera gothica, quam nunc putamus parum claram,  \n\
anteposuerit litterarum formas humanitatis per seacula quarta \n\
decima et quinta decima. Eodem modo typi, qui nunc nobis      \n\
videntur parum clari, fiant sollemnes in futurum.";


#define NPACK 10
#define _BUFSZ 2048

static int __server(void)
{
	CwtSocketNS *socketNS = cwt_socket_ns();

	int        i;
	CwtSocket *server;
	BYTE       buf[_BUFSZ];
	CwtSocket *peer;
	CWT_CHAR  *inetAddr;
	UINT16     inetPort;
	ssize_t    br;

	CWT_TEST_FAIL(server = socketNS->openTcpServerSocket(NULL, 12012, FALSE));
	CWT_TEST_FAIL(peer = socketNS->accept(server));

	inetAddr = socketNS->inetAddr(peer);
	inetPort = socketNS->inetPort(peer);

	for( i = 0; i < NPACK; i++ ) {
		while( !socketNS->bytesAvailable(peer) )
			{;}
		CWT_TEST_FAIL((br = socketNS->read(peer, buf, _BUFSZ)) > 0);
		cwt_logger_ns()->debug(_Tr("Received packet from %s:%u\nData: %s\n\n"), inetAddr, inetPort, buf);
		CWT_TEST_OK(strcmp(loremipsum, (char*)buf) == 0);
		CWT_TEST_FAIL(socketNS->write(peer, buf, _BUFSZ) > 0);
	}

	CWT_FREE(inetAddr);
	socketNS->close(peer);
	socketNS->close(server);
	return 0;
}

static int __client(void)
{
	CwtSocketNS *socketNS = cwt_socket_ns();

	int        i;
	CwtSocket *client;
	BYTE       buf[_BUFSZ];
	size_t     loremipsum_len;
	ssize_t    bw;

	CWT_TEST_FAIL(client = socketNS->openTcpSocket(_T("localhost"), 12012, FALSE));

	loremipsum_len = strlen(loremipsum);

	for( i = 0; i < NPACK; i++ ) {
		cwt_logger_ns()->debug(_Tr("Sending packet..."));
		CWT_TEST_FAIL(socketNS->write(client, (BYTE*)loremipsum, loremipsum_len+1) > 0);

		while( !socketNS->bytesAvailable(client) )
			{;}
		CWT_TEST_FAIL((bw = socketNS->read(client, buf, _BUFSZ)) > 0);
		cwt_logger_ns()->debug(_Tr("Received packet\nData: %s\n\n"), buf);
	}

	socketNS->close(client);

	return 0;
}



int main(int argc, char *argv[])
{
	if( argc > 1 ) {
		if( strcmp(argv[1], "-server") == 0) {
			CWT_BEGIN_TESTS(32);
			__server();
		} else {
			CWT_BEGIN_TESTS(21);
			__client();
		}
		CWT_END_TESTS;
	}

	cwt_logger_ns()->info(_Tr("Run '%s -server' to start server."), argv[0]);
	cwt_logger_ns()->info(_Tr("Run '%s -client' to start client."), argv[0]);
	cwt_logger_ns()->info(_Tr("Note: run server before launching client."));

	return EXIT_FAILURE;
}


