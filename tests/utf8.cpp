#include <cwt/test.h>
#include "../include/cwt/io/reader.hpp"
#include "../include/cwt/io/writer.hpp"
#include "../include/cwt/io/buffer.hpp"
#include "../include/cwt/io/file.hpp"
#include "../include/cwt/io/codec_utf8.hpp"
#include <cstring>
#include <cstdio>

using namespace cwt;

struct _FileSpec { const char *path; size_t size; size_t nchars; };

static _FileSpec utf8_ucs2_files[] = {
	  { "rc/utf8/multilang.txt" , 11931, 0 }
	, { "rc/utf8/greek.txt"     ,   345, 0 }
	, { "rc/utf8/mideng.txt"    ,   288, 0 }
	, { "rc/utf8/midger.txt"    ,   372, 0 }
	, { "rc/utf8/rune.txt"      ,   359, 0 }
};

static _FileSpec utf8_ucs4_files[] = {
	  { "rc/utf8/multilang.txt" , 11931, 0 }
	, { "rc/utf8/greek.txt"     ,   345, 0 }
	, { "rc/utf8/mideng.txt"    ,   288, 0 }
	, { "rc/utf8/midger.txt"    ,   372, 0 }
	, { "rc/utf8/rune.txt"      ,   359, 0 }
	, { "rc/utf8/vietnamese.txt",    69, 0 }
	, { "rc/utf8/gothic.txt"    ,   172, 0 }
};


template <typename Decoder, typename Encoder>
void test_Utf8UcsCodec(_FileSpec file_spec[], int nspecs)
{
	typedef io::Reader<io::File, Decoder> Reader;
	typedef io::Reader<io::File, io::NullCodec<char> > BytesReader;
	typedef io::Writer<io::Buffer, Encoder> Writer;

    shared_ptr<io::File> file(new io::File);
    Reader reader(file);
    BytesReader bytesReader(file);

    shared_ptr<io::Buffer> buffer(new io::Buffer);
    Writer writer(buffer);

    for (int i = 0; i < nspecs; i++) {
    	file->open(file_spec[i].path, io::Device::ReadOnly);
    	printf("Test file: %s...\n", file_spec[i].path);
    	CWT_TEST_FAIL(file->opened());
    	CWT_TEST_FAIL(file->size() == file_spec[i].size);

    	typename Reader::vector_type decoded = reader.read(file->size());
    	//CWT_TEST_OK(decoded.size() == file_spec[i].nchars);

        file->close();

        buffer->buffer().clear();
        CWT_TEST_OK(writer.write(decoded) == ssize_t(file_spec[i].size));

        file->open(file_spec[i].path, io::Device::ReadOnly);
        typename BytesReader::vector_type bytes = bytesReader.read(file->size());
        CWT_TEST_OK(bytes.size() == file_spec[i].size);
        file->close();

        CWT_TEST_OK(buffer->buffer().at(0) == bytes.at(0));
        CWT_TEST_OK(buffer->buffer().at(20) == bytes.at(20));
        CWT_TEST_OK(strncmp(buffer->buffer().constData(), bytes.constData(), bytes.size()) == 0);
    }
}

int main(int argc, char *argv[])
{
    CWT_CHECK_SIZEOF_TYPES;
    CWT_UNUSED2(argc, argv);
    CWT_BEGIN_TESTS(48);

    test_Utf8UcsCodec<io::Utf8Ucs2Codec, io::Ucs2Utf8Codec>(utf8_ucs2_files, sizeof(utf8_ucs2_files)/sizeof(utf8_ucs2_files[0]));
    test_Utf8UcsCodec<io::Utf8Ucs4Codec, io::Ucs4Utf8Codec>(utf8_ucs4_files, sizeof(utf8_ucs4_files)/sizeof(utf8_ucs4_files[0]));

    CWT_END_TESTS;
    return 0;
}
