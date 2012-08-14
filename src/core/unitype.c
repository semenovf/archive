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
static inline BOOL      __setBOOL      (CwtUniType *ut, BOOL b)       { return __setType(ut, CwtType_BOOL, &b, 0); }
static inline BOOL      __setCwtChar   (CwtUniType *ut, CWT_CHAR ch)  { return __setType(ut, CwtType_CWT_CHAR, &ch, 0); }
static inline BOOL      __setSBYTE     (CwtUniType *ut, SBYTE n)      { return __setType(ut, CwtType_SBYTE, &n, 0); }
static inline BOOL      __setBYTE      (CwtUniType *ut, BYTE n)       { return __setType(ut, CwtType_BYTE, &n, 0); }
static inline BOOL      __setSHORT     (CwtUniType *ut, SHORT n)      { return __setType(ut, CwtType_SHORT, &n, 0); }
static inline BOOL      __setUSHORT    (CwtUniType *ut, USHORT n)     { return __setType(ut, CwtType_USHORT, &n, 0); }
static inline BOOL      __setINT       (CwtUniType *ut, int n)        { return __setType(ut, CwtType_INT, &n, 0); }
static inline BOOL      __setUINT      (CwtUniType *ut, UINT n)       { return __setType(ut, CwtType_UINT, &n, 0); }
static inline BOOL      __setLONG      (CwtUniType *ut, LONG n)       { return __setType(ut, CwtType_LONG, &n, 0); }
static inline BOOL      __setULONG     (CwtUniType *ut, ULONG n)      { return __setType(ut, CwtType_ULONG, &n, 0); }
static inline BOOL      __setLONGLONG  (CwtUniType *ut, LONGLONG n)   { return __setType(ut, CwtType_LONGLONG, &n, 0); }
static inline BOOL      __setULONGLONG (CwtUniType *ut, ULONGLONG n)  { return __setType(ut, CwtType_ULONGLONG, &n, 0); }
static inline BOOL      __setFloat     (CwtUniType *ut, float n)      { return __setType(ut, CwtType_FLOAT, &n, 0); }
static inline BOOL      __setDouble    (CwtUniType *ut, double n)     { return __setType(ut, CwtType_DOUBLE, &n, 0); }
static inline BOOL      __setCwtText   (CwtUniType *ut, const CWT_CHAR *p, size_t length) { return __setType(ut, CwtType_CWT_TEXT, p, length ? length : cwtStrNS()->strlen(p)); }
static inline BOOL      __setBLOB      (CwtUniType *ut, const void *p, size_t sz)         { return __setType(ut, CwtType_BLOB, p, sz); }
static inline BOOL      __setTIME      (CwtUniType *ut, const CWT_TIME *p, size_t sz)     { return __setType(ut, CwtType_TIME, p, sz ? sz : sizeof(CWT_TIME)); }
static inline BOOL      __setDATE      (CwtUniType *ut, const CWT_TIME *p, size_t sz)     { return __setType(ut, CwtType_DATE, p, sz ? sz : sizeof(CWT_TIME)); }
static inline BOOL      __setDATETIME  (CwtUniType *ut, const CWT_TIME *p, size_t sz)     { return __setType(ut, CwtType_DATETIME, p, sz ? sz : sizeof(CWT_TIME)); }

static BOOL             __toBOOL       (CwtUniType *ut, BOOL *ok);
static CWT_CHAR         __toCwtChar    (CwtUniType *ut, BOOL *ok);
static SBYTE            __toSBYTE      (CwtUniType *ut, BOOL *ok);
static BYTE             __toBYTE       (CwtUniType *ut, BOOL *ok);
static SHORT            __toSHORT      (CwtUniType *ut, BOOL *ok);
static USHORT           __toUSHORT     (CwtUniType *ut, BOOL *ok);
static INT              __toINT        (CwtUniType *ut, BOOL *ok);
static UINT             __toUINT       (CwtUniType *ut, BOOL *ok);
static LONG             __toLONG       (CwtUniType *ut, BOOL *ok);
static ULONG            __toULONG      (CwtUniType *ut, BOOL *ok);
static LONGLONG         __toLONGLONG   (CwtUniType *ut, BOOL *ok);
static ULONGLONG        __toULONGLONG  (CwtUniType *ut, BOOL *ok);
static float            __toFloat      (CwtUniType *ut, BOOL *ok);
static double           __toDouble     (CwtUniType *ut, BOOL *ok);
static CWT_CHAR*        __toCwtText    (CwtUniType *ut, BOOL *ok);
static void*            __toBLOB       (CwtUniType *ut, BOOL *ok);
static CWT_TIME*        __toTIME       (CwtUniType *ut, CWT_TIME *tm, BOOL *ok);
static CWT_TIME*        __toDATE       (CwtUniType *ut, CWT_TIME *tm, BOOL *ok);
static CWT_TIME*        __toDATETIME   (CwtUniType *ut, CWT_TIME *tm, BOOL *ok);

