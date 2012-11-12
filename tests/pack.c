/**
 * @file   pack.c
 * @author wladt
 * @date   Nov 12, 2012 10:12:39 AM
 *
 * @brief
 */


#include <cwt/utils.h>
#include <cwt/test.h>

void test_pack_unpack_int(void)
{
	BYTE buf[256];
	struct _SampleIntStruct {
		INT64 i64;
		INT32 i32;
		INT16 i16;
		INT8  i8;
	} origin_max = {
		  CWT_INT64_MAX
		, CWT_INT32_MAX
		, CWT_INT16_MAX
		, CWT_INT8_MAX
	}, origin_min = {
		  CWT_INT64_MIN
		, CWT_INT32_MIN
		, CWT_INT16_MIN
		, CWT_INT8_MIN
	}, dest_max, dest_min;

	void *data_origin_max[] = {
		  &origin_max.i64
		, &origin_max.i32
		, &origin_max.i16
		, &origin_max.i8
	};

	void *data_origin_min[] = {
		  &origin_min.i64
		, &origin_min.i32
		, &origin_min.i16
		, &origin_min.i8
	};

	void *data_dest_max[] = {
		  &dest_max.i64
		, &dest_max.i32
		, &dest_max.i16
		, &dest_max.i8
	};

	void *data_dest_min[] = {
		  &dest_min.i64
		, &dest_min.i32
		, &dest_min.i16
		, &dest_min.i8
	};

	CWT_TEST_FAIL(cwtUtilsNS()->pack(_T("qlsc"), buf, 256, data_origin_max, sizeof(data_origin_max)/sizeof(data_origin_max[0])) > 0);
	CWT_TEST_FAIL(cwtUtilsNS()->unpack(_T("qlsc"), buf, 256, data_dest_max, sizeof(data_dest_max)/sizeof(data_dest_max[0])) > 0);

	CWT_TEST_OK(dest_max.i64 == origin_max.i64);
	CWT_TEST_OK(dest_max.i32 == origin_max.i32);
	CWT_TEST_OK(dest_max.i16 == origin_max.i16);
	CWT_TEST_OK(dest_max.i8  == origin_max.i8);

	CWT_TEST_FAIL(cwtUtilsNS()->pack(_T("qlsc"), buf, 256, data_origin_min, sizeof(data_origin_min)/sizeof(data_origin_min[0])) > 0);
	CWT_TEST_FAIL(cwtUtilsNS()->unpack(_T("qlsc"), buf, 256, data_dest_min, sizeof(data_dest_min)/sizeof(data_dest_min[0])) > 0);

	CWT_TEST_OK(dest_min.i64 == origin_min.i64);
	CWT_TEST_OK(dest_min.i32 == origin_min.i32);
	CWT_TEST_OK(dest_min.i16 == origin_min.i16);
	CWT_TEST_OK(dest_min.i8  == origin_min.i8);
}

void test_pack_unpack_float(void)
{
	BYTE buf[256];

	struct _SampleFloatStruct {
		float f;
		double d;
	} origin = {
		  3.14159
		, 3.14160
	}, dest;

	void *data_origin[] = {
		  &origin.f
		, &origin.d
	};

	void *data_dest[] = {
		  &dest.f
		, &dest.d
	};

	CWT_TEST_FAIL(cwtUtilsNS()->pack(_T("fd"), buf, 256, data_origin, sizeof(data_origin)/sizeof(data_origin[0])) > 0);
	CWT_TEST_FAIL(cwtUtilsNS()->unpack(_T("fd"), buf, 256, data_dest, sizeof(data_dest)/sizeof(data_dest[0])) > 0);

	CWT_TEST_OK(dest.f   == origin.f);
	CWT_TEST_OK(dest.d   == origin.d);

}


int main(int argc, char *argv[])
{
	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	CWT_BEGIN_TESTS(16);

	test_pack_unpack_int();
	test_pack_unpack_float();

	CWT_END_TESTS;
}
