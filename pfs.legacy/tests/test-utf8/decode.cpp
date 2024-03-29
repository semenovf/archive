/*
 * decode.cpp
 *
 *  Created on: Nov 7, 2015
 *      Author: wladt
 */

#include <pfs/test/test.hpp>
#include <pfs/utf8/advance.hpp>
#include <pfs/utf8/decode.hpp>
#include <pfs/sstream.hpp>
#include <pfs/iostream.hpp>
#include <string>

#include "test_data.hpp"

using pfs::cout;
using pfs::endl;
using pfs::ostringstream;

template <typename Iterator>
void __test_decode (const char * itertype)
{
	int ntests = sizeof(data)/sizeof(data[0]);
	ADD_TESTS(ntests);

	for (int i = 0; i < ntests; ++i) {
		Iterator it(iter_cast<Iterator>(data[i].text));
		Iterator itEnd(iter_cast<Iterator>(data[i].text) + data[i].len);

		size_t count = 0;
		uint32_t uc = 0;

		while (it < itEnd) {
			uc = pfs::utf8::decode(it);
			++count;
		}

		ostringstream desc;

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


