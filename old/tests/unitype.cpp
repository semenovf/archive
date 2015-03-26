/*
 * unitype.cpp
 *
 *  Created on: 14.08.2012
 *      Author: wladt
 *  Modified on: 03.06.2013 Replaced with C++ implementation.
 */


#include <pfs/test.hpp>
#include <pfs/unitype.hpp>
#include <pfs/string.hpp>
#include <pfs/bytearray.hpp>
#include <cstring>
#include <iostream>

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


void test_base(void)
{
//	using namespace pfs::unitype1;
//	using pfs::unitype1::unitype;
//	TEST_OK(unitype<bool>(true).get<bool>() == true);
//	TEST_OK(unitype<bool>(false).get<bool>() == false);
//	TEST_OK(unitype<int>(1).get<int>() != 0);
//	TEST_OK(unitype<int>(0).get<int>() == 0);
//	TEST_OK(unitype<int>(1).get<bool>() == true);
//	TEST_OK(unitype<int>(0).get<bool>() == false);
//	TEST_OK(make_unitype(true).get<pfs::string>() == _l1("true"));
//	TEST_OK(make_unitype(false).get<pfs::string>() == _l1("false"));
//	TEST_OK(make_unitype(true).get<pfs::bytearray>() == pfs::bytearray(1, '\x1'));
//	TEST_OK(make_unitype(false).get<pfs::bytearray>() == pfs::bytearray(1, '\x0'));
//
//	TEST_OK(make_unitype('\x80').get<bool>() == true);
//	TEST_OK(make_unitype('\x00').get<bool>() == false);
//
//	for (int i = PFS_CHAR_MIN; i <= PFS_CHAR_MAX; ++i)
//		TEST_OK(make_unitype(char(i)).get<int>() == i);
//
//	TEST_OK(make_unitype(char(0)).get<pfs::string>().size() == 1);
//	TEST_OK(pfs::string("\x0", 1).size() == 1);
//	TEST_FAIL(make_unitype(char(0)).get<pfs::string>() == pfs::string(1,char(0)));
//
//	for (int i = PFS_CHAR_MIN; i <= PFS_CHAR_MAX; ++i)
//		TEST_OK(make_unitype(char(i)).get<pfs::string>() == pfs::string(1,char(i)));
//
//	for (int i = PFS_CHAR_MIN; i <= PFS_CHAR_MAX; ++i)
//		TEST_OK(make_unitype(char(i)).get<pfs::bytearray>() == pfs::bytearray(1,char(i)));

//	sbyte_val.setInt(PFS_SBYTE_MIN);
//	TEST_OK(sbyte_val.toSByte(&ok) == PFS_SBYTE_MIN);
//	TEST_OK2(ok, "sbyte_val == PFS_SBYTE_MIN");
//
//	sbyte_val.setInt(PFS_SBYTE_MAX);
//	TEST_OK(sbyte_val.toSByte(&ok) == PFS_SBYTE_MAX);
//	TEST_OK2(ok, "sbyte_val == PFS_SBYTE_MAX");
//
//	byte_val.setUInt(0);
//	TEST_OK(byte_val.toByte(&ok) == 0);
//	TEST_OK2(ok, "byte_val == 0");
//
//	byte_val.setUInt(PFS_BYTE_MAX);
//	TEST_OK(byte_val.toByte(&ok) == PFS_BYTE_MAX);
//	TEST_OK2(ok, "byte_val == PFS_BYTE_MAX");
//
//	short_val.setInt(PFS_SHORT_MIN);
//	TEST_OK(short_val.toShort(&ok) == PFS_SHORT_MIN);
//	TEST_OK2(ok, "short_val == PFS_SHORT_MIN");
//
//	short_val.setInt(PFS_SHORT_MAX);
//	TEST_OK(short_val.toShort(&ok) == PFS_SHORT_MAX);
//	TEST_OK2(ok, "short_val == PFS_SHORT_MAX");
//
//	ushort_val.setUInt(0);
//	TEST_OK(ushort_val.toUShort(&ok) == 0);
//	TEST_OK2(ok, "ushort_val == 0");
//
//	ushort_val.setUInt(PFS_USHORT_MAX);
//	TEST_OK(ushort_val.toUShort(&ok) == PFS_USHORT_MAX);
//	TEST_OK2(ok, "ushort_val == PFS_USHORT_MAX");
//
//	int_val.setInt(PFS_INT_MIN);
//	TEST_OK(int_val.toInt(&ok) == PFS_INT_MIN);
//	TEST_OK2(ok, "int_val == PFS_INT_MIN");
//
//	int_val.setInt(PFS_INT_MAX);
//	TEST_OK(int_val.toInt(&ok) == PFS_INT_MAX);
//	TEST_OK2(ok, "int_val == PFS_INT_MAX");
//
//	uint_val.setUInt(0);
//	TEST_OK(uint_val.toUInt(&ok) == 0);
//	TEST_OK2(ok, "uint_val == 0");
//
//	uint_val.setUInt(PFS_UINT_MAX);
//	TEST_OK(uint_val.toUInt(&ok) == PFS_UINT_MAX);
//	TEST_OK2(ok, "uint_val == PFS_UINT_MAX");
//
//	long_val.setLong(PFS_LONG_MIN);
//	TEST_OK(long_val.toLong(&ok) == PFS_LONG_MIN);
//	TEST_OK2(ok, "long_val == PFS_LONG_MIN");
//
//	long_val.setLong(PFS_LONG_MAX);
//	TEST_OK(long_val.toLong(&ok) == PFS_LONG_MAX);
//	TEST_OK2(ok, "long_val == PFS_LONG_MAX");
//
//	ulong_val.setULong(0);
//	TEST_OK(ulong_val.toULong(&ok) == 0);
//	TEST_OK2(ok, "ulong_val == 0");
//
//	ulong_val.setULong(PFS_ULONG_MAX);
//	TEST_OK(ulong_val.toULong(&ok) == PFS_ULONG_MAX);
//	TEST_OK2(ok, "ulong_val == PFS_ULONG_MAX");
//
//	float_val.setFloat(PFS_FLOAT_MAX);
//	TEST_OK(float_val.toFloat(&ok) == PFS_FLOAT_MAX);
//	TEST_OK2(ok, "float_val == PFS_FLOAT_MAX");
//
//	double_val.setDouble(PFS_DOUBLE_MAX);
//	TEST_OK(double_val.toDouble(&ok) == PFS_DOUBLE_MAX);
//	TEST_OK2(ok, "double_val == PFS_DOUBLE_MAX");
//
//	pfs::string text_en(pfs::string::fromUtf8("The quick brown fox jumps over the lazy dog"));
//	pfs::string text_ru(pfs::string::fromUtf8("Съешь ещё этих мягких французских булок, да выпей чаю"));
//	pfs::string text;
//
//	text_val.setString(text_en);
//	text = text_val.toString(&ok);
//	TEST_OK(ok && text == text_en);
//
//	text_val.setString(text_ru);
//	text = text_val.toString(&ok);
//	TEST_OK(ok && text == text_ru);
//
//	pfs::bytearray blob;
//	blob_val.setBlob(loremipsum, ::strlen(loremipsum));
//	blob = blob_val.toBlob();
//	TEST_OK(blob.size() == ::strlen(loremipsum));
//	TEST_OK(::strcmp(loremipsum, blob.data()) == 0);

}

