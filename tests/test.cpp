#include <cwt/test.h>
#include <cwt/safeformat.hpp>
#include "../include/cwt/io/reader.hpp"
#include "../include/cwt/io/writer.hpp"
#include "../include/cwt/io/file.hpp"
#include "../include/cwt/io/buffer.hpp"
#include "../include/cwt/io/datareader.hpp"
#include <cstring>
#include <cstdio>

using namespace std;
using namespace cwt;

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


const char *loremipsum_lines[] = {
   "1.Lorem ipsum dolor sit amet, consectetuer adipiscing elit,    "
,  "2.sed diam nonummy nibh euismod tincidunt ut laoreet dolore     "
,  "3.magna aliquam erat volutpat. Ut wisi enim ad minim veniam,    "
,  "4.quis nostrud exerci tation ullamcorper suscipit lobortis      "
,  "5.nisl ut aliquip ex ea commodo consequat. Duis autem vel eum   "
,  "6.iriure dolor in hendrerit in vulputate velit esse molestie    "
,  "7.consequat, vel illum dolore eu feugiat nulla facilisis at     "
,  "8.vero eros et accumsan et iusto odio dignissim qui blandit     "
,  "9.praesent luptatum zzril delenit augue duis dolore te feugait  "
,  "10.nulla facilisi. Nam liber tempor cum soluta nobis eleifend    "
,  "11.option congue nihil imperdiet doming id quod mazim placerat   "
,  "12.facer possim assum. Typi non habent claritatem insitam; est   "
,  "13.usus legentis in iis qui facit eorum claritatem.              "
,  "14.Investigationes demonstraverunt lectores legere me lius quod  "
,  "15.ii legunt saepius. Claritas est etiam processus dynamicus,    "
,  "16.qui sequitur mutationem consuetudium lectorum. Mirum est      "
,  "17.notare quam littera gothica, quam nunc putamus parum claram,  "
,  "18.anteposuerit litterarum formas humanitatis per seacula quarta "
,  "19.decima et quinta decima. Eodem modo typi, qui nunc nobis      "
,  "20.videntur parum clari, fiant sollemnes in futurum.             "
,  "21.Lorem ipsum dolor sit amet, consectetuer adipiscing elit,     "
,  "22.sed diam nonummy nibh euismod tincidunt ut laoreet dolore     "
,  "23.magna aliquam erat volutpat. \"Ut wisi enim ad minim veniam,  "
,  "24.quis nostrud exerci tation ullamcorper suscipit lobortis      "
,  "25.nisl ut aliquip ex ea commodo consequat. Duis autem vel eum   "
,  "26.iriure dolor in hendrerit in vulputate velit esse molestie    "
,  "27.consequat, vel illum dolore eu feugiat nulla facilisis at     "
,  "28.vero eros et accumsan et iusto odio dignissim qui blandit     "
,  "29.praesent luptatum zzril delenit augue duis dolore te feugait  "
,  "30.nulla facilisi. Nam liber tempor cum soluta nobis eleifend    "
,  "31.option congue nihil imperdiet doming id quod mazim placerat   "
,  "32.facer possim assum. Typi non habent claritatem insitam; est   "
,  "33.usus legentis in iis qui facit eorum claritatem.              "
,  "34.Investigationes demonstraverunt lectores legere me lius quod  "
,  "35.ii legunt saepius. Claritas est etiam processus dynamicus,    "
,  "36.qui sequitur mutationem consuetudium lectorum. Mirum est      "
,  "37.notare quam littera gothica, quam nunc putamus parum claram,  "
,  "38.anteposuerit litterarum formas humanitatis per seacula quarta "
,  "39.decima et quinta decima.\" Eodem modo typi, qui nunc nobis    "
,  "40.videntur parum clari, fiant sollemnes in futurum." };


void test_writer()
{
	typedef io::Writer<io::Buffer, io::NullCodec<ByteArray> > Writer;

	shared_ptr<io::Buffer> buffer(new io::Buffer);
    Writer writer(buffer);
    CWT_TEST_FAIL(ssize_t(strlen(loremipsum)) == writer.write(ByteArray(loremipsum, strlen(loremipsum))));
    CWT_TEST_OK(buffer->buffer().size() == strlen(loremipsum));
	CWT_TEST_OK(strncmp(buffer->buffer().data(), loremipsum, strlen(loremipsum)) == 0);
}

