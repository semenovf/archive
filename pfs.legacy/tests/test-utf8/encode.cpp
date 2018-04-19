/*
 * encode.cpp
 *
 *  Created on: Nov 7, 2015
 *      Author: wladt
 */

#include <pfs/test/test.hpp>
#include <pfs/utf8/decode.hpp>
#include <pfs/utf8/encode.hpp>
#include <pfs/utf8/advance.hpp>
#include <string>
#include <sstream>
#include <iostream>
#include "test_data.hpp"

using std::cout;
using std::endl;

template <typename Iterator>
void __test_encode (const char * itertype)
{
	int ntests = sizeof(data)/sizeof(data[0]);
	ADD_TESTS(ntests);

	for (int i = 0; i < ntests; ++i) {
		Iterator it(iter_cast<Iterator>(data[i].text));
		Iterator itEnd(iter_cast<Iterator>(data[i].text) + data[i].len);

		size_t count = 0;
		uint32_t uc = 0;
		std::string result;

		while (it < itEnd) {
			uc = pfs::utf8::decode(it);
			pfs::utf8::encode(uc, std::back_inserter(result));
		}

		std::ostringstream desc;

		desc << "Decode/Encode to UTF-8 using `" << itertype << "' as pointer. String `"
				<< data[i].name	<< "'.";
		TEST_OK2(result == std::string(reinterpret_cast<char *>(data[i].text), data[i].len)
			, desc.str().c_str());
	}
}

void test_encode ()
{
	__test_encode<unsigned char *>("unsigned char *");
	__test_encode<const unsigned char *>("const unsigned char *");
	__test_encode<char *>("char *");
	__test_encode<const char *>("const char *");
	__test_encode<std::string::iterator>("std::string::iterator");
	__test_encode<std::string::const_iterator>("std::string::const_iterator");
}



