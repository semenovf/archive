/*
 * utf8string.cpp
 *
 *  Created on: Jul 21, 2013
 *      Author: wladt
 */

#include <cwt/test.hpp>
#include <pfs/utf8string.hpp>
#include <pfs/bits/uccharptr.hpp>
#include <cstring>
#include <iostream>

void test_basic()
{
	const char * latin1Chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
	pfs::utf8string s;
	TEST_OK(s.isEmpty());

	pfs::utf8string latin1String(latin1Chars);
	TEST_OK(strcmp(latin1Chars, latin1String.c_str()) == 0);

	pfs::utf8string otherLatin1String;

	otherLatin1String.append(latin1String);
	TEST_OK(latin1String == otherLatin1String);

	TEST_OK(s != latin1String);
	s.append(latin1Chars);
	TEST_OK(s == latin1String);

	latin1String.clear();
	TEST_OK(latin1String.isEmpty());
}

void test_init()
{
	pfs::utf8string::ConvertState state;
	const char * cyrillicChars = "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЬЫЪЭЮЯабвгдеёжзийклмнопрстуфхцчшщьыъэюя";
	pfs::utf8string cyrillic (pfs::utf8string::fromUtf8(cyrillicChars, & state));

	TEST_OK2(state.invalidChars == 0, "pfs::utf8string initialized from UTF-8 encoded cyrillic string");
	TEST_OK(strlen(cyrillic.c_str()) == strlen(cyrillicChars));
//	TEST_OK2(cyrillic.begin().distance(cyrillic.end()) == 66 * 2, "Cyrillic alphabet consists of 66 characters (multiple to 2 bytes per character in UTF-8)");
	TEST_OK2(cyrillic.length() == 66, "Cyrillic alphabet consists of 66 characters");
}

void test_swap()
{
	const char * latinChars    = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"; // 52
	const char * cyrillicChars = "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЬЫЪЭЮЯабвгдеёжзийклмнопрстуфхцчшщьыъэюя"; // 66
	pfs::utf8string cyrillic (_u8(cyrillicChars));
	pfs::utf8string latin    (_u8(latinChars));

	TEST_OK(latin.length() == 52);
	TEST_OK(cyrillic.length() == 66);
	TEST_OK(strcmp(latin.c_str(), latinChars) == 0);
	TEST_OK(strcmp(cyrillic.c_str(), cyrillicChars) == 0);

	cyrillic.swap(latin);

	TEST_OK2(strcmp(latin.c_str(), cyrillicChars) == 0, "Latin string contains cyrillic characters after swap");
	TEST_OK2(strcmp(cyrillic.c_str(), latinChars) == 0, "Cyrillic string contains latin characters after swap");

	cyrillic.swap(latin);

	pfs::utf8string empty;
	TEST_OK(empty.isEmpty());
	empty.swap(cyrillic);
	TEST_OK2(!empty.isEmpty(), "'empty' string is not empty after swap with 'cyrillic'");
	TEST_OK2(strcmp(empty.c_str(), cyrillicChars) == 0, "'empty' string contains cyrillic characters after swap");
	TEST_OK(cyrillic.isEmpty());

	empty.swap(cyrillic);
	TEST_OK2(empty.isEmpty(), "'empty' string restored to empty");
	TEST_OK2(strcmp(cyrillic.c_str(), cyrillicChars) == 0, "Cyrillic string restored");
}

void test_find()
{
	pfs::utf8string haystack (_u8("АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЬЫЪЭЮЯабвгдеёжзийклмнопрстуфхцчшщьыъэюя"));
	pfs::utf8string needles[3];
	pfs::utf8string::iterator its[3];

	needles[0] = _u8("АБВГ");
	its[0] = haystack.begin();
	needles[1] = _u8("а");
	its[1] = haystack.begin() + 33;
	needles[2] = _u8("W");
	its[2] = haystack.end();

	for (size_t i = 0; i < sizeof(needles)/sizeof(needles[0]); ++i) {
		pfs::utf8string::const_iterator it = haystack.find(needles[i], haystack.begin());
		TEST_OK(it == its[i]);
	}
}

