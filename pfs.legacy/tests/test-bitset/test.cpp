/*
 * bit_array.cpp
 *
 *  Created on: Jun 27, 2013
 *      Author: wladt
 */

#include <pfs/test/test.hpp>
#include <pfs/bitset.hpp>
#include <cstring>

void test_subscript_operator ()
{
    ADD_TESTS(9);
    
    pfs::bitset<8> b(42);
    
    TEST_OK(b[0] == false);
    TEST_OK(b[1] == true);
    TEST_OK(b[2] == false);
    TEST_OK(b[3] == true);
    TEST_OK(b[4] == false);
    TEST_OK(b[5] == true);
    TEST_OK(b[6] == false);
    TEST_OK(b[7] == false);
    
    b[0] = true; // modifies the first bit through bitset::reference
    
    TEST_OK(b[0] == true);
}

void test_set_bits()
{
    ADD_TESTS(4);
    
	pfs::bitset<20000> ba; // set all bits to 1
    ba.set(); // set to true

	for (size_t i = 0; i < ba.size() ; i += 2) {
		ba.set(i, false);
	}

	int c = 0;
    
	for (size_t i = 0; i < ba.size() ; ++i) {
		if (ba[i])
			++c;
		else
			--c;
	}

	TEST_OK2(c == 0, "Test interleaved bits");
	TEST_OK(ba.count(true) == ba.count(false));
	TEST_OK(ba.count(true) == ba.size()/2);
	TEST_OK(ba.count(false) == ba.size()/2);
}

void test_logic_operations()
{
#if __COMMENT__
	for(size_t sz = 2; sz < 1034; sz += 2) {
		pfs::bit_array ba1(sz), ba2(sz);

		ba1.fill(true);
		ba2.fill(false);
		TEST_OK(ba1 != ba2);

		ba2.fill(true);
		TEST_OK(ba1 == ba2);

		ba2 = ~ba2;
		TEST_OK(ba2.count(false) == ba2.size());

		ba1.fill(true);
		ba2.truncate(ba1.size());
		ba2.fill(true);

		for(size_t i = 0, j = ba1.size()/2; i < ba1.size()/2; ++i, ++j) {
			ba1.toggleBit(i);
			ba2.toggleBit(j);
		}

		TEST_OK(ba1.count(false) == ba1.size()/2);
		TEST_OK(ba2.count(true) == ba2.size()/2);
		TEST_OK(ba1 != ba2);

		pfs::bit_array ba = ba1 | ba2;
		TEST_OK(ba.size() == ba1.size());
		TEST_OK(ba.count(true) == ba.size());

		ba = ba1 & ba2;
		TEST_OK(ba.count(false) == ba.size());

		ba = ba1 ^ ba2;
		TEST_OK(ba.count(true) == ba.size());

		ba = ba ^ ba1;
		TEST_OK(ba == ba2);

		ba = ba ^ ba2;
		TEST_OK(ba.count(false) == ba.size());
	}
#endif
}

void test_test ()
{
    ADD_TESTS(2);
    
    bool ok = false;
    
    pfs::bitset<10> b = pfs::make_bitset<10>(pfs::byte_string("1111010000"), & ok);

    TEST_FAIL2(ok, "Made bitset from byte string");
    
    size_t i = 0;
    size_t ntrue = 0;
           
    while (i < b.size()) {
        if (b.test(i))
            ++ntrue;
        
        ++i;
    }
 
    TEST_OK(ntrue == 5);
}

int main(int argc, char *argv[])
{
    PFS_UNUSED2(argc, argv);
	//BEGIN_TESTS(6199);
    BEGIN_TESTS(0);

    test_subscript_operator();
	test_set_bits();
	test_logic_operations();
    test_test();

    return END_TESTS;
}
