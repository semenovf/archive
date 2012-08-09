/*
 * unitype.c
 *
 *  Created on: 08.08.2012
 *      Author: wladt
 */


#include <cwt/unitype.h>
#include <cwt/str.h>
#include <cwt/txtcodec.h>

static inline void __init         (CwtUniType *ut)              { CWT_ASSERT(ut); ut->is_null = TRUE;  ut->type = CwtType_UNKNOWN; }
static void __setType      (CwtUniType *ut, CwtTypeEnum type, void *copy);
static BOOL __setFromString(CwtUniType *ut, CwtTypeEnum type, const CWT_CHAR *s);
static inline void __setBool      (CwtUniType *ut, BOOL b)      { CWT_ASSERT(ut); ut->is_null = FALSE; ut->type = CwtType_BOOL; ut->value.bool_val = b; }
static inline void __setChar      (CwtUniType *ut, char ch)     { CWT_ASSERT(ut); ut->is_null = FALSE; ut->type = CwtType_CHAR; ut->value.char_val = ch; }
static inline void __setCwtChar   (CwtUniType *ut, CWT_CHAR ch) { CWT_ASSERT(ut); ut->is_null = FALSE; ut->type = CwtType_CWT_CHAR; ut->value.cwt_char_val = ch; }
static inline void __setSByte     (CwtUniType *ut, SBYTE n)     { CWT_ASSERT(ut); ut->is_null = FALSE; ut->type = CwtType_SBYTE; ut->value.sbyte_val = n; }
static inline void __setByte      (CwtUniType *ut, BYTE n)      { CWT_ASSERT(ut); ut->is_null = FALSE; ut->type = CwtType_BYTE; ut->value.byte_val = n; }
static inline void __setShort     (CwtUniType *ut, SHORT n)     { CWT_ASSERT(ut); ut->is_null = FALSE; ut->type = CwtType_SHORT; ut->value.short_val = n; }
static inline void __setUShort    (CwtUniType *ut, USHORT n)    { CWT_ASSERT(ut); ut->is_null = FALSE; ut->type = CwtType_USHORT; ut->value.ushort_val = n; }
static inline void __setInt       (CwtUniType *ut, int n)       { CWT_ASSERT(ut); ut->is_null = FALSE; ut->type = CwtType_INT; ut->value.int_val = n; }
static inline void __setUInt      (CwtUniType *ut, UINT n)      { CWT_ASSERT(ut); ut->is_null = FALSE; ut->type = CwtType_UINT; ut->value.uint_val = n; }
static inline void __setLong      (CwtUniType *ut, LONG n)      { CWT_ASSERT(ut); ut->is_null = FALSE; ut->type = CwtType_LONG; ut->value.long_val = n; }
static inline void __setULong     (CwtUniType *ut, ULONG n)     { CWT_ASSERT(ut); ut->is_null = FALSE; ut->type = CwtType_ULONG; ut->value.ulong_val = n; }
static inline void __setLongLong  (CwtUniType *ut, LONGLONG n)  { CWT_ASSERT(ut); ut->is_null = FALSE; ut->type = CwtType_LONGLONG; ut->value.llong_val = n; }
static inline void __setULongLong (CwtUniType *ut, ULONGLONG n) { CWT_ASSERT(ut); ut->is_null = FALSE; ut->type = CwtType_ULONGLONG; ut->value.ullong_val = n; }
static inline void __setFloat     (CwtUniType *ut, float n)     { CWT_ASSERT(ut); ut->is_null = FALSE; ut->type = CwtType_FLOAT; ut->value.float_val = n; }
static inline void __setDouble    (CwtUniType *ut, double n)    { CWT_ASSERT(ut); ut->is_null = FALSE; ut->type = CwtType_DOUBLE; ut->value.double_val = n; }
static inline void __setText      (CwtUniType *ut, char *s)     { CWT_ASSERT(ut); ut->is_null = FALSE; ut->type = CwtType_TEXT; ut->value.ptr = s; ut->length = (ULONGLONG)strlen(s); }
static inline void __setBlob      (CwtUniType *ut, void *p, size_t sz) { CWT_ASSERT(ut); ut->is_null = FALSE; ut->type = CwtType_BLOB; ut->value.ptr = p; ut->length = sz; }
static inline void __setTime      (CwtUniType *ut, CWT_TIME *tm) { CWT_ASSERT(ut); ut->is_null = FALSE; ut->type = CwtType_TIME; ut->value.ptr = tm; }
static inline void __setDate      (CwtUniType *ut, CWT_TIME *tm) { CWT_ASSERT(ut); ut->is_null = FALSE; ut->type = CwtType_DATE; ut->value.ptr = tm; }
static inline void __setDateTime  (CwtUniType *ut, CWT_TIME *tm) { CWT_ASSERT(ut); ut->is_null = FALSE; ut->type = CwtType_DATETIME; ut->value.ptr = tm; }
static inline void __setCwtText   (CwtUniType *ut, CWT_CHAR *s)  { CWT_ASSERT(ut); ut->is_null = FALSE; ut->type = CwtType_CWT_STRING; ut->value.ptr = s; ut->length = cwtStrNS()->strlen(s) * sizeof(CWT_CHAR); }