template <typename _iterator>
void test_iterator()
{
	pfs::utf8string s (_u8("GIJKLЁЖЗИЙЭЮЯgijklёжзийэюя"));
	size_t n = 0;

	_iterator it = s.begin();
	while (it++ < s.end()) {
		++n;
	}

	TEST_OK(n == s.length());

	it = s.end();
	n = 0;
	while (it-- > s.begin()) {
		++n;
	}

	TEST_OK(n == s.length());

	pfs::utf8string threechars(_u8("\xf0\x90\x8d\x86\xe6\x97\xa5\xd1\x88"));
	it = threechars.begin();
	_iterator it2 = it;
	TEST_OK(it2 == it);
	TEST_OK(*it == 0x10346);
	TEST_OK(*(++it) == 0x65e5);
	TEST_OK((*it++) == 0x65e5);
	TEST_OK(*it == 0x0448);
	TEST_OK(it != it2);
	_iterator itEnd = threechars.end();
	TEST_OK(++it == itEnd);
	TEST_OK(*(--it) == 0x0448);
	TEST_OK((*it--) == 0x0448);
	TEST_OK(*it == 0x65e5);
	TEST_OK(--it == threechars.begin());
	TEST_OK(*it == 0x10346);
}

template <typename _reverse_iterator>
void test_reverse_iterator()
{
	pfs::utf8string s (_u8("GIJKLЁЖЗИЙЭЮЯgijklёжзийэюя"));
	size_t n = 0;

	_reverse_iterator it = s.rbegin();
	while (it++ < s.rend()) {
		++n;
	}

	TEST_OK(n == s.length());

	it = s.rend();
	n = 0;
	while (it-- > s.rbegin()) {
		++n;
	}

	TEST_OK(n == s.length());
	TEST_OK(n == size_t(s.crend() - s.crbegin()));
}

void test_insert()
{
	pfs::utf8string s (_u8("GIJKLЁЖЗИЙЭЮЯgijklёжзийэюя"));

	TEST_OK(_u8("GIJKLЁЖЗИЙЭЮЯ").append(_u8("gijklёжзийэюя")) == s);
	TEST_OK(_u8("GIJKL").insert(5, _u8("ЁЖЗИЙЭЮЯgijklёжзийэюя")) == s);
	TEST_OK(_u8("GIJKLЁ").insert(6, _u8("ЖЗИЙЭЮЯgijklёжзийэюя")) == s);
	TEST_OK(_u8("GIJKLЁЖЗИgijklёжзийэюя").insert(9, _u8("ЙЭЮЯ")) == s);
	TEST_OK(_u8("gijklёжзийэюя").prepend(_u8("GIJKLЁЖЗИЙЭЮЯ")) == s);
}

void test_substr()
{
	pfs::utf8string s (_u8("GIJKLЁЖЗИЙЭЮЯgijklёжзийэюя"));

	TEST_OK(s.substr(0)  == s);
	TEST_OK(s.substr(1)  == _u8("IJKLЁЖЗИЙЭЮЯgijklёжзийэюя"));
	TEST_OK(s.substr(2)  == _u8("JKLЁЖЗИЙЭЮЯgijklёжзийэюя"));
	TEST_OK(s.substr(3)  == _u8("KLЁЖЗИЙЭЮЯgijklёжзийэюя"));
	TEST_OK(s.substr(13) == _u8("gijklёжзийэюя"));
	TEST_OK(s.mid(12,1)  == _u8("Я"));

	TEST_OK(s.substr(0,0) == pfs::utf8string());
	TEST_OK(s.substr(26)  == pfs::utf8string());
	TEST_OK(s.substr(126) == pfs::utf8string());
}

