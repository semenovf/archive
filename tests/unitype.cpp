/*
 * unitype.cpp
 *
 *  Created on: 14.08.2012
 *      Author: wladt
 *  Modified on: 03.06.2013 Replaced with C++ implementation.
 */


#include <cwt/test.h>
#include <cstring>
#include <cwt/unitype.hpp>

using namespace cwt;

const char *loremipsum =
"Lorem ipsum dolor sit amet, consectetuer adipiscing elit,    \
sed diam nonummy nibh euismod tincidunt ut laoreet dolore     \
magna aliquam erat volutpat. Ut wisi enim ad minim veniam,    \
quis nostrud exerci tation ullamcorper suscipit lobortis      \
nisl ut aliquip ex ea commodo consequat. Duis autem vel eum   \
iriure dolor in hendrerit in vulputate velit esse molestie    \
consequat, vel illum dolore eu feugiat nulla facilisis at     \
vero eros et accumsan et iusto odio dignissim qui blandit     \
praesent luptatum zzril delenit augue duis dolore te feugait  \
nulla facilisi. Nam liber tempor cum soluta nobis eleifend    \
option congue nihil imperdiet doming id quod mazim placerat   \
facer possim assum. Typi non habent claritatem insitam; est   \
usus legentis in iis qui facit eorum claritatem.              \
Investigationes demonstraverunt lectores legere me lius quod  \
ii legunt saepius. Claritas est etiam processus dynamicus,    \
qui sequitur mutationem consuetudium lectorum. Mirum est      \
notare quam littera gothica, quam nunc putamus parum claram,  \
anteposuerit litterarum formas humanitatis per seacula quarta \
decima et quinta decima. Eodem modo typi, qui nunc nobis      \
videntur parum clari, fiant sollemnes in futurum.";


