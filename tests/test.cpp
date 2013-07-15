#include <cwt/test.h>
#include <cwt/io/reader.hpp>
#include <cwt/io/file.hpp>
#include <cstring>

using namespace std;
using namespace cwt;

static const char *filename = "test_file.txt";
const char *loremipsum =
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

struct Codec
{
	typedef char orig_char_type;
	typedef char dest_char_type;
	ssize_t convert(char output[], size_t osize, const char input[], size_t isize, size_t * remain)
	{
		size_t n = CWT_MIN(osize, isize);
		CWT_ASSERT( n > 0 && n <= CWT_SSIZE_MAX);
		memcpy(output, input, sizeof(char) * n);
		*remain = 0;
		return ssize_t(n);
	}
};

void test_line_reader()
{
    shared_ptr<io::File> file(new io::File());
    CWT_TEST_NOK(file->opened());

    file->open(filename);
    CWT_TEST_FAIL(file->opened());

    io::Reader<io::File, Codec> reader(file);
    Vector<char> bytes = reader.read(strlen(loremipsum));
    CWT_TEST_OK(bytes.size() == strlen(loremipsum));
    CWT_TEST_OK(strncmp(bytes.data(), loremipsum, strlen(loremipsum)) == 0);
    file->close();


#ifdef __COMMENT__
    file->open(filename);
    CWT_TEST_FAIL(file->opened());
    char ch;
    while (reader.get(ch)) {
    	printf("%c", ch);
    }
    printf("\n");
    file->close();


    file->open(filename);
    CWT_TEST_FAIL(file->opened());
    io::LineReader<io::Reader<io::File, Codec> > lineReader(&reader);
    lineReader.setEndLine(Vector<char>("\n", 1));

    Vector<char> line;
    io::ReadLineStatus rc = io::ReadLine_Success;

    while (rc >= 0) {
    	rc = lineReader.readLine(line);

    	if (rc == io::ReadLine_Success) {
    		line.append(0);
    		printf("LINE: \"%s\"\n", line.data());
    		line.clear();
    	} else if (rc == io::ReadLine_AtEnd) {
    		printf("LINE: \"%s\"\n", line.data());
    		break;
    	}
    }

	file->close();
#endif
}

int main(int argc, char *argv[])
{
    CWT_CHECK_SIZEOF_TYPES;
    CWT_UNUSED2(argc, argv);
    CWT_BEGIN_TESTS(4);

    test_line_reader();

    CWT_END_TESTS;
    return 0;
}