void test_compare()
{
	TEST_OK(_u8("Б").compare(_u8("Б")) == 0);
	TEST_OK(_u8("Б").compare(_u8("А")) > 0);
	TEST_OK(_u8("Б").compare(_u8("В")) < 0);
	TEST_OK(_u8("АБВ").compare("АБВ") == 0);
}

void test_startWith()
{
	pfs::utf8string s (_u8("ЁЖЗИЙЭЮЯGIJKLёжзийэюяgijkl"));

	TEST_OK(s.startsWith(s));
	TEST_OK(s.startsWith("Ё"));
	TEST_OK(s.startsWith("ЁЖ"));
	TEST_OK(s.startsWith("ЁЖЗИЙЭЮЯGIJKL"));
	TEST_OK(s.startsWith("ЁЖЗИЙЭЮЯGIJKLёжзийэюя"));
	TEST_OK(s.startsWith("ЁЖЗИЙЭЮЯGIJKLёжзийэюяgijkl"));
	TEST_OK(!s.startsWith("A"));
	TEST_OK(!s.startsWith("Жизнь"));
}

void test_endsWith()
{
	pfs::utf8string s (_u8("ЁЖЗИЙЭЮЯGIJKLёжзийэюяgijkl"));

	TEST_OK(s.endsWith(s));
	TEST_OK(s.endsWith("l"));
	TEST_OK(s.endsWith("gijkl"));
	TEST_OK(s.endsWith("ёжзийэюяgijkl"));
	TEST_OK(s.endsWith("ЭЮЯGIJKLёжзийэюяgijkl"));
	TEST_OK(s.endsWith("ЁЖЗИЙЭЮЯGIJKLёжзийэюяgijkl"));
	TEST_OK(!s.endsWith("A"));
	TEST_OK(!s.endsWith("Жизнь"));
}

void test_number_conversions()
{
	bool ok;
	TEST_OK(_u8("123456").toUInt( &ok) == 123456 && ok);
	TEST_OK(_u8("-123456").toInt(&ok) == -123456 && ok);
	TEST_OK(_u8("/123456").toInt(&ok) == 0 && !ok);
	TEST_OK(_u8("123.456").toInt(&ok) == 0 && !ok);
	TEST_OK(_u8("123.456").toDouble(&ok) == double(123.456) && ok);
}

void test_replace()
{
	pfs::utf8string s (_u8("АБВABCАБВABCАБВABCАБВABCАБВABCАБВABCАБВ"));

	pfs::utf8string after ("=XYZ=");
	pfs::utf8string dest1 ("=XYZ=ABC=XYZ=ABC=XYZ=ABC=XYZ=ABC=XYZ=ABC=XYZ=ABC=XYZ=");

	pfs::utf8string before (_u8("АБВ"));

	std::cout << "orig: " << s << std::endl;
	std::cout << "dest: " << dest1 << std::endl;

	s.replace(before, after);
	std::cout << "repl: " << s << std::endl;
	TEST_OK(s == dest1);

	pfs::utf8string s1("ABC-XYZ");
	s1.replace(pfs::utf8string("-"), pfs::utf8string("_"));
	TEST_OK(s1 == pfs::utf8string("ABC_XYZ"));
	std::cout << "s1: " << s1 << std::endl;

	pfs::utf8string base("this is a test string.");
	pfs::utf8string str2("n example");
	pfs::utf8string str3("sample phrase");
	pfs::utf8string str4("useful.");

	// replace signatures used in the same order as described above:

	// Using positions:                 0123456789*123456789*12345
	pfs::utf8string str(base);        // "this is a test string."

	TEST_OK(str
			== _u8("this is a test string."));
	TEST_OK(str.replace(9, 5, str2)
			== _u8("this is an example string."));
	TEST_OK(str.replace(19, 6, str3, 7, 6)
			== _u8("this is an example phrase."));
	TEST_OK(str.replace(8, 10, _u8("just a"))
			== _u8("this is just a phrase."));
	TEST_OK(str.replace(8, 6, _u8("a shorty"), 0, 7)
			== _u8("this is a short phrase."));
	TEST_OK(str.replace(22, 1, 3, '!')
			== _u8("this is a short phrase!!!"));

	std::cout << '[' << str << ']' << std::endl;
}