static inline BOOL      __toBool       (CwtUniType *ut) { CWT_ASSERT(ut); return ut->value.bool_val; }
static inline char      __toChar       (CwtUniType *ut) { CWT_ASSERT(ut); return ut->value.char_val; }
static inline CWT_CHAR  __toCwtChar    (CwtUniType *ut) { CWT_ASSERT(ut); return ut->value.cwt_char_val; }
static inline SBYTE     __toSByte      (CwtUniType *ut) { CWT_ASSERT(ut); return ut->value.sbyte_val; }
static inline BYTE      __toByte       (CwtUniType *ut) { CWT_ASSERT(ut); return ut->value.byte_val; }
static inline SHORT     __toShort      (CwtUniType *ut) { CWT_ASSERT(ut); return ut->value.short_val; }
static inline USHORT    __toUShort     (CwtUniType *ut) { CWT_ASSERT(ut); return ut->value.ushort_val; }
static inline INT       __toInt        (CwtUniType *ut) { CWT_ASSERT(ut); return ut->value.int_val; }
static inline UINT      __toUInt       (CwtUniType *ut) { CWT_ASSERT(ut); return ut->value.uint_val; }
static inline LONG      __toLong       (CwtUniType *ut) { CWT_ASSERT(ut); return ut->value.long_val; }
static inline ULONG     __toULong      (CwtUniType *ut) { CWT_ASSERT(ut); return ut->value.ulong_val; }
static inline LONGLONG  __toLongLong   (CwtUniType *ut) { CWT_ASSERT(ut); return ut->value.llong_val; }
static inline ULONGLONG __toULongLong  (CwtUniType *ut) { CWT_ASSERT(ut); return ut->value.ullong_val; }
static inline float     __toFloat      (CwtUniType *ut) { CWT_ASSERT(ut); return ut->value.float_val; }
static inline double    __toDouble     (CwtUniType *ut) { CWT_ASSERT(ut); return ut->value.float_val; }
static inline char*     __toText       (CwtUniType *ut) { CWT_ASSERT(ut); return (char*)ut->value.ptr; }
static inline void*     __toBlob       (CwtUniType *ut) { CWT_ASSERT(ut); return ut->value.ptr; }
static inline CWT_TIME* __toTime       (CwtUniType *ut) { CWT_ASSERT(ut); return (CWT_TIME*)ut->value.ptr; }
static inline CWT_TIME* __toDate       (CwtUniType *ut) { CWT_ASSERT(ut); return (CWT_TIME*)ut->value.ptr; }
static inline CWT_TIME* __toDateTime   (CwtUniType *ut) { CWT_ASSERT(ut); return (CWT_TIME*)ut->value.ptr; }
static inline CWT_CHAR* __toCwtText    (CwtUniType *ut) { CWT_ASSERT(ut); return (CWT_CHAR*)ut->value.ptr; }


