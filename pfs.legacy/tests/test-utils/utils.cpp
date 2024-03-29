/**
 * @file crc32.cpp
 * @author wladt
 * @date May 6, 2014
 */

#include <cstring>
#include <cstdio>
#include <pfs/test/test.hpp>
#include <pfs/utils.hpp>

#define PFS_INT32_C(x) int32_t(x##L)
#define PFS_INT64_C(x) int64_t(x##LL)

static int32_t __crc32_lookup_table[] = {
	  PFS_INT32_C(0x00000000), PFS_INT32_C(0x77073096), PFS_INT32_C(0xee0e612c), PFS_INT32_C(0x990951ba), PFS_INT32_C(0x076dc419), PFS_INT32_C(0x706af48f), PFS_INT32_C(0xe963a535), PFS_INT32_C(0x9e6495a3)
	, PFS_INT32_C(0x0edb8832), PFS_INT32_C(0x79dcb8a4), PFS_INT32_C(0xe0d5e91e), PFS_INT32_C(0x97d2d988), PFS_INT32_C(0x09b64c2b), PFS_INT32_C(0x7eb17cbd), PFS_INT32_C(0xe7b82d07), PFS_INT32_C(0x90bf1d91)
	, PFS_INT32_C(0x1db71064), PFS_INT32_C(0x6ab020f2), PFS_INT32_C(0xf3b97148), PFS_INT32_C(0x84be41de), PFS_INT32_C(0x1adad47d), PFS_INT32_C(0x6ddde4eb), PFS_INT32_C(0xf4d4b551), PFS_INT32_C(0x83d385c7)
	, PFS_INT32_C(0x136c9856), PFS_INT32_C(0x646ba8c0), PFS_INT32_C(0xfd62f97a), PFS_INT32_C(0x8a65c9ec), PFS_INT32_C(0x14015c4f), PFS_INT32_C(0x63066cd9), PFS_INT32_C(0xfa0f3d63), PFS_INT32_C(0x8d080df5)
	, PFS_INT32_C(0x3b6e20c8), PFS_INT32_C(0x4c69105e), PFS_INT32_C(0xd56041e4), PFS_INT32_C(0xa2677172), PFS_INT32_C(0x3c03e4d1), PFS_INT32_C(0x4b04d447), PFS_INT32_C(0xd20d85fd), PFS_INT32_C(0xa50ab56b)
	, PFS_INT32_C(0x35b5a8fa), PFS_INT32_C(0x42b2986c), PFS_INT32_C(0xdbbbc9d6), PFS_INT32_C(0xacbcf940), PFS_INT32_C(0x32d86ce3), PFS_INT32_C(0x45df5c75), PFS_INT32_C(0xdcd60dcf), PFS_INT32_C(0xabd13d59)
	, PFS_INT32_C(0x26d930ac), PFS_INT32_C(0x51de003a), PFS_INT32_C(0xc8d75180), PFS_INT32_C(0xbfd06116), PFS_INT32_C(0x21b4f4b5), PFS_INT32_C(0x56b3c423), PFS_INT32_C(0xcfba9599), PFS_INT32_C(0xb8bda50f)
	, PFS_INT32_C(0x2802b89e), PFS_INT32_C(0x5f058808), PFS_INT32_C(0xc60cd9b2), PFS_INT32_C(0xb10be924), PFS_INT32_C(0x2f6f7c87), PFS_INT32_C(0x58684c11), PFS_INT32_C(0xc1611dab), PFS_INT32_C(0xb6662d3d)
	, PFS_INT32_C(0x76dc4190), PFS_INT32_C(0x01db7106), PFS_INT32_C(0x98d220bc), PFS_INT32_C(0xefd5102a), PFS_INT32_C(0x71b18589), PFS_INT32_C(0x06b6b51f), PFS_INT32_C(0x9fbfe4a5), PFS_INT32_C(0xe8b8d433)
	, PFS_INT32_C(0x7807c9a2), PFS_INT32_C(0x0f00f934), PFS_INT32_C(0x9609a88e), PFS_INT32_C(0xe10e9818), PFS_INT32_C(0x7f6a0dbb), PFS_INT32_C(0x086d3d2d), PFS_INT32_C(0x91646c97), PFS_INT32_C(0xe6635c01)
	, PFS_INT32_C(0x6b6b51f4), PFS_INT32_C(0x1c6c6162), PFS_INT32_C(0x856530d8), PFS_INT32_C(0xf262004e), PFS_INT32_C(0x6c0695ed), PFS_INT32_C(0x1b01a57b), PFS_INT32_C(0x8208f4c1), PFS_INT32_C(0xf50fc457)
	, PFS_INT32_C(0x65b0d9c6), PFS_INT32_C(0x12b7e950), PFS_INT32_C(0x8bbeb8ea), PFS_INT32_C(0xfcb9887c), PFS_INT32_C(0x62dd1ddf), PFS_INT32_C(0x15da2d49), PFS_INT32_C(0x8cd37cf3), PFS_INT32_C(0xfbd44c65)
	, PFS_INT32_C(0x4db26158), PFS_INT32_C(0x3ab551ce), PFS_INT32_C(0xa3bc0074), PFS_INT32_C(0xd4bb30e2), PFS_INT32_C(0x4adfa541), PFS_INT32_C(0x3dd895d7), PFS_INT32_C(0xa4d1c46d), PFS_INT32_C(0xd3d6f4fb)
	, PFS_INT32_C(0x4369e96a), PFS_INT32_C(0x346ed9fc), PFS_INT32_C(0xad678846), PFS_INT32_C(0xda60b8d0), PFS_INT32_C(0x44042d73), PFS_INT32_C(0x33031de5), PFS_INT32_C(0xaa0a4c5f), PFS_INT32_C(0xdd0d7cc9)
	, PFS_INT32_C(0x5005713c), PFS_INT32_C(0x270241aa), PFS_INT32_C(0xbe0b1010), PFS_INT32_C(0xc90c2086), PFS_INT32_C(0x5768b525), PFS_INT32_C(0x206f85b3), PFS_INT32_C(0xb966d409), PFS_INT32_C(0xce61e49f)
	, PFS_INT32_C(0x5edef90e), PFS_INT32_C(0x29d9c998), PFS_INT32_C(0xb0d09822), PFS_INT32_C(0xc7d7a8b4), PFS_INT32_C(0x59b33d17), PFS_INT32_C(0x2eb40d81), PFS_INT32_C(0xb7bd5c3b), PFS_INT32_C(0xc0ba6cad)
	, PFS_INT32_C(0xedb88320), PFS_INT32_C(0x9abfb3b6), PFS_INT32_C(0x03b6e20c), PFS_INT32_C(0x74b1d29a), PFS_INT32_C(0xead54739), PFS_INT32_C(0x9dd277af), PFS_INT32_C(0x04db2615), PFS_INT32_C(0x73dc1683)
	, PFS_INT32_C(0xe3630b12), PFS_INT32_C(0x94643b84), PFS_INT32_C(0x0d6d6a3e), PFS_INT32_C(0x7a6a5aa8), PFS_INT32_C(0xe40ecf0b), PFS_INT32_C(0x9309ff9d), PFS_INT32_C(0x0a00ae27), PFS_INT32_C(0x7d079eb1)
	, PFS_INT32_C(0xf00f9344), PFS_INT32_C(0x8708a3d2), PFS_INT32_C(0x1e01f268), PFS_INT32_C(0x6906c2fe), PFS_INT32_C(0xf762575d), PFS_INT32_C(0x806567cb), PFS_INT32_C(0x196c3671), PFS_INT32_C(0x6e6b06e7)
	, PFS_INT32_C(0xfed41b76), PFS_INT32_C(0x89d32be0), PFS_INT32_C(0x10da7a5a), PFS_INT32_C(0x67dd4acc), PFS_INT32_C(0xf9b9df6f), PFS_INT32_C(0x8ebeeff9), PFS_INT32_C(0x17b7be43), PFS_INT32_C(0x60b08ed5)
	, PFS_INT32_C(0xd6d6a3e8), PFS_INT32_C(0xa1d1937e), PFS_INT32_C(0x38d8c2c4), PFS_INT32_C(0x4fdff252), PFS_INT32_C(0xd1bb67f1), PFS_INT32_C(0xa6bc5767), PFS_INT32_C(0x3fb506dd), PFS_INT32_C(0x48b2364b)
	, PFS_INT32_C(0xd80d2bda), PFS_INT32_C(0xaf0a1b4c), PFS_INT32_C(0x36034af6), PFS_INT32_C(0x41047a60), PFS_INT32_C(0xdf60efc3), PFS_INT32_C(0xa867df55), PFS_INT32_C(0x316e8eef), PFS_INT32_C(0x4669be79)
	, PFS_INT32_C(0xcb61b38c), PFS_INT32_C(0xbc66831a), PFS_INT32_C(0x256fd2a0), PFS_INT32_C(0x5268e236), PFS_INT32_C(0xcc0c7795), PFS_INT32_C(0xbb0b4703), PFS_INT32_C(0x220216b9), PFS_INT32_C(0x5505262f)
	, PFS_INT32_C(0xc5ba3bbe), PFS_INT32_C(0xb2bd0b28), PFS_INT32_C(0x2bb45a92), PFS_INT32_C(0x5cb36a04), PFS_INT32_C(0xc2d7ffa7), PFS_INT32_C(0xb5d0cf31), PFS_INT32_C(0x2cd99e8b), PFS_INT32_C(0x5bdeae1d)
	, PFS_INT32_C(0x9b64c2b0), PFS_INT32_C(0xec63f226), PFS_INT32_C(0x756aa39c), PFS_INT32_C(0x026d930a), PFS_INT32_C(0x9c0906a9), PFS_INT32_C(0xeb0e363f), PFS_INT32_C(0x72076785), PFS_INT32_C(0x05005713)
	, PFS_INT32_C(0x95bf4a82), PFS_INT32_C(0xe2b87a14), PFS_INT32_C(0x7bb12bae), PFS_INT32_C(0x0cb61b38), PFS_INT32_C(0x92d28e9b), PFS_INT32_C(0xe5d5be0d), PFS_INT32_C(0x7cdcefb7), PFS_INT32_C(0x0bdbdf21)
	, PFS_INT32_C(0x86d3d2d4), PFS_INT32_C(0xf1d4e242), PFS_INT32_C(0x68ddb3f8), PFS_INT32_C(0x1fda836e), PFS_INT32_C(0x81be16cd), PFS_INT32_C(0xf6b9265b), PFS_INT32_C(0x6fb077e1), PFS_INT32_C(0x18b74777)
	, PFS_INT32_C(0x88085ae6), PFS_INT32_C(0xff0f6a70), PFS_INT32_C(0x66063bca), PFS_INT32_C(0x11010b5c), PFS_INT32_C(0x8f659eff), PFS_INT32_C(0xf862ae69), PFS_INT32_C(0x616bffd3), PFS_INT32_C(0x166ccf45)
	, PFS_INT32_C(0xa00ae278), PFS_INT32_C(0xd70dd2ee), PFS_INT32_C(0x4e048354), PFS_INT32_C(0x3903b3c2), PFS_INT32_C(0xa7672661), PFS_INT32_C(0xd06016f7), PFS_INT32_C(0x4969474d), PFS_INT32_C(0x3e6e77db)
	, PFS_INT32_C(0xaed16a4a), PFS_INT32_C(0xd9d65adc), PFS_INT32_C(0x40df0b66), PFS_INT32_C(0x37d83bf0), PFS_INT32_C(0xa9bcae53), PFS_INT32_C(0xdebb9ec5), PFS_INT32_C(0x47b2cf7f), PFS_INT32_C(0x30b5ffe9)
	, PFS_INT32_C(0xbdbdf21c), PFS_INT32_C(0xcabac28a), PFS_INT32_C(0x53b39330), PFS_INT32_C(0x24b4a3a6), PFS_INT32_C(0xbad03605), PFS_INT32_C(0xcdd70693), PFS_INT32_C(0x54de5729), PFS_INT32_C(0x23d967bf)
	, PFS_INT32_C(0xb3667a2e), PFS_INT32_C(0xc4614ab8), PFS_INT32_C(0x5d681b02), PFS_INT32_C(0x2a6f2b94), PFS_INT32_C(0xb40bbe37), PFS_INT32_C(0xc30c8ea1), PFS_INT32_C(0x5a05df1b), PFS_INT32_C(0x2d02ef8d)
};

