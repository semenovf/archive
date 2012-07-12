/*
 * crc64.c
 *
 *  Created on: 28.06.2012
 *      Author: wladt
 */

#include <cwt/types.h>

static INT64 __crc64_lookup_table[] = {
	0x0000000000000000LL, 0x01b0000000000000LL, 0x0360000000000000LL,
	0x02d0000000000000LL, 0x06c0000000000000LL, 0x0770000000000000LL,
	0x05a0000000000000LL, 0x0410000000000000LL, 0x0d80000000000000LL,
	0x0c30000000000000LL, 0x0ee0000000000000LL, 0x0f50000000000000LL,
	0x0b40000000000000LL, 0x0af0000000000000LL, 0x0820000000000000LL,
	0x0990000000000000LL, 0x1b00000000000000LL, 0x1ab0000000000000LL,
	0x1860000000000000LL, 0x19d0000000000000LL, 0x1dc0000000000000LL,
	0x1c70000000000000LL, 0x1ea0000000000000LL, 0x1f10000000000000LL,
	0x1680000000000000LL, 0x1730000000000000LL, 0x15e0000000000000LL,
	0x1450000000000000LL, 0x1040000000000000LL, 0x11f0000000000000LL,
	0x1320000000000000LL, 0x1290000000000000LL, 0x3600000000000000LL,
	0x37b0000000000000LL, 0x3560000000000000LL, 0x34d0000000000000LL,
	0x30c0000000000000LL, 0x3170000000000000LL, 0x33a0000000000000LL,
	0x3210000000000000LL, 0x3b80000000000000LL, 0x3a30000000000000LL,
	0x38e0000000000000LL, 0x3950000000000000LL, 0x3d40000000000000LL,
	0x3cf0000000000000LL, 0x3e20000000000000LL, 0x3f90000000000000LL,
	0x2d00000000000000LL, 0x2cb0000000000000LL, 0x2e60000000000000LL,
	0x2fd0000000000000LL, 0x2bc0000000000000LL, 0x2a70000000000000LL,
	0x28a0000000000000LL, 0x2910000000000000LL, 0x2080000000000000LL,
	0x2130000000000000LL, 0x23e0000000000000LL, 0x2250000000000000LL,
	0x2640000000000000LL, 0x27f0000000000000LL, 0x2520000000000000LL,
	0x2490000000000000LL, 0x6c00000000000000LL, 0x6db0000000000000LL,
	0x6f60000000000000LL, 0x6ed0000000000000LL, 0x6ac0000000000000LL,
	0x6b70000000000000LL, 0x69a0000000000000LL, 0x6810000000000000LL,
	0x6180000000000000LL, 0x6030000000000000LL, 0x62e0000000000000LL,
	0x6350000000000000LL, 0x6740000000000000LL, 0x66f0000000000000LL,
	0x6420000000000000LL, 0x6590000000000000LL, 0x7700000000000000LL,
	0x76b0000000000000LL, 0x7460000000000000LL, 0x75d0000000000000LL,
	0x71c0000000000000LL, 0x7070000000000000LL, 0x72a0000000000000LL,
	0x7310000000000000LL, 0x7a80000000000000LL, 0x7b30000000000000LL,
	0x79e0000000000000LL, 0x7850000000000000LL, 0x7c40000000000000LL,
	0x7df0000000000000LL, 0x7f20000000000000LL, 0x7e90000000000000LL,
	0x5a00000000000000LL, 0x5bb0000000000000LL, 0x5960000000000000LL,
	0x58d0000000000000LL, 0x5cc0000000000000LL, 0x5d70000000000000LL,
	0x5fa0000000000000LL, 0x5e10000000000000LL, 0x5780000000000000LL,
	0x5630000000000000LL, 0x54e0000000000000LL, 0x5550000000000000LL,
	0x5140000000000000LL, 0x50f0000000000000LL, 0x5220000000000000LL,
	0x5390000000000000LL, 0x4100000000000000LL, 0x40b0000000000000LL,
	0x4260000000000000LL, 0x43d0000000000000LL, 0x47c0000000000000LL,
	0x4670000000000000LL, 0x44a0000000000000LL, 0x4510000000000000LL,
	0x4c80000000000000LL, 0x4d30000000000000LL, 0x4fe0000000000000LL,
	0x4e50000000000000LL, 0x4a40000000000000LL, 0x4bf0000000000000LL,
	0x4920000000000000LL, 0x4890000000000000LL, 0xd800000000000000LL,
	0xd9b0000000000000LL, 0xdb60000000000000LL, 0xdad0000000000000LL,
	0xdec0000000000000LL, 0xdf70000000000000LL, 0xdda0000000000000LL,
	0xdc10000000000000LL, 0xd580000000000000LL, 0xd430000000000000LL,
	0xd6e0000000000000LL, 0xd750000000000000LL, 0xd340000000000000LL,
	0xd2f0000000000000LL, 0xd020000000000000LL, 0xd190000000000000LL,
	0xc300000000000000LL, 0xc2b0000000000000LL, 0xc060000000000000LL,
	0xc1d0000000000000LL, 0xc5c0000000000000LL, 0xc470000000000000LL,
	0xc6a0000000000000LL, 0xc710000000000000LL, 0xce80000000000000LL,
	0xcf30000000000000LL, 0xcde0000000000000LL, 0xcc50000000000000LL,
	0xc840000000000000LL, 0xc9f0000000000000LL, 0xcb20000000000000LL,
	0xca90000000000000LL, 0xee00000000000000LL, 0xefb0000000000000LL,
	0xed60000000000000LL, 0xecd0000000000000LL, 0xe8c0000000000000LL,
	0xe970000000000000LL, 0xeba0000000000000LL, 0xea10000000000000LL,
	0xe380000000000000LL, 0xe230000000000000LL, 0xe0e0000000000000LL,
	0xe150000000000000LL, 0xe540000000000000LL, 0xe4f0000000000000LL,
	0xe620000000000000LL, 0xe790000000000000LL, 0xf500000000000000LL,
	0xf4b0000000000000LL, 0xf660000000000000LL, 0xf7d0000000000000LL,
	0xf3c0000000000000LL, 0xf270000000000000LL, 0xf0a0000000000000LL,
	0xf110000000000000LL, 0xf880000000000000LL, 0xf930000000000000LL,
	0xfbe0000000000000LL, 0xfa50000000000000LL, 0xfe40000000000000LL,
	0xfff0000000000000LL, 0xfd20000000000000LL, 0xfc90000000000000LL,
	0xb400000000000000LL, 0xb5b0000000000000LL, 0xb760000000000000LL,
	0xb6d0000000000000LL, 0xb2c0000000000000LL, 0xb370000000000000LL,
	0xb1a0000000000000LL, 0xb010000000000000LL, 0xb980000000000000LL,
	0xb830000000000000LL, 0xbae0000000000000LL, 0xbb50000000000000LL,
	0xbf40000000000000LL, 0xbef0000000000000LL, 0xbc20000000000000LL,
	0xbd90000000000000LL, 0xaf00000000000000LL, 0xaeb0000000000000LL,
	0xac60000000000000LL, 0xadd0000000000000LL, 0xa9c0000000000000LL,
	0xa870000000000000LL, 0xaaa0000000000000LL, 0xab10000000000000LL,
	0xa280000000000000LL, 0xa330000000000000LL, 0xa1e0000000000000LL,
	0xa050000000000000LL, 0xa440000000000000LL, 0xa5f0000000000000LL,
	0xa720000000000000LL, 0xa690000000000000LL, 0x8200000000000000LL,
	0x83b0000000000000LL, 0x8160000000000000LL, 0x80d0000000000000LL,
	0x84c0000000000000LL, 0x8570000000000000LL, 0x87a0000000000000LL,
	0x8610000000000000LL, 0x8f80000000000000LL, 0x8e30000000000000LL,
	0x8ce0000000000000LL, 0x8d50000000000000LL, 0x8940000000000000LL,
	0x88f0000000000000LL, 0x8a20000000000000LL, 0x8b90000000000000LL,
	0x9900000000000000LL, 0x98b0000000000000LL, 0x9a60000000000000LL,
	0x9bd0000000000000LL, 0x9fc0000000000000LL, 0x9e70000000000000LL,
	0x9ca0000000000000LL, 0x9d10000000000000LL, 0x9480000000000000LL,
	0x9530000000000000LL, 0x97e0000000000000LL, 0x9650000000000000LL,
	0x9240000000000000LL, 0x93f0000000000000LL, 0x9120000000000000LL,
	0x9090000000000000LL
};