void test_split()
{
	pfs::vector<pfs::utf8string> tokens = _u8("А,ББ,ВВ,ГГГ,Д,Е").split(pfs::utf8string(","));
	TEST_FAIL(tokens.size() == 6);
	TEST_OK(tokens[0] == _u8("А"));
	TEST_OK(tokens[1] == _u8("ББ"));
	TEST_OK(tokens[2] == _u8("ВВ"));
	TEST_OK(tokens[3] == _u8("ГГГ"));
	TEST_OK(tokens[4] == _u8("Д"));
	TEST_OK(tokens[5] == _u8("Е"));

	tokens = _u8(",ББ,ВВ,ГГГ,Д,Е").split(pfs::utf8string(","));
	TEST_FAIL(tokens.size() == 6);
	TEST_OK(tokens[0].isEmpty());
	TEST_OK(tokens[1] == _u8("ББ"));
	TEST_OK(tokens[2] == _u8("ВВ"));
	TEST_OK(tokens[3] == _u8("ГГГ"));
	TEST_OK(tokens[4] == _u8("Д"));
	TEST_OK(tokens[5] == _u8("Е"));

	tokens = _u8(",ББ,ВВ,ГГГ,Д,").split(pfs::utf8string(","));
	TEST_FAIL(tokens.size() == 6);
	TEST_OK(tokens[0].isEmpty());
	TEST_OK(tokens[1] == _u8("ББ"));
	TEST_OK(tokens[2] == _u8("ВВ"));
	TEST_OK(tokens[3] == _u8("ГГГ"));
	TEST_OK(tokens[4] == _u8("Д"));
	TEST_OK(tokens[5].isEmpty());

	tokens = _u8(",ББ,ВВ,ГГГ,Д,Е").split(pfs::utf8string(","), false); // do not keep empty
	TEST_FAIL(tokens.size() == 5);
	TEST_OK(tokens[0] == _u8("ББ"));
	TEST_OK(tokens[1] == _u8("ВВ"));
	TEST_OK(tokens[2] == _u8("ГГГ"));
	TEST_OK(tokens[3] == _u8("Д"));
	TEST_OK(tokens[4] == _u8("Е"));

	tokens = _u8(",ББ,ВВ,ГГГ,Д,").split(pfs::utf8string(","), false); // do not keep empty
	TEST_FAIL(tokens.size() == 4);
	TEST_OK(tokens[0] == _u8("ББ"));
	TEST_OK(tokens[1] == _u8("ВВ"));
	TEST_OK(tokens[2] == _u8("ГГГ"));
	TEST_OK(tokens[3] == _u8("Д"));

	tokens = _u8("АА,\"ББ,ВВ,\"ГГГ,Д,").split(pfs::utf8string(","), false, pfs::ucchar('"')); // do not keep empty
	TEST_FAIL(tokens.size() == 3);
	TEST_OK(tokens[0] == _u8("АА"));
	TEST_OK(tokens[1] == _u8("\"ББ,ВВ,\"ГГГ"));
	TEST_OK(tokens[2] == _u8("Д"));

	pfs::utf8string unbalanced(_u8("АА,\"ББ,ВВ,ГГГ,Д,"));
	tokens = unbalanced.split(pfs::utf8string(","), false, pfs::ucchar('"')); // do not keep empty
	TEST_OK2(tokens.isEmpty(), (_u8("Unbalanced quote character in test string: [") + unbalanced + "]").c_str());

	tokens = _u8("one,two;three four/five\tsix").splitOneOf(pfs::utf8string(",; /\t"), false, pfs::ucchar('"')); // do not keep empty
	TEST_FAIL(tokens.size() == 6);
	TEST_OK(tokens[0] == _u8("one"));
	TEST_OK(tokens[1] == _u8("two"));
	TEST_OK(tokens[2] == _u8("three"));
	TEST_OK(tokens[3] == _u8("four"));
	TEST_OK(tokens[4] == _u8("five"));
	TEST_OK(tokens[5] == _u8("six"));

	tokens = _u8("one  , two ; three four / five\t six").splitOneOf(pfs::utf8string(",; /\t"), false, pfs::ucchar('"')); // do not keep empty
	TEST_FAIL(tokens.size() == 6);
	TEST_OK(tokens[0] == _u8("one"));
	TEST_OK(tokens[1] == _u8("two"));
	TEST_OK(tokens[2] == _u8("three"));
	TEST_OK(tokens[3] == _u8("four"));
	TEST_OK(tokens[4] == _u8("five"));
	TEST_OK(tokens[5] == _u8("six"));
}

