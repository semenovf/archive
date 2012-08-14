/*
 * unitype.c
 *
 *  Created on: 08.08.2012
 *      Author: wladt
 */


#include <cwt/unitype.h>
#include <cwt/str.h>
#include <cwt/txtcodec.h>


static CwtUniType*      __create       (void);
static void             __free         (CwtUniType *ut);
static inline CwtTypeEnum __type       (CwtUniType *ut) { CWT_ASSERT(ut); return ut->type; }
/*static BOOL             __canCast      (CwtUniType *ut, CwtTypeEnum type);*/
static BOOL             __setType      (CwtUniType *ut, CwtTypeEnum type, const void *copy, size_t sz);
static BOOL             __setFromString(CwtUniType *ut, CwtTypeEnum type, const CWT_CHAR *s);
static inline BOOL      __setBool      (CwtUniType *ut, BOOL b)       { return __setType(ut, CwtType_BOOL, &b, 0); }
static inline BOOL      __setChar      (CwtUniType *ut, char ch)      { return __setType(ut, CwtType_CHAR, &ch, 0); }
static inline BOOL      __setCwtChar   (CwtUniType *ut, CWT_CHAR ch)  { return __setType(ut, CwtType_CWT_CHAR, &ch, 0); }
static inline BOOL      __setSByte     (CwtUniType *ut, SBYTE n)      { return __setType(ut, CwtType_SBYTE, &n, 0); }
static inline BOOL      __setByte      (CwtUniType *ut, BYTE n)       { return __setType(ut, CwtType_BYTE, &n, 0); }
static inline BOOL      __setShort     (CwtUniType *ut, SHORT n)      { return __setType(ut, CwtType_SHORT, &n, 0); }
static inline BOOL      __setUShort    (CwtUniType *ut, USHORT n)     { return __setType(ut, CwtType_USHORT, &n, 0); }
static inline BOOL      __setInt       (CwtUniType *ut, int n)        { return __setType(ut, CwtType_INT, &n, 0); }
static inline BOOL      __setUInt      (CwtUniType *ut, UINT n)       { return __setType(ut, CwtType_UINT, &n, 0); }
static inline BOOL      __setLong      (CwtUniType *ut, LONG n)       { return __setType(ut, CwtType_LONG, &n, 0); }
static inline BOOL      __setULong     (CwtUniType *ut, ULONG n)      { return __setType(ut, CwtType_ULONG, &n, 0); }
static inline BOOL      __setLongLong  (CwtUniType *ut, LONGLONG n)   { return __setType(ut, CwtType_LONGLONG, &n, 0); }
static inline BOOL      __setULongLong (CwtUniType *ut, ULONGLONG n)  { return __setType(ut, CwtType_ULONGLONG, &n, 0); }
static inline BOOL      __setFloat     (CwtUniType *ut, float n)      { return __setType(ut, CwtType_FLOAT, &n, 0); }
static inline BOOL      __setDouble    (CwtUniType *ut, double n)     { return __setType(ut, CwtType_DOUBLE, &n, 0); }
static inline BOOL      __setText      (CwtUniType *ut, const char *p, size_t length)     { return __setType(ut, CwtType_TEXT, p, length+1); }
static inline BOOL      __setCwtText   (CwtUniType *ut, const CWT_CHAR *p, size_t length) { return __setType(ut, CwtType_CWT_TEXT, p, (length + 1)*sizeof(CWT_CHAR)); }
static inline BOOL      __setBlob      (CwtUniType *ut, const void *p, size_t sz)         { return __setType(ut, CwtType_BLOB, p, sz); }
static inline BOOL      __setTime      (CwtUniType *ut, const CWT_TIME *p, size_t sz)     { return __setType(ut, CwtType_TIME, p, sz ? sz : sizeof(CWT_TIME)); }
static inline BOOL      __setDate      (CwtUniType *ut, const CWT_TIME *p, size_t sz)     { return __setType(ut, CwtType_DATE, p, sz ? sz : sizeof(CWT_TIME)); }
static inline BOOL      __setDateTime  (CwtUniType *ut, const CWT_TIME *p, size_t sz)     { return __setType(ut, CwtType_DATETIME, p, sz ? sz : sizeof(CWT_TIME)); }