void test_01 ()
{
/*	bool ok;
	pfs::unitype uval;

	TEST_OK(uval.isNull());

	uval.setBool(true);
	TEST_OK(uval.toBool(&ok) == true && ok);

	uval.setBool(false);
	TEST_OK(uval.toBool(&ok) == false && ok);

	uval.setChar('W');
	TEST_OK(uval.toUCChar(&ok) == pfs::ucchar('W') && ok);

	uval.setInt(PFS_SBYTE_MIN);
	TEST_OK(uval.toSByte(&ok) == PFS_SBYTE_MIN && ok);

	uval.setInt(PFS_SBYTE_MAX);
	TEST_OK(uval.toSByte(&ok) == PFS_SBYTE_MAX && ok);

	uval.setUInt(PFS_BYTE_MAX);
	TEST_OK(uval.toByte(&ok) == PFS_BYTE_MAX && ok);

	uval.setInt(PFS_SHORT_MIN);
	TEST_OK(uval.toShort(&ok) == PFS_SHORT_MIN && ok);

	uval.setInt(PFS_SHORT_MAX);
	TEST_OK(uval.toShort(&ok) == PFS_SHORT_MAX && ok);

	uval.setUInt(PFS_USHORT_MAX);
	TEST_OK(uval.toUShort(&ok) == PFS_USHORT_MAX && ok);

	uval.setInt(PFS_INT_MIN);
	TEST_OK(uval.toInt(&ok) == PFS_INT_MIN && ok);

	uval.setInt(PFS_INT_MAX);
	TEST_OK(uval.toInt(&ok) == PFS_INT_MAX && ok);

	uval.setUInt(PFS_UINT_MAX);
	TEST_OK(uval.toUInt(&ok) == PFS_UINT_MAX && ok);

	uval.setLong(PFS_LONG_MIN);
	TEST_OK(uval.toLong(&ok) == PFS_LONG_MIN && ok);

	uval.setLong(PFS_LONG_MAX);
	TEST_OK(uval.toLong(&ok) == PFS_LONG_MAX && ok);

	uval.setULong(PFS_ULONG_MAX);
	TEST_OK(uval.toULong(&ok) == PFS_ULONG_MAX && ok);

	uval.setFloat(PFS_FLOAT_MAX);
	TEST_OK(uval.toFloat(&ok) == PFS_FLOAT_MAX && ok);

	uval.setDouble(PFS_DOUBLE_MAX);
	TEST_OK(uval.toDouble(&ok) == PFS_DOUBLE_MAX && ok);

	pfs::string text_en(pfs::string::fromUtf8("The quick brown fox jumps over the lazy dog"));
	pfs::string text_ru(pfs::string::fromUtf8("Съешь ещё этих мягких французских булок, да выпей чаю"));
	pfs::string text;

	uval.setString(text_en);
	text = uval.toString(&ok);
	TEST_OK(ok && text_en == text);

	uval.setString(text_ru);
	text = uval.toString(&ok);
	TEST_OK(ok && text_ru == text);

	pfs::bytearray blob;
	uval.setBlob(loremipsum, ::strlen(loremipsum));
	blob = uval.toBlob();
	TEST_OK(blob.size() == ::strlen(loremipsum));
	TEST_OK(::strcmp(loremipsum, blob.data()) == 0);*/
}