/* Helper function */
static void __setBuffer(CwtUniType *ut, CwtTypeEnum cwtType, const void *p, size_t sz);


static CwtUniTypeNS __cwtUniTypeNS = {
	  __create
	, __free
	, __type
/*	, __canCast*/
	, __setType
	, __setFromString
	, __setBOOL
	, __setCwtChar
	, __setSBYTE
	, __setBYTE
	, __setSHORT
	, __setUSHORT
	, __setINT
	, __setUINT
	, __setLONG
	, __setULONG
	, __setLONGLONG
	, __setULONGLONG
	, __setFloat
	, __setDouble
	, __setCwtText
	, __setBLOB
	, __setTIME
	, __setDATE
	, __setDATETIME

	, __toBOOL
	, __toCwtChar
	, __toSBYTE
	, __toBYTE
	, __toSHORT
	, __toUSHORT
	, __toINT
	, __toUINT
	, __toLONG
	, __toULONG
	, __toLONGLONG
	, __toULONGLONG
	, __toFloat
	, __toDouble
	, __toCwtText
	, __toBLOB
	, __toTIME
	, __toDATE
	, __toDATETIME
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
	ut->type = type;

	switch(type) {
	case CwtType_BOOL:
		ut->value.llong_val = *((BOOL*)copy);
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

	if( !__strNS->strlen(s) )
		return TRUE;

	switch(type) {
	case CwtType_BOOL:
		if( __strNS->strieq(_T("true"), s) || __strNS->streq(_T("1"), s) ) {
			__setBOOL(ut, TRUE);
			return TRUE;
		}
		if( __strNS->strieq(_T("false"), s) || __strNS->streq(_T("0"), s) ) {
			__setBOOL(ut, FALSE);
			return TRUE;
		}
		break;

/*
	case CwtType_CHAR: {
			char *latin1;
			CWT_CHAR buf[2] = {_T('\0'), _T('\0') };

			buf[0] = s[0];
			latin1 = cwtTextCodecNS()->toLatin1(buf);
			__setChar(ut, latin1[0]);
			CWT_FREE(latin1);
		}
		break;
*/

	case CwtType_CWT_CHAR:
		__setCwtChar(ut, s[0]);
		return TRUE;

	case CwtType_SBYTE: {
			SBYTE n = __strNS->toSBYTE(s, 10, &ok);
			if( ok ) {
				__setSBYTE(ut, n);
				return TRUE;
			}
		}
		break;

	case CwtType_BYTE: {
			BYTE n = __strNS->toBYTE(s, 10, &ok);
			if( ok ) {
				__setBYTE(ut, n);
				return TRUE;
			}
		}
		break;

	case CwtType_SHORT: {
			SHORT n = __strNS->toSHORT(s, 10, &ok);
			if( ok ) {
				__setSHORT(ut, n);
				return TRUE;
			}
		}
		break;

	case CwtType_USHORT: {
			USHORT n = __strNS->toUSHORT(s, 10, &ok);
			if( ok ) {
				__setUSHORT(ut, n);
				return TRUE;
			}
		}
		break;

	case CwtType_INT: {
			INT n = __strNS->toINT(s, 10, &ok);
			if( ok ) {
				__setINT(ut, n);
				return TRUE;
			}
		}
		break;

	case CwtType_UINT: {
			UINT n = __strNS->toUINT(s, 10, &ok);
			if( ok ) {
				__setUINT(ut, n);
				return TRUE;
			}
		}
		break;

	case CwtType_LONG: {
			UINT n = __strNS->toLONG(s, 10, &ok);
			if( ok ) {
				__setLONG(ut, n);
				return TRUE;
			}
		}
		break;

	case CwtType_ULONG: {
			ULONG n = __strNS->toULONG(s, 10, &ok);
			if( ok ) {
				__setULONG(ut, n);
				return TRUE;
			}
		}
		break;

	case CwtType_LONGLONG: {
			LONGLONG n = __strNS->toLONGLONG(s, 10, &ok);
			if( ok ) {
				__setLONGLONG(ut, n);
				return TRUE;
			}
		}
		break;

	case CwtType_ULONGLONG: {
			ULONGLONG n = __strNS->toULONGLONG(s, 10, &ok);
			if( ok ) {
				__setULONGLONG(ut, n);
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

/*
	case CwtType_TEXT: {
			char *latin1;
			latin1 = cwtTextCodecNS()->toLatin1(s);
			__setText(ut, latin1, strlen(latin1));
			CWT_FREE(latin1);
		}
		return TRUE;
*/

	case CwtType_CWT_TEXT:
		__setCwtText(ut, s, __strNS->strlen(s));
		return TRUE;


	case CwtType_TIME: {
			CWT_TIME tm;
			__strNS->toTIME(s, &tm, NULL, &ok);
			if( ok ) {
				__setTIME(ut, &tm, sizeof(CWT_TIME));
				return TRUE;
			}
		}
		break;

	case CwtType_DATE: {
			CWT_TIME tm;
			__strNS->toDATE(s, &tm, NULL, &ok);
			if( ok ) {
				__setDATE(ut, &tm, sizeof(CWT_TIME));
				return TRUE;
			}
		}
		break;

	case CwtType_DATETIME: {
			CWT_TIME tm;
			__strNS->toDATETIME(s, &tm, NULL, &ok);
			if( ok ) {
				__setDATETIME(ut, &tm, sizeof(CWT_TIME));
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
static inline BOOL __toBOOL(CwtUniType *ut, BOOL *ok)
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

	if( CwtType_CWT_TEXT == ut->type ) {
		if( ut->length > 0 ) {
			CWT_CHAR *s = (CWT_CHAR*)ut->value.ptr;
			CWT_ASSERT(ut->value.ptr);

			if( ut->length == 1 && s[0] == _T('0') )
				return FALSE;

			if( ut->length == 5 && __strNS->strnicmp(_T("false"), s, 5) == 0 )
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
/*
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
*/



static inline CWT_CHAR __toCwtChar(CwtUniType *ut, BOOL *ok)
{
	CWT_ASSERT(ut);

	if( ok )
		*ok = TRUE;

	if( CwtType_CWT_CHAR == ut->type )
		return (CWT_CHAR)ut->value.llong_val;

	if( CWT_TYPE_IS_INTEGER(ut->type)
			&& ut->value.llong_val >= (LONGLONG)CWT_CWT_CHAR_MIN
			&& ut->value.llong_val <= (LONGLONG)CWT_CWT_CHAR_MAX )
		return (CWT_CHAR)ut->value.llong_val;

	if( ok )
		*ok = FALSE;

	return (CWT_CHAR)0;
}


#define CWT_UNITYPE_TO_INTEGER(_IntType,_Min,_Max)                            \
		static _IntType __to##_IntType(CwtUniType *ut, BOOL *ok)              \
		{                                                                     \
			CWT_ASSERT(ut);                                                   \
                                                                              \
			if( ok )                                                          \
				*ok = TRUE;                                                   \
                                                                              \
			if( CwtType_##_IntType == ut->type )                              \
				return (_IntType)ut->value.llong_val;                         \
                                                                              \
            else if( CWT_TYPE_IS_INTEGER(ut->type)                            \
					&& ut->value.llong_val >= (LONGLONG)_Min                  \
					&& ut->value.llong_val <= (LONGLONG)_Max )                \
				return (_IntType)ut->value.llong_val;                         \
                                                                              \
            else if( CwtType_CWT_TEXT == ut->type && ut->length > 0) {        \
				BOOL okk;                                                     \
				_IntType n = __strNS->to##_IntType(                           \
					(const CWT_CHAR*)ut->value.ptr, 0, &okk);                 \
				if( okk )                                                     \
					return n;                                                 \
			}                                                                 \
                                                                              \
			if( ok )                                                          \
				*ok = FALSE;                                                  \
                                                                              \
			return (_IntType)0;                                               \
		}


CWT_UNITYPE_TO_INTEGER(SBYTE,  CWT_SBYTE_MIN, CWT_SBYTE_MAX)
CWT_UNITYPE_TO_INTEGER(BYTE,   0,             CWT_BYTE_MAX)
CWT_UNITYPE_TO_INTEGER(SHORT,  CWT_SHORT_MIN, CWT_SHORT_MAX)
CWT_UNITYPE_TO_INTEGER(USHORT, 0,             CWT_BYTE_MAX)
CWT_UNITYPE_TO_INTEGER(INT,    CWT_INT_MIN,   CWT_INT_MAX)
CWT_UNITYPE_TO_INTEGER(UINT,   0,             CWT_UINT_MAX)
CWT_UNITYPE_TO_INTEGER(LONG,   CWT_LONG_MIN,  CWT_LONG_MAX)
CWT_UNITYPE_TO_INTEGER(ULONG,  0,             CWT_ULONG_MAX)
CWT_UNITYPE_TO_INTEGER(LONGLONG,   CWT_LONGLONG_MIN,  CWT_LONGLONG_MAX)

static ULONGLONG __toULONGLONG(CwtUniType *ut, BOOL *ok)
{
	CWT_ASSERT(ut);

	if( ok )
		*ok = TRUE;

	if( CwtType_ULONGLONG == ut->type )
		return (ULONGLONG)ut->value.llong_val;

	else if( CWT_TYPE_IS_INTEGER(ut->type) )
		return (ULONGLONG)ut->value.llong_val;

	else if( CwtType_CWT_TEXT == ut->type && ut->length > 0) {
		BOOL okk;
		ULONGLONG n = __strNS->toULONGLONG((const CWT_CHAR*)ut->value.ptr, 0, &okk);
		if( okk )
			return n;
	}

	if( ok )
		*ok = FALSE;

	return (ULONGLONG)0;
}


static float __toFloat(CwtUniType *ut, BOOL *ok)
{
	CWT_ASSERT(ut);

	if( ok )
		*ok = TRUE;

	if( CwtType_FLOAT == ut->type ) {
		double d = ut->value.double_val;
		if( ut->value.double_val >= (double)CWT_FLOAT_MIN
				&& ut->value.double_val <= (double)CWT_FLOAT_MAX )
			return (float)ut->value.double_val;
	} else if( CWT_TYPE_IS_INTEGER(ut->type) ) {
		return (float)ut->value.llong_val;
	} else if( CwtType_CWT_TEXT == ut->type && ut->length > 0) {
		BOOL okk;
		float n = __strNS->toFloat((const CWT_CHAR*)ut->value.ptr, &okk);
		if( okk )
			return n;
	}

	if( ok )
		*ok = FALSE;

	return (float)0.0f;
}

static double __toDouble (CwtUniType *ut, BOOL *ok)
{
	CWT_ASSERT(ut);

	if( ok )
		*ok = TRUE;

	if( CwtType_DOUBLE == ut->type ) {
		return ut->value.double_val;
	} else if( CWT_TYPE_IS_INTEGER(ut->type) ) {
		return (double)ut->value.llong_val;
	} else if( CwtType_CWT_TEXT == ut->type && ut->length > 0) {
		BOOL okk;
		float n = __strNS->toDouble((const CWT_CHAR*)ut->value.ptr, &okk);
		if( okk )
			return n;
	}

	if( ok )
		*ok = FALSE;

	return (float)0.0f;
}

static CWT_CHAR* __toCwtText (CwtUniType *ut, BOOL *ok)
{
	CWT_ASSERT(ut);
	return (CWT_CHAR*)ut->value.ptr;
}

static void* __toBLOB (CwtUniType *ut, BOOL *ok)
{
	CWT_ASSERT(ut);
	return ut->value.ptr;
}

static CWT_TIME* __toTIME (CwtUniType *ut, CWT_TIME *tm, BOOL *ok)
{
	CWT_ASSERT(ut);
	return (CWT_TIME*)ut->value.ptr;
}

static CWT_TIME* __toDATE (CwtUniType *ut, CWT_TIME *tm, BOOL *ok)
{
	CWT_ASSERT(ut);
	return (CWT_TIME*)ut->value.ptr;
}

static CWT_TIME* __toDATETIME (CwtUniType *ut, CWT_TIME *tm, BOOL *ok)
{
	CWT_ASSERT(ut);
	return (CWT_TIME*)ut->value.ptr;
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
