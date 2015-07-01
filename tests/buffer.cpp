#include <pfs/test/test.hpp>
#include <pfs/byte_string.hpp>
#include "pfs/io/buffer.hpp"
#include <cstring>
#include <iostream>

using pfs::byte_string;
using std::cerr;
using std::cout;
using std::endl;

char loremipsum[] =
"1.Lorem ipsum dolor sit amet, consectetuer adipiscing elit,    \n\
2.sed diam nonummy nibh euismod tincidunt ut laoreet dolore     \n\
3.magna aliquam erat volutpat. Ut wisi enim ad minim veniam,    \n\
4.quis nostrud exerci tation ullamcorper suscipit lobortis      \n\
5.nisl ut aliquip ex ea commodo consequat. Duis autem vel eum   \n\
6.iriure dolor in hendrerit in vulputate velit esse molestie    \n\
7.consequat, vel illum dolore eu feugiat nulla facilisis at     \n\
8.vero eros et accumsan et iusto odio dignissim qui blandit     \n\
9.praesent luptatum zzril delenit augue duis dolore te feugait  \n\
10.nulla facilisi. Nam liber tempor cum soluta nobis eleifend    \n\
11.option congue nihil imperdiet doming id quod mazim placerat   \n\
12.facer possim assum. Typi non habent claritatem insitam; est   \n\
13.usus legentis in iis qui facit eorum claritatem.              \n\
14.Investigationes demonstraverunt lectores legere me lius quod  \n\
15.ii legunt saepius. Claritas est etiam processus dynamicus,    \n\
16.qui sequitur mutationem consuetudium lectorum. Mirum est      \n\
17.notare quam littera gothica, quam nunc putamus parum claram,  \n\
18.anteposuerit litterarum formas humanitatis per seacula quarta \n\
19.decima et quinta decima. Eodem modo typi, qui nunc nobis      \n\
20.videntur parum clari, fiant sollemnes in futurum.             \n\
21.Lorem ipsum dolor sit amet, consectetuer adipiscing elit,     \n\
22.sed diam nonummy nibh euismod tincidunt ut laoreet dolore     \n\
23.magna aliquam erat volutpat. \"Ut wisi enim ad minim veniam,  \n\
24.quis nostrud exerci tation ullamcorper suscipit lobortis      \n\
25.nisl ut aliquip ex ea commodo consequat. Duis autem vel eum   \n\
26.iriure dolor in hendrerit in vulputate velit esse molestie    \n\
27.consequat, vel illum dolore eu feugiat nulla facilisis at     \n\
28.vero eros et accumsan et iusto odio dignissim qui blandit     \n\
29.praesent luptatum zzril delenit augue duis dolore te feugait  \n\
30.nulla facilisi. Nam liber tempor cum soluta nobis eleifend    \n\
31.option congue nihil imperdiet doming id quod mazim placerat   \n\
32.facer possim assum. Typi non habent claritatem insitam; est   \n\
33.usus legentis in iis qui facit eorum claritatem.              \n\
34.Investigationes demonstraverunt lectores legere me lius quod  \n\
35.ii legunt saepius. Claritas est etiam processus dynamicus,    \n\
36.qui sequitur mutationem consuetudium lectorum. Mirum est      \n\
37.notare quam littera gothica, quam nunc putamus parum claram,  \n\
38.anteposuerit litterarum formas humanitatis per seacula quarta \n\
39.decima et quinta decima.\" Eodem modo typi, qui nunc nobis    \n\
40.videntur parum clari, fiant sollemnes in futurum.";

void test_read ()
{
    pfs::io::buffer buffer;
    TEST_FAIL(buffer.open(reinterpret_cast<byte_t *>(loremipsum), strlen(loremipsum), pfs::io::device::ReadOnly));
    TEST_FAIL(buffer.size() == strlen(loremipsum));

    pfs::io::device & d = buffer;

    TEST_OK(d.isReadable());
    TEST_OK(!d.isWritable());

    TEST_OK(d.write(byte_string()) < 0);

    byte_string result;
    size_t n = 0;

    while (!d.atEnd()) {
        byte_string bytes = buffer.read(n++);
        result.append(bytes);
    }

    TEST_OK(result == loremipsum);
}

void test_write ()
{
    pfs::io::buffer buffer;
    TEST_FAIL(buffer.open(10, pfs::io::device::WriteOnly));

    pfs::io::device & d = buffer;

    TEST_OK(!d.isReadable());
    TEST_OK(d.isWritable());

    char a1[1];
    TEST_OK(d.read(a1, 1) < 0); // is writable only

    TEST_OK(d.write(loremipsum, strlen(loremipsum) + 1) == static_cast<ssize_t>(strlen(loremipsum)) + 1);
    TEST_OK(buffer.size() == strlen(loremipsum) + 1);
 }

int main(int argc, char *argv[])
{
    PFS_UNUSED2(argc, argv);
    BEGIN_TESTS(12);

    test_read();
    test_write();

    END_TESTS;
}
