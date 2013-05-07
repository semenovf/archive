/**
 * @file   file.cpp
 * @author wladt
 * @date   Apr 16, 2013 10:50:05
 *
 * @brief
 */


#include <cwt/test.h>
#include <cwt/bytearray.hpp>
#include <cwt/file.hpp>

using namespace cwt;

const char *loremipsum =
"Lorem ipsum dolor sit amet, consectetuer adipiscing elit,    \n\
sed diam nonummy nibh euismod tincidunt ut laoreet dolore     \n\
magna aliquam erat volutpat. Ut wisi enim ad minim veniam,    \n\
quis nostrud exerci tation ullamcorper suscipit lobortis      \n\
nisl ut aliquip ex ea commodo consequat. Duis autem vel eum   \n\
iriure dolor in hendrerit in vulputate velit esse molestie    \n\
consequat, vel illum dolore eu feugiat nulla facilisis at     \n\
vero eros et accumsan et iusto odio dignissim qui blandit     \n\
praesent luptatum zzril delenit augue duis dolore te feugait  \n\
nulla facilisi. Nam liber tempor cum soluta nobis eleifend    \n\
option congue nihil imperdiet doming id quod mazim placerat   \n\
facer possim assum. Typi non habent claritatem insitam; est   \n\
usus legentis in iis qui facit eorum claritatem.              \n\
Investigationes demonstraverunt lectores legere me lius quod  \n\
ii legunt saepius. Claritas est etiam processus dynamicus,    \n\
qui sequitur mutationem consuetudium lectorum. Mirum est      \n\
notare quam littera gothica, quam nunc putamus parum claram,  \n\
anteposuerit litterarum formas humanitatis per seacula quarta \n\
decima et quinta decima. Eodem modo typi, qui nunc nobis      \n\
videntur parum clari, fiant sollemnes in futurum.             \n\
Lorem ipsum dolor sit amet, consectetuer adipiscing elit,     \n\
sed diam nonummy nibh euismod tincidunt ut laoreet dolore     \n\
magna aliquam erat volutpat. \"Ut wisi enim ad minim veniam,  \n\
quis nostrud exerci tation ullamcorper suscipit lobortis      \n\
nisl ut aliquip ex ea commodo consequat. Duis autem vel eum   \n\
iriure dolor in hendrerit in vulputate velit esse molestie    \n\
consequat, vel illum dolore eu feugiat nulla facilisis at     \n\
vero eros et accumsan et iusto odio dignissim qui blandit     \n\
praesent luptatum zzril delenit augue duis dolore te feugait  \n\
nulla facilisi. Nam liber tempor cum soluta nobis eleifend    \n\
option congue nihil imperdiet doming id quod mazim placerat   \n\
facer possim assum. Typi non habent claritatem insitam; est   \n\
usus legentis in iis qui facit eorum claritatem.              \n\
Investigationes demonstraverunt lectores legere me lius quod  \n\
ii legunt saepius. Claritas est etiam processus dynamicus,    \n\
qui sequitur mutationem consuetudium lectorum. Mirum est      \n\
notare quam littera gothica, quam nunc putamus parum claram,  \n\
anteposuerit litterarum formas humanitatis per seacula quarta \n\
decima et quinta decima.\" Eodem modo typi, qui nunc nobis    \n\
videntur parum clari, fiant sollemnes in futurum.";


#define FILENAME	"test_file.txt"

int main(int argc, char *argv[])
{
    CWT_CHECK_SIZEOF_TYPES;
    CWT_UNUSED2(argc, argv);
    CWT_BEGIN_TESTS(9);

    File file;
    CWT_TEST_FAIL(file.open(FILENAME, IODevice::WriteOnly)); //file.lastErrorText().toUtf8().data());
    CWT_TEST_OK(file.write(loremipsum, strlen(loremipsum)) == (ssize_t)strlen(loremipsum));
    file.close();

    CWT_TEST_OK(File::setPermissions(FILENAME, File::ReadUser | File::WriteUser));

    CWT_TEST_FAIL(file.open(FILENAME, IODevice::ReadOnly));
    CWT_TEST_OK(file.available() == strlen(loremipsum));

    char buf[32];
    size_t totalbytes = 0;
    ssize_t rc;
    ByteArray ba;
    while ((rc = file.read(buf, sizeof(buf))) > 0) {
    	ba.append(buf, rc);
    	totalbytes += rc;
    }

    CWT_TEST_OK(rc == 0);
    CWT_TEST_OK(totalbytes == strlen(loremipsum));
    CWT_TEST_OK(ba.length() == totalbytes);
    CWT_TEST_OK(strcmp(loremipsum, ba.data()) == 0);
    file.close();

    CWT_END_TESTS;
}