void test_bool(void)
{
/*	bool ok;
	pfs::unitype uval;

	uval.setBool(true);
	TEST_OK2(uval.toBool(&ok) == true && ok, "Boolean equals to true");
	uval.setBool(false);
	TEST_OK2(uval.toBool(&ok) == false && ok, "Boolean equals to false");

	uval.setChar('W');
	TEST_OK2(uval.toBool(&ok) == true && ok, "'W' equals to true");
	uval.setChar('\0');
	TEST_OK2(uval.toBool(&ok) == false && ok, "'\\0' equals to false");

	uval.setInt(PFS_SBYTE_MIN);
	TEST_OK2(uval.toBool(&ok) == true && ok, "PFS_SBYTE_MIN equals to true");
	uval.setInt(PFS_SBYTE_MAX);
	TEST_OK2(uval.toBool(&ok) == true && ok, "PFS_SBYTE_MAX equals to true");

	uval.setUInt(PFS_BYTE_MAX);
	TEST_OK2(uval.toBool(&ok) == true && ok, "PFS_BYTE_MAX equals to true");

	uval.setInt(PFS_SHORT_MIN);
	TEST_OK2(uval.toBool(&ok) == true && ok, "PFS_SHORT_MIN equals to true");
	uval.setInt(PFS_SHORT_MAX);
	TEST_OK2(uval.toBool(&ok) == true && ok, "PFS_SHORT_MAX equals to true");

	uval.setUInt(PFS_USHORT_MAX);
	TEST_OK2(uval.toBool(&ok) == true && ok, "PFS_USHORT_MAX equals to true");

	uval.setInt(PFS_INT_MIN);
	TEST_OK2(uval.toBool(&ok) == true && ok, "PFS_INT_MIN equals to true");
	uval.setInt(PFS_INT_MAX);
	TEST_OK2(uval.toBool(&ok) == true && ok, "PFS_INT_MAX equals to true");
	uval.setInt(0);
	TEST_OK2(uval.toBool(&ok) == false && ok, "Integer '0' equals to false");

	uval.setUInt(PFS_UINT_MAX);
	TEST_OK2(uval.toBool(&ok) == true && ok, "PFS_UINT_MAX equals to true");
	uval.setUInt(0);
	TEST_OK2(uval.toBool(&ok) == false && ok, "Unsigned integer '0' equals to false");

	uval.setLong(PFS_LONG_MIN);
	TEST_OK2(uval.toBool(&ok) == true && ok, "PFS_LONG_MIN equals to true");
	uval.setLong(PFS_LONG_MAX);
	TEST_OK2(uval.toBool(&ok) == true && ok, "PFS_LONG_MAX equals to true");
	uval.setLong(0L);
	TEST_OK2(uval.toBool(&ok) == false && ok, "Long '0' equals to false");

	uval.setULong(PFS_ULONG_MAX);
	TEST_OK2(uval.toBool(&ok) == true && ok, "PFS_ULONG_MAX equals to true");
	uval.setULong(0UL);
	TEST_OK2(uval.toBool(&ok) == false && ok, "Unsigned long '0' equals to false");

	uval.setFloat(PFS_FLOAT_MAX);
	TEST_OK2(uval.toBool(&ok) == true && ok, "PFS_FLOAT_MAX equals to true");

	uval.setFloat(0.0f);
	TEST_OK2(uval.toBool(&ok) == false && ok, "Float '0.0' equals to false");

	uval.setDouble(PFS_DOUBLE_MAX);
	TEST_OK2(uval.toBool(&ok) == true && ok, "PFS_DOUBLE_MAX equals to true");

	uval.setDouble(0.0f);
	TEST_OK2(uval.toBool(&ok) == false && ok, "Double '0.0' equals to false");

	uval.setString(pfs::string(""));
	TEST_OK2(uval.toBool() == false, "<empty text> == false");

	uval.setString(pfs::string("false"));
	TEST_OK2(uval.toBool() == false, "'false' == false");

	uval.setString(pfs::string("no"));
	TEST_OK2(uval.toBool() == true, "'no' == true");

	uval.setString(pfs::string("FALSE"));
	TEST_OK2(uval.toBool() == true, "'FALSE' == true");

	uval.setString(pfs::string("FaLsE"));
	TEST_OK2(uval.toBool() == true, "'FaLsE' == true");

	uval.setString(pfs::string("0"));
	TEST_OK2(uval.toBool() == false, "'0' == false");

	uval.setString(pfs::string("text"));
	TEST_OK2(uval.toBool() == true, "<any other text> is true");

	uval.setBlob("false", 5);
	TEST_OK2(uval.toBool() == true, "Blob(\"false\") == true");

	uval.setBlob("no", 2);
	TEST_OK2(uval.toBool() == true, "Blob(\"no\") == true");

	uval.setBlob("0", 1);
	TEST_OK2(uval.toBool() == true, "Blob(\"0\") == true");
	uval.setBlob(pfs::bytearray());
	TEST_OK2(uval.toBool() == false, "Blob() == false");
	TEST_OK2(!uval.isNull(), "Blob() is not Null");

	uval.setBlob("abracadabra", 11);
	TEST_OK2(uval.toBool() == true, "Blob(\"abracadabra\") == true");

	uval.setBlob("False", 5);
	TEST_OK2(uval.toBool() == true, "Blob(\"False\") == true");

	uval.setBlob("nO", 2);
	TEST_OK2(uval.toBool() == true, "Blob(\"nO\") == true");*/
}

