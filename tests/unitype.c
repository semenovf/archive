/*
 * unitype.c
 *
 *  Created on: 14.08.2012
 *      Author: wladt
 */


#include <cwt/test.h>
#include <string.h>
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


void test_size_of(void)
{
	CWT_TEST_OK(cwt_sizeof(CwtType_BOOL)      == CWT_SIZEOF(CwtType_BOOL));
    CWT_TEST_OK(cwt_sizeof(CwtType_CHAR)      == CWT_SIZEOF(CwtType_CHAR));
    CWT_TEST_OK(cwt_sizeof(CwtType_SBYTE)     == CWT_SIZEOF(CwtType_SBYTE));
    CWT_TEST_OK(cwt_sizeof(CwtType_INT8)      == CWT_SIZEOF(CwtType_INT8));
    CWT_TEST_OK(cwt_sizeof(CwtType_BYTE)      == CWT_SIZEOF(CwtType_BYTE));
    CWT_TEST_OK(cwt_sizeof(CwtType_UINT8)     == CWT_SIZEOF(CwtType_UINT8));
    CWT_TEST_OK(cwt_sizeof(CwtType_SHORT)     == CWT_SIZEOF(CwtType_SHORT));
    CWT_TEST_OK(cwt_sizeof(CwtType_INT16)     == CWT_SIZEOF(CwtType_INT16));
    CWT_TEST_OK(cwt_sizeof(CwtType_USHORT)    == CWT_SIZEOF(CwtType_USHORT));
    CWT_TEST_OK(cwt_sizeof(CwtType_UINT16)    == CWT_SIZEOF(CwtType_UINT16));
    CWT_TEST_OK(cwt_sizeof(CwtType_INT)       == CWT_SIZEOF(CwtType_INT));
    CWT_TEST_OK(cwt_sizeof(CwtType_INT32)     == CWT_SIZEOF(CwtType_INT32));
    CWT_TEST_OK(cwt_sizeof(CwtType_UINT)      == CWT_SIZEOF(CwtType_UINT));
    CWT_TEST_OK(cwt_sizeof(CwtType_UINT32)    == CWT_SIZEOF(CwtType_UINT32));
    CWT_TEST_OK(cwt_sizeof(CwtType_LONG)      == CWT_SIZEOF(CwtType_LONG));
    CWT_TEST_OK(cwt_sizeof(CwtType_ULONG)     == CWT_SIZEOF(CwtType_ULONG));
    CWT_TEST_OK(cwt_sizeof(CwtType_LONGLONG)  == CWT_SIZEOF(CwtType_LONGLONG));
    CWT_TEST_OK(cwt_sizeof(CwtType_INT64)     == CWT_SIZEOF(CwtType_INT64));
    CWT_TEST_OK(cwt_sizeof(CwtType_ULONGLONG) == CWT_SIZEOF(CwtType_ULONGLONG));
    CWT_TEST_OK(cwt_sizeof(CwtType_UINT64)    == CWT_SIZEOF(CwtType_UINT64));
    CWT_TEST_OK(cwt_sizeof(CwtType_FLOAT)     == CWT_SIZEOF(CwtType_FLOAT));
    CWT_TEST_OK(cwt_sizeof(CwtType_DOUBLE)    == CWT_SIZEOF(CwtType_DOUBLE));
    CWT_TEST_OK(cwt_sizeof(CwtType_TEXT)      == CWT_SIZEOF(CwtType_TEXT));
    CWT_TEST_OK(cwt_sizeof(CwtType_BLOB)      == CWT_SIZEOF(CwtType_BLOB));
    CWT_TEST_OK(cwt_sizeof(CwtType_TIME)      == CWT_SIZEOF(CwtType_TIME));
    CWT_TEST_OK(cwt_sizeof(CwtType_DATE)      == CWT_SIZEOF(CwtType_DATE));
    CWT_TEST_OK(cwt_sizeof(CwtType_DATETIME)  == CWT_SIZEOF(CwtType_DATETIME));

}

