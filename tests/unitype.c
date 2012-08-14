/*
 * unitype.c
 *
 *  Created on: 14.08.2012
 *      Author: wladt
 */


#include <cwt/test.h>
#include <cwt/str.h>
#include <cwt/utils.h>
#include <cwt/unitype.h>

static CwtStrNS     *__strNS = NULL;
static CwtUtilsNS   *__utilsNS = NULL;
static CwtUniTypeNS *__utNS = NULL;

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
	const char *text0 = "The quick brown fox jumps over the lazy dog";
	const char *text1 = "Съешь ещё этих мягких французских булок, да выпей чаю";
	const CWT_CHAR *text2 = _T("The quick brown fox jumps over the lazy dog");
	const CWT_CHAR *text3 = _T("Съешь ещё этих мягких французских булок, да выпей чаю");

	CWT_TIME tm;

	CwtUniType *bool_val    = __utNS->create();
	CwtUniType *char_val    = __utNS->create();
	CwtUniType *cwtchar_val = __utNS->create();
	CwtUniType *sbyte_val   = __utNS->create();
	CwtUniType *byte_val    = __utNS->create();
	CwtUniType *short_val   = __utNS->create();
	CwtUniType *ushort_val  = __utNS->create();
	CwtUniType *int_val     = __utNS->create();
	CwtUniType *uint_val    = __utNS->create();
	CwtUniType *long_val    = __utNS->create();
	CwtUniType *ulong_val   = __utNS->create();
	CwtUniType *llong_val   = __utNS->create();
	CwtUniType *ullong_val  = __utNS->create();
	CwtUniType *float_val   = __utNS->create();
	CwtUniType *double_val  = __utNS->create();
	CwtUniType *text_val    = __utNS->create();
	CwtUniType *cwttext_val = __utNS->create();
	CwtUniType *blob_val    = __utNS->create();
	CwtUniType *time_val    = __utNS->create();
	CwtUniType *date_val    = __utNS->create();
	CwtUniType *datetime_val= __utNS->create();

	__utNS->setBool(bool_val, TRUE);
	CWT_TEST_OK(__utNS->toBool(bool_val) == TRUE);

	__utNS->setBool(bool_val, FALSE);
	CWT_TEST_OK(__utNS->toBool(bool_val) == FALSE);

	__utNS->setChar(char_val, 'W');
	CWT_TEST_OK(__utNS->toChar(char_val) == 'W');

	__utNS->setCwtChar(cwtchar_val, _T('W'));
	CWT_TEST_OK(__utNS->toCwtChar(cwtchar_val) == _T('W'));

	__utNS->setSByte(sbyte_val, CWT_SBYTE_MIN);
	CWT_TEST_OK(__utNS->toSByte(sbyte_val) == CWT_SBYTE_MIN);

	__utNS->setSByte(sbyte_val, CWT_SBYTE_MAX);
	CWT_TEST_OK(__utNS->toSByte(sbyte_val) == CWT_SBYTE_MAX);

	__utNS->setByte(byte_val, CWT_BYTE_MAX);
	CWT_TEST_OK(__utNS->toByte(byte_val) == CWT_BYTE_MAX);

	__utNS->setShort(short_val, CWT_SHORT_MIN);
	CWT_TEST_OK(__utNS->toShort(short_val) == CWT_SHORT_MIN);

	__utNS->setShort(short_val, CWT_SHORT_MAX);
	CWT_TEST_OK(__utNS->toShort(short_val) == CWT_SHORT_MAX);

	__utNS->setUShort(ushort_val, CWT_USHORT_MAX);
	CWT_TEST_OK(__utNS->toUShort(ushort_val) == CWT_USHORT_MAX);

	__utNS->setInt(int_val, CWT_INT_MIN);
	CWT_TEST_OK(__utNS->toInt(int_val) == CWT_INT_MIN);

	__utNS->setInt(int_val, CWT_INT_MAX);
	CWT_TEST_OK(__utNS->toInt(int_val) == CWT_INT_MAX);

	__utNS->setUInt(uint_val, CWT_UINT_MAX);
	CWT_TEST_OK(__utNS->toUInt(uint_val) == CWT_UINT_MAX);

	__utNS->setLong(long_val, CWT_LONG_MIN);
	CWT_TEST_OK(__utNS->toLong(long_val) == CWT_LONG_MIN);

	__utNS->setLong(long_val, CWT_LONG_MAX);
	CWT_TEST_OK(__utNS->toLong(long_val) == CWT_LONG_MAX);

	__utNS->setULong(ulong_val, CWT_ULONG_MAX);
	CWT_TEST_OK(__utNS->toULong(ulong_val) == CWT_ULONG_MAX);

	__utNS->setLongLong(llong_val, CWT_LONGLONG_MIN);
	CWT_TEST_OK(__utNS->toLongLong(llong_val) == CWT_LONGLONG_MIN);

	__utNS->setLongLong(llong_val, CWT_LONGLONG_MAX);
	CWT_TEST_OK(__utNS->toLongLong(llong_val) == CWT_LONGLONG_MAX);

	__utNS->setULongLong(ullong_val, CWT_ULONGLONG_MAX);
	CWT_TEST_OK(__utNS->toULongLong(ullong_val) == CWT_ULONGLONG_MAX);

	__utNS->setFloat(float_val, CWT_FLOAT_MAX);
	CWT_TEST_OK(__utNS->toFloat(float_val) == CWT_FLOAT_MAX);

	__utNS->setDouble(double_val, CWT_FLOAT_MAX);
	CWT_TEST_OK(__utNS->toDouble(double_val) == CWT_FLOAT_MAX);

	__utNS->setText(text_val, text0, strlen(text0));
	CWT_TEST_OK(strcmp(text0, __utNS->toText(text_val)) == 0);

	__utNS->setText(text_val, text1, strlen(text1));
	CWT_TEST_OK(strcmp(text1, __utNS->toText(text_val)) == 0);

	__utNS->setCwtText(cwttext_val, text2, __strNS->len(text2));
	CWT_TEST_OK(__strNS->cmp(text2, __utNS->toCwtText(cwttext_val)) == 0);

	__utNS->setCwtText(cwttext_val, text3, __strNS->len(text3));
	CWT_TEST_OK(__strNS->cmp(text3, __utNS->toCwtText(cwttext_val)) == 0);

	__utNS->setBlob(blob_val, loremipsum, strlen(loremipsum)+1);
	CWT_TEST_OK(strcmp(loremipsum, (const char*) __utNS->toBlob(blob_val)) == 0);

	__utilsNS->now(&tm);
	__utNS->setTime(time_val, &tm, sizeof(CWT_TIME));
	CWT_TEST_OK(__utNS->toTime(time_val)->hour == tm.hour);
	CWT_TEST_OK(__utNS->toTime(time_val)->min == tm.min);
	CWT_TEST_OK(__utNS->toTime(time_val)->sec == tm.sec);

	__utNS->setDate(date_val, &tm, sizeof(CWT_TIME));
	CWT_TEST_OK(__utNS->toTime(date_val)->year == tm.year);
	CWT_TEST_OK(__utNS->toTime(date_val)->mon == tm.mon);
	CWT_TEST_OK(__utNS->toTime(date_val)->day == tm.day);

	__utNS->setDateTime(datetime_val, &tm, sizeof(CWT_TIME));
	CWT_TEST_OK(__utNS->toTime(datetime_val)->hour == tm.hour);
	CWT_TEST_OK(__utNS->toTime(datetime_val)->min == tm.min);
	CWT_TEST_OK(__utNS->toTime(datetime_val)->sec == tm.sec);
	CWT_TEST_OK(__utNS->toTime(datetime_val)->year == tm.year);
	CWT_TEST_OK(__utNS->toTime(datetime_val)->mon == tm.mon);
	CWT_TEST_OK(__utNS->toTime(datetime_val)->day == tm.day);

	__utNS->free(bool_val);
	__utNS->free(char_val);
	__utNS->free(cwtchar_val);
	__utNS->free(sbyte_val);
	__utNS->free(byte_val);
	__utNS->free(short_val);
	__utNS->free(ushort_val);
	__utNS->free(int_val);
	__utNS->free(uint_val);
	__utNS->free(long_val);
	__utNS->free(ulong_val);
	__utNS->free(llong_val);
	__utNS->free(ullong_val);
	__utNS->free(float_val);
	__utNS->free(double_val);
	__utNS->free(text_val);
	__utNS->free(cwttext_val);
	__utNS->free(blob_val);
	__utNS->free(time_val);
	__utNS->free(date_val);
	__utNS->free(datetime_val);
}