static CwtUniTypeNS __cwtUniTypeNS = {
	  __init
	, __setType
	, __setFromString
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


static BOOL __setFromString(CwtUniType *ut, CwtTypeEnum type, const CWT_CHAR *s)
{
	CwtStrNS *strNS = cwtStrNS();
	BOOL ok = TRUE;

	CWT_ASSERT(ut);
	CWT_ASSERT(ut->type != CwtType_BLOB);

	__init(ut);

	if( !s )
		return TRUE;

	if( !strNS->strlen(s) )
		return TRUE;

	switch(type) {
	case CwtType_BOOL:
		if( strNS->strieq(_T("true"), s) || strNS->strieq(_T("yes"), s) || strNS->streq(_T("1"), s) ) {
			__setBool(ut, TRUE);
			return TRUE;
		}
		if( strNS->strieq(_T("false"), s) || strNS->strieq(_T("no"), s) || strNS->streq(_T("0"), s) ) {
			__setBool(ut, FALSE);
			return TRUE;
		}
		break;

	case CwtType_CHAR: {
			char *latin1;
			CWT_CHAR buf[2] = {_T('\0'), _T('\0') };

			buf[0] = s[0];
			latin1 = cwtTextCodecNS()->toLatin1(buf);
			__setChar(ut, latin1[0]);
			CWT_FREE(latin1);
		}
		break;

	case CwtType_CWT_CHAR:
		__setCwtChar(ut, s[0]);
		return TRUE;

	case CwtType_SBYTE: {
			SBYTE n = strNS->toSBYTE(s, 10, &ok);
			if( ok ) {
				__setSByte(ut, n);
				return TRUE;
			}
		}
		break;

	case CwtType_BYTE: {
			BYTE n = strNS->toBYTE(s, 10, &ok);
			if( ok ) {
				__setByte(ut, n);
				return TRUE;
			}
		}
		break;

	case CwtType_SHORT: {
			SHORT n = strNS->toSHORT(s, 10, &ok);
			if( ok ) {
				__setShort(ut, n);
				return TRUE;
			}
		}
		break;

	case CwtType_USHORT: {
			USHORT n = strNS->toUSHORT(s, 10, &ok);
			if( ok ) {
				__setUShort(ut, n);
				return TRUE;
			}
		}
		break;

	case CwtType_INT: {
			INT n = strNS->toINT(s, 10, &ok);
			if( ok ) {
				__setInt(ut, n);
				return TRUE;
			}
		}
		break;

	case CwtType_UINT: {
			UINT n = strNS->toUINT(s, 10, &ok);
			if( ok ) {
				__setUInt(ut, n);
				return TRUE;
			}
		}
		break;

	case CwtType_LONG: {
			UINT n = strNS->toLONG(s, 10, &ok);
			if( ok ) {
				__setLong(ut, n);
				return TRUE;
			}
		}
		break;

	case CwtType_ULONG: {
			ULONG n = strNS->toULONG(s, 10, &ok);
			if( ok ) {
				__setULong(ut, n);
				return TRUE;
			}
		}
		break;

	case CwtType_LONGLONG: {
			LONGLONG n = strNS->toLONGLONG(s, 10, &ok);
			if( ok ) {
				__setLongLong(ut, n);
				return TRUE;
			}
		}
		break;

	case CwtType_ULONGLONG: {
			ULONGLONG n = strNS->toULONGLONG(s, 10, &ok);
			if( ok ) {
				__setULongLong(ut, n);
				return TRUE;
			}
		}
		break;

	case CwtType_FLOAT: {
			float n = strNS->toFloat(s, &ok);
			if( ok ) {
				__setFloat(ut, n);
				return TRUE;
			}
		}
		break;

	case CwtType_DOUBLE: {
			double n = strNS->toDouble(s, &ok);
			if( ok ) {
				__setDouble(ut, n);
				return TRUE;
			}
		}
		break;

	case CwtType_TEXT: {
			char *utf8;
			utf8 = cwtTextCodecNS()->toUtf8(s, strNS->strlen(s));
			__setText(ut, utf8);
			CWT_FREE(utf8);
		}
		break;

	case CwtType_TIME: {
			CWT_TIME tm;
			strNS->toTime(s, &tm, NULL, &ok);
			if( ok ) {
				__setTime(ut, &tm);
				return TRUE;
			}
		}
		break;

	case CwtType_DATE: {
			CWT_TIME tm;
			strNS->toDate(s, &tm, NULL, &ok);
			if( ok ) {
				__setDate(ut, &tm);
				return TRUE;
			}
		}
		break;

	case CwtType_DATETIME: {
			CWT_TIME tm;
			strNS->toDateTime(s, &tm, NULL, &ok);
			if( ok ) {
				__setDateTime(ut, &tm);
				return TRUE;
			}
		}
		break;

	case CwtType_CWT_STRING:
		__setCwtText(ut, (CWT_CHAR*)s);
		break;

	case CwtType_UNKNOWN:
	default:
		__init(ut);
		break;
	}

	return FALSE;
}
