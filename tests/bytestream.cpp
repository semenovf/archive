/**
 * @file   textstream.cpp
 * @author wladt
 * @date   Feb 27, 2013 4:54:05 PM
 *
 * @brief
 */


#include <cwt/test.h>
#include <cwt/logger.hpp>
#include <cwt/fileoutputstream.hpp>
#include <cwt/fileinputstream.hpp>
#include <cwt/bufferedstream.hpp>
#include <cwt/bytearraystream.hpp>
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

static const String filename(_U("test_textstream.txt"));
static const int _BUF_SZ = 32;


void test_bytearraystream()
{
	ByteArray loremipsum_bytes(loremipsum, strlen(loremipsum));
	ByteArray result;
	ByteArrayOutputStream os(result);
    CWT_TEST_FAIL(!os.isNull());
	os.write(loremipsum, strlen(loremipsum));
	CWT_TEST_OK(result == loremipsum_bytes);

    result.clear();

    {
		ByteArrayInputStream is(loremipsum_bytes);
		CWT_TEST_FAIL(!is.isNull());
		char buf[_BUF_SZ+1];
		ssize_t nbytes;
		while ((nbytes = is.read(buf, _BUF_SZ)) > 0) {
			result.append(buf, nbytes);
		}
		CWT_TEST_OK(nbytes == 0);
		CWT_TEST_OK(result.size() == strlen(loremipsum));
		CWT_TEST_OK(result.size() == loremipsum_bytes.size());
		CWT_TEST_OK(result == loremipsum_bytes);
    }

    result.clear();

    {
    	ByteArray sample_bytes(loremipsum + 100, 200);
		ByteArrayInputStream is(loremipsum_bytes, 100, 200);
		CWT_TEST_FAIL(!is.isNull());
		char buf[_BUF_SZ+1];
		ssize_t nbytes;
		while ((nbytes = is.read(buf, _BUF_SZ)) > 0) {
			result.append(buf, nbytes);
		}
		printf("result.size() = %u\n", result.size());
		printf("sample_bytes.size() = %u\n", sample_bytes.size());
		CWT_TEST_OK(nbytes == 0);
		CWT_TEST_OK(result.size() == sample_bytes.size());
		CWT_TEST_OK(result == sample_bytes);
    }
}

int main(int argc, char *argv[])
{
    CWT_CHECK_SIZEOF_TYPES;
    CWT_UNUSED2(argc, argv);
    CWT_BEGIN_TESTS(21);
    Logger::init();

    FileOutputStream fos(filename, false);
    BufferedOutputStream bos(fos);
    CWT_TEST_FAIL(!fos.isNull());
    CWT_TEST_FAIL(!bos.isNull());

    ssize_t nbytes = bos.write(loremipsum, strlen(loremipsum));
    CWT_TEST_FAIL(nbytes > 0);
    CWT_TEST_OK((size_t)nbytes == strlen(loremipsum));

    bos.close();

    FileInputStream fis(filename);
    BufferedInputStream bis(fis, _BUF_SZ);
    CWT_TEST_FAIL(!fis.isNull());
    CWT_TEST_FAIL(!bis.isNull());

    char buf[_BUF_SZ+1];
    ByteArray bytes;
    while ((nbytes = bis.read(buf, _BUF_SZ)) > 0) {
    	bytes.append(buf, nbytes);
    	buf[nbytes] = '\0';
    	printf("%s", buf);
    }

    printf("\n");
    Logger::debug("bytes.size() = %d", bytes.size());
    Logger::debug("strlen(loremipsum) = %d", strlen(loremipsum));
    CWT_TEST_OK(bytes.size() == strlen(loremipsum));
    CWT_TEST_OK(bytes == ByteArray(loremipsum, strlen(loremipsum)));

    FileInputStream fis_entire(filename);
    bytes.clear();
    bytes = fis_entire.readAll();
    CWT_TEST_OK(bytes.size() == strlen(loremipsum));
    CWT_TEST_OK(bytes == ByteArray(loremipsum, strlen(loremipsum)));


    test_bytearraystream();

    CWT_END_TESTS;
}