void test_00(void)
{
	BOOL ok;

	CwtUniType *bool_val    = __utNS->create();
	CwtUniType *char_val    = __utNS->create();
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
	CwtUniType *blob_val    = __utNS->create();
	CwtUniType *time_val    = __utNS->create();
	CwtUniType *date_val    = __utNS->create();
	CwtUniType *datetime_val= __utNS->create();

	CWT_TEST_OK((void*)&bool_val->value == (void*)&bool_val->value.llong_val);
	CWT_TEST_OK((void*)&bool_val->value == (void*)&bool_val->value.double_val);
	CWT_TEST_OK((void*)&bool_val->value == (void*)&bool_val->value.ptr);

	__utNS->setBOOL(bool_val, TRUE);
	__utNS->toBOOL(bool_val, &ok);
	CWT_TEST_OK2(ok, _T("bool_val == TRUE"));

	__utNS->setBOOL(bool_val, FALSE);
	__utNS->toBOOL(bool_val, &ok);
	CWT_TEST_OK2(ok, _T("bool_val == FALSE"));

	__utNS->setCHAR(char_val, _T('W'));
	__utNS->toCHAR(char_val, &ok);
	CWT_TEST_OK2(ok, _T("char_val == 'W'"));

	__utNS->setSBYTE(sbyte_val, CWT_SBYTE_MIN);
	__utNS->toSBYTE(sbyte_val, &ok);
	CWT_TEST_OK2(ok, _T("sbyte_val == CWT_SBYTE_MIN"));

	__utNS->setSBYTE(sbyte_val, CWT_SBYTE_MAX);
	__utNS->toSBYTE(sbyte_val, &ok);
	CWT_TEST_OK2(ok, _T("sbyte_val == CWT_SBYTE_MAX"));

	__utNS->setBYTE(byte_val, CWT_BYTE_MAX);
	__utNS->toBYTE(byte_val, &ok);
	CWT_TEST_OK2(ok, _T("byte_val == CWT_BYTE_MAX"));

	__utNS->setSHORT(short_val, CWT_SHORT_MIN);
	__utNS->toSHORT(short_val, &ok);
	CWT_TEST_OK2(ok, _T("short_val == CWT_SHORT_MIN"));

	__utNS->setSHORT(short_val, CWT_SHORT_MAX);
	__utNS->toSHORT(short_val, &ok);
	CWT_TEST_OK2(ok, _T("short_val == CWT_SHORT_MAX"));

	__utNS->setUSHORT(ushort_val, CWT_USHORT_MAX);
	__utNS->toUSHORT(ushort_val, &ok);
	CWT_TEST_OK2(ok, _T("ushort_val == CWT_USHORT_MAX"));

	__utNS->setINT(int_val, CWT_INT_MIN);
	__utNS->toINT(int_val, &ok);
	CWT_TEST_OK2(ok, _T("int_val == CWT_INT_MIN"));

	__utNS->setINT(int_val, CWT_INT_MAX);
	__utNS->toINT(int_val, &ok);
	CWT_TEST_OK2(ok, _T("int_val == CWT_INT_MAX"));

	__utNS->setUINT(uint_val, CWT_UINT_MAX);
	__utNS->toUINT(uint_val, &ok);
	CWT_TEST_OK2(ok, _T("uint_val == CWT_UINT_MAX"));

	__utNS->setLONG(long_val, CWT_LONG_MIN);
	__utNS->toLONG(long_val, &ok);
	CWT_TEST_OK2(ok, _T("long_val  == CWT_LONG_MIN"));

	__utNS->setLONG(long_val, CWT_LONG_MAX);
	__utNS->toLONG(long_val, &ok);
	CWT_TEST_OK2(ok, _T("long_val == CWT_LONG_MAX"));

	__utNS->setULONG(ulong_val, CWT_ULONG_MAX);
	__utNS->toULONG(ulong_val, &ok);
	CWT_TEST_OK2(ok, _T("ulong_val == CWT_ULONG_MAX"));

	__utNS->setLONGLONG(llong_val, CWT_LONGLONG_MIN);
	__utNS->toLONGLONG(llong_val, &ok);
	CWT_TEST_OK2(ok, _T("llong_val  == CWT_LONGLONG_MIN"));

	__utNS->setLONGLONG(llong_val, CWT_LONGLONG_MAX);
	__utNS->toLONGLONG(llong_val, &ok);
	CWT_TEST_OK2(ok, _T("llong_val == CWT_LONGLONG_MAX"));

	__utNS->setULONGLONG(ullong_val, CWT_ULONGLONG_MAX);
	__utNS->toULONGLONG(ullong_val, &ok);
	CWT_TEST_OK2(ok, _T("ullong_val == CWT_ULONGLONG_MAX"));

	__utNS->setFLOAT(float_val, CWT_FLOAT_MAX);
	__utNS->toFLOAT(float_val, &ok);
	CWT_TEST_OK2(ok, _T("float_val == CWT_FLOAT_MAX"));

	__utNS->setDOUBLE(double_val, CWT_DOUBLE_MAX);
	__utNS->toDOUBLE(double_val, &ok);
	CWT_TEST_OK2(ok, _T("double_val == CWT_DOUBLE_MAX"));

	{
		const CWT_CHAR *text_en = _T("The quick brown fox jumps over the lazy dog");
		const CWT_CHAR *text_ru = _T("Съешь ещё этих мягких французских булок, да выпей чаю");
		CWT_CHAR *text;

		__utNS->setTEXT(text_val, text_en, __strNS->strlen(text_en));
		text = __utNS->toTEXT(text_val, &ok);
		CWT_TEST_OK(ok && __strNS->strCmp(text_en, text) == 0);
		CWT_FREE(text);

		__utNS->setTEXT(text_val, text_ru, __strNS->strlen(text_ru));
		text = __utNS->toTEXT(text_val, &ok);
		CWT_TEST_OK(ok && __strNS->strCmp(text_ru, text) == 0);
		CWT_FREE(text);
	}

	{
		const char *blob;
		size_t sz;
		__utNS->setBLOB(blob_val, loremipsum, strlen(loremipsum));
		blob = (const char*)__utNS->toBLOB(blob_val, &sz);
		CWT_TEST_OK(sz == strlen(loremipsum));
		CWT_TEST_OK(strncmp(loremipsum, blob, strlen(loremipsum)) == 0);
	}

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

	__utNS->free(bool_val);
	__utNS->free(char_val);
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
	__utNS->free(blob_val);
	__utNS->free(time_val);
	__utNS->free(date_val);
	__utNS->free(datetime_val);
}


