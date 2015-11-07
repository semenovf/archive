/*
 * utf8.cpp
 *
 *  Created on: 31 мая 2015 г.
 *      Author: wladt
 */


#include <pfs/test/test.hpp>
#include <pfs/ucchar.hpp>
#include <pfs/string.hpp>
#include <cstring>
#include <iostream>

using namespace std;

extern void test_iterator ();
extern void test_decode ();
extern void test_encode ();

//const uint8_t latin1_alphabet_utf8[] = { // 1-byte utf8 encoding
//		  0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50
//		, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66
//		, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76
//		, 0x77, 0x78, 0x79, 0x7A
//};
//
//
//const uint8_t cyrillic_alphabet_utf8[] = { // 2-byte utf8 encoding
//	      0xD0, 0x90, 0xD0, 0x91, 0xD0, 0x92, 0xD0, 0x93, 0xD0, 0x94, 0xD0, 0x95, 0xD0, 0x81, 0xD0, 0x96
//		, 0xD0, 0x97, 0xD0, 0x98, 0xD0, 0x99, 0xD0, 0x9A, 0xD0, 0x9B, 0xD0, 0x9C, 0xD0, 0x9D, 0xD0, 0x9E
//		, 0xD0, 0x9F, 0xD0, 0xA0, 0xD0, 0xA1, 0xD0, 0xA2, 0xD0, 0xA3, 0xD0, 0xA4, 0xD0, 0xA5, 0xD0, 0xA6
//		, 0xD0, 0xA7, 0xD0, 0xA8, 0xD0, 0xA9, 0xD0, 0xAA, 0xD0, 0xAB, 0xD0, 0xAC, 0xD0, 0xAD, 0xD0, 0xAE
//		, 0xD0, 0xAF, 0xD0, 0xB0, 0xD0, 0xB1, 0xD0, 0xB2, 0xD0, 0xB3, 0xD0, 0xB4, 0xD0, 0xB5, 0xD1, 0x91
//		, 0xD0, 0xB6, 0xD0, 0xB7, 0xD0, 0xB8, 0xD0, 0xB9, 0xD0, 0xBA, 0xD0, 0xBB, 0xD0, 0xBC, 0xD0, 0xBD
//		, 0xD0, 0xBE, 0xD0, 0xBF, 0xD1, 0x80, 0xD1, 0x81, 0xD1, 0x82, 0xD1, 0x83, 0xD1, 0x84, 0xD1, 0x85
//		, 0xD1, 0x86, 0xD1, 0x87, 0xD1, 0x88, 0xD1, 0x89, 0xD1, 0x8A, 0xD1, 0x8B, 0xD1, 0x8C, 0xD1, 0x8D
//		, 0xD1, 0x8E, 0xD1, 0x8F
//};


int main(int argc, char *argv[])
{
    PFS_UNUSED2(argc, argv);
	BEGIN_TESTS(0);

	test_iterator();
	test_decode();
	test_encode();

    END_TESTS;
}
