/*
 * unitype.c
 *
 *  Created on: 08.08.2012
 *      Author: wladt
 */


#include <cwt/unitype.h>
#include <cwt/str.h>

static void __init         (CwtUniType *ut)              { CWT_ASSERT(ut); ut->is_null = TRUE;  ut->type = CwtType_UNKNOWN; }
static void __setType      (CwtUniType *ut, CwtTypeEnum type, void *copy);
static void __setBool      (CwtUniType *ut, BOOL b)      { CWT_ASSERT(ut); ut->is_null = FALSE; ut->type = CwtType_BOOL; ut->value.bool_val = b; }
static void __setChar      (CwtUniType *ut, char ch)     { CWT_ASSERT(ut); ut->is_null = FALSE; ut->type = CwtType_CHAR; ut->value.char_val = ch; }
static void __setCwtChar   (CwtUniType *ut, CWT_CHAR ch) { CWT_ASSERT(ut); ut->is_null = FALSE; ut->type = CwtType_CWT_CHAR; ut->value.cwt_char_val = ch; }
static void __setSByte     (CwtUniType *ut, SBYTE n)     { CWT_ASSERT(ut); ut->is_null = FALSE; ut->type = CwtType_SBYTE; ut->value.sbyte_val = n; }
static void __setByte      (CwtUniType *ut, BYTE n)      { CWT_ASSERT(ut); ut->is_null = FALSE; ut->type = CwtType_BYTE; ut->value.byte_val = n; }
static void __setShort     (CwtUniType *ut, SHORT n)     { CWT_ASSERT(ut); ut->is_null = FALSE; ut->type = CwtType_SHORT; ut->value.short_val = n; }
static void __setUShort    (CwtUniType *ut, USHORT n)    { CWT_ASSERT(ut); ut->is_null = FALSE; ut->type = CwtType_USHORT; ut->value.ushort_val = n; }
static void __setInt       (CwtUniType *ut, int n)       { CWT_ASSERT(ut); ut->is_null = FALSE; ut->type = CwtType_INT; ut->value.int_val = n; }
static void __setUInt      (CwtUniType *ut, UINT n)      { CWT_ASSERT(ut); ut->is_null = FALSE; ut->type = CwtType_UINT; ut->value.uint_val = n; }
static void __setLong      (CwtUniType *ut, LONG n)      { CWT_ASSERT(ut); ut->is_null = FALSE; ut->type = CwtType_LONG; ut->value.long_val = n; }
static void __setULong     (CwtUniType *ut, ULONG n)     { CWT_ASSERT(ut); ut->is_null = FALSE; ut->type = CwtType_ULONG; ut->value.ulong_val = n; }
static void __setLongLong  (CwtUniType *ut, LONGLONG n)  { CWT_ASSERT(ut); ut->is_null = FALSE; ut->type = CwtType_LONGLONG; ut->value.llong_val = n; }
static void __setULongLong (CwtUniType *ut, ULONGLONG n) { CWT_ASSERT(ut); ut->is_null = FALSE; ut->type = CwtType_ULONGLONG; ut->value.ullong_val = n; }
static void __setFloat     (CwtUniType *ut, float n)     { CWT_ASSERT(ut); ut->is_null = FALSE; ut->type = CwtType_FLOAT; ut->value.float_val = n; }
static void __setDouble    (CwtUniType *ut, double n)    { CWT_ASSERT(ut); ut->is_null = FALSE; ut->type = CwtType_DOUBLE; ut->value.double_val = n; }
static void __setText      (CwtUniType *ut, char *s)     { CWT_ASSERT(ut); ut->is_null = FALSE; ut->type = CwtType_TEXT; ut->value.ptr = s; ut->length = (ULONGLONG)strlen(s); }
static void __setBlob      (CwtUniType *ut, void *p, size_t sz) { CWT_ASSERT(ut); ut->is_null = FALSE; ut->type = CwtType_BLOB; ut->value.ptr = p; ut->length = sz; }
static void __setTime      (CwtUniType *ut, CWT_TIME *tm) { CWT_ASSERT(ut); ut->is_null = FALSE; ut->type = CwtType_TIME; ut->value.ptr = tm; }
static void __setDate      (CwtUniType *ut, CWT_TIME *tm) { CWT_ASSERT(ut); ut->is_null = FALSE; ut->type = CwtType_DATE; ut->value.ptr = tm; }
static void __setDateTime  (CwtUniType *ut, CWT_TIME *tm) { CWT_ASSERT(ut); ut->is_null = FALSE; ut->type = CwtType_DATETIME; ut->value.ptr = tm; }
static void __setCwtText   (CwtUniType *ut, CWT_CHAR *s)  { CWT_ASSERT(ut); ut->is_null = FALSE; ut->type = CwtType_CWT_STRING; ut->value.ptr = s; ut->length = cwtStrNS()->strlen(s) * sizeof(CWT_CHAR); }

