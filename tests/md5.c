/**
 * @file   md5.c
 * @author wladt
 * @date   Feb 21, 2013 1:35:00 PM
 *
 * @brief
 */

#include <cwt/test.h>
#include <cwt/md5.h>
#include <string.h>

int main(int argc, char *argv[])
{
	int i;
	static struct {
		const char *const s;
		const char *const md5_sample;
	} test[] = 	{
		  { "", "d41d8cd98f00b204e9800998ecf8427e" }
		, { "945399884.61923487334tuvga", "4d52535c7692376e7e7e205940a93ae9" }
		, { "abc", "900150983cd24fb0d6963f7d28e17f72" }
		, { "message digest", "f96b697d7cb7938d525a2f31aaf161d0" }
		, { "abcdefghijklmnopqrstuvwxyz", "c3fcd3d76192e4007dfb496cca67e13b" }
		, { "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789", "d174ab98d277d9f5a5611c2c9f419d9f" }
		, { "12345678901234567890123456789012345678901234567890123456789012345678901234567890", "57edf4a22be3c955ac49da2e2107b67a" }
	};

    CWT_CHECK_SIZEOF_TYPES;
    CWT_UNUSED2(argc, argv);
    CWT_BEGIN_TESTS(7);

	for (i = 0; i < 7; ++i) {
		char md5_sample[MD5_OUTPUT_SIZE * 2 + 1];
		char *p;
		md5_state_t state;
		md5_byte_t digest[16];
		int di;

		md5_init(&state);
		md5_update(&state, (const byte_t *)test[i].s, strlen(test[i].s));
		md5_finish(&state, digest);

		p = md5_sample;
		for (di = 0; di < 16; ++di, p += 2)
			sprintf(p, "%02x", digest[di]);
		*p = '\x0';
		CWT_TEST_OK(strcmp(md5_sample, test[i].md5_sample) == 0);
	}

    CWT_END_TESTS;
}




