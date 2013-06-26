/**
 * @file   uri.c
 * @author wladt
 * @date   Oct 12, 2012 11:18:36 AM
 *
 * @brief URI testing
 */

#include "../include/cwt/json.hpp"
#include "../include/cwt/fsm_test.hpp"
#include "../src/json_rfc4627.hpp"

using namespace cwt;

static const char *json_empty_object_str = "{}";
static const char *json_empty_array_str = "[]";

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


static const char *json_object_str1 = "                                                             \
{                                                                                                   \
      \"sep_char\"     : \",\"                                                                      \
    , \"quote_char\"   : \"\\\"\"                                                                   \
    , \"not_embed_nl\" : 0                                                                          \
    , \"encoding_in\"  : \"utf8\"                                                                   \
    , \"encoding_out\" : \"utf8\"                                                                   \
    , \"skip_header\"  : 1                                                                          \
    , \"comment_str\"  : \"#\"                                                                      \
    , \"fields\"       : [                                                                          \
              { \"index\": 1, \"name\" :\"code\", \"type\" : \"CHAR(2)\", \"spec\" : \"PK\" }       \
            , { \"index\": 2, \"name\" :\"name\", \"type\" : \"TEXT(45)\", \"spec\" : \"\" }        \
            , { \"index\": 5, \"name\" :\"currency\", \"type\" : \"CHAR(3)\", \"spec\" : \"\" }     \
    ]                                                                                               \
}";



static FsmTestEntry __fsmTestEntries[] = {
		  /* number = [ minus ] int [ frac ] [ exp ] */
		  { VHEADER(number_fsm)
			, { "987654321", "-12345.6789", "-123e+543", "987.2345e+234", VNULL }
			, { INULL }}

		/* exp = e [ minus / plus ] 1*DIGIT */
		, { VHEADER(exp_fsm)
			, { "e+1234", "E-987654321", "e1234", "E987654321", VNULL }
				, { INULL }}

		  /* frac = decimal-point 1*DIGIT */
		, { VHEADER(frac_fsm)
			, { ".123456", ".0123456789", VNULL }
			, {   {-1, ".abcd" }
				, { 4, ".012c" }
				, {-1, "1234" }
				, INULL }}

		  /* int = zero / ( digit1-9 *DIGIT ) */
		, { VHEADER(int_fsm)
			, { "0", "123456789", VNULL }
			, {   { 1, "01234" }
				, {-1, "A0" }
				, INULL }}

		  /* number = [ minus ] int [ frac ] [ exp ] */
		, { VHEADER(number_fsm)
			, { "987654321", "-12345.6789", "-123e+543", "987.2345e+234", VNULL }
			, { INULL }}


		  /* %x75 4HEXDIG )  ; uXXXX */
		, { VHEADER(unicode_char_fsm)
			, { "u1234", "uABCD", "uFFFF", "u1a2c", VNULL }
			, {   {-1, "UX124" }
				, {-1, "uFEAR" }
				, INULL }}

		  /*
		    char = unescaped /
				escape (reference
					%x22 /          ; "    quotation mark  U+0022
					%x5C /          ; \    reverse solidus U+005C
					%x2F /          ; /    solidus         U+002F
					%x62 /          ; b    backspace       U+0008
					%x66 /          ; f    form feed       U+000C
					%x6E /          ; n    line feed       U+000A
					%x72 /          ; r    carriage return U+000D
					%x74 /          ; t    tab             U+0009
					%x75 4HEXDIG )  ; uXXXX                U+XXXX
		  */
		, { VHEADER(char_fsm), { "\\\"", "\\\\", "\\/", "\\b", VNULL }, { INULL }}
		, { VHEADER(char_fsm), { "\\f", "\\n", "\\r", "\\t", VNULL }, { INULL }}
		, { VHEADER(char_fsm), { "\\u0123", "\\uABCD", VNULL }, { INULL }}
		, { VHEADER(char_fsm), { "A", " ", "\u1234", "\uFFFF", VNULL }, { INULL }}

		  /* string = quotation-mark *char quotation-mark */
		, { VHEADER(string_fsm)
			, { "\"This is a string with unescaped characters at all\""
					, "\"This is a string with \\\" double quote inside\""
					, "\"This \\b string \\n contains \\\\ unescaped \\t characters \\r \\/ \\\"\""
					, "\"\\u0030\\u0031\\u0032\\u0033\\uF100\\uABCD\\u0020\\u0020\\u0020\\u0020\\u0020\\u0020\""
					, VNULL }
			, { INULL }}

		  /* object = begin-object [ member *( value-separator member ) ] end-object */
		, { VHEADER(object_fsm)
			, { "{ }", " {\"Width\":  800} ", " {\"Width\":  800, \"Height\": 600 }"
					, VNULL }
			, { INULL }}

		, { VHEADER(json_fsm)
			, { json_object_str
			, json_array_str
			, VNULL }
		  , { INULL }}

};


