#include <cwt/test.h>
#include "../include/cwt/io/reader.hpp"
#include "../include/cwt/io/writer.hpp"
#include "../include/cwt/io/buffer.hpp"
#include "../include/cwt/io/file.hpp"
#include "../include/cwt/io/textreader.hpp"
#include "../include/cwt/io/datareader.hpp"
#include <cstring>
#include <cstdio>

using namespace cwt;

struct _FileSpec { const char *path; const char *opath; size_t size; size_t nchars; };

static _FileSpec utf8_ucs2_files[] = {
	  { "rc/utf8/multilang.txt" , nullptr, 11931, 9356 }
	, { "rc/utf8/cyrillic.txt"  , nullptr,   132,   66 }
	, { "rc/utf8/greek.txt"     , nullptr,   338,  204 }
	, { "rc/utf8/mideng.txt"    , nullptr,   288,  278 }
	, { "rc/utf8/midger.txt"    , nullptr,   372,  362 }
	, { "rc/utf8/rune.txt"      , nullptr,   359,  147 }
};

static _FileSpec utf8_ucs4_files[] = {
	  { "rc/utf8/multilang.txt" , nullptr, 11931, 9356 }
	, { "rc/utf8/cyrillic.txt"  , nullptr,   132,   66 }
	, { "rc/utf8/greek.txt"     , nullptr,   338,  204 }
	, { "rc/utf8/mideng.txt"    , nullptr,   288,  278 }
	, { "rc/utf8/midger.txt"    , nullptr,   372,  362 }
	, { "rc/utf8/rune.txt"      , nullptr,   359,  147 }
	, { "rc/utf8/vietnamese.txt", nullptr,    69,   43 }
	, { "rc/utf8/gothic.txt"    , nullptr,   172,   84 }
};


template <typename Decoder, typename Encoder>
void test_Utf8UcsCodec(_FileSpec file_spec[], int nspecs)
{
	typedef io::Reader<io::File, Decoder> Reader;
	typedef io::Reader<io::File, io::NullCodec<char> > BytesReader;
	typedef io::Writer<io::Buffer, Encoder> BufferWriter;
	typedef io::Writer<io::File, Encoder> FileWriter;

    io::File file;
    Reader reader(&file);
    BytesReader bytesReader(&file);

    io::Buffer buffer;
    BufferWriter writer(&buffer);

    io::File ofile;
    FileWriter fileWriter(ofile);

    for (int i = 0; i < nspecs; i++) {
    	file.open(file_spec[i].path, io::Device::ReadOnly);

    	printf("Test file: %s...\n", file_spec[i].path);
    	CWT_TEST_FAIL(file.opened());

    	CWT_TEST_FAIL(file.size() == file_spec[i].size);

    	typename Reader::vector_type decoded = reader.read(file.size());
    	CWT_TEST_OK(decoded.size() == file_spec[i].nchars);

    	if (file_spec[i].opath) {
    		ofile.open(file_spec[i].opath, io::Device::WriteOnly);
    		CWT_ASSERT(ofile.opened());
    		fileWriter.write(decoded);
    		ofile.close();
    	}

        file.close();

        buffer.buffer().clear();
        CWT_TEST_OK(writer.write(decoded) == ssize_t(file_spec[i].size));

        file.open(file_spec[i].path, io::Device::ReadOnly);
        typename BytesReader::vector_type bytes = bytesReader.read(file.size());
        CWT_TEST_OK(bytes.size() == file_spec[i].size);
        file.close();

        CWT_TEST_OK(strncmp(buffer.buffer().constData(), bytes.constData(), bytes.size()) == 0);
    }
}

void test_Utf8Codec(_FileSpec file_spec[], int nspecs, size_t chunkSize)
{
	io::File file;
	io::TextReader textReader(& file, new io::Utf8Utf8Converter, chunkSize);
    io::DataReader dataReader(& file, chunkSize);

    for (int i = 0; i < nspecs; i++) {
    	file.open(file_spec[i].path, io::Device::ReadOnly);

    	printf("test_Utf8Codec: test file: %s...\n", file_spec[i].path);
    	CWT_TEST_FAIL(file.opened());
    	CWT_TEST_FAIL(file.size() == file_spec[i].size);

    	String utf8 = textReader.read(file.size());
    	CWT_TEST_OK(utf8.size() == file_spec[i].size);
    	CWT_TEST_OK(utf8.length() == file_spec[i].nchars);
    	file.close();

    	file.open(file_spec[i].path, io::Device::ReadOnly);
    	ByteArray raw = dataReader.read(file.size());
    	CWT_TEST_OK(raw.size() == file_spec[i].size);
    	file.close();

        CWT_TEST_OK(raw.size() == utf8.size());
        CWT_TEST_OK(strncmp(utf8.constData(), raw.constData(), raw.size()) == 0);
    }
}

int main(int argc, char *argv[])
{
    CWT_CHECK_SIZEOF_TYPES;
    CWT_UNUSED2(argc, argv);
    CWT_BEGIN_TESTS(14084);

   	test_Utf8UcsCodec<io::Utf8Ucs2Codec, io::Ucs2Utf8Codec>(utf8_ucs2_files, sizeof(utf8_ucs2_files)/sizeof(utf8_ucs2_files[0]));
   	test_Utf8UcsCodec<io::Utf8Ucs4Codec, io::Ucs4Utf8Codec>(utf8_ucs4_files, sizeof(utf8_ucs4_files)/sizeof(utf8_ucs4_files[0]));
   	for (size_t i = 6; i < 256; ++i)
   		test_Utf8Codec(utf8_ucs4_files, sizeof(utf8_ucs4_files)/sizeof(utf8_ucs4_files[0]), i);

    CWT_END_TESTS;
    return 0;
}
