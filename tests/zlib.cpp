/**
 * @file   zlib.cpp
 * @author wladt
 * @date   Apr 25, 2014
 *
 * @brief zlib testing
 */

#include <cwt/test.hpp>
#include <cwt/zlib.hpp>


const char * loremipsum =
"Lorem ipsum dolor sit amet, consectetuer adipiscing elit,    \
sed diam nonummy nibh euismod tincidunt ut laoreet dolore     \
magna aliquam erat volutpat. Ut wisi enim ad minim veniam,    \
quis nostrud exerci tation ullamcorper suscipit lobortis      \
nisl ut aliquip ex ea commodo consequat. Duis autem vel eum   \
iriure dolor in hendrerit in vulputate velit esse molestie    \
consequat, vel illum dolore eu feugiat nulla facilisis at     \
vero eros et accumsan et iusto odio dignissim qui blandit     \
praesent luptatum zzril delenit augue duis dolore te feugait  \
nulla facilisi. Nam liber tempor cum soluta nobis eleifend    \
option congue nihil imperdiet doming id quod mazim placerat   \
facer possim assum. Typi non habent claritatem insitam; est   \
usus legentis in iis qui facit eorum claritatem.              \
Investigationes demonstraverunt lectores legere me lius quod  \
ii legunt saepius. Claritas est etiam processus dynamicus,    \
qui sequitur mutationem consuetudium lectorum. Mirum est      \
notare quam littera gothica, quam nunc putamus parum claram,  \
anteposuerit litterarum formas humanitatis per seacula quarta \
decima et quinta decima. Eodem modo typi, qui nunc nobis      \
videntur parum clari, fiant sollemnes in futurum.";

void test_compress ()
{
	pfs::byte_string src(loremipsum);
	pfs::byte_string uncompressed;
	pfs::byte_string compressed;
	pfs::zlib z;

	TEST_OK(!(compressed = z.compress(src)).isNull());
	TEST_OK(!(uncompressed = z.decompress(::strlen(loremipsum), compressed)).isNull());
	TEST_OK(src == uncompressed);
	TEST_OK(compressed.size() < uncompressed.size());
}

int main(int argc, char *argv[])
{
	PFS_CHECK_SIZEOF_TYPES;
	PFS_UNUSED2(argc, argv);

	BEGIN_TESTS(4);

	test_compress();

	END_TESTS;
}