void test_00(void)
{
	bool ok;

	UniType bool_val;
	UniType char_val;
	UniType sbyte_val;
	UniType byte_val;
	UniType short_val;
	UniType ushort_val;
	UniType int_val;
	UniType uint_val;
	UniType long_val;
	UniType ulong_val;
	UniType llong_val;
	UniType ullong_val;
	UniType float_val;
	UniType double_val;
	UniType text_val;
	UniType blob_val;
/*
	UniType time_val;
	UniType date_val;
	UniType datetime_val;
*/

	bool_val.setBool(true);
	CWT_TEST_OK(bool_val.toBool(&ok) == true);
	CWT_TEST_OK2(ok, "bool_val == true");

	bool_val.setBool(false);
	CWT_TEST_OK(bool_val.toBool(&ok) == false);
	CWT_TEST_OK2(ok, "bool_val == false");

	char_val.setChar(UChar('W'));
	CWT_TEST_OK(char_val.toUChar(&ok) == UChar('W'));
	CWT_TEST_OK2(ok, "char_val == 'W'");

	sbyte_val.setInt(CWT_SBYTE_MIN);
	CWT_TEST_OK(sbyte_val.toSByte(&ok) == CWT_SBYTE_MIN);
	CWT_TEST_OK2(ok, "sbyte_val == CWT_SBYTE_MIN");

	sbyte_val.setInt(CWT_SBYTE_MAX);
	CWT_TEST_OK(sbyte_val.toSByte(&ok) == CWT_SBYTE_MAX);
	CWT_TEST_OK2(ok, "sbyte_val == CWT_SBYTE_MAX");

	byte_val.setUInt(0);
	CWT_TEST_OK(byte_val.toByte(&ok) == 0);
	CWT_TEST_OK2(ok, "byte_val == 0");

	byte_val.setUInt(CWT_BYTE_MAX);
	CWT_TEST_OK(byte_val.toByte(&ok) == CWT_BYTE_MAX);
	CWT_TEST_OK2(ok, "byte_val == CWT_BYTE_MAX");

	short_val.setInt(CWT_SHORT_MIN);
	CWT_TEST_OK(short_val.toShort(&ok) == CWT_SHORT_MIN);
	CWT_TEST_OK2(ok, "short_val == CWT_SHORT_MIN");

	short_val.setInt(CWT_SHORT_MAX);
	CWT_TEST_OK(short_val.toShort(&ok) == CWT_SHORT_MAX);
	CWT_TEST_OK2(ok, "short_val == CWT_SHORT_MAX");

	ushort_val.setUInt(0);
	CWT_TEST_OK(ushort_val.toUShort(&ok) == 0);
	CWT_TEST_OK2(ok, "ushort_val == 0");

	ushort_val.setUInt(CWT_USHORT_MAX);
	CWT_TEST_OK(ushort_val.toUShort(&ok) == CWT_USHORT_MAX);
	CWT_TEST_OK2(ok, "ushort_val == CWT_USHORT_MAX");

	int_val.setInt(CWT_INT_MIN);
	CWT_TEST_OK(int_val.toInt(&ok) == CWT_INT_MIN);
	CWT_TEST_OK2(ok, "int_val == CWT_INT_MIN");

	int_val.setInt(CWT_INT_MAX);
	CWT_TEST_OK(int_val.toInt(&ok) == CWT_INT_MAX);
	CWT_TEST_OK2(ok, "int_val == CWT_INT_MAX");

	uint_val.setUInt(0);
	CWT_TEST_OK(uint_val.toUInt(&ok) == 0);
	CWT_TEST_OK2(ok, "uint_val == 0");

	uint_val.setUInt(CWT_UINT_MAX);
	CWT_TEST_OK(uint_val.toUInt(&ok) == CWT_UINT_MAX);
	CWT_TEST_OK2(ok, "uint_val == CWT_UINT_MAX");

	long_val.setLong(CWT_LONG_MIN);
	CWT_TEST_OK(long_val.toLong(&ok) == CWT_LONG_MIN);
	CWT_TEST_OK2(ok, "long_val == CWT_LONG_MIN");

	long_val.setLong(CWT_LONG_MAX);
	CWT_TEST_OK(long_val.toLong(&ok) == CWT_LONG_MAX);
	CWT_TEST_OK2(ok, "long_val == CWT_LONG_MAX");

	ulong_val.setULong(0);
	CWT_TEST_OK(ulong_val.toULong(&ok) == 0);
	CWT_TEST_OK2(ok, "ulong_val == 0");

	ulong_val.setULong(CWT_ULONG_MAX);
	CWT_TEST_OK(ulong_val.toULong(&ok) == CWT_ULONG_MAX);
	CWT_TEST_OK2(ok, "ulong_val == CWT_ULONG_MAX");

	float_val.setFloat(CWT_FLOAT_MAX);
	CWT_TEST_OK(float_val.toFloat(&ok) == CWT_FLOAT_MAX);
	CWT_TEST_OK2(ok, "float_val == CWT_FLOAT_MAX");

	double_val.setDouble(CWT_DOUBLE_MAX);
	CWT_TEST_OK(double_val.toDouble(&ok) == CWT_DOUBLE_MAX);
	CWT_TEST_OK2(ok, "double_val == CWT_DOUBLE_MAX");

	String text_en(String::fromUtf8("The quick brown fox jumps over the lazy dog"));
	String text_ru(String::fromUtf8("Съешь ещё этих мягких французских булок, да выпей чаю"));
	String text;

	text_val.setString(text_en);
	text = text_val.toString(&ok);
	CWT_TEST_OK(ok && text == text_en);

	text_val.setString(text_ru);
	text = text_val.toString(&ok);
	CWT_TEST_OK(ok && text == text_ru);

	ByteArray blob;
	blob_val.setBlob(loremipsum, ::strlen(loremipsum));
	blob = blob_val.toBlob();
	CWT_TEST_OK(blob.size() == ::strlen(loremipsum));
	CWT_TEST_OK(::strcmp(loremipsum, blob.data()) == 0);

/*
	UniType object_val = UniType::make_object<String>(text_en);
	text = object_val.toObject<String>();
	CWT_TEST_OK(ok && text == text_en);
*/

/*
	{
		CWT_TIME tm1;
		CWT_TIME tm2;

		__utilsNS->now(&tm1);
		__utNS->setTIME(time_val, &tm1);
		__utNS->toTIME(time_val, &tm2, &ok);
		CWT_TEST_OK(ok && tm2.hour == tm1.hour);
		CWT_TEST_OK(ok && tm2.min == tm1.min);
		CWT_TEST_OK(ok && tm2.sec == tm1.sec);

		__utilsNS->now(&tm1);
		__utNS->setDATE(date_val, &tm1);
		__utNS->toDATE(date_val, &tm2, &ok);
		CWT_TEST_OK(ok && tm2.year == tm1.year);
		CWT_TEST_OK(ok && tm2.mon == tm1.mon);
		CWT_TEST_OK(ok && tm2.day == tm1.day);


		__utilsNS->now(&tm1);
		__utNS->setDATETIME(datetime_val, &tm1);
		__utNS->toDATETIME(datetime_val, &tm2, &ok);
		CWT_TEST_OK(ok && tm2.hour == tm1.hour);
		CWT_TEST_OK(ok && tm2.min == tm1.min);
		CWT_TEST_OK(ok && tm2.sec == tm1.sec);
		CWT_TEST_OK(ok && tm2.year == tm1.year);
		CWT_TEST_OK(ok && tm2.mon == tm1.mon);
		CWT_TEST_OK(ok && tm2.day == tm1.day);
	}
*/
}

