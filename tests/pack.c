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

	CWT_TEST_FAIL(cwtUtilsNS()->pack(_T("q"), buf, 256, data_origin_max
			, sizeof(data_origin_max)/sizeof(data_origin_max[0])) > 0);
	CWT_TEST_FAIL(cwtUtilsNS()->pack(_T("qlwb"), buf, 256, data_origin_max
			, sizeof(data_origin_max)/sizeof(data_origin_max[0])) > 0);
	CWT_TEST_FAIL(cwtUtilsNS()->unpack(_T("qlwb"), buf, 256, data_dest_max
			, sizeof(data_dest_max)/sizeof(data_dest_max[0])) > 0);

	CWT_TEST_OK(dest_max.i64 == origin_max.i64);
	CWT_TEST_OK(dest_max.i32 == origin_max.i32);
	CWT_TEST_OK(dest_max.i16 == origin_max.i16);
	CWT_TEST_OK(dest_max.i8  == origin_max.i8);

	CWT_TEST_FAIL(cwtUtilsNS()->pack(_T("qlwb"), buf, 256, data_origin_min
			, sizeof(data_origin_min)/sizeof(data_origin_min[0])) > 0);
	CWT_TEST_FAIL(cwtUtilsNS()->unpack(_T("qlwb"), buf, 256, data_dest_min
			, sizeof(data_dest_min)/sizeof(data_dest_min[0])) > 0);

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
		  3.14159f
		, 3.14160f
	}, dest;

	void *data_origin[] = {
		  &origin.f
		, &origin.d
	};

	void *data_dest[] = {
		  &dest.f
		, &dest.d
	};

	CWT_TEST_FAIL(cwtUtilsNS()->pack(_T("fd"), buf, 256, data_origin
			, sizeof(data_origin)/sizeof(data_origin[0])) > 0);
	CWT_TEST_FAIL(cwtUtilsNS()->unpack(_T("fd"), buf, 256, data_dest
			, sizeof(data_dest)/sizeof(data_dest[0])) > 0);

	CWT_TEST_OK(dest.f   == origin.f);
	CWT_TEST_OK(dest.d   == origin.d);
}


void test_pack_unpack_int_array(void)
{
	BYTE buf[256];
	struct _SampleIntStruct {
		INT64 i64[4];
		INT32 i32;
	} origin = {
		  { CWT_INT64_MAX, CWT_INT64_MIN, 123456LL, -98765LL }
		, CWT_INT32_MAX
	}, dest;

	void *data_origin[] = { &origin.i64[0], &origin.i32 };
	void *data_dest[] = { &dest.i64[0], &dest.i32 };


	CWT_TEST_FAIL(cwtUtilsNS()->pack(_T("q[4]l"), buf, 256, data_origin, 2) > 0);
	CWT_TEST_FAIL(cwtUtilsNS()->unpack(_T("q[4]l"), buf, 256, data_dest, 2) > 0);

	CWT_TEST_OK(dest.i64[0] == origin.i64[0]);
	CWT_TEST_OK(dest.i64[1] == origin.i64[1]);
	CWT_TEST_OK(dest.i64[2] == origin.i64[2]);
	CWT_TEST_OK(dest.i64[3] == origin.i64[3]);
	CWT_TEST_OK(dest.i32 == origin.i32);
}


void test_pack_failed(void)
{
	BYTE buf[32];
	struct _SampleIntStruct {
		int unused0;
		int unused1;
		int unused2;
	} origin = {
		  CWT_INT_MAX
		, CWT_INT_MAX
		, CWT_INT_MIN
	};

	void *data_origin[] = { &origin.unused0, &origin.unused1, &origin.unused2 };

	CWT_TEST_OK(cwtUtilsNS()->pack(_T("`"), buf, sizeof(buf),   data_origin, 3) < 0);
	CWT_TEST_OK(cwtUtilsNS()->pack(_T("`l"), buf, sizeof(buf),  data_origin, 3) < 0);
	CWT_TEST_OK(cwtUtilsNS()->pack(_T("`ll"), buf, sizeof(buf), data_origin, 3) < 0);
	CWT_TEST_OK(cwtUtilsNS()->pack(_T("l`l"), buf, sizeof(buf), data_origin, 3) < 0);
	CWT_TEST_OK(cwtUtilsNS()->pack(_T("ll`"), buf, sizeof(buf), data_origin, 3) < 0);
}


void test_pack_unpack_unitype(void)
{
	CwtUniTypeNS *utNS = cwtUniTypeNS();

	BYTE buf[256];
	CwtUniType origin[7];
	CwtUniType dest[7];
	int i;

	for( i = 0; i < (int)(sizeof(origin)/sizeof(origin[0])); i++ ) {
		utNS->init(&origin[i]);
		utNS->init(&dest[i]);
	}

	utNS->setSBYTE    (&origin[0], CWT_INT8_MIN);
	utNS->setSBYTE    (&origin[1], CWT_INT8_MAX);
	utNS->setSBYTE    (&origin[2], CWT_INT8_MAX);
	utNS->setLONGLONG (&origin[3], CWT_INT64_MAX);
	utNS->setLONG     (&origin[4], CWT_INT32_MAX);
	utNS->setSHORT    (&origin[5], CWT_INT16_MAX);
	utNS->setBYTE     (&origin[6], CWT_INT8_MAX);

	CWT_TEST_FAIL(cwtUtilsNS()->packUniType(_T("b[3]qlwb"), buf, 256, &origin[0]
			, sizeof(origin)/sizeof(origin[0])) > 0);
	CWT_TEST_FAIL(cwtUtilsNS()->unpackUniType(_T("b[3]qlwb"), buf, 256, &dest[0]
			, sizeof(dest)/sizeof(dest[0])) > 0);

	for( i = 0; i < (int)(sizeof(origin)/sizeof(origin[0])); i++ )
		CWT_TEST_OK(utNS->eq(&origin[i], &dest[i]));

	for( i = 0; i < (int)(sizeof(origin)/sizeof(origin[0])); i++ ) {
		utNS->destroy(&origin[i]);
		utNS->destroy(&dest[i]);
	}

}

int main(int argc, char *argv[])
{
	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	CWT_BEGIN_TESTS(38);

	test_pack_unpack_int();
	test_pack_unpack_float();
	test_pack_unpack_int_array();
	test_pack_failed();
	test_pack_unpack_unitype();

	CWT_END_TESTS;
}
