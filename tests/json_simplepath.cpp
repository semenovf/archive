/**
 * @file   json_simplepath.cpp
 * @author wladt
 * @date   Jun 05, 2013
 *
 * @brief JsonSimplePath testing
 */

#include "../include/cwt/json.hpp"
#include "../include/cwt/fsm_test.hpp"
#include "../src/json_simplepath_fsm.hpp"

using namespace cwt;

/*
static String jpath_0(_U("/store/book[0]/title"));
static String jpath_1(_U("/[1]/book[0]/title"));
static String jpath_2(_U("[1]/[2]/[3]/title"));
*/

static FsmTestEntry __fsmTestEntries[] = {
		  { VHEADER(jpath_fsm)
			, { "/", VNULL }
			, { { 19, "/root[1]/invalid[2]key/[1]" }
				, INULL }}

		, { VHEADER(subscript_fsm)
			, { "[1]", "[12]", "[9876]", "[01234]", VNULL }
			, {   {-1, "[a0]" }
				, {-1, "[01A234]" }
			    , {-1, "[1234" }
			    , {-1, "[]" }
			    , INULL }}

		, { VHEADER(key_subscript_fsm)
			, { "key[123]", "123-key[9]", "===[999]", "||||[987654321]", VNULL }
				, { { -1, "key[]" }
					, INULL }}

		, { VHEADER(elem_fsm)
			, { "key[123]", "===[999]", "key", VNULL }
				, { INULL }}

		, { VHEADER(jpath_fsm)
			, { "/", "/root/elem[1]/elem[2]/elem3", "/root/element[1]/././..", "elem/parent[1]/../child", VNULL }
			, {   { 0, "[]" }
				, { 5, "/root[]" }
				, { 20, "/root[23]/invalid[2]key" }
				, INULL }}
};


void test_json_simplepath_fsm()
{
	static Fsm<Char> fsm(NULL, NULL);
	int nentries = sizeof(__fsmTestEntries)/sizeof(__fsmTestEntries[0]);

	for( int i = 0; i < nentries; i++ )
		fsm_test_entries(fsm, &__fsmTestEntries[i]);
}



static const char *json_object_str = "{                                \
		\"Image\": {                                                   \
			\"Width\":  800,                                           \
			\"Height\": 600,                                           \
			\"Title\":  \"View from 15th Floor\",                      \
			\"Thumbnail\": {                                           \
				\"Url\":    \"http://www.example.com/image/481989943\",\
				\"Height\": 125,                                       \
				\"Width\":  \"100\"                                    \
			},                                                         \
			\"IDs\": [116, 943, 234, 38793]                            \
		  }                                                            \
	 }";

static const char *json_array_str = "   [        \
		  {                                      \
			 \"precision\": \"zip\",             \
			 \"Latitude\":  37.7668,             \
			 \"Longitude\": -122.3959,           \
			 \"Address\":   \"\",                \
			 \"City\":      \"SAN FRANCISCO\",   \
			 \"State\":     \"CA\",              \
			 \"Zip\":       \"94107\",           \
			 \"Country\":   \"US\"               \
		  },                                     \
		  {                                      \
			 \"precision\": \"zip\",             \
			 \"Latitude\":  37.371991,           \
			 \"Longitude\": -122.026020,         \
			 \"Address\":   \"\",                \
			 \"City\":      \"SUNNYVALE\",       \
			 \"State\":     \"CA\",              \
			 \"Zip\":       \"94085\",           \
			 \"Country\":   \"US\"               \
		  }                                      \
	   ]";

