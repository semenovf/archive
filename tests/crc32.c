#include <stdio.h>
#include <cwt/types.h>
#include <cwt/test.h>
#include <cwt/utils.h>
#include <cwt/str.h>
#include <string.h>

extern void cwt_test_crc32(void);

int main(int argc, char *argv[])
{
	CwtUtilsNS *utilsNS = cwtUtilsNS();
	BYTE bytes[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	char *chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
	char bytes256[256];
	INT32 crc32;
	int i = 0;

	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	CWT_BEGIN_TESTS(260);

	for( i = 0; i < 256; i++ ) {
		bytes256[i] = 256 - i;
	}

	cwt_test_crc32();

	crc32 = utilsNS->calculateCRC32(bytes, sizeof(bytes), 0);
	CWT_TEST_OK2(1164760902 == crc32
		, _T("CRC32({0..9}) == 1164760902"));

	crc32 = utilsNS->calculateCRC32(chars, strlen(chars), 0);
	CWT_TEST_OK2( -1764981246 == crc32
		, _T("CRC32(\"ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890\") == -1764981246"));

	crc32 = utilsNS->calculateCRC32(bytes256, sizeof(bytes256), 0);
	CWT_TEST_OK2( -1386202695 == crc32
		, _T("CRC32({256..1}) == -1386202695"));

	crc32 = utilsNS->calculateCRC32(bytes256, sizeof(bytes256), -1386202695);
	CWT_TEST_OK2( 2043667161 == crc32
		, _T("CRC32({256..1}) == 2043667161 (with initial = -1386202695)"));


	CWT_END_TESTS;
}


