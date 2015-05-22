#include <cwt/test.hpp>
#include "../include/cwt/io/reader.hpp"
#include "../include/cwt/io/writer.hpp"
#include "../include/cwt/io/buffer.hpp"
#include "../include/cwt/io/file.hpp"
#include "../include/cwt/io/textreader.hpp"
#include "../include/cwt/io/datareader.hpp"
#include <cstring>
#include <cstdio>

struct _FileSpec { const char * path; const char * opath; size_t size; size_t nchars; };

#ifdef __COMMENT__
static _FileSpec utf8_ucs2_files[] = {
	  { "rc/utf8/multilang.txt" , nullptr, 11931, 9356 }
	, { "rc/utf8/cyrillic.txt"  , nullptr,   132,   66 }
	, { "rc/utf8/greek.txt"     , nullptr,   338,  204 }
	, { "rc/utf8/mideng.txt"    , nullptr,   288,  278 }
	, { "rc/utf8/midger.txt"    , nullptr,   372,  362 }
	, { "rc/utf8/rune.txt"      , nullptr,   359,  147 }
};
#endif

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

#ifdef __COMMENT__
template <typename Decoder, typename Encoder>
void test_Utf8UcsCodec(_FileSpec file_spec[], int nspecs)
{
	typedef io::Reader<io::File, Decoder> Reader;
	typedef io::Writer<io::Buffer, Encoder> BufferWriter;
	typedef io::Writer<io::File, Encoder> FileWriter;

    Reader reader(shared_ptr<io::File>(new io::File));
    io::DataReader dataReader(shared_ptr<io::File>(new io::File));

    BufferWriter bufferWriter(shared_ptr<io::Buffer>(new io::Buffer));
    FileWriter fileWriter(shared_ptr<io::File>(new io::File));

    for (int i = 0; i < nspecs; i++) {
    	reader.producer()->open(file_spec[i].path, io::Device::ReadOnly);

    	printf("Test file: %s...\n", file_spec[i].path);
    	CWT_TEST_FAIL(reader.producer()->opened());
    	CWT_TEST_FAIL(reader.producer()->size() == file_spec[i].size);

    	typename Reader::vector_type decoded = reader.read(file.size());
    	CWT_TEST_OK(decoded.size() == file_spec[i].nchars);

    	if (file_spec[i].opath) {
    		fileWriter.consumer()->open(file_spec[i].opath, io::Device::WriteOnly);
    		CWT_ASSERT(fileWriter.consumer()->opened());
    		fileWriter.write(decoded);
    		fileWriter.consumer()->close();
    	}

        file.close();

        bufferWriter.consumer()->buffer().clear();
        CWT_TEST_OK(bufferWriter.write(decoded) == ssize_t(file_spec[i].size));

        file.open(file_spec[i].path, io::Device::ReadOnly);
        typename BytesReader::vector_type bytes = dataReader.read(file.size());
        CWT_TEST_OK(bytes.size() == file_spec[i].size);
        file.close();

        CWT_TEST_OK(strncmp(bufferWriter.consumer()->buffer().constData(), bytes.constData(), bytes.size()) == 0);
    }
}
#endif



void test_Utf8Codec(_FileSpec file_spec[], int nspecs)
{
    for (int i = 0; i < nspecs; i++) {
    	cwt::io::file file;
    	file.open(_l1(file_spec[i].path), cwt::io::device::ReadOnly);

    	printf("test_Utf8Codec: test file: %s...\n", file_spec[i].path);
    	TEST_FAIL(file.opened());
    	TEST_FAIL(file.size() == file_spec[i].size);

    	cwt::io::text_reader textReader(file);
    	cwt::io::data_reader dataReader(file);

    	pfs::string utf8 = textReader.read(file.size());
    	TEST_OK(utf8.sizeInBytes() == file_spec[i].size);
    	TEST_OK(utf8.length() == file_spec[i].nchars);

    	file.rewind();

    	pfs::bytearray raw = dataReader.read(file.size());
    	TEST_OK(raw.size() == file_spec[i].size);

        TEST_OK(raw.size() == utf8.sizeInBytes());
        TEST_OK(strncmp(utf8.constData(), raw.constData(), raw.size()) == 0);

    	//file->close();
    }
}

int main(int argc, char *argv[])
{
    PFS_UNUSED2(argc, argv);
    BEGIN_TESTS(56);

#ifdef __COMMENT__
   	test_Utf8UcsCodec<io::Ucs2Encoder, io::Ucs2Decoder>(utf8_ucs2_files, sizeof(utf8_ucs2_files)/sizeof(utf8_ucs2_files[0]));
   	test_Utf8UcsCodec<io::Ucs4Encoder, io::Ucs4Decoder>(utf8_ucs4_files, sizeof(utf8_ucs4_files)/sizeof(utf8_ucs4_files[0]));
#endif
	test_Utf8Codec(utf8_ucs4_files, sizeof(utf8_ucs4_files)/sizeof(utf8_ucs4_files[0]));

    END_TESTS;
    return 0;
}
