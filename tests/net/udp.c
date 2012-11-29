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


/*
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
*/
#define NPACK 10


static int __server(void)
{
  CwtSocketNS *socketNS = cwt_socket_ns();
  CwtSocket *server;
  int i;
  BYTE buf[2048];

  CWT_TEST_FAIL(server = socketNS->openUdpServerSocket(NULL, 12012, FALSE));

  for (i = 0; i < NPACK; i++) {
	  CwtSocket *peer;
	  ssize_t br;
	  peer = socketNS->accept(server);

	  CWT_TEST_FAIL(peer);

	  if( peer ) {
		  CWT_CHAR *inetAddr = socketNS->inetAddr(peer);
		  UINT16    inetPort = socketNS->inetPort(peer);

		  br = socketNS->read(server, buf, 2048);

		  cwt_logger_ns()->debug(_Tr("Received packet (%ld) from %s:%u\nData: %s\n\n"), br, inetAddr, inetPort, buf);
		  socketNS->close(peer);
		  CWT_TEST_OK(strcmp(loremipsum, (char*)buf) == 0);
		  CWT_FREE(inetAddr);
	  }
  }

  socketNS->close(server);
  return 0;
}

static int __client(void)
{
	CwtSocketNS *socketNS = cwt_socket_ns();
	CwtSocket *client;
	size_t loremipsum_len;
	int i;

	CWT_TEST_FAIL(client = socketNS->openUdpSocket(_T("localhost"), 12012, FALSE));

	loremipsum_len = strlen(loremipsum);


	for (i = 0; i < NPACK; i++) {
		cwt_logger_ns()->debug(_Tr("Sending packet %d\n"), i);
		CWT_TEST_FAIL(socketNS->write(client, (BYTE*)loremipsum, loremipsum_len+1) > 0);
	}

	socketNS->close(client);

	return 0;
}



int main(int argc, char *argv[])
{
	if( argc > 1 ) {
		if( strcmp(argv[1], "-server") == 0) {
			CWT_BEGIN_TESTS(21);
			__server();
		} else {
			CWT_BEGIN_TESTS(11);
			__client();
		}
		CWT_END_TESTS;
	}

	cwt_logger_ns()->info(_Tr("Run '%s -server' to start server."), argv[0]);
	cwt_logger_ns()->info(_Tr("Run '%s -client' to start client."), argv[0]);
	cwt_logger_ns()->info(_Tr("Note: run server before launching client."));

	return EXIT_FAILURE;
}