static int64_t __crc64_lookup_table[] = {
	PFS_INT64_C(0x0000000000000000), PFS_INT64_C(0x01b0000000000000), PFS_INT64_C(0x0360000000000000),
	PFS_INT64_C(0x02d0000000000000), PFS_INT64_C(0x06c0000000000000), PFS_INT64_C(0x0770000000000000),
	PFS_INT64_C(0x05a0000000000000), PFS_INT64_C(0x0410000000000000), PFS_INT64_C(0x0d80000000000000),
	PFS_INT64_C(0x0c30000000000000), PFS_INT64_C(0x0ee0000000000000), PFS_INT64_C(0x0f50000000000000),
	PFS_INT64_C(0x0b40000000000000), PFS_INT64_C(0x0af0000000000000), PFS_INT64_C(0x0820000000000000),
	PFS_INT64_C(0x0990000000000000), PFS_INT64_C(0x1b00000000000000), PFS_INT64_C(0x1ab0000000000000),
	PFS_INT64_C(0x1860000000000000), PFS_INT64_C(0x19d0000000000000), PFS_INT64_C(0x1dc0000000000000),
	PFS_INT64_C(0x1c70000000000000), PFS_INT64_C(0x1ea0000000000000), PFS_INT64_C(0x1f10000000000000),
	PFS_INT64_C(0x1680000000000000), PFS_INT64_C(0x1730000000000000), PFS_INT64_C(0x15e0000000000000),
	PFS_INT64_C(0x1450000000000000), PFS_INT64_C(0x1040000000000000), PFS_INT64_C(0x11f0000000000000),
	PFS_INT64_C(0x1320000000000000), PFS_INT64_C(0x1290000000000000), PFS_INT64_C(0x3600000000000000),
	PFS_INT64_C(0x37b0000000000000), PFS_INT64_C(0x3560000000000000), PFS_INT64_C(0x34d0000000000000),
	PFS_INT64_C(0x30c0000000000000), PFS_INT64_C(0x3170000000000000), PFS_INT64_C(0x33a0000000000000),
	PFS_INT64_C(0x3210000000000000), PFS_INT64_C(0x3b80000000000000), PFS_INT64_C(0x3a30000000000000),
	PFS_INT64_C(0x38e0000000000000), PFS_INT64_C(0x3950000000000000), PFS_INT64_C(0x3d40000000000000),
	PFS_INT64_C(0x3cf0000000000000), PFS_INT64_C(0x3e20000000000000), PFS_INT64_C(0x3f90000000000000),
	PFS_INT64_C(0x2d00000000000000), PFS_INT64_C(0x2cb0000000000000), PFS_INT64_C(0x2e60000000000000),
	PFS_INT64_C(0x2fd0000000000000), PFS_INT64_C(0x2bc0000000000000), PFS_INT64_C(0x2a70000000000000),
	PFS_INT64_C(0x28a0000000000000), PFS_INT64_C(0x2910000000000000), PFS_INT64_C(0x2080000000000000),
	PFS_INT64_C(0x2130000000000000), PFS_INT64_C(0x23e0000000000000), PFS_INT64_C(0x2250000000000000),
	PFS_INT64_C(0x2640000000000000), PFS_INT64_C(0x27f0000000000000), PFS_INT64_C(0x2520000000000000),
	PFS_INT64_C(0x2490000000000000), PFS_INT64_C(0x6c00000000000000), PFS_INT64_C(0x6db0000000000000),
	PFS_INT64_C(0x6f60000000000000), PFS_INT64_C(0x6ed0000000000000), PFS_INT64_C(0x6ac0000000000000),
	PFS_INT64_C(0x6b70000000000000), PFS_INT64_C(0x69a0000000000000), PFS_INT64_C(0x6810000000000000),
	PFS_INT64_C(0x6180000000000000), PFS_INT64_C(0x6030000000000000), PFS_INT64_C(0x62e0000000000000),
	PFS_INT64_C(0x6350000000000000), PFS_INT64_C(0x6740000000000000), PFS_INT64_C(0x66f0000000000000),
	PFS_INT64_C(0x6420000000000000), PFS_INT64_C(0x6590000000000000), PFS_INT64_C(0x7700000000000000),
	PFS_INT64_C(0x76b0000000000000), PFS_INT64_C(0x7460000000000000), PFS_INT64_C(0x75d0000000000000),
	PFS_INT64_C(0x71c0000000000000), PFS_INT64_C(0x7070000000000000), PFS_INT64_C(0x72a0000000000000),
	PFS_INT64_C(0x7310000000000000), PFS_INT64_C(0x7a80000000000000), PFS_INT64_C(0x7b30000000000000),
	PFS_INT64_C(0x79e0000000000000), PFS_INT64_C(0x7850000000000000), PFS_INT64_C(0x7c40000000000000),
	PFS_INT64_C(0x7df0000000000000), PFS_INT64_C(0x7f20000000000000), PFS_INT64_C(0x7e90000000000000),
	PFS_INT64_C(0x5a00000000000000), PFS_INT64_C(0x5bb0000000000000), PFS_INT64_C(0x5960000000000000),
	PFS_INT64_C(0x58d0000000000000), PFS_INT64_C(0x5cc0000000000000), PFS_INT64_C(0x5d70000000000000),
	PFS_INT64_C(0x5fa0000000000000), PFS_INT64_C(0x5e10000000000000), PFS_INT64_C(0x5780000000000000),
	PFS_INT64_C(0x5630000000000000), PFS_INT64_C(0x54e0000000000000), PFS_INT64_C(0x5550000000000000),
	PFS_INT64_C(0x5140000000000000), PFS_INT64_C(0x50f0000000000000), PFS_INT64_C(0x5220000000000000),
	PFS_INT64_C(0x5390000000000000), PFS_INT64_C(0x4100000000000000), PFS_INT64_C(0x40b0000000000000),
	PFS_INT64_C(0x4260000000000000), PFS_INT64_C(0x43d0000000000000), PFS_INT64_C(0x47c0000000000000),
	PFS_INT64_C(0x4670000000000000), PFS_INT64_C(0x44a0000000000000), PFS_INT64_C(0x4510000000000000),
	PFS_INT64_C(0x4c80000000000000), PFS_INT64_C(0x4d30000000000000), PFS_INT64_C(0x4fe0000000000000),
	PFS_INT64_C(0x4e50000000000000), PFS_INT64_C(0x4a40000000000000), PFS_INT64_C(0x4bf0000000000000),
	PFS_INT64_C(0x4920000000000000), PFS_INT64_C(0x4890000000000000), PFS_INT64_C(0xd800000000000000),
	PFS_INT64_C(0xd9b0000000000000), PFS_INT64_C(0xdb60000000000000), PFS_INT64_C(0xdad0000000000000),
	PFS_INT64_C(0xdec0000000000000), PFS_INT64_C(0xdf70000000000000), PFS_INT64_C(0xdda0000000000000),
	PFS_INT64_C(0xdc10000000000000), PFS_INT64_C(0xd580000000000000), PFS_INT64_C(0xd430000000000000),
	PFS_INT64_C(0xd6e0000000000000), PFS_INT64_C(0xd750000000000000), PFS_INT64_C(0xd340000000000000),
	PFS_INT64_C(0xd2f0000000000000), PFS_INT64_C(0xd020000000000000), PFS_INT64_C(0xd190000000000000),
	PFS_INT64_C(0xc300000000000000), PFS_INT64_C(0xc2b0000000000000), PFS_INT64_C(0xc060000000000000),
	PFS_INT64_C(0xc1d0000000000000), PFS_INT64_C(0xc5c0000000000000), PFS_INT64_C(0xc470000000000000),
	PFS_INT64_C(0xc6a0000000000000), PFS_INT64_C(0xc710000000000000), PFS_INT64_C(0xce80000000000000),
	PFS_INT64_C(0xcf30000000000000), PFS_INT64_C(0xcde0000000000000), PFS_INT64_C(0xcc50000000000000),
	PFS_INT64_C(0xc840000000000000), PFS_INT64_C(0xc9f0000000000000), PFS_INT64_C(0xcb20000000000000),
	PFS_INT64_C(0xca90000000000000), PFS_INT64_C(0xee00000000000000), PFS_INT64_C(0xefb0000000000000),
	PFS_INT64_C(0xed60000000000000), PFS_INT64_C(0xecd0000000000000), PFS_INT64_C(0xe8c0000000000000),
	PFS_INT64_C(0xe970000000000000), PFS_INT64_C(0xeba0000000000000), PFS_INT64_C(0xea10000000000000),
	PFS_INT64_C(0xe380000000000000), PFS_INT64_C(0xe230000000000000), PFS_INT64_C(0xe0e0000000000000),
	PFS_INT64_C(0xe150000000000000), PFS_INT64_C(0xe540000000000000), PFS_INT64_C(0xe4f0000000000000),
	PFS_INT64_C(0xe620000000000000), PFS_INT64_C(0xe790000000000000), PFS_INT64_C(0xf500000000000000),
	PFS_INT64_C(0xf4b0000000000000), PFS_INT64_C(0xf660000000000000), PFS_INT64_C(0xf7d0000000000000),
	PFS_INT64_C(0xf3c0000000000000), PFS_INT64_C(0xf270000000000000), PFS_INT64_C(0xf0a0000000000000),
	PFS_INT64_C(0xf110000000000000), PFS_INT64_C(0xf880000000000000), PFS_INT64_C(0xf930000000000000),
	PFS_INT64_C(0xfbe0000000000000), PFS_INT64_C(0xfa50000000000000), PFS_INT64_C(0xfe40000000000000),
	PFS_INT64_C(0xfff0000000000000), PFS_INT64_C(0xfd20000000000000), PFS_INT64_C(0xfc90000000000000),
	PFS_INT64_C(0xb400000000000000), PFS_INT64_C(0xb5b0000000000000), PFS_INT64_C(0xb760000000000000),
	PFS_INT64_C(0xb6d0000000000000), PFS_INT64_C(0xb2c0000000000000), PFS_INT64_C(0xb370000000000000),
	PFS_INT64_C(0xb1a0000000000000), PFS_INT64_C(0xb010000000000000), PFS_INT64_C(0xb980000000000000),
	PFS_INT64_C(0xb830000000000000), PFS_INT64_C(0xbae0000000000000), PFS_INT64_C(0xbb50000000000000),
	PFS_INT64_C(0xbf40000000000000), PFS_INT64_C(0xbef0000000000000), PFS_INT64_C(0xbc20000000000000),
	PFS_INT64_C(0xbd90000000000000), PFS_INT64_C(0xaf00000000000000), PFS_INT64_C(0xaeb0000000000000),
	PFS_INT64_C(0xac60000000000000), PFS_INT64_C(0xadd0000000000000), PFS_INT64_C(0xa9c0000000000000),
	PFS_INT64_C(0xa870000000000000), PFS_INT64_C(0xaaa0000000000000), PFS_INT64_C(0xab10000000000000),
	PFS_INT64_C(0xa280000000000000), PFS_INT64_C(0xa330000000000000), PFS_INT64_C(0xa1e0000000000000),
	PFS_INT64_C(0xa050000000000000), PFS_INT64_C(0xa440000000000000), PFS_INT64_C(0xa5f0000000000000),
	PFS_INT64_C(0xa720000000000000), PFS_INT64_C(0xa690000000000000), PFS_INT64_C(0x8200000000000000),
	PFS_INT64_C(0x83b0000000000000), PFS_INT64_C(0x8160000000000000), PFS_INT64_C(0x80d0000000000000),
	PFS_INT64_C(0x84c0000000000000), PFS_INT64_C(0x8570000000000000), PFS_INT64_C(0x87a0000000000000),
	PFS_INT64_C(0x8610000000000000), PFS_INT64_C(0x8f80000000000000), PFS_INT64_C(0x8e30000000000000),
	PFS_INT64_C(0x8ce0000000000000), PFS_INT64_C(0x8d50000000000000), PFS_INT64_C(0x8940000000000000),
	PFS_INT64_C(0x88f0000000000000), PFS_INT64_C(0x8a20000000000000), PFS_INT64_C(0x8b90000000000000),
	PFS_INT64_C(0x9900000000000000), PFS_INT64_C(0x98b0000000000000), PFS_INT64_C(0x9a60000000000000),
	PFS_INT64_C(0x9bd0000000000000), PFS_INT64_C(0x9fc0000000000000), PFS_INT64_C(0x9e70000000000000),
	PFS_INT64_C(0x9ca0000000000000), PFS_INT64_C(0x9d10000000000000), PFS_INT64_C(0x9480000000000000),
	PFS_INT64_C(0x9530000000000000), PFS_INT64_C(0x97e0000000000000), PFS_INT64_C(0x9650000000000000),
	PFS_INT64_C(0x9240000000000000), PFS_INT64_C(0x93f0000000000000), PFS_INT64_C(0x9120000000000000),
	PFS_INT64_C(0x9090000000000000)
};

