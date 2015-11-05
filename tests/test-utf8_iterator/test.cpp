/**
 * @file   test.cpp
 * @author
 * @date
 *
 * @brief testing ...
 */

#include <pfs/iterator/utf8_iterator.hpp>
#include <pfs/test/test.hpp>
#include <string>
#include <sstream>
#include <iostream>

using std::cout;
using std::endl;

extern unsigned char cyrillic_txt[];
extern unsigned int  cyrillic_txt_len;
extern unsigned char greek_txt[];
extern unsigned int  greek_txt_len;
extern unsigned char gothic_txt[];
extern unsigned int  gothic_txt_len;
extern unsigned char mideng_txt[];
extern unsigned int  mideng_txt_len;
extern unsigned char midger_txt[];
extern unsigned int  midger_txt_len;
extern unsigned char multilang_txt[];
extern unsigned int  multilang_txt_len;
extern unsigned char rune_txt[];
extern unsigned int  rune_txt_len;
extern unsigned char vietnamese_txt[];
extern unsigned int  vietnamese_txt_len;

struct test_data {
	const char *    name;
	unsigned char * text;
	unsigned int    len;
	unsigned int    nchars;
};

test_data data[] = {
	  { "cyrillic"  , cyrillic_txt  , cyrillic_txt_len   ,   66 }
	, { "gothic"    , gothic_txt    , gothic_txt_len     ,   83 }
	, { "greek"     , greek_txt     , greek_txt_len      ,  204 }
	, { "mideng"    , mideng_txt    , mideng_txt_len     ,  272 }
	, { "midger"    , midger_txt    , midger_txt_len     ,  353 }
	, { "multilang" , multilang_txt , multilang_txt_len  , 9175 }
	, { "rune"      , rune_txt      , rune_txt_len       ,  145 }
	, { "vietnamese", vietnamese_txt, vietnamese_txt_len ,   43 }
};

template <typename Iterator>
Iterator iter_cast (unsigned char *);

template <>
unsigned char * iter_cast<unsigned char *> (unsigned char * it)
{
	return it;
}

template <>
const unsigned char * iter_cast<const unsigned char *> (unsigned char * it)
{
	return it;
}

template <>
char * iter_cast<char *> (unsigned char * it)
{
	return reinterpret_cast<char *>(it);
}

template <>
const char * iter_cast<const char *> (unsigned char * it)
{
	return reinterpret_cast<const char *>(it);
}


template <>
std::string::iterator iter_cast<std::string::iterator> (unsigned char * it)
{
	return std::string::iterator(reinterpret_cast<char *>(it));
}

template <>
std::string::const_iterator iter_cast<std::string::const_iterator> (unsigned char * it)
{
	return std::string::const_iterator(reinterpret_cast<char *>(it));
}


template <typename Iterator>
void test_forward (const char * itertype)
{
	ADD_TESTS(8);

	for (int i = 0; i < sizeof(data)/sizeof(data[0]); ++i) {
		pfs::utf8_iterator<Iterator> it(iter_cast<Iterator>(data[i].text));
		pfs::utf8_iterator<Iterator> itEnd(iter_cast<Iterator>(data[i].text) + data[i].len);
		size_t count = 0;

		while (it < itEnd) {
			++it;
			++count;
		}

		std::ostringstream desc;

		desc << "Forward: using `" << itertype << "' as pointer. String `"
				<< data[i].name
				<< "'. Number of unicode chars "
				<< count
				<< ", expected "
				<< data[i].nchars;
		TEST_OK2(count == data[i].nchars, desc.str().c_str());
	}
}

template <typename Iterator>
void test_backward (const char * itertype)
{
	ADD_TESTS(8);

	for (int i = 0; i < sizeof(data)/sizeof(data[0]); ++i) {
		pfs::utf8_iterator<Iterator> itBegin(iter_cast<Iterator>(data[i].text));
		pfs::utf8_iterator<Iterator> it(iter_cast<Iterator>(data[i].text) + data[i].len);
		size_t count = 0;

		while (it-- > itBegin) {
			++count;
		}

		std::ostringstream desc;

		desc << "Backward: using `" << itertype << "' as pointer. String `"
				<< data[i].name
				<< "'. Number of unicode chars "
				<< count
				<< ", expected "
				<< data[i].nchars;
		TEST_OK2(count == data[i].nchars, desc.str().c_str());
	}
}
int main(int argc, char *argv[])
{
    PFS_UNUSED2(argc, argv);
	BEGIN_TESTS(0);

	test_forward<unsigned char *>("unsigned char *");
	test_forward<const unsigned char *>("const unsigned char *");
	test_forward<char *>("char *");
	test_forward<const char *>("const char *");
	test_forward<std::string::iterator>("std::string::iterator");
	test_forward<std::string::const_iterator>("std::string::const_iterator");

	test_backward<unsigned char *>("unsigned char *");
	test_backward<const unsigned char *>("const unsigned char *");
	test_backward<char *>("char *");
	test_backward<const char *>("const char *");
	test_backward<std::string::iterator>("std::string::iterator");
	test_backward<std::string::const_iterator>("std::string::const_iterator");

    END_TESTS;
}
