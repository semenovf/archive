#include <pfs/test.hpp>
#include <pfs/safeformat.hpp>
#include "pfs/io/reader.hpp"
#include "pfs/io/writer.hpp"
#include "pfs/io/file.hpp"
#include "pfs/io/buffer.hpp"
#include "pfs/io/datareader.hpp"
#include "pfs/io/datawriter.hpp"
#include <cstring>
#include <cstdio>

#include <iostream> // TODO Remove

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

const char * loremipsum_lines[] = {
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
	pfs::io::buffer buffer;
	pfs::io::data_writer writer(buffer);
	pfs::byte_string ba(loremipsum, strlen(loremipsum));
	ssize_t written = writer.write(ba);
    TEST_FAIL(ssize_t(strlen(loremipsum)) == written);
    TEST_OK(buffer.data().size() == strlen(loremipsum));
	TEST_OK(strncmp(buffer.data().data(), loremipsum, strlen(loremipsum)) == 0);
}

void test_line_reader()
{
	cwt::io::buffer buffer;
	buffer.write(loremipsum, strlen(loremipsum));
	TEST_FAIL(buffer.available() == strlen(loremipsum));

	cwt::io::data_reader lineReader(buffer);

	size_t nlines = sizeof(loremipsum_lines)/sizeof(loremipsum_lines[0]);
	size_t iline = 0;

	pfs::byte_string r;

	while (!(r = lineReader.readLine(pfs::byte_string(1, '\n'), 80)).isEmpty()) {
		if (r.endsWith(pfs::byte_string(1, '\n'))) {
			TEST_OK(strncmp(loremipsum_lines[iline], r.constData(), r.size() - 1) == 0);
		} else {
			TEST_OK(strncmp(loremipsum_lines[iline], r.constData(), r.size()) == 0);
		}
		++iline;
	}

	TEST_OK(lineReader.atEnd());
	TEST_OK(!lineReader.isError());

	TEST_OK2(iline == nlines, pfs::string(_Fr("All lines are checked (%d from %d)") % iline % nlines).c_str());
}

void test_reader_iterator()
{
	size_t nlines = sizeof(loremipsum_lines)/sizeof(loremipsum_lines[0]);

	for (size_t iline = 0; iline < nlines; ++iline) {
		pfs::vector<char> r;

		cwt::io::buffer buffer;
		buffer.write(loremipsum_lines[iline], strlen(loremipsum_lines[iline]));
		TEST_FAIL(buffer.available() == strlen(loremipsum_lines[iline]));

		cwt::io::data_reader charReader(buffer);

		cwt::io::data_reader::iterator it(charReader);
		cwt::io::data_reader::iterator itEnd;

		while (it != itEnd) {
			r.append(*it++);
		}
		r.append(0);
		TEST_OK(strncmp(loremipsum_lines[iline], r.constData(), r.size()) == 0);
	}
}

void test_reader_iterator_ext()
{
	const char * str = "Lorem ipsum dolor sit amet,";
	cwt::io::buffer buffer;
	buffer.write(str, strlen(str));
	TEST_FAIL(buffer.available() == strlen(str));

	cwt::io::data_reader charReader(buffer);

	cwt::io::data_reader::iterator it(charReader);
	cwt::io::data_reader::iterator itEnd;

	TEST_OK(*it   == 'L');
	TEST_OK(*it++ == 'L');
	TEST_OK(*it++ == 'o');
	TEST_OK(*it++ == 'r');
	TEST_OK(*it   == 'e');
	TEST_OK(*++it == 'm');
	it += 8;
	TEST_OK(*it++ == 'd');
	TEST_OK(*it++ == 'o');
	TEST_OK(*it++ == 'l');
	TEST_OK(*it++ == 'o');
	TEST_OK(*it++ == 'r');
	TEST_OK(*it++ == ' ');

	TEST_OK(*it == 's');
	TEST_OK(*it.at(0) == 's');
	TEST_OK(*it.at(1) == 'i');
	TEST_OK(*it.at(2) == 't');
	TEST_OK(*it == 's');
	TEST_OK(*it.at(8) == ',');
	it += 8;
	TEST_OK(*it == ',');
	++it;
	//it += 100;
	TEST_OK(it == itEnd);
}

void test_file ()
{
	cwt::io::file file;
	pfs::string unknownPath("!@#$%");
	file.open(unknownPath, cwt::io::device::ReadOnly);

	TEST_FAIL(!file.opened());
	file.logErrors();
}

int main(int argc, char *argv[])
{
    PFS_UNUSED2(argc, argv);
    BEGIN_TESTS(149);

    if(1) test_line_reader();
	if(1) test_reader_iterator();
	if(1) test_reader_iterator_ext();
	if(1) test_writer();
	if(1) test_file();
//  test_mix_get_read(); // TODO

    END_TESTS;
}
