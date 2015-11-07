/*
 * decode.cpp
 *
 *  Created on: Nov 7, 2015
 *      Author: wladt
 */

#include <pfs/test/test.hpp>
#include <pfs/utf8/decode.hpp>
#include <pfs/utf8/iterator.hpp>
#include <string>
#include <sstream>
#include <iostream>
#include "test_data.hpp"

using std::cout;
using std::endl;

template <typename Iterator>
void __test_decode (const char * itertype)
{
	int ntests = sizeof(data)/sizeof(data[0]);
	ADD_TESTS(ntests);

	for (int i = 0; i < ntests; ++i) {
		Iterator it(iter_cast<Iterator>(data[i].text));
		Iterator itEnd(iter_cast<Iterator>(data[i].text) + data[i].len);

		size_t count = 0;
		int32_t uc = 0;

		while (it < itEnd) {
			it = pfs::utf8::decode(it, uc);
			++count;
		}

		std::ostringstream desc;

		desc << "Decode UTF-8 using `" << itertype << "' as pointer. String `"
				<< data[i].name
				<< "'. Number of unicode chars "
				<< count
				<< ", expected "
				<< data[i].nchars;
		TEST_OK2(count == data[i].nchars, desc.str().c_str());
	}
}

void test_decode ()
{
	__test_decode<unsigned char *>("unsigned char *");
	__test_decode<const unsigned char *>("const unsigned char *");
	__test_decode<char *>("char *");
	__test_decode<const char *>("const char *");
	__test_decode<std::string::iterator>("std::string::iterator");
	__test_decode<std::string::const_iterator>("std::string::const_iterator");
}