void test_01(void)
{
	const char *text0 = "The quick brown fox jumps over the lazy dog";
	const char *text1 = "Съешь ещё этих мягких французских булок, да выпей чаю";
	const CWT_CHAR *text2 = _T("The quick brown fox jumps over the lazy dog");
	const CWT_CHAR *text3 = _T("Съешь ещё этих мягких французских булок, да выпей чаю");

	CWT_TIME tm;

	CwtUniType *uval    = __utNS->create();

	__utNS->setBool(uval, TRUE);
	CWT_TEST_OK(__utNS->toBool(uval) == TRUE);

	__utNS->setBool(uval, FALSE);
	CWT_TEST_OK(__utNS->toBool(uval) == FALSE);

	__utNS->setChar(uval, 'W');
	CWT_TEST_OK(__utNS->toChar(uval) == 'W');

	__utNS->setCwtChar(uval, _T('W'));
	CWT_TEST_OK(__utNS->toCwtChar(uval) == _T('W'));

	__utNS->setSByte(uval, CWT_SBYTE_MIN);
	CWT_TEST_OK(__utNS->toSByte(uval) == CWT_SBYTE_MIN);

	__utNS->setSByte(uval, CWT_SBYTE_MAX);
	CWT_TEST_OK(__utNS->toSByte(uval) == CWT_SBYTE_MAX);

	__utNS->setByte(uval, CWT_BYTE_MAX);
	CWT_TEST_OK(__utNS->toByte(uval) == CWT_BYTE_MAX);

	__utNS->setShort(uval, CWT_SHORT_MIN);
	CWT_TEST_OK(__utNS->toShort(uval) == CWT_SHORT_MIN);

	__utNS->setShort(uval, CWT_SHORT_MAX);
	CWT_TEST_OK(__utNS->toShort(uval) == CWT_SHORT_MAX);

	__utNS->setUShort(uval, CWT_USHORT_MAX);
	CWT_TEST_OK(__utNS->toUShort(uval) == CWT_USHORT_MAX);

	__utNS->setInt(uval, CWT_INT_MIN);
	CWT_TEST_OK(__utNS->toInt(uval) == CWT_INT_MIN);

	__utNS->setInt(uval, CWT_INT_MAX);
	CWT_TEST_OK(__utNS->toInt(uval) == CWT_INT_MAX);

	__utNS->setUInt(uval, CWT_UINT_MAX);
	CWT_TEST_OK(__utNS->toUInt(uval) == CWT_UINT_MAX);

	__utNS->setLong(uval, CWT_LONG_MIN);
	CWT_TEST_OK(__utNS->toLong(uval) == CWT_LONG_MIN);

	__utNS->setLong(uval, CWT_LONG_MAX);
	CWT_TEST_OK(__utNS->toLong(uval) == CWT_LONG_MAX);

	__utNS->setULong(uval, CWT_ULONG_MAX);
	CWT_TEST_OK(__utNS->toULong(uval) == CWT_ULONG_MAX);

	__utNS->setLongLong(uval, CWT_LONGLONG_MIN);
	CWT_TEST_OK(__utNS->toLongLong(uval) == CWT_LONGLONG_MIN);

	__utNS->setLongLong(uval, CWT_LONGLONG_MAX);
	CWT_TEST_OK(__utNS->toLongLong(uval) == CWT_LONGLONG_MAX);

	__utNS->setULongLong(uval, CWT_ULONGLONG_MAX);
	CWT_TEST_OK(__utNS->toULongLong(uval) == CWT_ULONGLONG_MAX);

	__utNS->setFloat(uval, CWT_FLOAT_MAX);
	CWT_TEST_OK(__utNS->toFloat(uval) == CWT_FLOAT_MAX);

	__utNS->setDouble(uval, CWT_FLOAT_MAX);
	CWT_TEST_OK(__utNS->toDouble(uval) == CWT_FLOAT_MAX);

	__utNS->setText(uval, text0, strlen(text0));
	CWT_TEST_OK(strcmp(text0, __utNS->toText(uval)) == 0);

	__utNS->setText(uval, text1, strlen(text1));
	CWT_TEST_OK(strcmp(text1, __utNS->toText(uval)) == 0);

	__utNS->setCwtText(uval, text2, __strNS->len(text2));
	CWT_TEST_OK(__strNS->cmp(text2, __utNS->toCwtText(uval)) == 0);

	__utNS->setCwtText(uval, text3, __strNS->len(text3));
	CWT_TEST_OK(__strNS->cmp(text3, __utNS->toCwtText(uval)) == 0);

	__utNS->setBlob(uval, loremipsum, strlen(loremipsum)+1);
	CWT_TEST_OK(strcmp(loremipsum, (const char*) __utNS->toBlob(uval)) == 0);

	__utilsNS->now(&tm);
	__utNS->setTime(uval, &tm, sizeof(CWT_TIME));
	CWT_TEST_OK(__utNS->toTime(uval)->hour == tm.hour);
	CWT_TEST_OK(__utNS->toTime(uval)->min == tm.min);
	CWT_TEST_OK(__utNS->toTime(uval)->sec == tm.sec);

	__utNS->setDate(uval, &tm, sizeof(CWT_TIME));
	CWT_TEST_OK(__utNS->toTime(uval)->year == tm.year);
	CWT_TEST_OK(__utNS->toTime(uval)->mon == tm.mon);
	CWT_TEST_OK(__utNS->toTime(uval)->day == tm.day);

	__utNS->setDateTime(uval, &tm, sizeof(CWT_TIME));
	CWT_TEST_OK(__utNS->toTime(uval)->hour == tm.hour);
	CWT_TEST_OK(__utNS->toTime(uval)->min == tm.min);
	CWT_TEST_OK(__utNS->toTime(uval)->sec == tm.sec);
	CWT_TEST_OK(__utNS->toTime(uval)->year == tm.year);
	CWT_TEST_OK(__utNS->toTime(uval)->mon == tm.mon);
	CWT_TEST_OK(__utNS->toTime(uval)->day == tm.day);

	__utNS->free(uval);
}


