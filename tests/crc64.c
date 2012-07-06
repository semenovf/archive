#include <stdio.h>
#include <cwt/types.h>
#include <cwt/test.h>
#include <cwt/utils.h>
#include <cwt/string.h>

extern void cwt_test_crc64(void);

int main(int argc, char *argv[])
{
	CHAR chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
	BYTE bytes[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	BYTE bytes256[256];
	int i = 0;

	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	CWT_BEGIN_TESTS(260);

	for( i = 0; i < 256; i++ ) {
		bytes256[i] = 256 - i;
	}

	cwt_test_crc64();
	CWT_TEST_OK2( -4480566129418311872LL == cwtCalculateCRC64((BYTE*)chars, cwtStrLen(chars), 0)
		, "CRC64(\"ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890\") == -4480566129418311872");

	CWT_TEST_OK2( 6302932907043766995LL == cwtCalculateCRC64(bytes, sizeof(bytes), 0)
		, "CRC64({1..9}) == 6302932907043766995");

	CWT_TEST_OK2( 2876295860935205507LL == cwtCalculateCRC64(bytes256, sizeof(bytes256), 0)
		, "CRC64({256..1}) == 2876295860935205507");

	CWT_TEST_OK2( -6008134657629535LL == cwtCalculateCRC64(bytes256, sizeof(bytes256), 128)
		, "CRC64({256..1}) == -6008134657629535 (with initial = 128)");

	CWT_END_TESTS;
}


