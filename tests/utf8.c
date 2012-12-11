/**
 * @file   utf8.c
 * @author wladt
 * @date   Dec 11, 2012 1:27:46 PM
 *
 * @brief
 */


#include "cwt/test.h"
#include "cwt/txtcodec.h"
#include "cwt/fs.h"
#include "cwt/logger.h"
#include "cwt/io/channel.h"
#include "cwt/io/filedev.h"

/*
#ifdef CWT_CC_MSC
#	include <tchar.h>
#	ifdef __cplusplus
#		include <xstring>
#	endif
#endif
*/
#include <wchar.h>
#include <wctype.h>

static const CWT_CHAR* utf8_sample_files[] = {
	  _T("mideng.txt")
	, _T("midger.txt")
	, _T("greek.txt")
	, _T("rune.txt")
	, _T("multilang.txt")
};

int main(int argc, char *argv[])
{
	CwtByteArrayNS *ba_ns = cwt_bytearray_ns();
	int i, nfiles;
	CwtChannel    *chan;
	CwtByteArray  ba;

	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	CWT_BEGIN_TESTS(1);

	nfiles = sizeof(utf8_sample_files)/sizeof(utf8_sample_files[0]);
	ba_ns->init(&ba);

	for (i = 0; i < nfiles; i++) {
		CWT_CHAR *path = cwt_filesystem_ns()->buildPath(
			  _T("..")
			, _T("..")
			, _T("..")
			, _T("rc")
			, _T("utf8")
			, utf8_sample_files[i]
			, NULL);

		do {
			chan = cwt_channel_ns()->open(cwt_open_file(path, Cwt_FileRead));
			if (!chan)
				break;

			cwt_bytearray_ns()->clear(&ba);

			if (cwt_channel_ns()->slurp(chan, &ba)) {
				wchar_t *uc;
				char    *utf8;

				uc   = cwt_decode_utf8((char*)ba_ns->data(&ba), CWT_CAST_SIZE_T(ba_ns->size(&ba)));
				CWT_TEST_FAIL(uc);

				utf8 = cwt_encode_utf8(uc, wcslen(uc));
				CWT_TEST_FAIL(utf8);

				CWT_TEST_OK(strlen(utf8) == ba_ns->size(&ba));
				CWT_TEST_OK(strncmp(utf8, (char*)ba_ns->data(&ba), ba_ns->size(&ba)) == 0);

				CWT_FREE(uc);
				CWT_FREE(utf8);
			}

			cwt_channel_ns()->close(chan);

			break;
		} while (TRUE);

		CWT_FREE(path);
		path = NULL;
	}

	cwt_bytearray_ns()->destroy(&ba);

	CWT_END_TESTS;
}