void test_01(void)
{
	bool ok;
	UniType uval;

	CWT_TEST_OK(uval.isNull());

	uval.setBool(true);
	CWT_TEST_OK(uval.toBool(&ok) == true && ok);

	uval.setBool(false);
	CWT_TEST_OK(uval.toBool(&ok) == false && ok);

	uval.setChar('W');
	CWT_TEST_OK(uval.toUChar(&ok) == UChar('W') && ok);

	uval.setInt(CWT_SBYTE_MIN);
	CWT_TEST_OK(uval.toSByte(&ok) == CWT_SBYTE_MIN && ok);

	uval.setInt(CWT_SBYTE_MAX);
	CWT_TEST_OK(uval.toSByte(&ok) == CWT_SBYTE_MAX && ok);

	uval.setUInt(CWT_BYTE_MAX);
	CWT_TEST_OK(uval.toByte(&ok) == CWT_BYTE_MAX && ok);

	uval.setInt(CWT_SHORT_MIN);
	CWT_TEST_OK(uval.toShort(&ok) == CWT_SHORT_MIN && ok);

	uval.setInt(CWT_SHORT_MAX);
	CWT_TEST_OK(uval.toShort(&ok) == CWT_SHORT_MAX && ok);

	uval.setUInt(CWT_USHORT_MAX);
	CWT_TEST_OK(uval.toUShort(&ok) == CWT_USHORT_MAX && ok);

	uval.setInt(CWT_INT_MIN);
	CWT_TEST_OK(uval.toInt(&ok) == CWT_INT_MIN && ok);

	uval.setInt(CWT_INT_MAX);
	CWT_TEST_OK(uval.toInt(&ok) == CWT_INT_MAX && ok);

	uval.setUInt(CWT_UINT_MAX);
	CWT_TEST_OK(uval.toUInt(&ok) == CWT_UINT_MAX && ok);

	uval.setLong(CWT_LONG_MIN);
	CWT_TEST_OK(uval.toLong(&ok) == CWT_LONG_MIN && ok);

	uval.setLong(CWT_LONG_MAX);
	CWT_TEST_OK(uval.toLong(&ok) == CWT_LONG_MAX && ok);

	uval.setULong(CWT_ULONG_MAX);
	CWT_TEST_OK(uval.toULong(&ok) == CWT_ULONG_MAX && ok);

	uval.setFloat(CWT_FLOAT_MAX);
	CWT_TEST_OK(uval.toFloat(&ok) == CWT_FLOAT_MAX && ok);

	uval.setDouble(CWT_DOUBLE_MAX);
	CWT_TEST_OK(uval.toDouble(&ok) == CWT_DOUBLE_MAX && ok);

	String text_en(String::fromUtf8("The quick brown fox jumps over the lazy dog"));
	String text_ru(String::fromUtf8("Съешь ещё этих мягких французских булок, да выпей чаю"));
	String text;

	uval.setString(text_en);
	text = uval.toString(&ok);
	CWT_TEST_OK(ok && text_en == text);

	uval.setString(text_ru);
	text = uval.toString(&ok);
	CWT_TEST_OK(ok && text_ru == text);

	ByteArray blob;
	uval.setBlob(loremipsum, ::strlen(loremipsum));
	blob = uval.toBlob();
	CWT_TEST_OK(blob.size() == ::strlen(loremipsum));
	CWT_TEST_OK(::strcmp(loremipsum, blob.data()) == 0);
/*
	{
		CWT_TIME tm1;
		CWT_TIME tm2;

		__utilsNS->now(&tm1);
		__utNS->setTIME(uval, &tm1);
		__utNS->toTIME(uval, &tm2, &ok);
		CWT_TEST_OK(ok && tm2.hour == tm1.hour);
		CWT_TEST_OK(ok && tm2.min == tm1.min);
		CWT_TEST_OK(ok && tm2.sec == tm1.sec);

		__utilsNS->now(&tm1);
		__utNS->setDATE(uval, &tm1);
		__utNS->toDATE(uval, &tm2, &ok);
		CWT_TEST_OK(ok && tm2.year == tm1.year);
		CWT_TEST_OK(ok && tm2.mon == tm1.mon);
		CWT_TEST_OK(ok && tm2.day == tm1.day);


		__utilsNS->now(&tm1);
		__utNS->setDATETIME(uval, &tm1);
		__utNS->toDATETIME(uval, &tm2, &ok);
		CWT_TEST_OK(ok && tm2.hour == tm1.hour);
		CWT_TEST_OK(ok && tm2.min == tm1.min);
		CWT_TEST_OK(ok && tm2.sec == tm1.sec);
		CWT_TEST_OK(ok && tm2.year == tm1.year);
		CWT_TEST_OK(ok && tm2.mon == tm1.mon);
		CWT_TEST_OK(ok && tm2.day == tm1.day);
	}*/
}