void test_01(void)
{
	BOOL ok;
	CwtUniType *uval    = __utNS->create();

	__utNS->setBOOL(uval, TRUE);
	CWT_TEST_OK(__utNS->toBOOL(uval, &ok) == TRUE && ok);

	__utNS->setBOOL(uval, FALSE);
	CWT_TEST_OK(__utNS->toBOOL(uval, &ok) == FALSE && ok);

	__utNS->setCHAR(uval, _T('W'));
	CWT_TEST_OK(__utNS->toCHAR(uval, &ok) == _T('W') && ok);

	__utNS->setSBYTE(uval, CWT_SBYTE_MIN);
	CWT_TEST_OK(__utNS->toSBYTE(uval, &ok) == CWT_SBYTE_MIN && ok);

	__utNS->setSBYTE(uval, CWT_SBYTE_MAX);
	CWT_TEST_OK(__utNS->toSBYTE(uval, &ok) == CWT_SBYTE_MAX && ok);

	__utNS->setBYTE(uval, CWT_BYTE_MAX);
	CWT_TEST_OK(__utNS->toBYTE(uval, &ok) == CWT_BYTE_MAX && ok);

	__utNS->setSHORT(uval, CWT_SHORT_MIN);
	CWT_TEST_OK(__utNS->toSHORT(uval, &ok) == CWT_SHORT_MIN && ok);

	__utNS->setSHORT(uval, CWT_SHORT_MAX);
	CWT_TEST_OK(__utNS->toSHORT(uval, &ok) == CWT_SHORT_MAX && ok);

	__utNS->setUSHORT(uval, CWT_USHORT_MAX);
	CWT_TEST_OK(__utNS->toUSHORT(uval, &ok) == CWT_USHORT_MAX && ok);

	__utNS->setINT(uval, CWT_INT_MIN);
	CWT_TEST_OK(__utNS->toINT(uval, &ok) == CWT_INT_MIN && ok);

	__utNS->setINT(uval, CWT_INT_MAX);
	CWT_TEST_OK(__utNS->toINT(uval, &ok) == CWT_INT_MAX && ok);

	__utNS->setUINT(uval, CWT_UINT_MAX);
	CWT_TEST_OK(__utNS->toUINT(uval, &ok) == CWT_UINT_MAX && ok);

	__utNS->setLONG(uval, CWT_LONG_MIN);
	CWT_TEST_OK(__utNS->toLONG(uval, &ok) == CWT_LONG_MIN && ok);

	__utNS->setLONG(uval, CWT_LONG_MAX);
	CWT_TEST_OK(__utNS->toLONG(uval, &ok) == CWT_LONG_MAX && ok);

	__utNS->setULONG(uval, CWT_ULONG_MAX);
	CWT_TEST_OK(__utNS->toULONG(uval, &ok) == CWT_ULONG_MAX && ok);

	__utNS->setLONGLONG(uval, CWT_LONGLONG_MIN);
	CWT_TEST_OK(__utNS->toLONGLONG(uval, &ok) == CWT_LONGLONG_MIN && ok);

	__utNS->setLONGLONG(uval, CWT_LONGLONG_MAX);
	CWT_TEST_OK(__utNS->toLONGLONG(uval, &ok) == CWT_LONGLONG_MAX && ok);

	__utNS->setULONGLONG(uval, CWT_ULONGLONG_MAX);
	CWT_TEST_OK(__utNS->toULONGLONG(uval, &ok) == CWT_ULONGLONG_MAX && ok);

	__utNS->setFLOAT(uval, CWT_FLOAT_MAX);
	CWT_TEST_OK(__utNS->toFLOAT(uval, &ok) == CWT_FLOAT_MAX && ok);

	__utNS->setDOUBLE(uval, CWT_DOUBLE_MAX);
	CWT_TEST_OK(__utNS->toDOUBLE(uval, &ok) == CWT_DOUBLE_MAX && ok);

	{
		const CWT_CHAR *text_en = _T("The quick brown fox jumps over the lazy dog");
		const CWT_CHAR *text_ru = _T("Съешь ещё этих мягких французских булок, да выпей чаю");
		CWT_CHAR *text;

		__utNS->setTEXT(uval, text_en, __strNS->strlen(text_en));
		text = __utNS->toTEXT(uval, &ok);
		CWT_TEST_OK(ok && __strNS->strCmp(text_en, text) == 0);
		CWT_FREE(text);

		__utNS->setTEXT(uval, text_ru, __strNS->strlen(text_ru));
		text = __utNS->toTEXT(uval, &ok);
		CWT_TEST_OK(ok && __strNS->strCmp(text_ru, text) == 0);
		CWT_FREE(text);
	}

	{
		const char *blob;
		size_t sz;
		__utNS->setBLOB(uval, loremipsum, strlen(loremipsum));
		blob = (const char*)__utNS->toBLOB(uval, &sz);
		CWT_TEST_OK(sz == strlen(loremipsum));
		CWT_TEST_OK(strncmp(loremipsum, blob, strlen(loremipsum)) == 0);
	}

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
	}

	__utNS->free(uval);
}


