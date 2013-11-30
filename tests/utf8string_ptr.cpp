/*
 * utf8string_constptr.cpp
 *
 *  Created on: Nov 15, 2013
 *  Author: wladt
 */

#include <cwt/test.hpp>
#include <pfs/bits/utf8string_ptr.hpp>
#include <string>
#include <iostream>

void test_constptr()
{
	std::string s("GIJKLЁЖЗИЙЭЮЯgijklёжзийэюя");
	pfs::utf8string_ptr ptr(s.data());
	pfs::utf8string_ptr begin(s.data());
	pfs::utf8string_ptr end(s.data() + s.size());

	std::cout << "Iterate through string \"" << s << '"' << std::endl;

	TEST_OK(ptr == begin);

	TEST_OK(*ptr++ == 'G');
	TEST_OK(*ptr++ == 'I');
	TEST_OK(*ptr++ == 'J');
	TEST_OK(*ptr++ == 'K');
	TEST_OK(*ptr++ == 'L');
	TEST_OK2(*ptr++ == pfs::ucchar(0x401), "*ptr++ == 'Ё' (0x401, Cyrillic Capital Letter Io)");
	TEST_OK2(*ptr++ == pfs::ucchar(0x416), "*ptr++ == 'Ж' (0x416, Cyrillic Capital Letter Zhe)");
	TEST_OK2(*ptr++ == pfs::ucchar(0x417), "*ptr++ == 'З' (0x417, Cyrillic Capital Letter Ze)");
	TEST_OK2(*ptr++ == pfs::ucchar(0x418), "*ptr++ == 'И' (0x418, Cyrillic Capital Letter I)");
	TEST_OK2(*ptr++ == pfs::ucchar(0x419), "*ptr++ == 'Й  (0x419, Cyrillic Capital Letter Short I)");
	TEST_OK2(*ptr++ == pfs::ucchar(0x42D), "*ptr++ == 'Э' (0x42D, Cyrillic Capital Letter E)");
	TEST_OK2(*ptr++ == pfs::ucchar(0x42E), "*ptr++ == 'Ю' (0x42E, Cyrillic Capital Letter Yu)");
	TEST_OK2(*ptr++ == pfs::ucchar(0x42F), "*ptr++ == 'Я' (0x42F, Cyrillic Capital Letter Ya)");
	TEST_OK(*ptr++ == 'g');
	TEST_OK(*ptr++ == 'i');
	TEST_OK(*ptr++ == 'j');
	TEST_OK(*ptr++ == 'k');
	TEST_OK(*ptr++ == 'l');
	TEST_OK2(*ptr++ == pfs::ucchar(0x451), "*ptr++ == 'ё' (0x451, Cyrillic Small Letter Io)");
	TEST_OK2(*ptr++ == pfs::ucchar(0x436), "*ptr++ == 'ж' (0x436, Cyrillic Small Letter Zhe)");
	TEST_OK2(*ptr++ == pfs::ucchar(0x437), "*ptr++ == 'з' (0x437, Cyrillic Small Letter Ze)");
	TEST_OK2(*ptr++ == pfs::ucchar(0x438), "*ptr++ == 'и' (0x438, Cyrillic Small Letter I)");
	TEST_OK2(*ptr++ == pfs::ucchar(0x439), "*ptr++ == 'й' (0x439, Cyrillic Small Letter Short I)");
	TEST_OK2(*ptr++ == pfs::ucchar(0x44D), "*ptr++ == 'э' (0x44D, Cyrillic Small Letter E)");
	TEST_OK2(*ptr++ == pfs::ucchar(0x44E), "*ptr++ == 'ю' (0x44E, Cyrillic Small Letter Yu)");
	TEST_OK2(*ptr++ == pfs::ucchar(0x44F), "*ptr++ == 'я' (0x44F, Cyrillic Small Letter Ya)");

	TEST_OK(ptr == end);

	ptr = begin;
	TEST_OK(ptr == begin);

	TEST_OK(ptr[0] == 'G');
	TEST_OK(ptr[1] == 'I');
	TEST_OK(ptr[2] == 'J');
	TEST_OK(ptr[3] == 'K');
	TEST_OK(ptr[4] == 'L');
	TEST_OK2(ptr[5] == pfs::ucchar(0x401),  "ptr[5] == 'Ё' (0x401, Cyrillic Capital Letter Io)");
	TEST_OK2(ptr[6] == pfs::ucchar(0x416),  "ptr[6] == 'Ж' (0x416, Cyrillic Capital Letter Zhe)");
	TEST_OK2(ptr[7] == pfs::ucchar(0x417),  "ptr[7] == 'З' (0x417, Cyrillic Capital Letter Ze)");
	TEST_OK2(ptr[8] == pfs::ucchar(0x418),  "ptr[8] == 'И' (0x418, Cyrillic Capital Letter I)");
	TEST_OK2(ptr[9] == pfs::ucchar(0x419),  "ptr[9] == 'Й  (0x419, Cyrillic Capital Letter Short I)");
	TEST_OK2(ptr[10] == pfs::ucchar(0x42D), "ptr[10] == 'Э' (0x42D, Cyrillic Capital Letter E)");
	TEST_OK2(ptr[11] == pfs::ucchar(0x42E), "ptr[11] == 'Ю' (0x42E, Cyrillic Capital Letter Yu)");
	TEST_OK2(ptr[12] == pfs::ucchar(0x42F), "ptr[12] == 'Я' (0x42F, Cyrillic Capital Letter Ya)");
	TEST_OK(ptr[13] == 'g');
	TEST_OK(ptr[14] == 'i');
	TEST_OK(ptr[15] == 'j');
	TEST_OK(ptr[16] == 'k');
	TEST_OK(ptr[17] == 'l');
	TEST_OK2(ptr[18] == pfs::ucchar(0x451), "ptr[18] == 'ё' (0x451, Cyrillic Small Letter Io)");
	TEST_OK2(ptr[19] == pfs::ucchar(0x436), "ptr[19] == 'ж' (0x436, Cyrillic Small Letter Zhe)");
	TEST_OK2(ptr[20] == pfs::ucchar(0x437), "ptr[20] == 'з' (0x437, Cyrillic Small Letter Ze)");
	TEST_OK2(ptr[21] == pfs::ucchar(0x438), "ptr[21] == 'и' (0x438, Cyrillic Small Letter I)");
	TEST_OK2(ptr[22] == pfs::ucchar(0x439), "ptr[22] == 'й' (0x439, Cyrillic Small Letter Short I)");
	TEST_OK2(ptr[23] == pfs::ucchar(0x44D), "ptr[23] == 'э' (0x44D, Cyrillic Small Letter E)");
	TEST_OK2(ptr[24] == pfs::ucchar(0x44E), "ptr[24] == 'ю' (0x44E, Cyrillic Small Letter Yu)");
	TEST_OK2(ptr[25] == pfs::ucchar(0x44F), "ptr[25] == 'я' (0x44F, Cyrillic Small Letter Ya)");

	ptr = begin;
	TEST_OK(ptr == begin);
	TEST_OK(ptr >= begin);
	TEST_OK(ptr <= begin);
	TEST_OK(--ptr < begin);
	TEST_OK(--ptr < begin);
	TEST_OK(++ptr < begin);
	TEST_OK(++ptr == begin);

	ptr = end;
	TEST_OK(ptr == end);
	TEST_OK(ptr >= end);
	TEST_OK(ptr <= end);
	TEST_OK(++ptr > end);
	TEST_OK(++ptr > end);
	TEST_OK(--ptr > end);
	TEST_OK(--ptr == end);

}


int main(int argc, char *argv[])
{
    PFS_CHECK_SIZEOF_TYPES;
    PFS_UNUSED2(argc, argv);
	BEGIN_TESTS(69);

	test_constptr();

    END_TESTS;
}




