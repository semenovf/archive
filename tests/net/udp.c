#include <stdio.h>
#include <string.h>
#include <cwt/types.h>
#include <cwt/test.h>
#include <cwt/str.h>
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
const char *loremipsum1 =
"Lorem ipsum dolor sit amet, consectetuer adipiscing elit,\n\
sed diam nonummy nibh euismod tincidunt ut laoreet dolore\n\
magna aliquam erat volutpat. Ut wisi enim ad minim veniam,\n\
quis nostrud exerci tation ullamcorper suscipit lobortis\n\
nisl ut aliquip ex ea commodo consequat. Duis autem vel eum";
*/


int main(int argc, char *argv[])
{
	CwtSocketNS *socketNS = cwtSocketNS();
	CwtSocket *server;
	CwtSocket *client;
	BYTE buf[2048];
	size_t loremipsum_len;
	size_t counter;
	ssize_t br;


	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	CWT_BEGIN_TESTS(2);

	CWT_TEST_FAIL(server = socketNS->openUdpSocket(TRUE));
	CWT_TEST_FAIL(client = socketNS->openUdpSocket(FALSE));

	CWT_TEST_FAIL(socketNS->listen(server, _T("localhost"), 12012));
	CWT_TEST_FAIL(socketNS->connect(client, _T("localhost"), 12012));

	loremipsum_len = cwtStrNS()->strlen(loremipsum);
	CWT_TEST_FAIL(socketNS->write(client, (BYTE*)loremipsum, loremipsum_len) > 0);

	br = 0;
	counter = 0;
	CWT_TEST_FAIL((br = socketNS->read(server, buf+counter, 2048-counter)) >= 0);

	while( br >= 0 ) {
		br = socketNS->read(server, buf+counter, 2048-counter);
		if( br <= 0 )
			break;
		counter += (size_t)br;
	}

	CWT_TEST_OK(br >= 0);

	socketNS->close(client);
	socketNS->close(server);
	CWT_END_TESTS;
}


