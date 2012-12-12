/**
 * @file   utf8.c
 * @author wladt
 * @date   Dec 11, 2012 1:27:46 PM
 *
 * @brief
 */


#include "cwt/test.h"
#include "cwt/txtcodec.h"
#include "cwt/unicode.h"
#include "cwt/fs.h"
#include "cwt/logger.h"
#include "cwt/io/channel.h"
#include "cwt/io/filedev.h"

#include <wchar.h>
#include <wctype.h>

#if CWT_SIZEOF_WCHAR == 4
#	define _NTESTS 76
#else
#	define _NTESTS 74
#endif

static const CWT_CHAR* utf8_ucs2_sample_files[] = {
	  _T("mideng.txt")
	, _T("midger.txt")
	, _T("greek.txt")
	, _T("rune.txt")
	, _T("multilang.txt")
};

static const CWT_CHAR* utf8_ucs4_sample_files[] = {
	  _T("mideng.txt")
	, _T("midger.txt")
	, _T("greek.txt")
	, _T("rune.txt")
	, _T("multilang.txt")
	, _T("gothic.txt")
	, _T("vietnamese.txt")
};


void test_ucs2(void)
{
	CwtChannelNS   *chan_ns = cwt_channel_ns();
	CwtByteArrayNS *ba_ns = cwt_bytearray_ns();
	CwtUnicodeNS   *uc_ns = cwt_unicode_ns();

	int i, nfiles;
	CwtChannel    *chan;
	CwtByteArray  ba;

	nfiles = sizeof(utf8_ucs2_sample_files)/sizeof(utf8_ucs2_sample_files[0]);
	ba_ns->init(&ba);

	for (i = 0; i < nfiles; i++) {
		CWT_CHAR *path = cwt_filesystem_ns()->buildPath(
			  _T("..")
			, _T("..")
			, _T("..")
			, _T("rc")
			, _T("utf8")
			, utf8_ucs2_sample_files[i]
			, NULL);

		cwt_logger_ns()->trace(_T("Testing %s ..."), utf8_ucs2_sample_files[i]);

		do {
			chan = chan_ns->open(cwt_open_file(path, Cwt_FileRead));
			if (!chan)
				break;

			ba_ns->clear(&ba);

			if (chan_ns->slurp(chan, &ba)) {
				CWT_UCS2 *ucs2;
				char     *utf8;

				ucs2   = uc_ns->decodeUtf8ToUcs2((char*)ba_ns->data(&ba), CWT_CAST_SIZE_T(ba_ns->size(&ba)));
				CWT_TEST_FAIL(ucs2);

				utf8 = uc_ns->encodeUcs2ToUtf8(ucs2, uc_ns->ucs2len(ucs2));
				CWT_TEST_FAIL(utf8);

				CWT_TEST_OK(strlen(utf8) == ba_ns->size(&ba));
				CWT_TEST_OK(strncmp(utf8, (char*)ba_ns->data(&ba), ba_ns->size(&ba)) == 0);

				CWT_FREE(ucs2);
				CWT_FREE(utf8);
			}

			chan_ns->close(chan);

			break;
		} while (TRUE);

		CWT_FREE(path);
		path = NULL;
	}

	ba_ns->destroy(&ba);
}


void test_ucs4(void)
{
	CwtChannelNS   *chan_ns = cwt_channel_ns();
	CwtByteArrayNS *ba_ns = cwt_bytearray_ns();
	CwtUnicodeNS   *uc_ns = cwt_unicode_ns();

	int i, nfiles;
	CwtChannel    *chan;
	CwtByteArray  ba;

	nfiles = sizeof(utf8_ucs4_sample_files)/sizeof(utf8_ucs4_sample_files[0]);
	ba_ns->init(&ba);

	for (i = 0; i < nfiles; i++) {
		CWT_CHAR *path = cwt_filesystem_ns()->buildPath(
			  _T("..")
			, _T("..")
			, _T("..")
			, _T("rc")
			, _T("utf8")
			, utf8_ucs4_sample_files[i]
			, NULL);

		cwt_logger_ns()->trace(_T("Testing %s ..."), utf8_ucs4_sample_files[i]);

		do {
			chan = chan_ns->open(cwt_open_file(path, Cwt_FileRead));
			if (!chan)
				break;

			ba_ns->clear(&ba);

			if (chan_ns->slurp(chan, &ba)) {
				CWT_UCS4 *ucs4;
				char     *utf8;

				ucs4   = uc_ns->decodeUtf8ToUcs4((char*)ba_ns->data(&ba), CWT_CAST_SIZE_T(ba_ns->size(&ba)));
				CWT_TEST_FAIL(ucs4);

				utf8 = uc_ns->encodeUcs4ToUtf8(ucs4, uc_ns->ucs4len(ucs4));
				CWT_TEST_FAIL(utf8);

				CWT_TEST_OK(strlen(utf8) == ba_ns->size(&ba));
				CWT_TEST_OK(strncmp(utf8, (char*)ba_ns->data(&ba), ba_ns->size(&ba)) == 0);

				CWT_FREE(ucs4);
				CWT_FREE(utf8);
			}

			chan_ns->close(chan);

			break;
		} while (TRUE);

		CWT_FREE(path);
		path = NULL;
	}

	ba_ns->destroy(&ba);
}


void test_wchar(void)
{
	CwtChannelNS   *chan_ns = cwt_channel_ns();
	CwtByteArrayNS *ba_ns = cwt_bytearray_ns();

	int i, nfiles;
	CwtChannel    *chan;
	CwtByteArray  ba;

	cwt_logger_ns()->trace(_T("Sizeof system wchar_t = %d"), sizeof(wchar_t));
	cwt_logger_ns()->trace(_T("Sizeof CWT wchar_t = %d"), CWT_SIZEOF_WCHAR);

#if CWT_SIZEOF_WCHAR == 4
#	define utf8_sample_files utf8_ucs4_sample_files
#else
#	define utf8_sample_files utf8_ucs2_sample_files
#endif

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

		cwt_logger_ns()->trace(_T("Testing %s ..."), utf8_sample_files[i]);

		do {
			chan = chan_ns->open(cwt_open_file(path, Cwt_FileRead));
			if (!chan)
				break;

			ba_ns->clear(&ba);

			if (chan_ns->slurp(chan, &ba)) {
				wchar_t *wcs;
				char    *utf8;

				wcs   = cwt_decode_utf8((char*)ba_ns->data(&ba), CWT_CAST_SIZE_T(ba_ns->size(&ba)));
				CWT_TEST_FAIL(wcs);

				utf8 = cwt_encode_utf8(wcs, wcslen(wcs));
				CWT_TEST_FAIL(utf8);

				CWT_TEST_OK(strlen(utf8) == ba_ns->size(&ba));
				CWT_TEST_OK(strncmp(utf8, (char*)ba_ns->data(&ba), ba_ns->size(&ba)) == 0);

				CWT_FREE(wcs);
				CWT_FREE(utf8);
			}

			chan_ns->close(chan);

			break;
		} while (TRUE);

		CWT_FREE(path);
		path = NULL;
	}

	ba_ns->destroy(&ba);

}

int main(int argc, char *argv[])
{
	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	CWT_BEGIN_TESTS(_NTESTS);

	test_ucs2();
	test_ucs4();
	test_wchar();

	CWT_END_TESTS;
}
