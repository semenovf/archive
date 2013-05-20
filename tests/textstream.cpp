/**
 * @file   textstream.cpp
 * @author wladt
 * @date   Feb 27, 2013 4:54:05 PM
 *
 * @brief
 */


#include <cwt/test.h>
#include <cwt/textstream.hpp>
#include <cwt/file.hpp>

using namespace cwt;

static const char *utf8_files[] = {
	  "rc/utf8/UTF-8-test.txt"
	, "rc/utf8/multilang.txt"
	, "rc/utf8/greek.txt"
	, "rc/utf8/gothic.txt"
	, "rc/utf8/mideng.txt"
	, "rc/utf8/midger.txt"
	, "rc/utf8/rune.txt"
	, "rc/utf8/vietnamese.txt"
};

int main(int argc, char *argv[])
{
    CWT_CHECK_SIZEOF_TYPES;
    CWT_UNUSED2(argc, argv);
    CWT_BEGIN_TESTS(21);

    size_t nfiles = sizeof(utf8_files)/sizeof(utf8_files[0]);

    for (size_t i = 0; i < nfiles; i++) {
    	File file;
    	String trace_msg;
    	trace_msg.sprintf(_Tr("Open %s"), utf8_files[i]);
        CWT_TEST_FAIL2(file.open(utf8_files[i], IODevice::ReadOnly), trace_msg.toUtf8().data());

        TextStream ts(file);
        Decoder *utf8Decoder = Decoder::forName("utf8");
        ts.setDecoder(utf8Decoder);
        String s = ts.readAll();

        file.close();

        CWT_TEST_FAIL2(file.open(utf8_files[i], IODevice::ReadOnly), trace_msg.toUtf8().data());
        ByteArray complete = file.readAll();
        file.close();

        CWT_TEST_OK(complete == s.toUtf8());
    }

    Decoder::cleanup();
    Encoder::cleanup();

    CWT_END_TESTS;
}

