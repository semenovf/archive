#include <cwt/test.h>
#include <cwt/io/reader.hpp>
#include <cwt/io/file.hpp>

using namespace cwt;

static const char *filename = "test_file.txt";

CWT_NS_BEGIN

namespace io {
template <>
struct Codec<char, char>
{
	Vector<char> convert(const Vector<char> & inBuffer, ssize_t * nonconverted) { CWT_UNUSED(nonconverted); return inBuffer; }
};

template <>
struct Filter<char>
{
	Vector<char> process(const Vector<char> & input, bool * ok) { CWT_UNUSED(ok); return input; }
};

}
CWT_NS_END

void test_line_reader()
{
    shared_ptr<io::File> file(new io::File());
    CWT_TEST_NOK(file->opened());

    file->open(filename);
    CWT_TEST_FAIL(file->opened());

    io::Reader<io::File, char> reader(file);
    reader.read(256);
    file->close();
}

int main(int argc, char *argv[])
{
    CWT_CHECK_SIZEOF_TYPES;
    CWT_UNUSED2(argc, argv);
    CWT_BEGIN_TESTS(2);

    test_line_reader();

    CWT_END_TESTS;
    return 0;
}