void test_integer(void)
{
/*	bool ok;
	short_t  s = 10;
	int_t    d = 10;
	pfs::unitype short_val;
	pfs::unitype int_val;

	short_val.setInt(s);
	TEST_OK(short_val.toShort(&ok) == s && ok);

	short_val.setFromString(pfs::string("10"));
	TEST_OK(short_val.toShort(&ok) == 10 && ok);

	int_val.setInt(d);
	TEST_OK(int_val.toInt(&ok) == d && ok);

	int_val.setFromString(pfs::string("10"));
	TEST_OK(int_val.toInt(&ok) == 10 && ok);*/
}

void test_float(void)
{
/*	bool ok;
	double d = 10.0f;
	float  f = 10.0f;
	pfs::unitype float_val;
	pfs::unitype double_val;

	float_val.setFloat(f);
	TEST_OK(float_val.toFloat(&ok) == f && ok);

	float_val.setFromString(pfs::string("3.14159"));
	TEST_OK(float_val.toDouble(&ok) == pfs::string("3.14159").toDouble() && ok);

	double_val.setDouble(d);
	TEST_OK(double_val.toDouble(&ok) == d && ok);

	double_val.setFromString(pfs::string("3.14e+10"));
	double d1 = double_val.toDouble(&ok);
	double d2 = pfs::string("3.14e+10").toDouble();
	TEST_OK(d1 == d2);
	TEST_OK(double_val.toDouble(&ok) == pfs::string("3.14e+10").toDouble() && ok);*/
}