void test_bool(void)
{
	BOOL ok;
	CwtUniType *uval = __utNS->create();

	__utNS->setBOOL(uval, TRUE);
	CWT_TEST_OK2(__utNS->toBOOL(uval, &ok) == TRUE && ok, _T("'BOOL' equality to TRUE"));
	__utNS->setBOOL(uval, FALSE);
	CWT_TEST_OK2(__utNS->toBOOL(uval, &ok) == FALSE && ok, _T("'BOOL' equality to FALSE"));

	__utNS->setCHAR(uval, _T('W'));
	CWT_TEST_OK2(__utNS->toBOOL(uval, &ok) == TRUE && ok, _T("'CWT_CHAR' equality to TRUE"));
	__utNS->setCHAR(uval, _T('\0'));
	CWT_TEST_OK2(__utNS->toBOOL(uval, &ok) == FALSE && ok, _T("'CWT_CHAR' equality to FALSE"));

	__utNS->setSBYTE(uval, CWT_SBYTE_MIN);
	CWT_TEST_OK2(__utNS->toBOOL(uval, &ok) == TRUE && ok, _T("'SBYTE' equality to TRUE"));
	__utNS->setSBYTE(uval, CWT_SBYTE_MAX);
	CWT_TEST_OK2(__utNS->toBOOL(uval, &ok) == TRUE && ok, _T("'SBYTE' equality to TRUE"));
	__utNS->setSBYTE(uval, 0);
	CWT_TEST_OK2(__utNS->toBOOL(uval, &ok) == FALSE && ok, _T("'SBYTE' equality to FALSE"));

	__utNS->setBYTE(uval, CWT_BYTE_MAX);
	CWT_TEST_OK2(__utNS->toBOOL(uval, &ok) == TRUE && ok, _T("'BYTE' equality to TRUE"));
	__utNS->setBYTE(uval, 0);
	CWT_TEST_OK2(__utNS->toBOOL(uval, &ok) == FALSE && ok, _T("'BYTE' equality to FALSE"));

	__utNS->setSHORT(uval, CWT_SHORT_MIN);
	CWT_TEST_OK2(__utNS->toBOOL(uval, &ok) == TRUE && ok, _T("'SHORT' equality to TRUE"));
	__utNS->setSHORT(uval, CWT_SHORT_MAX);
	CWT_TEST_OK2(__utNS->toBOOL(uval, &ok) == TRUE && ok, _T("'SHORT' equality to TRUE"));
	__utNS->setSHORT(uval, 0);
	CWT_TEST_OK2(__utNS->toBOOL(uval, &ok) == FALSE && ok, _T("'SHORT' equality to FALSE"));

	__utNS->setUSHORT(uval, CWT_USHORT_MAX);
	CWT_TEST_OK2(__utNS->toBOOL(uval, &ok) == TRUE && ok, _T("'USHORT' equality to TRUE"));
	__utNS->setUSHORT(uval, 0);
	CWT_TEST_OK2(__utNS->toBOOL(uval, &ok) == FALSE && ok, _T("'USHORT' equality to FALSE"));

	__utNS->setINT(uval, CWT_INT_MIN);
	CWT_TEST_OK2(__utNS->toBOOL(uval, &ok) == TRUE && ok, _T("'INT' equality to TRUE"));
	__utNS->setINT(uval, CWT_INT_MAX);
	CWT_TEST_OK2(__utNS->toBOOL(uval, &ok) == TRUE && ok, _T("'INT' equality to TRUE"));
	__utNS->setINT(uval, 0);
	CWT_TEST_OK2(__utNS->toBOOL(uval, &ok) == FALSE && ok, _T("'INT' equality to FALSE"));

	__utNS->setUINT(uval, CWT_UINT_MAX);
	CWT_TEST_OK2(__utNS->toBOOL(uval, &ok) == TRUE && ok, _T("'UINT' equality to TRUE"));
	__utNS->setUINT(uval, 0);
	CWT_TEST_OK2(__utNS->toBOOL(uval, &ok) == FALSE && ok, _T("'UINT' equality to FALSE"));

	__utNS->setLONG(uval, CWT_LONG_MIN);
	CWT_TEST_OK2(__utNS->toBOOL(uval, &ok) == TRUE && ok, _T("'LONG' equality to TRUE"));
	__utNS->setLONG(uval, CWT_LONG_MAX);
	CWT_TEST_OK2(__utNS->toBOOL(uval, &ok) == TRUE && ok, _T("'LONG' equality to TRUE"));
	__utNS->setLONG(uval, 0L);
	CWT_TEST_OK2(__utNS->toBOOL(uval, &ok) == FALSE && ok, _T("'LONG' equality to FALSE"));

	__utNS->setULONG(uval, CWT_ULONG_MAX);
	CWT_TEST_OK2(__utNS->toBOOL(uval, &ok) == TRUE && ok, _T("'ULONG' equality to TRUE"));
	__utNS->setULONG(uval, 0UL);
	CWT_TEST_OK2(__utNS->toBOOL(uval, &ok) == FALSE && ok, _T("'ULONG' equality to FALSE"));

	__utNS->setLONGLONG(uval, CWT_LONGLONG_MIN);
	CWT_TEST_OK2(__utNS->toBOOL(uval, &ok) == TRUE && ok, _T("'LONGLONG' equality to TRUE"));
	__utNS->setLONGLONG(uval, CWT_LONGLONG_MAX);
	CWT_TEST_OK2(__utNS->toBOOL(uval, &ok) == TRUE && ok, _T("'LONGLONG' equality to TRUE"));
	__utNS->setLONGLONG(uval, 0LL);
	CWT_TEST_OK2(__utNS->toBOOL(uval, &ok) == FALSE && ok, _T("'LONGLONG' equality to FALSE"));

	__utNS->setULONGLONG(uval, CWT_ULONGLONG_MAX);
	CWT_TEST_OK2(__utNS->toBOOL(uval, &ok) == TRUE && ok, _T("'ULONGLONG' equality to TRUE"));
	__utNS->setULONGLONG(uval, 0ULL);
	CWT_TEST_OK2(__utNS->toBOOL(uval, &ok) == FALSE && ok, _T("'ULONGLONG' equality to FALSE"));

	__utNS->setFLOAT(uval, CWT_FLOAT_MAX);
	CWT_TEST_OK2(__utNS->toBOOL(uval, &ok) == TRUE && ok, _T("'FLOAT' equality to TRUE"));

	__utNS->setFLOAT(uval, 0.0f);
	CWT_TEST_OK2(__utNS->toBOOL(uval, &ok) == FALSE && ok, _T("'FLOAT' equality to FALSE"));

	__utNS->setDOUBLE(uval, CWT_DOUBLE_MAX);
	CWT_TEST_OK2(__utNS->toBOOL(uval, &ok) == TRUE && ok, _T("'DOUBLE' equality to TRUE"));

	__utNS->setDOUBLE(uval, 0.0f);
	CWT_TEST_OK2(__utNS->toBOOL(uval, &ok) == FALSE && ok, _T("'DOUBLE' equality to FALSE"));

	{
		const CWT_CHAR *text_empty = _T("");
		const CWT_CHAR *text_false = _T("false");
		const CWT_CHAR *text_FALSE = _T("FALSE");
		const CWT_CHAR *text_FaLsE = _T("FaLsE");
		const CWT_CHAR *text_0     = _T("0");
		const CWT_CHAR *text_true  = _T("text");

		__utNS->setTEXT(uval, text_empty, __strNS->strlen(text_empty));
		CWT_TEST_OK2(__utNS->toBOOL(uval, &ok) == FALSE && ok, _T("<empty text> == FALSE"));

		__utNS->setTEXT(uval, text_false, __strNS->strlen(text_false));
		CWT_TEST_OK2(__utNS->toBOOL(uval, &ok) == FALSE && ok, _T("'false' == FALSE"));

		__utNS->setTEXT(uval, text_FALSE, __strNS->strlen(text_FALSE));
		CWT_TEST_OK2(__utNS->toBOOL(uval, &ok) == FALSE && ok, _T("'FALSE' == FALSE"));

		__utNS->setTEXT(uval, text_FaLsE, __strNS->strlen(text_FaLsE));
		CWT_TEST_OK2(__utNS->toBOOL(uval, &ok) == FALSE && ok, _T("'FaLsE' == FALSE"));

		__utNS->setTEXT(uval, text_0, __strNS->strlen(text_0));
		CWT_TEST_OK2(__utNS->toBOOL(uval, &ok) == FALSE && ok, _T("'0' == FALSE"));

		__utNS->setTEXT(uval, text_true, __strNS->strlen(text_true));
		CWT_TEST_OK2(__utNS->toBOOL(uval, &ok) == TRUE && ok, _T("<any other text> is TRUE"));
	}

	__utNS->setBLOB(uval, loremipsum, strlen(loremipsum));
	CWT_TEST_OK2(__utNS->toBOOL(uval, &ok) == FALSE && !ok, _T("BLOB does not cast to BOOL"));

	{
	    CWT_TIME tm;

		__utilsNS->now(&tm);
		__utNS->setTIME(uval, &tm);
		CWT_TEST_OK2(__utNS->toBOOL(uval, &ok) == FALSE && !ok, _T("TIME does not cast to BOOL"));

		__utNS->setDATE(uval, &tm);
		CWT_TEST_OK2(__utNS->toBOOL(uval, &ok) == FALSE && !ok, _T("DATE does not cast to BOOL"));

		__utNS->setDATETIME(uval, &tm);
		CWT_TEST_OK2(__utNS->toBOOL(uval, &ok) == FALSE && !ok, _T("DATETIME does not cast to BOOL"));
	}

	__utNS->free(uval);
}


