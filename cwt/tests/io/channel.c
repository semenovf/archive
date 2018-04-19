#include <stdio.h>
#include <string.h>
#include <cwt/types.h>
#include <cwt/test.h>
#include <cwt/bytearr.h>
#include <cwt/io/channel.h>
#include <cwt/io/bufdev.h>


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


const char *loremipsum1 =
"Lorem ipsum dolor sit amet, consectetuer adipiscing elit,\n\
sed diam nonummy nibh euismod tincidunt ut laoreet dolore\n\
magna aliquam erat volutpat. Ut wisi enim ad minim veniam,\n\
quis nostrud exerci tation ullamcorper suscipit lobortis\n\
nisl ut aliquip ex ea commodo consequat. Duis autem vel eum";


int main(int argc, char *argv[])
{
	CwtChannelNS   *chNS = cwt_channel_ns();
	CwtByteArrayNS *baNS = cwt_bytearray_ns();
	CwtChannel    *pchan0;
	CwtChannel    *pchan1;
	CwtByteArray  *ba;
	int nlines    = 0;

	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	CWT_BEGIN_TESTS(2);

	CWT_ASSERT(pchan0 = chNS->open(cwtBufferDeviceOpen()));
	CWT_ASSERT(pchan1 = chNS->open(cwtBufferDeviceOpenPeer(chNS->device(pchan0))));
	CWT_ASSERT(ba = baNS->create());


	chNS->write(pchan0, (BYTE*)"Line 1\n", 7);
	chNS->write(pchan0, (BYTE*)"Line 2\r", 7);
	chNS->write(pchan0, (BYTE*)"Line 3\n\r", 8);
	chNS->write(pchan0, (BYTE*)"Line 4\r\n", 8);
	chNS->write(pchan0, (BYTE*)"Line 5", 6);
	chNS->write(pchan0, (BYTE*)loremipsum, strlen(loremipsum));

	nlines = 0;
	while( !chNS->atEnd(pchan1) ) {
		baNS->clear(ba);
		if( chNS->readLine(pchan1, ba) ) {
			printf("length=%u: \"%s\"\n", baNS->size(ba), baNS->cstr(ba));
		}
		nlines++;
	}

	CWT_TEST_OK(nlines == 28);

	baNS->clear(ba);
	chNS->write(pchan0, (BYTE*)loremipsum1, strlen(loremipsum1));
	while( !chNS->atEnd(pchan1) ) {
		if( chNS->readLine(pchan1, ba) ) {
			baNS->appendElem(ba, '\n');
		}
	}

	/* Remove trailing new line symbol */
	baNS->resize(ba, baNS->size(ba)-1);
	CWT_TEST_OK(strcmp(baNS->cstr(ba), loremipsum1) == 0);
	printf("%s\n", baNS->cstr(ba));

	baNS->free(ba);
	chNS->close(pchan0);
	chNS->close(pchan1);

	CWT_END_TESTS;
}