/**
 * @brief Calculates the CRC64 checksum for the given array of bytes
 *        using ⠡��筮�� �����⬠ crc64 � ��������� x^64 + x^4 + x^3 + x + 1.
 *
 * @param pdata data for checksum calculation
 * @param nbytes data length in bytes
 * @param crc64 initial value for checksum
 * @return CRC64 checksum value
 *
 * @see ���᭨⥫쭠� ����᪠ ����.466535.003 ��
 * @see http://en.wikipedia.org/wiki/Cyclic_redundancy_check
 */
/*
#define CRC32(oldcrc, curByte) (m_crc32table[BYTE(oldcrc)^BYTE(curByte)]^(DWORD(oldcrc)>>8))
#define CRC64(oldcrc, curByte) (m_crc64table[BYTE(oldcrc)^BYTE(curByte)]^(QWORD(oldcrc)>>8))
*/

INT64 __calculateCRC64(const void *pdata, size_t nbytes, INT64 crc64)
{
	const BYTE *pbytes = (const BYTE*)pdata;
	while( nbytes-- )
		crc64 = __crc64_lookup_table[(crc64 ^ *pbytes++) & 0xff ] ^ (((UINT64)crc64) >> 8);
	return crc64;
}


#ifdef CWT_USE_TESTS
#include <cwt/test.h>

static INT64 __crc64_poly64rev = 0xd800000000000000LL;

static void __cwtCrc64GenLookupTable(INT64 tab[256])
{
	int i, j;

	for( i = 0; i < 256; i++ ) {
		UINT64 v = i;
		for( j = 0; j < 8; j++ ) {
			if( (v & 1) == 1 ) {
				v = (v >> 1) ^ __crc64_poly64rev;
			} else {
				v = (v >> 1);
			}
		}
		tab[i] = (INT64)v;
	}
}

DLL_API_EXPORT void cwt_test_crc64(void)
{
	int i;
	INT64 tab[256];
	CHAR teststr[256];

	__cwtCrc64GenLookupTable(tab);

	for( i = 0; i < 256; i++ ) {
		sprintf(teststr, "Element %d in static crc64_lookup_table[%d] == generated_table[%d]", i, i);
		CWT_TEST_OK2(__crc64_lookup_table[i] == tab[i], "teststr");
	}
}
#endif