void test_integer(void)
{
	BOOL ok;
	SHORT    s = 10;
	INT      d = 10;
	LONG     ld = 10L;
	LONGLONG lld = 10LL;
	CwtUniType *short_val  = __utNS->create();
	CwtUniType *int_val  = __utNS->create();

	__utNS->setSHORT(short_val, s);
	CWT_TEST_OK(__utNS->toSHORT(short_val, &ok) == s && ok);

	__utNS->setFromString(short_val, CwtType_SHORT, _T("10"));
	CWT_TEST_OK(__utNS->toINT(short_val, &ok) == 10 && ok);

	__utNS->set(short_val, CwtType_SHORT, &ld, 0);
	CWT_TEST_OK(__utNS->toSHORT(short_val, &ok) == (SHORT)ld && ok);

	__utNS->set(short_val, CwtType_SHORT, &lld, 0);
	CWT_TEST_OK(__utNS->toSHORT(short_val, &ok) == (SHORT)lld && ok);


	__utNS->setINT(int_val, d);
	CWT_TEST_OK(__utNS->toINT(int_val, &ok) == d && ok);

	__utNS->setFromString(int_val, CwtType_INT, _T("10"));
	CWT_TEST_OK(__utNS->toINT(int_val, &ok) == 10 && ok);

	__utNS->set(int_val, CwtType_INT, &ld, 0);
	CWT_TEST_OK(__utNS->toINT(int_val, &ok) == (INT)ld && ok);

	__utNS->set(int_val, CwtType_INT, &lld, 0);
	CWT_TEST_OK(__utNS->toINT(int_val, &ok) == (INT)lld && ok);

	__utNS->free(short_val);
	__utNS->free(int_val);
}

