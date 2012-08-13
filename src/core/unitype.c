/*
 * unitype.c
 *
 *  Created on: 08.08.2012
 *      Author: wladt
 */


#include <cwt/unitype.h>
#include <cwt/str.h>
#include <cwt/txtcodec.h>

static CwtUniType* __create       (void);
static void        __free         (CwtUniType *ut);
/*static void        __cleanup      (CwtUniType*);*/
static BOOL        __setType      (CwtUniType *ut, CwtTypeEnum type, const void *copy, size_t sz);
static BOOL        __setFromString(CwtUniType *ut, CwtTypeEnum type, const CWT_CHAR *s);
static inline BOOL __setBool      (CwtUniType *ut, BOOL b)       { return __setType(ut, CwtType_BOOL, &b, 0); }
static inline BOOL __setChar      (CwtUniType *ut, char ch)      { return __setType(ut, CwtType_CHAR, &ch, 0); }
static inline BOOL __setCwtChar   (CwtUniType *ut, CWT_CHAR ch)  { return __setType(ut, CwtType_CWT_CHAR, &ch, 0); }
static inline BOOL __setSByte     (CwtUniType *ut, SBYTE n)      { return __setType(ut, CwtType_SBYTE, &n, 0); }
static inline BOOL __setByte      (CwtUniType *ut, BYTE n)       { return __setType(ut, CwtType_SBYTE, &n, 0); }
static inline BOOL __setShort     (CwtUniType *ut, SHORT n)      { return __setType(ut, CwtType_BYTE, &n, 0); }
static inline BOOL __setUShort    (CwtUniType *ut, USHORT n)     { return __setType(ut, CwtType_SHORT, &n, 0); }
static inline BOOL __setInt       (CwtUniType *ut, int n)        { return __setType(ut, CwtType_USHORT, &n, 0); }
static inline BOOL __setUInt      (CwtUniType *ut, UINT n)       { return __setType(ut, CwtType_UINT, &n, 0); }
static inline BOOL __setLong      (CwtUniType *ut, LONG n)       { return __setType(ut, CwtType_LONG, &n, 0); }
static inline BOOL __setULong     (CwtUniType *ut, ULONG n)      { return __setType(ut, CwtType_ULONG, &n, 0); }
static inline BOOL __setLongLong  (CwtUniType *ut, LONGLONG n)   { return __setType(ut, CwtType_LONGLONG, &n, 0); }
static inline BOOL __setULongLong (CwtUniType *ut, ULONGLONG n)  { return __setType(ut, CwtType_ULONGLONG, &n, 0); }
static inline BOOL __setFloat     (CwtUniType *ut, float n)      { return __setType(ut, CwtType_FLOAT, &n, 0); }
static inline BOOL __setDouble    (CwtUniType *ut, double n)     { return __setType(ut, CwtType_DOUBLE, &n, 0); }
static inline BOOL __setText      (CwtUniType *ut, const char *p, size_t length)     { return __setType(ut, CwtType_TEXT, p, length+1); }
static inline BOOL __setCwtText   (CwtUniType *ut, const CWT_CHAR *p, size_t length) { return __setType(ut, CwtType_CWT_TEXT, p, (length + 1)*sizeof(CWT_CHAR)); }
static inline BOOL __setBlob      (CwtUniType *ut, const void *p, size_t sz)         { return __setType(ut, CwtType_BLOB, p, sz); }
static inline BOOL __setTime      (CwtUniType *ut, const CWT_TIME *p, size_t sz)     { return __setType(ut, CwtType_TIME, p, sz ? sz : sizeof(CWT_TIME)); }
static inline BOOL __setDate      (CwtUniType *ut, const CWT_TIME *p, size_t sz)     { return __setType(ut, CwtType_DATE, p, sz ? sz : sizeof(CWT_TIME)); }
static inline BOOL __setDateTime  (CwtUniType *ut, const CWT_TIME *p, size_t sz)     { return __setType(ut, CwtType_DATETIME, p, sz ? sz : sizeof(CWT_TIME)); }

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
static inline CWT_CHAR* __toCwtText    (CwtUniType *ut) { CWT_ASSERT(ut); return (CWT_CHAR*)ut->value.ptr; }
static inline void*     __toBlob       (CwtUniType *ut) { CWT_ASSERT(ut); return ut->value.ptr; }
static inline CWT_TIME* __toTime       (CwtUniType *ut) { CWT_ASSERT(ut); return (CWT_TIME*)ut->value.ptr; }
static inline CWT_TIME* __toDate       (CwtUniType *ut) { CWT_ASSERT(ut); return (CWT_TIME*)ut->value.ptr; }
static inline CWT_TIME* __toDateTime   (CwtUniType *ut) { CWT_ASSERT(ut); return (CWT_TIME*)ut->value.ptr; }

