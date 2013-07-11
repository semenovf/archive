#include <cwt/test.h>
#include <cwt/reader.hpp>
#include <cwt/file.hpp>

using namespace cwt;

template<>
struct io::Producer<char>
{

}


void test_line_reader()
{
    CWT_TEST_OK(true == true);

    shared_ptr<Reader<char> > reader(new Reader<char>);
    LineReader<char> lineReader(reader);

    ReadLineStatus status;

    while()
    (status = readLine(Vector<char> & line));

}

int main(int argc, char *argv[])
{
    CWT_CHECK_SIZEOF_TYPES;
    CWT_UNUSED2(argc, argv);
    CWT_BEGIN_TESTS(1);

    test_line_reader();

    CWT_END_TESTS;
    return 0;
}