void test_bool(void)
{
	bool ok;
	UniType uval;

	uval.setBool(true);
	CWT_TEST_OK2(uval.toBool(&ok) == true && ok, _Tr("Boolean equals to true"));
	uval.setBool(false);
	CWT_TEST_OK2(uval.toBool(&ok) == false && ok, _Tr("Boolean equals to false"));

	uval.setChar('W');
	CWT_TEST_OK2(uval.toBool(&ok) == true && ok, _Tr("'W' equals to true"));
	uval.setChar('\0');
	CWT_TEST_OK2(uval.toBool(&ok) == false && ok, _Tr("'\\0' equals to false"));

	uval.setInt(CWT_SBYTE_MIN);
	CWT_TEST_OK2(uval.toBool(&ok) == true && ok, _Tr("CWT_SBYTE_MIN equals to true"));
	uval.setInt(CWT_SBYTE_MAX);
	CWT_TEST_OK2(uval.toBool(&ok) == true && ok, _Tr("CWT_SBYTE_MAX equals to true"));

	uval.setUInt(CWT_BYTE_MAX);
	CWT_TEST_OK2(uval.toBool(&ok) == true && ok, _Tr("CWT_BYTE_MAX equals to true"));

	uval.setInt(CWT_SHORT_MIN);
	CWT_TEST_OK2(uval.toBool(&ok) == true && ok, _Tr("CWT_SHORT_MIN equals to true"));
	uval.setInt(CWT_SHORT_MAX);
	CWT_TEST_OK2(uval.toBool(&ok) == true && ok, _Tr("CWT_SHORT_MAX equals to true"));

	uval.setUInt(CWT_USHORT_MAX);
	CWT_TEST_OK2(uval.toBool(&ok) == true && ok, _Tr("CWT_USHORT_MAX equals to true"));

	uval.setInt(CWT_INT_MIN);
	CWT_TEST_OK2(uval.toBool(&ok) == true && ok, _Tr("CWT_INT_MIN equals to true"));
	uval.setInt(CWT_INT_MAX);
	CWT_TEST_OK2(uval.toBool(&ok) == true && ok, _Tr("CWT_INT_MAX equals to true"));
	uval.setInt(0);
	CWT_TEST_OK2(uval.toBool(&ok) == false && ok, _Tr("Integer '0' equals to false"));

	uval.setUInt(CWT_UINT_MAX);
	CWT_TEST_OK2(uval.toBool(&ok) == true && ok, _Tr("CWT_UINT_MAX equals to true"));
	uval.setUInt(0);
	CWT_TEST_OK2(uval.toBool(&ok) == false && ok, _Tr("Unsigned integer '0' equals to false"));

	uval.setLong(CWT_LONG_MIN);
	CWT_TEST_OK2(uval.toBool(&ok) == true && ok, _Tr("CWT_LONG_MIN equals to true"));
	uval.setLong(CWT_LONG_MAX);
	CWT_TEST_OK2(uval.toBool(&ok) == true && ok, _Tr("CWT_LONG_MAX equals to true"));
	uval.setLong(0L);
	CWT_TEST_OK2(uval.toBool(&ok) == false && ok, _Tr("Long '0' equals to false"));

	uval.setULong(CWT_ULONG_MAX);
	CWT_TEST_OK2(uval.toBool(&ok) == true && ok, _Tr("CWT_ULONG_MAX equals to true"));
	uval.setULong(0UL);
	CWT_TEST_OK2(uval.toBool(&ok) == false && ok, _Tr("Unsigned long '0' equals to false"));

	uval.setFloat(CWT_FLOAT_MAX);
	CWT_TEST_OK2(uval.toBool(&ok) == true && ok, _Tr("CWT_FLOAT_MAX equals to true"));

	uval.setFloat(0.0f);
	CWT_TEST_OK2(uval.toBool(&ok) == false && ok, _Tr("Float '0.0' equals to false"));

	uval.setDouble(CWT_DOUBLE_MAX);
	CWT_TEST_OK2(uval.toBool(&ok) == true && ok, _Tr("CWT_DOUBLE_MAX equals to true"));

	uval.setDouble(0.0f);
	CWT_TEST_OK2(uval.toBool(&ok) == false && ok, _Tr("Double '0.0' equals to false"));

	uval.setString(String(""));
	CWT_TEST_OK2(uval.toBool() == false, _Tr("<empty text> == false"));

	uval.setString(String("false"));
	CWT_TEST_OK2(uval.toBool() == false, _Tr("'false' == false"));

	uval.setString(String("no"));
	CWT_TEST_OK2(uval.toBool() == false, _Tr("'no' == false"));

	uval.setString(String("FALSE"));
	CWT_TEST_OK2(uval.toBool() == true, _Tr("'FALSE' == true"));

	uval.setString(String("FaLsE"));
	CWT_TEST_OK2(uval.toBool() == true, _Tr("'FaLsE' == true"));

	uval.setString(String("0"));
	CWT_TEST_OK2(uval.toBool() == false, _Tr("'0' == false"));

	uval.setString(String("text"));
	CWT_TEST_OK2(uval.toBool() == true, _Tr("<any other text> is true"));

	uval.setBlob("false", 5);
	CWT_TEST_OK2(uval.toBool() == false, _Tr("Blob(\"false\") == false"));

	uval.setBlob("no", 2);
	CWT_TEST_OK2(uval.toBool() == false, _Tr("Blob(\"no\") == false"));

	uval.setBlob("0", 1);
	CWT_TEST_OK2(uval.toBool() == false, _Tr("Blob(\"0\") == false"));

	uval.setBlob("abracadabra", 11);
	CWT_TEST_OK2(uval.toBool() == true, _Tr("Blob(\"abracadabra\") == true"));

	uval.setBlob("False", 5);
	CWT_TEST_OK2(uval.toBool() == true, _Tr("Blob(\"False\") == true"));

	uval.setBlob("nO", 2);
	CWT_TEST_OK2(uval.toBool() == true, _Tr("Blob(\"nO\") == true"));
/*
	{
	    CWT_TIME tm;

		__utilsNS->now(&tm);
		__utNS->setTIME(uval, &tm);
		CWT_TEST_OK2(__utNS->toBool(uval, &ok) == false && !ok, _T("TIME does not cast to BOOL"));

		__utNS->setDATE(uval, &tm);
		CWT_TEST_OK2(__utNS->toBool(uval, &ok) == false && !ok, _T("DATE does not cast to BOOL"));

		__utNS->setDATETIME(uval, &tm);
		CWT_TEST_OK2(__utNS->toBool(uval, &ok) == false && !ok, _T("DATETIME does not cast to BOOL"));
	}
*/
}