void test_json_simplepath()
{
	Json json_object(String::fromUtf8(json_object_str));

	CWT_TEST_FAIL(json_object.isGood());
	CWT_TEST_OK(json_object.isObject());

	JsonSimplePath jpath(json_object);
	jpath.changeRoot(_U("/Image"));
	CWT_TEST_OK(jpath.contains(_U("../Image/IDs[0]")));

	CWT_TEST_OK(jpath.contains(_U("/")));
	CWT_TEST_OK(jpath.contains(_U("/Image")));
	CWT_TEST_OK(jpath.contains(_U("/Image/Width")));
	CWT_TEST_OK(jpath.contains(_U("/Image/Height")));
	CWT_TEST_OK(jpath.contains(_U("/Image/Title")));
	CWT_TEST_OK(jpath.contains(_U("/Image/Thumbnail")));
	CWT_TEST_OK(jpath.contains(_U("/Image/Thumbnail/Url")));
	CWT_TEST_OK(jpath.contains(_U("/Image/Thumbnail/Height")));
	CWT_TEST_OK(jpath.contains(_U("/Image/Thumbnail/Width")));
	CWT_TEST_OK(jpath.contains(_U("/Image/IDs")));
	CWT_TEST_OK(jpath.contains(_U("/Image/IDs/[0]")));
	CWT_TEST_OK(jpath.contains(_U("/Image/IDs/[1]")));
	CWT_TEST_OK(jpath.contains(_U("/Image/IDs/[2]")));
	CWT_TEST_OK(jpath.contains(_U("/Image/IDs/[3]")));
	CWT_TEST_OK(jpath.contains(_U("/Image/IDs[0]")));
	CWT_TEST_OK(jpath.contains(_U("/Image/IDs[1]")));
	CWT_TEST_OK(jpath.contains(_U("/Image/IDs[2]")));
	CWT_TEST_OK(jpath.contains(_U("/Image/IDs[3]")));

	jpath.changeRoot(_U("/Image"));
	CWT_TEST_OK(jpath.contains(_U("IDs[0]")));
	CWT_TEST_OK(jpath.contains(_U("IDs[1]")));
	CWT_TEST_OK(jpath.contains(_U("IDs[2]")));
	CWT_TEST_OK(jpath.contains(_U("IDs[3]")));
	CWT_TEST_OK(jpath.contains(_U("./IDs[0]")));
	CWT_TEST_OK(jpath.contains(_U("./IDs[1]")));
	CWT_TEST_OK(jpath.contains(_U("./IDs[2]")));
	CWT_TEST_OK(jpath.contains(_U("./IDs[3]")));
	CWT_TEST_OK(jpath.contains(_U("../Image/IDs[0]")));
	CWT_TEST_OK(jpath.contains(_U("../Image/IDs[1]")));
	CWT_TEST_OK(jpath.contains(_U("../Image/IDs[2]")));
	CWT_TEST_OK(jpath.contains(_U("../Image/IDs[3]")));
	CWT_TEST_OK(jpath[_U("../Image/IDs[0]")].number() == double(116));
	CWT_TEST_OK(jpath[_U("../Image/IDs[1]")].number() == double(943));
	CWT_TEST_OK(jpath[_U("../Image/IDs[2]")].number() == double(234));
	CWT_TEST_OK(jpath[_U("../Image/IDs[3]")].number() == double(38793));

	jpath.changeRoot(_U("/"));
	CWT_TEST_OK(jpath.contains(_U("Image/IDs[0]")));
	CWT_TEST_OK(jpath.contains(_U("Image/IDs[1]")));
	CWT_TEST_OK(jpath.contains(_U("Image/IDs[2]")));
	CWT_TEST_OK(jpath.contains(_U("Image/IDs[3]")));

	//	jpath[_U("/Image/Width")].setValue(1024);
	//	CWT_TEST_OK(jpath.contains(_U("/Image/Width")));
	//  jpath.remove(_U("/Image/Width"));
	//	CWT_TEST_NOK(jpath.contains(_U("/Image/Width")));


	Json json_array(String::fromUtf8(json_array_str));

	CWT_TEST_FAIL(json_array.isGood());
	CWT_TEST_OK(json_array.isArray());

	jpath.attachJson(json_array);
	CWT_TEST_OK(jpath.contains(_U("/")));
	CWT_TEST_OK(jpath.contains(_U("/[0]")));
	CWT_TEST_OK(jpath.contains(_U("/[0]/precision")));
	CWT_TEST_OK(jpath.contains(_U("/[0]/Latitude")));
	CWT_TEST_OK(jpath.contains(_U("/[0]/Longitude")));
	CWT_TEST_OK(jpath.contains(_U("/[0]/Address")));
	CWT_TEST_OK(jpath.contains(_U("/[0]/City")));
	CWT_TEST_OK(jpath.contains(_U("/[0]/State")));
	CWT_TEST_OK(jpath.contains(_U("/[0]/Zip")));
	CWT_TEST_OK(jpath.contains(_U("/[0]/Country")));
	CWT_TEST_OK(jpath.contains(_U("/[1]")));
	CWT_TEST_OK(jpath.contains(_U("/[1]/precision")));
	CWT_TEST_OK(jpath.contains(_U("/[1]/Latitude")));
	CWT_TEST_OK(jpath.contains(_U("/[1]/Longitude")));
	CWT_TEST_OK(jpath.contains(_U("/[1]/Address")));
	CWT_TEST_OK(jpath.contains(_U("/[1]/City")));
	CWT_TEST_OK(jpath.contains(_U("/[1]/State")));
	CWT_TEST_OK(jpath.contains(_U("/[1]/Zip")));
	CWT_TEST_OK(jpath.contains(_U("/[1]/Country")));
}


void test_json_makepath()
{
	Json json;
	JsonSimplePath jpath(json);
	jpath.setValue(_U("/Image/Width"), JsonNumber(800));
	CWT_TEST_OK(json.isObject());
	jpath.setValue(_U("/Image/Height"), JsonNumber(600));
	CWT_TEST_OK(json.isObject());
	jpath.setValue(_U("/Image/Title"), JsonString(_U("View from 15th Floor")));
	CWT_TEST_OK(json.isObject());

	CWT_TEST_OK(jpath[_U("/Image/Width")].number() == double(800));
	CWT_TEST_OK(jpath[_U("/Image/Height")].number() == double(600));
	CWT_TEST_OK(jpath[_U("/Image/Title")].string() == _U("View from 15th Floor"));

	printf("%s\n", json.toString().toUtf8().data());

/*
	jpath.setValue(_U("/Image/IDs/[2]"), JsonNumber(234));
	CWT_TEST_OK(json.isObject());
	jpath.setValue(_U("/Image/IDs/[3]"), JsonNumber(38793));
	CWT_TEST_OK(json.isObject());
	jpath.setValue(_U("/Image/IDs/[1]"), JsonNumber(943));
	CWT_TEST_OK(json.isObject());
	jpath.setValue(_U("/Image/IDs/[0]"), JsonNumber(116));
	CWT_TEST_OK(json.isObject());

	CWT_TEST_OK(jpath[_U("/Image/IDs[0]")].number() == double(116));
	CWT_TEST_OK(jpath[_U("/Image/IDs[1]")].number() == double(943));
	CWT_TEST_OK(jpath[_U("/Image/IDs[2]")].number() == double(234));
	CWT_TEST_OK(jpath[_U("/Image/IDs[3]")].number() == double(38793));
*/
}

int main(int argc, char *argv[])
{
	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	CWT_BEGIN_TESTS(87);

	test_json_simplepath_fsm();
	test_json_simplepath();
	test_json_makepath();

	CWT_END_TESTS;
}