static BOOL             __toBool       (CwtUniType *ut, BOOL *ok);
static char             __toChar       (CwtUniType *ut, BOOL *ok);
static inline CWT_CHAR  __toCwtChar    (CwtUniType *ut, BOOL *ok);
static inline SBYTE     __toSByte      (CwtUniType *ut, BOOL *ok);
static inline BYTE      __toByte       (CwtUniType *ut, BOOL *ok) { CWT_ASSERT(ut); return ut->value.llong_val; }
static inline SHORT     __toShort      (CwtUniType *ut, BOOL *ok) { CWT_ASSERT(ut); return ut->value.llong_val; }
static inline USHORT    __toUShort     (CwtUniType *ut, BOOL *ok) { CWT_ASSERT(ut); return ut->value.llong_val; }
static inline INT       __toInt        (CwtUniType *ut, BOOL *ok) { CWT_ASSERT(ut); return ut->value.llong_val; }
static inline UINT      __toUInt       (CwtUniType *ut, BOOL *ok) { CWT_ASSERT(ut); return ut->value.llong_val; }
static inline LONG      __toLong       (CwtUniType *ut, BOOL *ok) { CWT_ASSERT(ut); return ut->value.llong_val; }
static inline ULONG     __toULong      (CwtUniType *ut, BOOL *ok) { CWT_ASSERT(ut); return ut->value.llong_val; }
static inline LONGLONG  __toLongLong   (CwtUniType *ut, BOOL *ok) { CWT_ASSERT(ut); return ut->value.llong_val; }
static inline ULONGLONG __toULongLong  (CwtUniType *ut, BOOL *ok) { CWT_ASSERT(ut); return ut->value.llong_val; }
static inline float     __toFloat      (CwtUniType *ut, BOOL *ok) { CWT_ASSERT(ut); return ut->value.double_val; }
static inline double    __toDouble     (CwtUniType *ut, BOOL *ok) { CWT_ASSERT(ut); return ut->value.double_val; }
static inline char*     __toText       (CwtUniType *ut, BOOL *ok) { CWT_ASSERT(ut); return (char*)ut->value.ptr; }
static inline CWT_CHAR* __toCwtText    (CwtUniType *ut, BOOL *ok) { CWT_ASSERT(ut); return (CWT_CHAR*)ut->value.ptr; }
static inline void*     __toBlob       (CwtUniType *ut, BOOL *ok) { CWT_ASSERT(ut); return ut->value.ptr; }
static inline CWT_TIME* __toTime       (CwtUniType *ut, BOOL *ok) { CWT_ASSERT(ut); return (CWT_TIME*)ut->value.ptr; }
static inline CWT_TIME* __toDate       (CwtUniType *ut, BOOL *ok) { CWT_ASSERT(ut); return (CWT_TIME*)ut->value.ptr; }
static inline CWT_TIME* __toDateTime   (CwtUniType *ut, BOOL *ok) { CWT_ASSERT(ut); return (CWT_TIME*)ut->value.ptr; }

/* Helper function */
static void __setBuffer(CwtUniType *ut, CwtTypeEnum cwtType, const void *p, size_t sz);


