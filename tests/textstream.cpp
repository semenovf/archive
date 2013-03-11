/**
 * @file   textstream.cpp
 * @author wladt
 * @date   Feb 27, 2013 4:54:05 PM
 *
 * @brief
 */


#include <cwt/test.h>
#include <cwt/logger.hpp>
#include <cwt/filestream.hpp>
#include <cwt/inputstreamreader.hpp>

using namespace cwt;

static const char *utf8_files[] = {
	  "rc/utf8/greek.txt"
	, "rc/utf8/gothic.txt"
	, "rc/utf8/mideng.txt"
	, "rc/utf8/midger.txt"
	, "rc/utf8/rune.txt"
	, "rc/utf8/vietnamese.txt"
	, "rc/utf8/multilang.txt"
};



int main(int argc, char *argv[])
{
    CWT_CHECK_SIZEOF_TYPES;
    CWT_UNUSED2(argc, argv);
    CWT_BEGIN_TESTS(1);
    Logger::init();

    size_t nfiles = sizeof(utf8_files)/sizeof(utf8_files[0]);

    for (size_t i = 0; i < nfiles; i++) {
    	FileInputStream fis(utf8_files[i]);
    	CWT_TEST_FAIL(!fis.isNull());

    	InputStreamReader reader(fis, "UTF-8");
    	CWT_TEST_FAIL(!reader.isNull());

    	String s = reader.readAll();
    	CWT_TEST_OK(!s.isNull());

    	printf("%s", s.toUtf8().data());

    	reader.close();
    	fis.close();
    }

    CWT_END_TESTS;
}