void test_line_reader()
{
	typedef io::Reader<io::Buffer, io::NullCodec<ByteArray> > Reader;

	shared_ptr<io::Buffer> buffer(new io::Buffer);
	buffer->write(loremipsum, strlen(loremipsum));
	CWT_TEST_FAIL(buffer->available() == strlen(loremipsum));

	Reader lineReader(buffer);

	size_t nlines = sizeof(loremipsum_lines)/sizeof(loremipsum_lines[0]);
	size_t iline = 0;

	ByteArray r;
	while (!(r = lineReader.readLine(ByteArray(1, '\n'), 80)).isEmpty()) {
		if (r.endsWith(ByteArray::EndOfLine)) {
			CWT_TEST_OK(strncmp(loremipsum_lines[iline], r.constData(), r.size() - ByteArray::EndOfLine.size()) == 0);
		} else {
			CWT_TEST_OK(strncmp(loremipsum_lines[iline], r.constData(), r.size()) == 0);
		}
		++iline;
	}

	CWT_TEST_OK(lineReader.atEnd());
	CWT_TEST_OK(!lineReader.isError());

	CWT_TEST_OK2(iline == nlines, String(_Fr("All lines are checked (%d from %d)") % iline % nlines).c_str());
}

void test_reader_iterator()
{
	typedef io::Reader<io::Buffer, io::NullCodec<ByteArray> > Reader;

	size_t nlines = sizeof(loremipsum_lines)/sizeof(loremipsum_lines[0]);

	for (size_t iline = 0; iline < nlines; ++iline) {
		Vector<char> r;

		shared_ptr<io::Buffer> buffer(new io::Buffer);
		buffer->write(loremipsum_lines[iline], strlen(loremipsum_lines[iline]));
		CWT_TEST_FAIL(buffer->available() == strlen(loremipsum_lines[iline]));

		Reader charReader(buffer);

		Reader::iterator it(charReader);
		Reader::iterator itEnd;

		while (it != itEnd) {
			r.append(*it++);
		}
		r.append(0);
		CWT_TEST_OK(strncmp(loremipsum_lines[iline], r.constData(), r.size()) == 0);
	}
}

void test_reader_iterator_ext()
{
	typedef io::Reader<io::Buffer, io::NullCodec<ByteArray> > Reader;

	const char * str = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit";
	shared_ptr<io::Buffer> buffer(new io::Buffer);
	buffer->write(str, strlen(str));
	CWT_TEST_FAIL(buffer->available() == strlen(str));

	Reader charReader(buffer);

	Reader::iterator it(charReader);
	Reader::iterator itEnd;

	CWT_TEST_OK(*it++ == 'L');
	CWT_TEST_OK(*it++ == 'o');
	CWT_TEST_OK(*it++ == 'r');
	CWT_TEST_OK(*it   == 'e');
	CWT_TEST_OK(*++it == 'm');
	it += 8;
	CWT_TEST_OK(*it++ == 'd');
	CWT_TEST_OK(*it++ == 'o');
	CWT_TEST_OK(*it++ == 'l');
	CWT_TEST_OK(*it++ == 'o');
	CWT_TEST_OK(*it++ == 'r');
	CWT_TEST_OK(*it++ == ' ');

	CWT_TEST_OK(*it == 's');
	CWT_TEST_OK(*it.at(0) == 's');
	CWT_TEST_OK(*it.at(1) == 'i');
	CWT_TEST_OK(*it.at(2) == 't');
	CWT_TEST_OK(*it == 's');

	it += 100;
	CWT_TEST_OK(it == itEnd);
}

int main(int argc, char *argv[])
{
    CWT_CHECK_SIZEOF_TYPES;
    CWT_UNUSED2(argc, argv);
    CWT_BEGIN_TESTS(145);

   	test_line_reader();
    test_reader_iterator();
    test_reader_iterator_ext();
    test_writer();

//  test_mix_get_read(); // TODO

    CWT_END_TESTS;
    return 0;
}