void test_bool(void)
{
	const char *text0 = "The quick brown fox jumps over the lazy dog";
	const char *text1 = "Съешь ещё этих мягких французских булок, да выпей чаю";
	const CWT_CHAR *text2 = _T("The quick brown fox jumps over the lazy dog");
	const CWT_CHAR *text3 = _T("Съешь ещё этих мягких французских булок, да выпей чаю");

	CWT_TIME tm;

	CwtUniType *uval = __utNS->create();

	__utNS->setBool(uval, TRUE);
	CWT_TEST_OK2(__utNS->toBool(uval) == TRUE, "'BOOL' equality to TRUE");
	__utNS->setBool(uval, FALSE);
	CWT_TEST_OK2(__utNS->toBool(uval) == FALSE, "'BOOL' equality to FALSE");

	__utNS->setChar(uval, 'W');
	CWT_TEST_OK2(__utNS->toBool(uval) == TRUE, "'char' equality to TRUE");
	__utNS->setChar(uval, '\0');
	CWT_TEST_OK2(__utNS->toBool(uval) == FALSE, "'char' equality to FALSE");

	__utNS->setCwtChar(uval, _T('W'));
	CWT_TEST_OK2(__utNS->toBool(uval) == TRUE, "'CWT_CHAR' equality to TRUE");
	__utNS->setCwtChar(uval, _T('\0'));
	CWT_TEST_OK2(__utNS->toBool(uval) == FALSE, "'CWT_CHAR' equality to FALSE");


	__utNS->setSByte(uval, CWT_SBYTE_MIN);
	CWT_TEST_OK2(__utNS->toBool(uval) == TRUE, "'SBYTE' equality to TRUE");
	__utNS->setSByte(uval, CWT_SBYTE_MAX);
	CWT_TEST_OK2(__utNS->toBool(uval) == TRUE, "'SBYTE' equality to TRUE");
	__utNS->setSByte(uval, 0);
	CWT_TEST_OK2(__utNS->toBool(uval) == FALSE, "'SBYTE' equality to FALSE");

	__utNS->setByte(uval, CWT_BYTE_MAX);
	CWT_TEST_OK2(__utNS->toBool(uval) == TRUE, "'BYTE' equality to TRUE");
	__utNS->setByte(uval, 0);
	CWT_TEST_OK2(__utNS->toBool(uval) == FALSE, "'BYTE' equality to FALSE");

	__utNS->setShort(uval, CWT_SHORT_MIN);
	CWT_TEST_OK2(__utNS->toBool(uval) == TRUE, "'SHORT' equality to TRUE");
	__utNS->setShort(uval, CWT_SHORT_MAX);
	CWT_TEST_OK2(__utNS->toBool(uval) == TRUE, "'SHORT' equality to TRUE");
	__utNS->setShort(uval, 0);
	CWT_TEST_OK2(__utNS->toBool(uval) == FALSE, "'SHORT' equality to FALSE");

	__utNS->setUShort(uval, CWT_USHORT_MAX);
	CWT_TEST_OK2(__utNS->toBool(uval) == TRUE, "'USHORT' equality to TRUE");
	__utNS->setUShort(uval, 0);
	CWT_TEST_OK2(__utNS->toBool(uval) == FALSE, "'USHORT' equality to FALSE");

	__utNS->setInt(uval, CWT_INT_MIN);
	CWT_TEST_OK2(__utNS->toBool(uval) == TRUE, "'INT' equality to TRUE");
	__utNS->setInt(uval, CWT_INT_MAX);
	CWT_TEST_OK2(__utNS->toBool(uval) == TRUE, "'INT' equality to TRUE");
	__utNS->setInt(uval, 0);
	CWT_TEST_OK2(__utNS->toBool(uval) == FALSE, "'INT' equality to FALSE");

	__utNS->setUInt(uval, CWT_UINT_MAX);
	CWT_TEST_OK2(__utNS->toBool(uval) == TRUE, "'UINT' equality to TRUE");
	__utNS->setUInt(uval, 0);
	CWT_TEST_OK2(__utNS->toBool(uval) == FALSE, "'UINT' equality to FALSE");

	__utNS->setLong(uval, CWT_LONG_MIN);
	CWT_TEST_OK2(__utNS->toBool(uval) == TRUE, "'LONG' equality to TRUE");
	__utNS->setLong(uval, CWT_LONG_MAX);
	CWT_TEST_OK2(__utNS->toBool(uval) == TRUE, "'LONG' equality to TRUE");
	__utNS->setLong(uval, 0L);
	CWT_TEST_OK2(__utNS->toBool(uval) == FALSE, "'LONG' equality to FALSE");

	__utNS->setULong(uval, CWT_ULONG_MAX);
	CWT_TEST_OK2(__utNS->toBool(uval) == TRUE, "'ULONG' equality to TRUE");
	__utNS->setULong(uval, 0UL);
	CWT_TEST_OK2(__utNS->toBool(uval) == FALSE, "'ULONG' equality to FALSE");

	__utNS->setLongLong(uval, CWT_LONGLONG_MIN);
	CWT_TEST_OK2(__utNS->toBool(uval) == TRUE, "'LONGLONG' equality to TRUE");
	__utNS->setLongLong(uval, CWT_LONGLONG_MAX);
	CWT_TEST_OK2(__utNS->toBool(uval) == TRUE, "'LONGLONG' equality to TRUE");
	__utNS->setLongLong(uval, 0LL);
	CWT_TEST_OK2(__utNS->toBool(uval) == FALSE, "'LONGLONG' equality to FALSE");

	__utNS->setULongLong(uval, CWT_ULONGLONG_MAX);
	CWT_TEST_OK2(__utNS->toBool(uval) == TRUE, "'ULONGLONG' equality to TRUE");
	__utNS->setULongLong(uval, 0ULL);
	CWT_TEST_OK2(__utNS->toBool(uval) == FALSE, "'ULONGLONG' equality to FALSE");

	__utNS->setFloat(uval, CWT_FLOAT_MAX);
	CWT_TEST_OK(__utNS->toBool(uval) == TRUE);

	__utNS->setDouble(uval, CWT_FLOAT_MAX);
	CWT_TEST_OK(__utNS->toBool(uval) == TRUE);

	__utNS->setText(uval, text0, strlen(text0));
	CWT_TEST_OK(__utNS->toBool(uval) == TRUE);

	__utNS->setText(uval, text1, strlen(text1));
	CWT_TEST_OK(__utNS->toBool(uval) == TRUE);

	__utNS->setCwtText(uval, text2, __strNS->len(text2));
	CWT_TEST_OK(__utNS->toBool(uval) == TRUE);

	__utNS->setCwtText(uval, text3, __strNS->len(text3));
	CWT_TEST_OK(__utNS->toBool(uval) == TRUE);

	__utNS->setBlob(uval, loremipsum, strlen(loremipsum)+1);
	CWT_TEST_OK(__utNS->toBool(uval) == TRUE);

	__utilsNS->now(&tm);
	__utNS->setTime(uval, &tm, sizeof(CWT_TIME));
	CWT_TEST_OK(__utNS->toBool(uval) == TRUE);

	__utNS->setDate(uval, &tm, sizeof(CWT_TIME));
	CWT_TEST_OK(__utNS->toBool(uval) == TRUE);

	__utNS->setDateTime(uval, &tm, sizeof(CWT_TIME));
	CWT_TEST_OK(__utNS->toBool(uval) == TRUE);

	__utNS->free(uval);
}


int main(int argc, char *argv[])
{
	__strNS   = cwtStrNS();
	__utilsNS = cwtUtilsNS();
	__utNS    = cwtUniTypeNS();

	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	CWT_BEGIN_TESTS(76);

	test_00();
	test_01();
	test_bool();

	CWT_END_TESTS;
}