#ifdef __COMMENT__
struct TestElem
{
	TestElem  () : str()                        { /*puts("TestElem()");*/ }
	TestElem  (const TestElem & o) : str(o.str) { if (!str.isEmpty()) ++counter; printf("TestElem (TestElem(\"%s\"))\n", str.c_str()); }
	TestElem  (const pfs::string & s) : str(s)       { ++counter; printf ("TestElem (\"%s\")\n", str.c_str()); }
	~TestElem ()                                { if (!str.isEmpty()) --counter; /*printf ("~TestElem (\"%s\")\n", str.c_str());*/ }

	TestElem & operator = (const TestElem & other)
	{
		++counter;
		str = other.str;
		printf ("TestElem::operator = (\"%s\")\n", str.c_str());
		return *this;
	}
	static int counter;
	pfs::string str;
};
#endif


#ifdef __COMMENT__

int TestElem::counter = 0;

void test_object ()
{
	{
		pfs::unitype ut = pfs::unitype::make_object<Vector<TestElem> >();
		Vector<TestElem> & p = ut.objectRef<Vector<TestElem> >();

		p.append(TestElem(pfs::string("One")));
		p.append(TestElem(pfs::string("Two")));
		p.append(TestElem(pfs::string("Three")));
		p.append(TestElem(pfs::string("Four")));

		TEST_OK(p.size() == 4);
		TEST_OK(TestElem::counter == 4);
	}

	TEST_OK(TestElem::counter == 0);
}


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
    PFS_CHECK_SIZEOF_TYPES;
    PFS_UNUSED2(argc, argv);
	BEGIN_TESTS(783);

	test_base();
	test_01();
	test_bool();
	test_integer();
	test_float();
//	test_object();
/*
	test_text();
	test_blob();
	test_time();
*/

	END_TESTS;
}