void test_trim()
{
	pfs::utf8string empty;
	pfs::utf8string s (_u8("\n  \t  ЁЖЗИЙЭЮЯGIJKLёжзийэюяgijkl \t \n \t\t\n"));
	pfs::utf8string ltrimed (_u8("ЁЖЗИЙЭЮЯGIJKLёжзийэюяgijkl \t \n \t\t\n"));
	pfs::utf8string rtrimed (_u8("\n  \t  ЁЖЗИЙЭЮЯGIJKLёжзийэюяgijkl"));
	pfs::utf8string trimed (_u8("ЁЖЗИЙЭЮЯGIJKLёжзийэюяgijkl"));

	TEST_OK(empty.ltrim() == empty);
	TEST_OK(empty.rtrim() == empty);
	TEST_OK(empty.trim()  == empty);

	TEST_OK(s.ltrim() == ltrimed);
	TEST_OK(s.rtrim() == rtrimed);
	TEST_OK(s.trim()  == trimed);
}

void test_std_string ()
{
	std::string s("ABCDEF");
	std::string::iterator it = s.begin();
	std::string::iterator itBegin = s.begin();
	TEST_OK(it == itBegin);
	TEST_OK(--it < itBegin);
	TEST_OK(--it < itBegin);
	TEST_OK(++it < itBegin);
	TEST_OK(++it == itBegin);
	TEST_OK(it >= itBegin);
	TEST_OK(++it > itBegin);
	TEST_OK(*it == 'B');

	*it = 'W';
	TEST_OK(*it == 'W');
	TEST_OK(s == "AWCDEF")
}


void test_ucchar_ptr ()
{
	pfs::utf8string s (_u8("ЁЖЗИЙЭЮЯGIJKLёжзийэюяgijkl"));

	pfs::ucchar_ref uref0(0, s);
	TEST_OK(uref0 == pfs::ucchar(0x401));

	pfs::ucchar_ref uref1(1, s);
	TEST_OK(uref1 == pfs::ucchar(0x416));

	pfs::ucchar_ptr p(s);
	TEST_OK(*p == pfs::ucchar(0x401));


	// Test left bound
	int n = 5;
	while (n-- > 0) {
		--p;
		TEST_OK(p.isOutOfBounds());
	}

	n = 5;
	while (n-- > 0) {
		TEST_OK(p.isOutOfBounds());
		++p;
	}

	TEST_OK(!p.isOutOfBounds());
	TEST_OK(*p == pfs::ucchar(0x401));
}


int main(int argc, char *argv[])
{
    PFS_CHECK_SIZEOF_TYPES;
    PFS_UNUSED2(argc, argv);
	BEGIN_TESTS(146);

    test_basic();
    test_init();
    test_swap();
    test_find();
    test_iterator<pfs::utf8string::const_iterator>();
    test_reverse_iterator<pfs::utf8string::const_reverse_iterator>();
    test_insert();
    test_substr();
    test_compare();
    test_startWith();
    test_endsWith();
    test_number_conversions();
    test_replace();
    test_split();
    test_trim();
    test_std_string();
    test_ucchar_ptr();

    END_TESTS;
}