void test_integer(void)
{
	bool ok;
	short_t  s = 10;
	int_t    d = 10;
	UniType short_val;
	UniType int_val;

	short_val.setInt(s);
	CWT_TEST_OK(short_val.toShort(&ok) == s && ok);

	short_val.setFromString(String("10"));
	CWT_TEST_OK(short_val.toShort(&ok) == 10 && ok);

	int_val.setInt(d);
	CWT_TEST_OK(int_val.toInt(&ok) == d && ok);

	int_val.setFromString(String("10"));
	CWT_TEST_OK(int_val.toInt(&ok) == 10 && ok);
}

void test_float(void)
{
	bool ok;
	double d = 10.0f;
	float  f = 10.0f;
	UniType float_val;
	UniType double_val;

	float_val.setFloat(f);
	CWT_TEST_OK(float_val.toFloat(&ok) == f && ok);

	float_val.setFromString(String("3.14159"));
	CWT_TEST_OK(float_val.toFloat(&ok) == String("3.14159").toFloat() && ok);

	double_val.setDouble(d);
	CWT_TEST_OK(double_val.toDouble(&ok) == d && ok);

	double_val.setFromString(String("3.14e+10"));
	double d1 = double_val.toDouble(&ok);
	double d2 = String("3.14e+10").toDouble();
	CWT_TEST_OK(d1 == d2);
	CWT_TEST_OK(double_val.toDouble(&ok) == String("3.14e+10").toDouble() && ok);
}

