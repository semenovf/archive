/*
 * utils.c
 *
 *  Created on: 29.06.2012
 *      Author: user
 */


#include <stdio.h>
#include <cwt/types.h>
#include <cwt/test.h>
#include <cwt/utils.h>
#include <cwt/string.h>

int main(int argc, char *argv[])
{
	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	CWT_BEGIN_TESTS(9);

	/* INT16 <=> bytes transformations */
	{
		INT16 i16 = CWT_INT16_MAX;
		BYTE bytes[2];

		cwtInt16ToBytes(i16, bytes);
		CWT_TEST_OK2(i16 == cwtBytesToInt16(bytes), "CWT_MAX_INT16 => To Bytes => From Bytes => CWT_MAX_INT16");

		i16 = CWT_INT16_MIN;
		cwtInt16ToBytes(i16, bytes);
		CWT_TEST_OK2(i16 == cwtBytesToInt16(bytes), "CWT_MIN_INT16 => To Bytes => From Bytes => CWT_MIN_INT16");

		i16 = 0;
		cwtInt16ToBytes(i16, bytes);
		CWT_TEST_OK2(i16 == cwtBytesToInt16(bytes), "0 => To Bytes => From Bytes => 0");
	}


	/* INT32 <=> bytes transformations */
	{
		INT32 i32 = CWT_INT32_MAX;
		BYTE bytes[4];

		cwtInt32ToBytes(i32, bytes);
		CWT_TEST_OK2(i32 == cwtBytesToInt32(bytes), "CWT_MAX_INT32 => To Bytes => From Bytes => CWT_MAX_INT32");

		i32 = CWT_INT32_MIN;
		cwtInt32ToBytes(i32, bytes);
		CWT_TEST_OK2(i32 == cwtBytesToInt32(bytes), "CWT_MIN_INT32 => To Bytes => From Bytes => CWT_MIN_INT32");

		i32 = 0;
		cwtInt32ToBytes(i32, bytes);
		CWT_TEST_OK2(i32 == cwtBytesToInt32(bytes), "0 => To Bytes => From Bytes => 0");
	}

	/* INT64 <=> bytes transformations */
	{
		INT64 i64 = CWT_INT64_MAX;
		BYTE bytes[8];

		cwtInt64ToBytes(i64, bytes);
		CWT_TEST_OK2(i64 == cwtBytesToInt64(bytes), "CWT_MAX_INT64 => To Bytes => From Bytes => CWT_MAX_INT64");

		i64 = CWT_INT64_MIN;
		cwtInt64ToBytes(i64, bytes);
		CWT_TEST_OK2(i64 == cwtBytesToInt64(bytes), "CWT_MIN_INT64 => To Bytes => From Bytes => CWT_MIN_INT64");

		i64 = 0;
		cwtInt64ToBytes(i64, bytes);
		CWT_TEST_OK2(i64 == cwtBytesToInt64(bytes), "0 => To Bytes => From Bytes => 0");
	}


	CWT_END_TESTS;
}


