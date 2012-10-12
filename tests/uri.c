/**
 * @file   uri.c
 * @author wladt
 * @date   Oct 12, 2012 11:18:36 AM
 *
 * @brief URI testing
 */
#include <cwt/test.h>
#include <cwt/utils.h>
#include <cwt/str.h>

int main(int argc, char *argv[])
{
	CwtUtilsNS *utilsNS = cwtUtilsNS();
	CwtStrNS *strNS = cwtStrNS();
	CwtUri uri;
	const CWT_CHAR *uri_string = _T("https://192.168.1.1");

	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	CWT_BEGIN_TESTS(2);

	utilsNS->initURI(&uri);
	CWT_TEST_OK(utilsNS->parseURI(uri_string, &uri));
	CWT_TEST_OK(strNS->strEq(_T("https"), uri.scheme));
	CWT_TEST_OK(strNS->strEq(_T("192.168.1.1"), uri.host));
	utilsNS->destroyURI(&uri);

	CWT_END_TESTS;
}