/* Helper function */
static void __setBuffer(CwtUniType *ut, CwtTypeEnum cwtType, const void *p, size_t sz);


static CwtUniTypeNS __cwtUniTypeNS = {
	  __create
	, __free
	/*, __cleanup*/
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
	, __setCwtText
	, __setBlob
	, __setTime
	, __setDate
	, __setDateTime

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
	, __toCwtText
	, __toBlob
	, __toTime
	, __toDate
	, __toDateTime
};

CwtStrNS *__strNS = NULL;


DLL_API_EXPORT CwtUniTypeNS* cwtUniTypeNS(void)
{
	if( !__strNS ) {
		__strNS = cwtStrNS();
	}

	return &__cwtUniTypeNS;
}



static CwtUniType* __create(void)
{
	CwtUniType *ut;

	ut = CWT_MALLOC(CwtUniType);
	__strNS->bzero(ut, sizeof(CwtUniType));
	ut->is_null = TRUE;
	ut->type = CwtType_UNKNOWN;

	return ut;
}


static void __free(CwtUniType *ut)
{
	if( ut ) {
		if( ut->length && ut->value.ptr ) {
			CWT_FREE(ut->value.ptr);
			ut->value.ptr = NULL;
			ut->sz = 0;
			ut->length = 0;
		}

		CWT_FREE(ut);
	}
}


/*
static void __cleanup(CwtUniType *ut)
{
	if( ut ) {
		if( ut->length && ut->value.ptr ) {
			CWT_FREE(ut->value.ptr);
			ut->value.ptr = NULL;
			ut->length = 0;
		}
		ut->is_null = TRUE;
		ut->type = CwtType_UNKNOWN;
	}
}
*/

/**
 * @fn CwtUniTypeNS::setType(CwtUniType *ut, CwtTypeEnum type, void *copy)
 *
 * @brief Sets any value of specified type excluding BLOB, TEXT and CWt_TEXT
 * (use @ref CwtUniTypeNS::setBlob(),  @ref CwtUniTypeNS::setText()
 * or @ref CwtUniTypeNS::setCwtText() instead)
 *
 * @param ut unitype value
 * @param type value type
 * @param copy value
 */