struct TestElem
{
	TestElem  () : str()                        { /*puts("TestElem()");*/ }
	TestElem  (const TestElem & o) : str(o.str) { if (!str.isEmpty()) ++counter; printf("TestElem (TestElem(\"%s\"))\n", str.c_str()); }
	TestElem  (const String & s) : str(s)       { ++counter; printf ("TestElem (\"%s\")\n", str.c_str()); }
	~TestElem ()                                { if (!str.isEmpty()) --counter; /*printf ("~TestElem (\"%s\")\n", str.c_str());*/ }

	TestElem & operator = (const TestElem & other)
	{
		++counter;
		str = other.str;
		printf ("TestElem::operator = (\"%s\")\n", str.c_str());
		return *this;
	}
	static int counter;
	String str;
};

int TestElem::counter = 0;

void test_object ()
{
	{
		UniType ut = UniType::make_object<Vector<TestElem> >();
		Vector<TestElem> & p = ut.objectRef<Vector<TestElem> >();

		p.append(TestElem(String("One")));
		p.append(TestElem(String("Two")));
		p.append(TestElem(String("Three")));
		p.append(TestElem(String("Four")));

		CWT_TEST_OK(p.size() == 4);
		CWT_TEST_OK(TestElem::counter == 4);
	}

	CWT_TEST_OK(TestElem::counter == 0);
}

#ifdef __COMMENT__

/* TODO need to implement */
void test_text(void)
{
}

/* TODO need to implement */
void test_blob(void)
{
}

/* TODO need to implement */
void test_time(void)
{
}

#endif

int main(int argc, char *argv[])
{
    CWT_CHECK_SIZEOF_TYPES;
	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	CWT_BEGIN_TESTS(117);

	test_00();
	test_01();
	test_bool();
	test_integer();
	test_float();
	test_object();
/*
	test_text();
	test_blob();
	test_time();
*/

	CWT_END_TESTS;
}