void test_float(void)
{
	BOOL ok;
	double d = 10.0f;
	float  f = 10.0f;

	CwtUniType *float_val  = __utNS->create();
	CwtUniType *double_val = __utNS->create();

	__utNS->setFLOAT(float_val, f);
	CWT_TEST_OK(__utNS->toFLOAT(float_val, &ok) == f && ok);

	__utNS->setFromString(float_val, CwtType_FLOAT, _T("10.0"));
	CWT_TEST_OK(__utNS->toFLOAT(float_val, &ok) == 10.0f && ok);

	__utNS->set(float_val, CwtType_FLOAT, &f, 0);
	CWT_TEST_OK(__utNS->toFLOAT(float_val, &ok) == f && ok);

	/* Failed */
/*
	__utNS->set(float_val, CwtType_FLOAT, (float*)&d, 0);
	CWT_TEST_OK(__utNS->toFLOAT(float_val, &ok) == (float)d && ok);
*/


	__utNS->setDOUBLE(double_val, d);
	CWT_TEST_OK(__utNS->toDOUBLE(double_val, &ok) == f && ok);

	__utNS->setFromString(double_val, CwtType_DOUBLE, _T("10.0"));
	CWT_TEST_OK(__utNS->toDOUBLE(double_val, &ok) == 10.0f && ok);

	__utNS->set(double_val, CwtType_DOUBLE, &d, 0);
	CWT_TEST_OK(__utNS->toDOUBLE(double_val, &ok) == d && ok);

	/* Failed */
/*
	__utNS->set(double_val, CwtType_DOUBLE, (double*)&f, 0);
	CWT_TEST_OK(__utNS->toDOUBLE(double_val, &ok) == (double)f && ok);
*/


	__utNS->free(float_val);
	__utNS->free(double_val);
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


int main(int argc, char *argv[])
{
	__strNS   = cwtStrNS();
	__utilsNS = cwtUtilsNS();
	__utNS    = cwtUniTypeNS();

	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	CWT_BEGIN_TESTS(159);

	test_size_of();
	test_00();
	test_01();
	test_bool();
	test_integer();
	test_float();
	test_text();
	test_blob();
	test_time();

	CWT_END_TESTS;
}