static int32_t __crc32_poly32rev = int32_t(0xEDB88320);
static int64_t __crc64_poly64rev = PFS_INT64_C(0xd800000000000000);

static void __crc32_gen_lookup_table (int32_t tab[256])
{
	int i, j;

	for( i = 0; i < 256; ++i ) {
		uint32_t v = i;
		for( j = 0; j < 8; ++j ) {
			if( (v & 1) == 1 ) {
				v = (v >> 1) ^ __crc32_poly32rev;
			} else {
				v = (v >> 1);
			}
		}
		tab[i] = (int32_t)v;
	}
}

void test_crc32_lookup_table ()
{
	int i;
	int32_t tab[256];
	char teststr[256];

	__crc32_gen_lookup_table(tab);

	for( i = 0; i < 256; i++ ) {
		sprintf(teststr, "Element %d in static crc32_lookup_table[%d] == generated_table[%d]", i, i, i);
		TEST_OK2(__crc32_lookup_table[i] == tab[i], teststr);
	}
}

static void __crc64_gen_lookup_table (int64_t tab[256])
{
	int i, j;

	for (i = 0; i < 256; ++i) {
		uint64_t v = i;
		for (j = 0; j < 8; ++j) {
			if ((v & 1) == 1) {
				v = (v >> 1) ^ __crc64_poly64rev;
			} else {
				v = (v >> 1);
			}
		}
		tab[i] = (int64_t)v;
	}
}