static CwtUniTypeNS __cwtUniTypeNS = {
	  __create
	, __free
	, __type
/*	, __canCast*/
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


static BOOL __canCast(CwtUniType *ut, CwtTypeEnum type)
{

/* CwtType_UNKNOWN  */
/* CwtType_BOOL     */
/* CwtType_CHAR     */
/* CwtType_CWT_CHAR */
/* CwtType_SBYTE    */
/* CwtType_UCHAR    */
/* CwtType_SHORT    */
/* CwtType_USHORT   */
/* CwtType_INT      */
/* CwtType_UINT     */
/* CwtType_LONG     */
/* CwtType_ULONG    */
/* CwtType_LONGLONG */
/* CwtType_ULONGLONG*/
/* CwtType_FLOAT    */
/* CwtType_DOUBLE   */
/* CwtType_TEXT     */
/* CwtType_CWT_TEXT */
/* CwtType_BLOB     */
/* CwtType_TIME     */
/* CwtType_DATE     */
/* CwtType_DATETIME */
	return FALSE;
}

/**
 * @fn CwtUniTypeNS::set(CwtUniType *ut, CwtTypeEnum type, const void *copy, size_t sz)
 *
 * @brief Sets any value of specified type
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
		ut->value.llong_val = *((BOOL*)copy);
		break;
	case CwtType_CHAR:
		ut->value.llong_val = *((char*)copy);
		break;
	case CwtType_CWT_CHAR:
		ut->value.llong_val = *((CWT_CHAR*)copy);
		break;
	case CwtType_SBYTE:
		ut->value.llong_val = *((SBYTE*)copy);
		break;
	case CwtType_BYTE:
		ut->value.llong_val = *((BYTE*)copy);
		break;
	case CwtType_SHORT:
		ut->value.llong_val = *((SHORT*)copy);
		break;
	case CwtType_USHORT:
		ut->value.llong_val = *((USHORT*)copy);
		break;
	case CwtType_INT:
		ut->value.llong_val = *((INT*)copy);
		break;
	case CwtType_UINT:
		ut->value.llong_val = *((UINT*)copy);
		break;
	case CwtType_LONG:
		ut->value.llong_val = *((LONG*)copy);
		break;
	case CwtType_ULONG:
		ut->value.llong_val = *((ULONG*)copy);
		break;
	case CwtType_LONGLONG:
		ut->value.llong_val = *((LONGLONG*)copy);
		break;
	case CwtType_ULONGLONG:
		ut->value.llong_val = *((ULONGLONG*)copy);
		break;
	case CwtType_FLOAT:
		ut->value.double_val = *((float*)copy);
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
	BOOL ok = FALSE;

	CWT_ASSERT(ut);

	if( ut->type == CwtType_BLOB )
		return FALSE;

	if( !s )
		return TRUE;

	if( !__strNS->len(s) )
		return TRUE;

	switch(type) {
	case CwtType_BOOL:
		if( __strNS->eqcase(_T("true"), s) || __strNS->eq(_T("1"), s) ) {
			__setBool(ut, TRUE);
			return TRUE;
		}
		if( __strNS->eqcase(_T("false"), s) || __strNS->eq(_T("0"), s) ) {
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
			SBYTE n = __strNS->toSBYTE(s, 10, &ok);
			if( ok ) {
				__setSByte(ut, n);
				return TRUE;
			}
		}
		break;

	case CwtType_BYTE: {
			BYTE n = __strNS->toBYTE(s, 10, &ok);
			if( ok ) {
				__setByte(ut, n);
				return TRUE;
			}
		}
		break;

	case CwtType_SHORT: {
			SHORT n = __strNS->toSHORT(s, 10, &ok);
			if( ok ) {
				__setShort(ut, n);
				return TRUE;
			}
		}
		break;

	case CwtType_USHORT: {
			USHORT n = __strNS->toUSHORT(s, 10, &ok);
			if( ok ) {
				__setUShort(ut, n);
				return TRUE;
			}
		}
		break;

	case CwtType_INT: {
			INT n = __strNS->toINT(s, 10, &ok);
			if( ok ) {
				__setInt(ut, n);
				return TRUE;
			}
		}
		break;

	case CwtType_UINT: {
			UINT n = __strNS->toUINT(s, 10, &ok);
			if( ok ) {
				__setUInt(ut, n);
				return TRUE;
			}
		}
		break;

	case CwtType_LONG: {
			UINT n = __strNS->toLONG(s, 10, &ok);
			if( ok ) {
				__setLong(ut, n);
				return TRUE;
			}
		}
		break;

	case CwtType_ULONG: {
			ULONG n = __strNS->toULONG(s, 10, &ok);
			if( ok ) {
				__setULong(ut, n);
				return TRUE;
			}
		}
		break;

	case CwtType_LONGLONG: {
			LONGLONG n = __strNS->toLONGLONG(s, 10, &ok);
			if( ok ) {
				__setLongLong(ut, n);
				return TRUE;
			}
		}
		break;

	case CwtType_ULONGLONG: {
			ULONGLONG n = __strNS->toULONGLONG(s, 10, &ok);
			if( ok ) {
				__setULongLong(ut, n);
				return TRUE;
			}
		}
		break;

	case CwtType_FLOAT: {
			float n = __strNS->toFloat(s, &ok);
			if( ok ) {
				__setFloat(ut, n);
				return TRUE;
			}
		}
		break;

	case CwtType_DOUBLE: {
			double n = __strNS->toDouble(s, &ok);
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
		__setCwtText(ut, s, __strNS->len(s));
		return TRUE;


	case CwtType_TIME: {
			CWT_TIME tm;
			__strNS->toTime(s, &tm, NULL, &ok);
			if( ok ) {
				__setTime(ut, &tm, sizeof(CWT_TIME));
				return TRUE;
			}
		}
		break;

	case CwtType_DATE: {
			CWT_TIME tm;
			__strNS->toDate(s, &tm, NULL, &ok);
			if( ok ) {
				__setDate(ut, &tm, sizeof(CWT_TIME));
				return TRUE;
			}
		}
		break;

	case CwtType_DATETIME: {
			CWT_TIME tm;
			__strNS->toDateTime(s, &tm, NULL, &ok);
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


/**
 * @fn CwtUniTypeNS::toBool(CwtUniType *ut)
 *
 * @brief Returns unitype as @c BOOL.
 *
 * Returns the unitype as a @c BOOL if the unitype has type BOOL.
 * Returns @c TRUE if the unitype has scalar type and the value is non-zero,
 * or if the unitype has type char* or CWT_CHAR* and its content is not empty, "0", or "false";
 * otherwise returns @c FALSE.
 *
 * @param ut unitype value.
 * @return unitype as a BOOL.
 */
static inline BOOL __toBool(CwtUniType *ut, BOOL *ok)
{
	CWT_ASSERT(ut);

	if( ok )
		*ok = TRUE;

	if( CwtType_BOOL == ut->type )
		return (BOOL)ut->value.llong_val;

	if( CWT_TYPE_IS_INTEGER(ut->type) )
		return ut->value.llong_val ? TRUE : FALSE;

	if( CWT_TYPE_IS_FLOAT(ut->type) )
		return ut->value.double_val != 0.0f ? TRUE : FALSE;

	if( CwtType_TEXT == ut->type ) {
		if( ut->length > 0 ) {
			char *s = (char*)ut->value.ptr;
			CWT_ASSERT(ut->value.ptr);

			if( ut->length == 1 && s[0] == '0' )
				return FALSE;

			if( ut->length == 5 && strncasecmp("false", s, 5) == 0 )
				return FALSE;

			return TRUE;
		}
	} else if( CwtType_CWT_TEXT == ut->type ) {
		if( ut->length > 0 ) {
			CWT_CHAR *s = (CWT_CHAR*)ut->value.ptr;
			CWT_ASSERT(ut->value.ptr);

			if( ut->length == 1 && s[0] == _T('0') )
				return FALSE;

			if( ut->length == 5 && __strNS->ncasecmp(_T("false"), s, 5) == 0 )
				return FALSE;

			return TRUE;
		}
	} else {
		if( ok )
			*ok = FALSE;
	}

	return FALSE;
}



/**
 * @fn CwtUniTypeNS::toChar(CwtUniType *ut)
 *
 * @brief Returns the unitype as a @c char.
 *
 * Returns the unitype as a @c char if the unitype has type @c char, @c SBYTE or @c BYTE.
 * Returns the unitype as a @c char if the unitype is scalar in unsigned range from 0 to 255;
 * otherwise returns null-char.
 *
 * @param ut
 * @return
 */
static char __toChar(CwtUniType *ut, BOOL *ok)
{
	CWT_ASSERT(ut);

	if( ok )
		*ok = TRUE;

	if( CwtType_CHAR == ut->type || CwtType_SBYTE == ut->type || CwtType_BYTE == ut->type )
		return (char)ut->value.llong_val;

	if( CWT_TYPE_IS_INTEGER(ut->type)
			&& ut->value.llong_val >= CWT_CHAR_MIN
			&& ut->value.llong_val <= CWT_CHAR_MAX )
		return (char)ut->value.llong_val;

	if( ok )
		*ok = FALSE;

	return (char)0;
}



static inline CWT_CHAR __toCwtChar(CwtUniType *ut, BOOL *ok)
{
	CWT_ASSERT(ut);

	if( ok )
		*ok = TRUE;

	if( CwtType_CWT_CHAR == ut->type )
		return (CWT_CHAR)ut->value.llong_val;

	if( CWT_TYPE_IS_INTEGER(ut->type)
			&& ut->value.llong_val >= CWT_CWT_CHAR_MIN
			&& ut->value.llong_val <= CWT_CWT_CHAR_MAX )
		return (CWT_CHAR)ut->value.llong_val;

	if( ok )
		*ok = FALSE;

	return (CWT_CHAR)0;
}

static inline SBYTE __toSByte(CwtUniType *ut, BOOL *ok)
{
	CWT_ASSERT(ut);

	if( ok )
		*ok = TRUE;

	if( CwtType_SBYTE == ut->type )
		return (SBYTE)ut->value.llong_val;

	if( CWT_TYPE_IS_INTEGER(ut->type)
			&& ut->value.llong_val >= CWT_SBYTE_MIN
			&& ut->value.llong_val <= CWT_SBYTE_MAX )
		return (SBYTE)ut->value.llong_val;

	if( CwtType_TEXT == ut->type && ut->length > 0 ) {
		BOOL okk;
		SBYTE n = __strNS->toSBYTE((const char*)ut->value.ptr, 0, &okk);
		if( okk )
			return n;
	} else if( CwtType_CWT_TEXT == ut->type && ut->length > 0) {
		BOOL okk;
		SBYTE n = __strNS->toSBYTE((const CWT_CHAR *)ut->value.ptr, 0, &okk);
		if( okk )
			return n;
	}

	if( ok )
		*ok = FALSE;

	return (SBYTE)0;
}
/* Helper function.
 * For CwtType_TEXT and CwtType_CWT_TEXT sz is number of chars.
 * For other types sz is number of bytes.
 *
 * If sz > 0 buffer will be (re)allocated (if previously did not allocated or buffer size is insufficient)
 * */
static void __setBuffer(CwtUniType *ut, CwtTypeEnum cwtType, const void *p, size_t sz)
{
	size_t nbytes = sz;
	size_t length = sz;

	CWT_ASSERT(ut);

	if( sz > 0 ) {
		if( CwtType_CWT_TEXT == cwtType ) {
			nbytes = (sz + 1) * sizeof(CWT_CHAR);
		} else if( CwtType_TEXT == cwtType ) {
			nbytes = (sz + 1) * sizeof(char);
		}
	}

	if( ut->sz < sz ) {
		if( ut->sz > 0 && ut->value.ptr ) {
			CWT_FREE(ut->value.ptr);
			ut->value.ptr = NULL;
			ut->sz = 0;
			ut->length = 0;
		}
	}

	if( nbytes > 0 ) {
		if( !ut->sz ) {
			ut->value.ptr = CWT_MALLOCA(char, nbytes);
			ut->sz = nbytes;
		}

		cwtStrNS()->memcpy(ut->value.ptr, p, sz);
		ut->length = length;
	}

	ut->type = cwtType;
}