static BOOL      __toBool       (CwtUniType *ut) { CWT_ASSERT(ut); return ut->value.bool_val; }
static char      __toChar       (CwtUniType *ut) { CWT_ASSERT(ut); return ut->value.char_val; }
static CWT_CHAR  __toCwtChar    (CwtUniType *ut) { CWT_ASSERT(ut); return ut->value.cwt_char_val; }
static SBYTE     __toSByte      (CwtUniType *ut) { CWT_ASSERT(ut); return ut->value.sbyte_val; }
static BYTE      __toByte       (CwtUniType *ut) { CWT_ASSERT(ut); return ut->value.byte_val; }
static SHORT     __toShort      (CwtUniType *ut) { CWT_ASSERT(ut); return ut->value.short_val; }
static USHORT    __toUShort     (CwtUniType *ut) { CWT_ASSERT(ut); return ut->value.ushort_val; }
static INT       __toInt        (CwtUniType *ut) { CWT_ASSERT(ut); return ut->value.int_val; }
static UINT      __toUInt       (CwtUniType *ut) { CWT_ASSERT(ut); return ut->value.uint_val; }
static LONG      __toLong       (CwtUniType *ut) { CWT_ASSERT(ut); return ut->value.long_val; }
static ULONG     __toULong      (CwtUniType *ut) { CWT_ASSERT(ut); return ut->value.ulong_val; }
static LONGLONG  __toLongLong   (CwtUniType *ut) { CWT_ASSERT(ut); return ut->value.llong_val; }
static ULONGLONG __toULongLong  (CwtUniType *ut) { CWT_ASSERT(ut); return ut->value.ullong_val; }
static float     __toFloat      (CwtUniType *ut) { CWT_ASSERT(ut); return ut->value.float_val; }
static double    __toDouble     (CwtUniType *ut) { CWT_ASSERT(ut); return ut->value.float_val; }
static char*     __toText       (CwtUniType *ut) { CWT_ASSERT(ut); return (char*)ut->value.ptr; }
static void*     __toBlob       (CwtUniType *ut) { CWT_ASSERT(ut); return ut->value.ptr; }
static CWT_TIME* __toTime       (CwtUniType *ut) { CWT_ASSERT(ut); return (CWT_TIME*)ut->value.ptr; }
static CWT_TIME* __toDate       (CwtUniType *ut) { CWT_ASSERT(ut); return (CWT_TIME*)ut->value.ptr; }
static CWT_TIME* __toDateTime   (CwtUniType *ut) { CWT_ASSERT(ut); return (CWT_TIME*)ut->value.ptr; }
static CWT_CHAR* __toCwtText    (CwtUniType *ut) { CWT_ASSERT(ut); return (CWT_CHAR*)ut->value.ptr; }


static CwtUniTypeNS __cwtUniTypeNS = {
	  __init
	, __setType
	, __setBool
	, __setChar
	, __setCwtChar
	, __setSByte
	, __setByte
	, __setShort
	, __setUShort
	, __setInt
	, __setUInt
	, __setLong
	, __setULong
	, __setLongLong
	, __setULongLong
	, __setFloat
	, __setDouble
	, __setText
	, __setBlob
	, __setTime
	, __setDate
	, __setDateTime
	, __setCwtText

	, __toBool
	, __toChar
	, __toCwtChar
	, __toSByte
	, __toByte
	, __toShort
	, __toUShort
	, __toInt
	, __toUInt
	, __toLong
	, __toULong
	, __toLongLong
	, __toULongLong
	, __toFloat
	, __toDouble
	, __toText
	, __toBlob
	, __toTime
	, __toDate
	, __toDateTime
	, __toCwtText
};

DLL_API_EXPORT CwtUniTypeNS* cwtUniTypeNS(void)
{
	return &__cwtUniTypeNS;
}


/**
 * @fn CwtUniTypeNS::setType(CwtUniType *ut, CwtTypeEnum type, void *copy)
 *
 * @brief Sets any value of specified type excluding BLOB (use @ref CwtUniTypeNS::setBlob() instead)
 *
 * @param ut unitype value
 * @param type value type
 * @param copy value
 */
static void __setType(CwtUniType *ut, CwtTypeEnum type, void *copy)
{
	CWT_ASSERT(ut);
	CWT_ASSERT(ut->type != CwtType_BLOB);

	if( !copy ) {
		__init(ut);
		return;
	}

	switch(type) {
	case CwtType_BOOL:
		__setBool(ut, *((BOOL*)copy));
		break;
	case CwtType_CHAR:
		__setChar(ut, *((CHAR*)copy));
		break;
	case CwtType_CWT_CHAR:
		__setCwtChar(ut, *((CWT_CHAR*)copy));
		break;
	case CwtType_SBYTE:
		__setSByte(ut, *((SBYTE*)copy));
		break;
	case CwtType_BYTE:
		__setByte(ut, *((BYTE*)copy));
		break;
	case CwtType_SHORT:
		__setShort(ut, *((SHORT*)copy));
		break;
	case CwtType_USHORT:
		__setUShort(ut, *((USHORT*)copy));
		break;
	case CwtType_INT:
		__setInt(ut, *((INT*)copy));
		break;
	case CwtType_UINT:
		__setUInt(ut, *((UINT*)copy));
		break;
	case CwtType_LONG:
		__setLong(ut, *((LONG*)copy));
		break;
	case CwtType_ULONG:
		__setULong(ut, *((ULONG*)copy));
		break;
	case CwtType_LONGLONG:
		__setLongLong(ut, *((LONGLONG*)copy));
		break;
	case CwtType_ULONGLONG:
		__setULongLong(ut, *((ULONGLONG*)copy));
		break;
	case CwtType_FLOAT:
		__setFloat(ut, *((float*)copy));
		break;
	case CwtType_DOUBLE:
		__setDouble(ut, *((double*)copy));
		break;
	case CwtType_TEXT:
		__setText(ut, (char*)copy);
		break;
	case CwtType_TIME:
		__setTime(ut, (CWT_TIME*)copy);
		break;
	case CwtType_DATE:
		__setDate(ut, (CWT_TIME*)copy);
		break;
	case CwtType_DATETIME:
		__setDateTime(ut, (CWT_TIME*)copy);
		break;
	case CwtType_CWT_STRING:
		__setCwtText(ut, (CWT_CHAR*)copy);
		break;
	case CwtType_UNKNOWN:
	default:
		__init(ut);
		break;
	}
}