void test_crc64_lookup_table ()
{
	int i;
	int64_t tab[256];
	char teststr[256];

	__crc64_gen_lookup_table(tab);

	for( i = 0; i < 256; i++ ) {
		sprintf(teststr, "Element %d in static crc64_lookup_table[%d] == generated_table[%d]", i, i, i);
		TEST_OK2(__crc64_lookup_table[i] == tab[i], teststr);
	}
}

void test_crc32 ()
{
	byte_t bytes[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	const char * chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
	char bytes256[256];
	int32_t crc32;
	int i = 0;

	for( i = 0; i < 256; i++ ) {
		bytes256[i] = 256 - i;
	}

	crc32 = pfs::crc32(bytes, sizeof(bytes), 0);
	TEST_OK2(1164760902 == crc32
		, "CRC32({0..9}) == 1164760902");

	crc32 = pfs::crc32(chars, strlen(chars), 0);
	TEST_OK2( -1764981246 == crc32
		, "CRC32(\"ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890\") == -1764981246");

	crc32 = pfs::crc32(bytes256, sizeof(bytes256), 0);
	TEST_OK2( -1386202695 == crc32
		, "CRC32({256..1}) == -1386202695");

	crc32 = pfs::crc32(bytes256, sizeof(bytes256), -1386202695);
	TEST_OK2( 2043667161 == crc32
		, "CRC32({256..1}) == 2043667161 (with initial = -1386202695)");

}

void test_crc64 ()
{
	char chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
	byte_t bytes[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	byte_t bytes256[256];
	int i = 0;

	for( i = 0; i < 256; i++ ) {
		bytes256[i] = 256 - i;
	}

	TEST_OK2( -4480566129418311872LL == pfs::crc64(chars, strlen(chars), 0)
		, "CRC64(\"ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890\") == -4480566129418311872");

	TEST_OK2( 6302932907043766995LL == pfs::crc64(bytes, sizeof(bytes), 0)
		, "CRC64({1..9}) == 6302932907043766995");

	TEST_OK2( 2876295860935205507LL == pfs::crc64(bytes256, sizeof(bytes256), 0)
		, "CRC64({256..1}) == 2876295860935205507");

	TEST_OK2( -6008134657629535LL == pfs::crc64(bytes256, sizeof(bytes256), 128)
		, "CRC64({256..1}) == -6008134657629535 (with initial = 128)");
}

int main(int argc, char *argv[])
{
    PFS_UNUSED2(argc, argv);
	BEGIN_TESTS(520);

	test_crc32_lookup_table();
	test_crc64_lookup_table();
	test_crc32();
	test_crc64();

	return END_TESTS;
}



