/**
 * @file   textstream.cpp
 * @author wladt
 * @date   Feb 27, 2013 4:54:05 PM
 *
 * @brief
 */


#include <cwt/test.h>
#include <cwt/logger.hpp>
#include <cwt/textstream.hpp>
#include <cwt/fileoutputstream.hpp>
#include <cwt/fileinputstream.hpp>
#include <cwt/bufferedoutputstream.hpp>
#include <cwt/bufferedinputstream.hpp>
#include <cstring>

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

#include <unistd.h>
#include <fcntl.h>
/*
#include <sys/types.h>
#include <sys/stat.h>
*/

int main(int argc, char *argv[])
{
    CWT_CHECK_SIZEOF_TYPES;
    CWT_UNUSED2(argc, argv);
    CWT_BEGIN_TESTS(1);
    Logger::init();
    int fd = ::open("aaaa", O_CREAT | O_WRONLY);
    close(fd);

    FileOutputStream fos(_U("test_textstream.txt"), false);
    BufferedOutputStream *bos = new BufferedOutputStream(&fos);
    CWT_TEST_FAIL(!fos.isNull());
    CWT_TEST_FAIL(!bos->isNull());

    ssize_t nbytes = bos->write(reinterpret_cast<const byte_t*>(loremipsum), strlen(loremipsum));
    CWT_TEST_FAIL(nbytes > 0);
    CWT_TEST_OK((size_t)nbytes == strlen(loremipsum));

    delete bos;
    CWT_END_TESTS;
}

