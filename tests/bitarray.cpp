/*
 * bitarray.cpp
 *
 *  Created on: Jun 27, 2013
 *      Author: wladt
 */

#include "../include/cwt/test.h"
#include "../include/cwt/bitarray.hpp"
#include <cstring>

using namespace cwt;


void test_set_bits()
{
	const size_t size = 20000;
	BitArray ba(size, true); // set all bits to 1

	for (size_t i = 0; i < size ; i += 2) {
		ba.setBit(i, false);
	}

	int c = 0;
	for (size_t i = 0; i < size ; ++i) {
		if (ba[i])
			++c;
		else
			--c;
	}

	CWT_TEST_OK2(c == 0, _Tr("Test interleaved bits"));
	CWT_TEST_OK(ba.count(true) == ba.count(false));
	CWT_TEST_OK(ba.count(true) == size/2);
	CWT_TEST_OK(ba.count(false) == size/2);

	ba.truncate(10);
	CWT_TEST_OK2(ba.size() == 10, _Tr("Truncate successfull"));

	ba.fill(true);
	CWT_TEST_OK(ba.count(true) == 10);

	for (size_t i = 0; i < 10; ++i)
		ba.toggleBit(i);

	CWT_TEST_OK2(ba.count(false) == 10, _Tr("Toggle bits successfull"));
}

void test_logic_operations()
{
	for(size_t sz = 2; sz < 1034; sz += 2) {
		BitArray ba1(sz), ba2(sz);

		ba1.fill(true);
		ba2.fill(false);
		CWT_TEST_OK(ba1 != ba2);

		ba2.fill(true);
		CWT_TEST_OK(ba1 == ba2);

		ba2 = ~ba2;
		CWT_TEST_OK(ba2.count(false) == ba2.size());

		ba1.fill(true);
		ba2.truncate(ba1.size());
		ba2.fill(true);

		for(size_t i = 0, j = ba1.size()/2; i < ba1.size()/2; ++i, ++j) {
			ba1.toggleBit(i);
			ba2.toggleBit(j);
		}

		CWT_TEST_OK(ba1.count(false) == ba1.size()/2);
		CWT_TEST_OK(ba2.count(true) == ba2.size()/2);
		CWT_TEST_OK(ba1 != ba2);

		BitArray ba = ba1 | ba2;
		CWT_TEST_OK(ba.size() == ba1.size());
		CWT_TEST_OK(ba.count(true) == ba.size());

		ba = ba1 & ba2;
		CWT_TEST_OK(ba.count(false) == ba.size());

		ba = ba1 ^ ba2;
		CWT_TEST_OK(ba.count(true) == ba.size());

		ba = ba ^ ba1;
		CWT_TEST_OK(ba == ba2);

		ba = ba ^ ba2;
		CWT_TEST_OK(ba.count(false) == ba.size());
	}
}

int main(int argc, char *argv[])
{
    CWT_CHECK_SIZEOF_TYPES;
    CWT_UNUSED2(argc, argv);
	CWT_BEGIN_TESTS(6199);

	test_set_bits();
	test_logic_operations();

    CWT_END_TESTS;
}