void test_json_fsm()
{
	/*Json json;*/
	static Fsm<Char> fsm(NULL, NULL);
	int nentries = sizeof(__fsmTestEntries)/sizeof(__fsmTestEntries[0]);

	for( int i = 0; i < nentries; i++ )
		fsm_test_entries(fsm, &__fsmTestEntries[i]);
}


void test_json(void)
{
	Json json_object(String::fromUtf8(json_object_str));
	Json json_array(String::fromUtf8(json_array_str));
	Json json_empty_object(String::fromUtf8(json_empty_object_str));
	Json json_empty_array(String::fromUtf8(json_empty_array_str));

	Json json_object1(String::fromUtf8(json_object_str1));

	CWT_TEST_FAIL(json_object.isGood());
	CWT_TEST_FAIL(json_array.isGood());
	CWT_TEST_FAIL(json_empty_object.isGood());
	CWT_TEST_FAIL(json_empty_array.isGood());
	CWT_TEST_FAIL(json_object1.isGood());

	CWT_TEST_OK(json_object.isObject());
	CWT_TEST_OK(json_array.isArray());
	CWT_TEST_OK(json_empty_object.isObject());
	CWT_TEST_OK(json_empty_array.isArray());

	CWT_TEST_OK(json_object.size() > 0);
	CWT_TEST_OK(json_array.size() > 0);

	CWT_TEST_OK(json_object[_U("Image")][_U("Width")].number() == double(800));
	CWT_TEST_OK(json_object[_U("Image")][_U("Height")].number() == double(600));
	CWT_TEST_OK(json_object[_U("Image")][_U("Title")].string() == _U("View from 15th Floor"));
	CWT_TEST_OK(json_object[_U("Image")][_U("Thumbnail")][_U("Url")].string() == _U("http://www.example.com/image/481989943"));
	CWT_TEST_OK(json_object[_U("Image")][_U("Thumbnail")][_U("Height")].number() == double(125));
	CWT_TEST_OK(json_object[_U("Image")][_U("Thumbnail")][_U("Width")].string() == _U("100"));
	CWT_TEST_OK(json_object[_U("Image")][_U("IDs")][0].number() == double(116));
	CWT_TEST_OK(json_object[_U("Image")][_U("IDs")][1].number() == double(943));
	CWT_TEST_OK(json_object[_U("Image")][_U("IDs")][2].number() == double(234));
	CWT_TEST_OK(json_object[_U("Image")][_U("IDs")][3].number() == double(38793));

	CWT_TEST_OK(json_object[_U("Image")][_U("IDs")][4].number() == double(0));
	CWT_TEST_OK(json_object[_U("Image")][_U("Unknown")].isNull());
	CWT_TEST_OK(json_object[0][1].isNull());

	CWT_TEST_OK(json_array[0][_U("precision")].string() == _U("zip"));
	CWT_TEST_OK(json_array[0][_U("Latitude")].number() == double(37.7668));
	CWT_TEST_OK(json_array[0][_U("Longitude")].number() == double(-122.3959));
	CWT_TEST_OK(json_array[0][_U("Address")].string() == _U(""));
	CWT_TEST_OK(json_array[0][_U("City")].string() == _U("SAN FRANCISCO"));
	CWT_TEST_OK(json_array[0][_U("State")].string() == _U("CA"));
	CWT_TEST_OK(json_array[0][_U("Zip")].string() == _U("94107"));
	CWT_TEST_OK(json_array[0][_U("Country")].string() == _U("US"));

	CWT_TEST_OK(json_array[1][_U("precision")].string() == _U("zip"));
	CWT_TEST_OK(json_array[1][_U("Latitude")].number()  == double(37.371991));
	CWT_TEST_OK(json_array[1][_U("Longitude")].number() == double(-122.026020));
	CWT_TEST_OK(json_array[1][_U("Address")].string()   == _U(""));
	CWT_TEST_OK(json_array[1][_U("City")].string()      == _U("SUNNYVALE"));
	CWT_TEST_OK(json_array[1][_U("State")].string()     == _U("CA"));
	CWT_TEST_OK(json_array[1][_U("Zip")].string()       == _U("94085"));
	CWT_TEST_OK(json_array[1][_U("Country")].string()   == _U("US"));
}

void test_json_remove(void)
{
	Json json_object(String::fromUtf8(json_object_str));
	Json json_array(String::fromUtf8(json_array_str));

	CWT_TEST_OK(json_object[_U("Image")][_U("Width")].number() == double(800));
	CWT_TEST_FAIL(json_object[_U("Image")].isObject());
	json_object[_U("Image")].remove(_U("Width"));
	CWT_TEST_OK(json_object[_U("Image")][_U("Width")].isNull());
}

int main(int argc, char *argv[])
{
	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	CWT_BEGIN_TESTS(93);

	test_json_fsm();
	test_json();
	test_json_remove();

	CWT_END_TESTS;
}