static BOOL __setType(CwtUniType *ut, CwtTypeEnum type, const void *copy, size_t sz)
{
	BOOL ok = FALSE;

	CWT_ASSERT(ut);
	CWT_ASSERT(copy);


	if( type != ut->type ) {
		if( ut->length && ut->value.ptr ) {
			CWT_FREE(ut->value.ptr);
			ut->value.ptr = NULL;
			ut->sz = 0;
			ut->length = 0;
		}
	}

	ut->is_null = FALSE;
	ut->type = CwtType_CHAR;

	switch(type) {
	case CwtType_BOOL:
		ut->value.bool_val = *((BOOL*)copy);
		break;
	case CwtType_CHAR:
		ut->value.char_val = *((char*)copy);
		break;
	case CwtType_CWT_CHAR:
		ut->value.cwt_char_val = *((CWT_CHAR*)copy);
		break;
	case CwtType_SBYTE:
		ut->value.sbyte_val = *((SBYTE*)copy);
		break;
	case CwtType_BYTE:
		ut->value.byte_val = *((BYTE*)copy);
		break;
	case CwtType_SHORT:
		ut->value.short_val = *((SHORT*)copy);
		break;
	case CwtType_USHORT:
		ut->value.ushort_val = *((USHORT*)copy);
		break;
	case CwtType_INT:
		ut->value.int_val = *((INT*)copy);
		break;
	case CwtType_UINT:
		ut->value.uint_val = *((UINT*)copy);
		break;
	case CwtType_LONG:
		ut->value.long_val = *((LONG*)copy);
		break;
	case CwtType_ULONG:
		ut->value.ulong_val = *((ULONG*)copy);
		break;
	case CwtType_LONGLONG:
		ut->value.llong_val = *((LONGLONG*)copy);
		break;
	case CwtType_ULONGLONG:
		ut->value.ullong_val = *((ULONGLONG*)copy);
		break;
	case CwtType_FLOAT:
		ut->value.float_val = *((float*)copy);
		break;
	case CwtType_DOUBLE:
		ut->value.double_val = *((double*)copy);
		break;
	case CwtType_TEXT:
		__setBuffer(ut, CwtType_TEXT, copy, sz);
		break;
	case CwtType_CWT_TEXT:
		__setBuffer(ut, CwtType_CWT_TEXT, copy, sz);
		break;
	case CwtType_BLOB:
		__setBuffer(ut, CwtType_BLOB, copy, sz);
		break;
	case CwtType_TIME:
		__setBuffer(ut, CwtType_TIME, copy, sz);
		break;
	case CwtType_DATE:
		__setBuffer(ut, CwtType_DATE, copy, sz);
		break;
	case CwtType_DATETIME:
		__setBuffer(ut, CwtType_DATETIME, copy, sz);
		break;
	case CwtType_UNKNOWN:
	default:
		return FALSE;
	}

	return TRUE;
}


static BOOL __setFromString(CwtUniType *ut, CwtTypeEnum type, const CWT_CHAR *s)
{
	CwtStrNS *strNS = cwtStrNS();
	BOOL ok = FALSE;

	CWT_ASSERT(ut);

	if( ut->type == CwtType_BLOB )
		return FALSE;

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
			char *latin1;
			latin1 = cwtTextCodecNS()->toLatin1(s);
			__setText(ut, latin1, strlen(latin1));
			CWT_FREE(latin1);
		}
		return TRUE;

	case CwtType_CWT_TEXT:
		__setCwtText(ut, s, strNS->strlen(s));
		return TRUE;

	case CwtType_TIME: {
			CWT_TIME tm;
			strNS->toTime(s, &tm, NULL, &ok);
			if( ok ) {
				__setTime(ut, &tm, sizeof(CWT_TIME));
				return TRUE;
			}
		}
		break;

	case CwtType_DATE: {
			CWT_TIME tm;
			strNS->toDate(s, &tm, NULL, &ok);
			if( ok ) {
				__setDate(ut, &tm, sizeof(CWT_TIME));
				return TRUE;
			}
		}
		break;

	case CwtType_DATETIME: {
			CWT_TIME tm;
			strNS->toDateTime(s, &tm, NULL, &ok);
			if( ok ) {
				__setDateTime(ut, &tm, sizeof(CWT_TIME));
				return TRUE;
			}
		}
		break;

	case CwtType_UNKNOWN:
	default:
		break;
	}

	return FALSE;
}


/* Helper function.
 * For CwtType_TEXT and CwtType_CWT_TEXT sz is number of chars.
 * For other types sz is number of bytes.
 * */
static void __setBuffer(CwtUniType *ut, CwtTypeEnum cwtType, const void *p, size_t sz)
{
	CWT_ASSERT(ut);

	if( !p || !sz) {
		ut->is_null = TRUE;
	} else {
		if( ut->sz < sz ) {
			CWT_FREE(ut->value.ptr);
		}

		if( !ut->sz ) {
			if( CwtType_CWT_TEXT == cwtType ) {
				ut->value.ptr = CWT_MALLOCA(CWT_CHAR, sz);
				ut->length = sz * sizeof(CWT_CHAR);
			} else {
				ut->value.ptr = CWT_MALLOCA(char, sz);
			}

			ut->sz = sz;
		} else if( ut->sz < sz ) {

		}

		sz = CWT_MIN(sz, ut->sz);

		cwtStrNS()->memcpy(ut->value.ptr, p, sz);
	}

	ut->type = cwtType;
}
