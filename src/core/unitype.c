/*
 * unitype.c
 *
 *  Created on: 08.08.2012
 *      Author: wladt
 */


#include <cwt/unitype.h>
#include <cwt/str.h>
#include <cwt/string.h>
#include <cwt/txtcodec.h>


static CwtUniType*      __create       (void);
static void             __free         (CwtUniType *ut);
static void             __init         (CwtUniType *ut);
static void             __destroy      (CwtUniType *ut);

static inline CwtTypeEnum __type       (CwtUniType *ut) { CWT_ASSERT(ut); return ut->type; }
static BOOL             __ut_eq        (CwtUniType *ut0, CwtUniType *ut1);
static BOOL             __setType      (CwtUniType *ut, CwtTypeEnum type, const void *copy, size_t sz);
static BOOL             __setFromString(CwtUniType *ut, CwtTypeEnum type, const CWT_CHAR *s);
static inline BOOL      __setBOOL      (CwtUniType *ut, BOOL b)       { return __setType(ut, CwtType_BOOL, &b, 0); }
static inline BOOL      __setCHAR      (CwtUniType *ut, CWT_CHAR ch)  { return __setType(ut, CwtType_CHAR, &ch, 0); }
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
static inline BOOL      __setFLOAT     (CwtUniType *ut, float n)      { return __setType(ut, CwtType_FLOAT, &n, 0); }
static inline BOOL      __setDOUBLE    (CwtUniType *ut, double n)     { return __setType(ut, CwtType_DOUBLE, &n, 0); }
static inline BOOL      __setTEXT      (CwtUniType *ut, const CWT_CHAR *p, size_t length) { return __setType(ut, CwtType_TEXT, p, length ? length : cwt_str_ns()->strLen(p)); }
static inline BOOL      __setBLOB      (CwtUniType *ut, const void *p, size_t sz) { return __setType(ut, CwtType_BLOB, p, sz); }
static inline BOOL      __setTIME      (CwtUniType *ut, const CWT_TIME *p) { return __setType(ut, CwtType_TIME, p, sizeof(CWT_TIME)); }
static inline BOOL      __setDATE      (CwtUniType *ut, const CWT_TIME *p) { return __setType(ut, CwtType_DATE, p, sizeof(CWT_TIME)); }
static inline BOOL      __setDATETIME  (CwtUniType *ut, const CWT_TIME *p) { return __setType(ut, CwtType_DATETIME, p, sizeof(CWT_TIME)); }

static BOOL             __toBOOL       (CwtUniType *ut, BOOL *ok);
static CWT_CHAR         __toCHAR       (CwtUniType *ut, BOOL *ok);
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
static float            __toFLOAT      (CwtUniType *ut, BOOL *ok);
static double           __toDOUBLE     (CwtUniType *ut, BOOL *ok);
static CWT_CHAR*        __toTEXT       (CwtUniType *ut, BOOL *ok);
static const void*      __toBLOB       (CwtUniType *ut, size_t *sz);
static CWT_TIME*        __toTIME       (CwtUniType *ut, CWT_TIME *tm, BOOL *ok);
static CWT_TIME*        __toDATE       (CwtUniType *ut, CWT_TIME *tm, BOOL *ok);
static CWT_TIME*        __toDATETIME   (CwtUniType *ut, CWT_TIME *tm, BOOL *ok);
static const CWT_CHAR*  ut_to_string   (CwtUniType *ut, CwtString *str);

/* Helper function */
static void __setBuffer(CwtUniType *ut, CwtTypeEnum cwtType, const void *p, size_t sz);


static CwtUniTypeNS __cwtUniTypeNS = {
	  __create
	, __free
	, __init
	, __destroy
	, __type
	, __ut_eq
/*	, __canCast*/
	, __setType
	, __setFromString
	, __setBOOL
	, __setCHAR
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
	, __setFLOAT
	, __setDOUBLE
	, __setTEXT
	, __setBLOB
	, __setTIME
	, __setDATE
	, __setDATETIME

	, __toBOOL
	, __toCHAR
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
	, __toFLOAT
	, __toDOUBLE
	, __toTEXT
	, __toBLOB
	, __toTIME
	, __toDATE
	, __toDATETIME
	, ut_to_string
};

static CwtStrNS *__strNS = NULL;


DLL_API_EXPORT CwtUniTypeNS* cwt_unitype_ns(void)
{
	if( !__strNS ) {
		__strNS = cwt_str_ns();
	}

	return &__cwtUniTypeNS;
}


static void __init (CwtUniType *ut)
{
	cwt_bzero(ut, sizeof(CwtUniType));
	ut->type = CwtType_UNKNOWN;
}

static void __destroy (CwtUniType *ut)
{
	if( ut && ut->type != CwtType_UNKNOWN ) {
		if( ut->length && ut->value.ptr ) {
			CWT_FREE(ut->value.ptr);
			ut->value.ptr = NULL;
			ut->capacity  = 0;
			ut->length    = 0;
		}
	}
}

static CwtUniType* __create(void)
{
	CwtUniType *ut;

	ut = CWT_MALLOC(CwtUniType);
	__init(ut);
	return ut;
}


static void __free(CwtUniType *ut)
{
	if( ut ) {
		__destroy(ut);
		CWT_FREE(ut);
	}
}

/**/
static int __ut_signed_map[] = {
/*	UNKNOWN  BOOL CHAR SBYTE BYTE SHORT USHORT INT UINT LONG ULONG LONGLONG ULONGLONG FLOAT DOUBLE TEXT BLOB TIME DATE DATETIME */
	0,       1,
#ifdef CWT_UNICODE
	              1,
#else
	             -1,
#endif
	                   -1,   1,   -1,    1,    -1, 1,   -1,  1,    -1,      1,        0,    0,     0,   0,   0,   0,   0
};



/**
 * @fn BOOL CwtUniTypeNS::eq(CwtUniType *ut0, CwtUniType *ut1)
 *
 * @brief Identifies the equality of two UniType type values.
 *
 * @details Two UniType type values are equal if they:
 * 		both are integer and have equal real values (without any cast),
 * 		or both are scalar and there values cast to double are equal,
 * 		or both are allocated memory blocks with the same length and content
 * 		(compared with CwtStrNS::memcmp() call).
 *
 * @param ut0 First CwtUniType value.
 * @param ut1 Second CwtUniType value.
 * @return @c TRUE if two values are equal, or @c FALSE.
 *
 */
static BOOL __ut_eq (CwtUniType *ut0, CwtUniType *ut1)
{
	if( ut0 == NULL || ut1 == NULL ) {
		if( ut0 == ut1 )
			return TRUE;
	}

	if( CWT_TYPE_IS_INTEGER(ut0->type) && CWT_TYPE_IS_INTEGER(ut1->type) ) {
		if( ut0->value.llong_val == ut1->value.llong_val ) {
			if( (__ut_signed_map[ut0->type] + __ut_signed_map[ut1->type]) == 2
					|| (__ut_signed_map[ut0->type] + __ut_signed_map[ut1->type]) == -2) {
				return TRUE;
			}

			if( __ut_signed_map[ut0->type] > 0 &&  ut1->value.llong_val > 0 ) {
				return TRUE;
			}

			if( __ut_signed_map[ut1->type] > 0 &&  ut0->value.llong_val > 0 ) {
				return TRUE;
			}

		}
	} else if( CWT_TYPE_IS_SCALAR(ut0->type) && CWT_TYPE_IS_SCALAR(ut1->type) ) {
		BOOL ok0, ok1;
		return __toDOUBLE(ut0, &ok0) == __toDOUBLE(ut1, &ok1)
				&& ok0 && ok1
				? TRUE : FALSE;
	} else {
		if( ut0->type == ut1->type ) {
			if( ut0->length == ut1->length ) {
				if( ut0->type == CwtType_TEXT )
					return cwt_str_ns()->memcmp(ut0->value.ptr, ut1->value.ptr, ut0->length * sizeof(CWT_CHAR)) == 0
							? TRUE : FALSE;
				else
					return cwt_str_ns()->memcmp(ut0->value.ptr, ut1->value.ptr, ut0->length) == 0
							? TRUE : FALSE;
			}
		}
	}

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
 *
 * @note for CwtType_FLOAT and CwtType_DOUBLE types @c copy must points to float and
 * double values. There is no correct way to cast float* to double* and vice versa.
 */
static BOOL __setType(CwtUniType *ut, CwtTypeEnum type, const void *copy, size_t sz)
{
	CWT_ASSERT(ut);

	if( type != ut->type ) {
		if( ut->length && ut->value.ptr ) {
			CWT_FREE(ut->value.ptr);
			ut->value.ptr = NULL;
			ut->capacity = 0;
			ut->length = 0;
		}
	}


	ut->type = type;

	if( !copy ) { /* only reserve */

		switch(type) {
		case CwtType_TEXT:
		case CwtType_BLOB:
		case CwtType_TIME:
		case CwtType_DATE:
		case CwtType_DATETIME:
			__setBuffer(ut, type, NULL, sz);
			break;
		default:
			break;
		}
		return TRUE;
	}

	switch(type) {
	case CwtType_BOOL:
		ut->value.llong_val = *((BOOL*)copy);
		break;
	case CwtType_CHAR:
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
		ut->value.float_val = *((float*)copy);
		break;
	case CwtType_DOUBLE:
		ut->value.double_val = *((double*)copy);
		break;
	case CwtType_TEXT:
		__setBuffer(ut, CwtType_TEXT, copy, sz);
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

	if( !__strNS->strLen(s) )
		return TRUE;

	switch(type) {
	case CwtType_BOOL:
		if( __strNS->strCaseEq(_T("true"), s) || __strNS->strEq(_T("1"), s) ) {
			__setBOOL(ut, TRUE);
			return TRUE;
		}
		if( __strNS->strCaseEq(_T("false"), s) || __strNS->strEq(_T("0"), s) ) {
			__setBOOL(ut, FALSE);
			return TRUE;
		}
		break;

	case CwtType_CHAR:
		__setCHAR(ut, s[0]);
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
				__setFLOAT(ut, n);
				return TRUE;
			}
		}
		break;

	case CwtType_DOUBLE: {
			double n = __strNS->toDouble(s, &ok);
			if( ok ) {
				__setDOUBLE(ut, n);
				return TRUE;
			}
		}
		break;

	case CwtType_TEXT:
		__setTEXT(ut, s, __strNS->strLen(s));
		return TRUE;


	case CwtType_TIME: {
			CWT_TIME tm;
			__strNS->toTIME(s, &tm, NULL, &ok);
			if( ok ) {
				__setTIME(ut, &tm);
				return TRUE;
			}
		}
		break;

	case CwtType_DATE: {
			CWT_TIME tm;
			__strNS->toDATE(s, &tm, NULL, &ok);
			if( ok ) {
				__setDATE(ut, &tm);
				return TRUE;
			}
		}
		break;

	case CwtType_DATETIME: {
			CWT_TIME tm;
			__strNS->toDATETIME(s, &tm, NULL, &ok);
			if( ok ) {
				__setDATETIME(ut, &tm);
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
 * @fn CwtUniTypeNS::toBOOL(CwtUniType *ut, BOOL *ok)
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
static BOOL __toBOOL(CwtUniType *ut, BOOL *ok)
{
	CWT_ASSERT(ut);

	if( ok )
		*ok = TRUE;

	if( CwtType_BOOL == ut->type )
		return (BOOL)ut->value.llong_val;

	if( CWT_TYPE_IS_INTEGER(ut->type) )
		return ut->value.llong_val ? TRUE : FALSE;

	if( CwtType_FLOAT == ut->type )
		return ut->value.float_val != 0.0f ? TRUE : FALSE;

	if( CwtType_DOUBLE == ut->type )
		return ut->value.double_val != 0.0f ? TRUE : FALSE;

	if( CwtType_TEXT == ut->type ) {
		if( ut->length > 0 ) {
			CWT_CHAR *s = (CWT_CHAR*)ut->value.ptr;
			CWT_ASSERT(ut->value.ptr);

			if( ut->length == 1 && s[0] == _T('0') )
				return FALSE;

			if( ut->length == 5 && __strNS->strNCaseCmp(_T("false"), s, 5) == 0 )
				return FALSE;

			return TRUE;
		}

		return FALSE;
	} else {
		if( ok )
			*ok = FALSE;
	}

	return FALSE;
}



/**
 * @fn CwtUniTypeNS::toCHAR(CwtUniType *ut)
 *
 * @brief Returns the unitype as a @c CWT_CHAR.
 *
 * @param ut
 * @return
 */
static inline CWT_CHAR __toCHAR(CwtUniType *ut, BOOL *ok)
{
	CWT_ASSERT(ut);

	if( ok )
		*ok = TRUE;

	if( CwtType_CHAR == ut->type )
		return (CWT_CHAR)ut->value.llong_val;

	if( CWT_TYPE_IS_INTEGER(ut->type)
			&& ut->value.llong_val >= (LONGLONG)CWT_CHAR_MIN
			&& ut->value.llong_val <= (LONGLONG)CWT_CHAR_MAX )
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
					/*&& ut->value.llong_val >= (LONGLONG)_Min*/              \
					/*&& ut->value.llong_val <= (LONGLONG)_Max*/ )            \
				return (_IntType)ut->value.llong_val;                         \
                                                                              \
            else if( CwtType_TEXT == ut->type && ut->length > 0) {            \
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

	else if( CwtType_TEXT == ut->type && ut->length > 0) {
		BOOL okk;
		ULONGLONG n = __strNS->toULONGLONG((const CWT_CHAR*)ut->value.ptr, 0, &okk);
		if( okk )
			return n;
	}

	if( ok )
		*ok = FALSE;

	return (ULONGLONG)0;
}


static float __toFLOAT(CwtUniType *ut, BOOL *ok)
{
	CWT_ASSERT(ut);

	if( ok )
		*ok = TRUE;

	if( CwtType_FLOAT == ut->type ) {
		return ut->value.float_val;
	} else if( CWT_TYPE_IS_INTEGER(ut->type) ) {
		return (float)ut->value.llong_val;
	} else if( CwtType_TEXT == ut->type && ut->length > 0) {
		BOOL okk;
		float n = __strNS->toFloat((const CWT_CHAR*)ut->value.ptr, &okk);
		if( okk )
			return n;
	}

	if( ok )
		*ok = FALSE;

	return (float)0.0f;
}

static double __toDOUBLE (CwtUniType *ut, BOOL *ok)
{
	CWT_ASSERT(ut);

	if( ok )
		*ok = TRUE;

	if( CwtType_DOUBLE == ut->type ) {
		return ut->value.double_val;
	} else if( CWT_TYPE_IS_INTEGER(ut->type) ) {
		return (double)ut->value.llong_val;
	} else if( CwtType_TEXT == ut->type && ut->length > 0) {
		BOOL okk;
		double n = __strNS->toDouble((const CWT_CHAR*)ut->value.ptr, &okk);
		if( okk )
			return n;
	}

	if( ok )
		*ok = FALSE;

	return (double)0.0f;
}


/**
 * @fn CwtUniTypeNS::toTEXT(CwtUniType *ut, BOOL *ok)
 *
 * @brief Text representation of unitype.
 *
 * Returns date/time according to ISO 8601 date/time format:
 * either YYYY-MM-DD for dates, HH:MM:SS for times or
 * YYYY-MM-DD HH:MM:SS for combined dates and times.
 *
 * @param ut unitype value.
 * @param ok
 * @return
 */
static CWT_CHAR* __toTEXT (CwtUniType *ut, BOOL *ok)
{
	CWT_CHAR *s = NULL;

	CWT_ASSERT(ut);

	if( ok )
		*ok = TRUE;

	if( CwtType_TEXT == ut->type ) {
		if( ut->length > 0 )
			s = __strNS->strNdup((CWT_CHAR*)ut->value.ptr, ut->length);
		else
			s = __strNS->strDup(_T(""));

		return s;
	} else {

		CwtString *buf;
		CwtStringNS *stringNS = cwt_string_ns();

		buf = stringNS->create();

		switch( ut->type ) {
		case CwtType_BOOL:
			stringNS->append(buf, __toBOOL(ut, NULL) ? _Tr("true") : _Tr("false"));
			break;
		case CwtType_CHAR:
			stringNS->appendChar(buf, __toCHAR(ut, NULL));
			break;
		case CwtType_SBYTE:
		case CwtType_SHORT:
		case CwtType_INT:
			stringNS->sprintf(buf, _T("%d"), __toINT(ut, NULL));
			break;
		case CwtType_BYTE:
		case CwtType_USHORT:
		case CwtType_UINT:
			stringNS->sprintf(buf, _T("%u"), __toUINT(ut, NULL));
			break;
		case CwtType_LONG:
			stringNS->sprintf(buf, _T("%ld"), __toLONG(ut, NULL));
			break;
		case CwtType_ULONG:
			stringNS->sprintf(buf, _T("%lu"), __toULONG(ut, NULL));
			break;
		case CwtType_LONGLONG:
			stringNS->sprintf(buf, _T("%lld"), __toLONGLONG(ut, NULL));
			break;
		case CwtType_ULONGLONG:
			stringNS->sprintf(buf, _T("%llu"), __toULONGLONG(ut, NULL));
			break;
		case CwtType_FLOAT:
			stringNS->sprintf(buf, _T("%f"), __toFLOAT(ut, NULL));
			break;
		case CwtType_DOUBLE:
			stringNS->sprintf(buf, _T("%g"), __toFLOAT(ut, NULL));
			break;
		case CwtType_TIME: {
				CWT_TIME tm;
				__toTIME(ut, &tm, NULL);
				stringNS->sprintf(buf, _T("%2u:%2u:%2u"), tm.hour, tm.min, tm.sec);
			}
			break;
		case CwtType_DATE: {
				CWT_TIME tm;
				__toTIME(ut, &tm, NULL);
				stringNS->sprintf(buf, _T("%4u-%2u-%2u"), tm.year, tm.mon, tm.day);
			}
			break;
		case CwtType_DATETIME: {
				CWT_TIME tm;
				__toTIME(ut, &tm, NULL);
				stringNS->sprintf(buf, _T("%4u-%2u-%2u %2u:%2u:%2u")
					, tm.year, tm.mon, tm.day
					, tm.hour, tm.min, tm.sec);
			}
			break;
		default:
			break;
		}

		if( stringNS->length(buf) > 0 ) {
			s = __strNS->strDup(stringNS->cstr(buf));
		}

		stringNS->free(buf);

		if( s )
			return s;
	}

	if( ok )
		*ok = FALSE;

	return s;
}

/**
 * @fn CwtUniTypeNS::toBLOB(CwtUniType *ut, size_t *sz)
 *
 * @brief
 *
 * @param ut
 * @param sz
 */
static const void* __toBLOB (CwtUniType *ut, size_t *sz)
{
	CWT_ASSERT(ut);

	if( sz )
		*sz = ut->length;
	return ut->value.ptr;
}


/**
 * @fn CwtUniTypeNS::toTIME(CwtUniType *ut, CWT_TIME *tm, BOOL *ok)
 *
 * @brief
 *
 * @param ut
 * @param tm
 * @param ok
 * @return
 */
static CWT_TIME* __toTIME (CwtUniType *ut, CWT_TIME *tm, BOOL *ok)
{
	CWT_ASSERT(ut);

	if( ok )
		*ok = FALSE;

	if( ut->length < sizeof(CWT_TIME) ) {
		return NULL;
	}

	if( CwtType_TIME == ut->type || CwtType_DATETIME == ut->type ) {
		CWT_TIME *tm_ = (CWT_TIME*)ut->value.ptr;
		tm->hour = tm_->hour;
		tm->min  = tm_->min;
		tm->sec  = tm_->sec;
		tm->sec_part = tm_->sec_part;

		if( ok )
			*ok = TRUE;

		return tm;
	}

	return NULL;
}

/**
 * @fn CwtUniTypeNS::toDATE(CwtUniType *ut, CWT_TIME *tm, BOOL *ok)
 *
 * @param ut
 * @param tm
 * @param ok
 * @return
 */
static CWT_TIME* __toDATE (CwtUniType *ut, CWT_TIME *tm, BOOL *ok)
{
	CWT_ASSERT(ut);

	if( ok )
		*ok = FALSE;

	if( ut->length < sizeof(CWT_TIME) ) {
		return NULL;
	}

	if( CwtType_DATE == ut->type || CwtType_DATETIME == ut->type ) {
		CWT_TIME *tm_ = (CWT_TIME*)ut->value.ptr;
		tm->year = tm_->year;
		tm->mon  = tm_->mon;
		tm->day  = tm_->day;

		if( ok )
			*ok = TRUE;

		return tm;
	}

	return NULL;
}


/**
 * @fn CwtUniTypeNS::toDATETIME(CwtUniType *ut, CWT_TIME *tm, BOOL *ok)
 *
 * @param ut
 * @param tm
 * @param ok
 * @return
 */
static CWT_TIME* __toDATETIME (CwtUniType *ut, CWT_TIME *tm, BOOL *ok)
{
	CWT_ASSERT(ut);

	if( ok )
		*ok = FALSE;

	if( ut->length < sizeof(CWT_TIME) ) {
		return NULL;
	}

	if( CwtType_DATETIME == ut->type ) {
		CWT_TIME *tm_ = (CWT_TIME*)ut->value.ptr;
		tm->hour = tm_->hour;
		tm->min  = tm_->min;
		tm->sec  = tm_->sec;
		tm->sec_part = tm_->sec_part;
		tm->year = tm_->year;
		tm->mon  = tm_->mon;
		tm->day  = tm_->day;

		if( ok )
			*ok = TRUE;

		return tm;
	}

	return NULL;
}

static const CWT_CHAR* ut_to_string (CwtUniType *ut, CwtString *str)
{
	CwtStringNS *string_ns = cwt_string_ns();

	CWT_ASSERT(ut);
	CWT_ASSERT(str);

	string_ns->clear(str);

	if( CwtType_TEXT == ut->type ) {
		if( ut->length > 0 )
			string_ns->appendChars(str, (CWT_CHAR*)ut->value.ptr, ut->length);
	} else {
		switch( ut->type ) {
		case CwtType_BOOL:
			string_ns->append(str, __toBOOL(ut, NULL) ? _Tr("true") : _Tr("false"));
			break;
		case CwtType_CHAR:
			string_ns->appendChar(str, __toCHAR(ut, NULL));
			break;
		case CwtType_SBYTE:
		case CwtType_SHORT:
		case CwtType_INT:
			string_ns->sprintf(str, _T("%d"), __toINT(ut, NULL));
			break;
		case CwtType_BYTE:
		case CwtType_USHORT:
		case CwtType_UINT:
			string_ns->sprintf(str, _T("%u"), __toUINT(ut, NULL));
			break;
		case CwtType_LONG:
			string_ns->sprintf(str, _T("%ld"), __toLONG(ut, NULL));
			break;
		case CwtType_ULONG:
			string_ns->sprintf(str, _T("%lu"), __toULONG(ut, NULL));
			break;
		case CwtType_LONGLONG:
			string_ns->sprintf(str, _T("%lld"), __toLONGLONG(ut, NULL));
			break;
		case CwtType_ULONGLONG:
			string_ns->sprintf(str, _T("%llu"), __toULONGLONG(ut, NULL));
			break;
		case CwtType_FLOAT:
			string_ns->sprintf(str, _T("%f"), __toFLOAT(ut, NULL));
			break;
		case CwtType_DOUBLE:
			string_ns->sprintf(str, _T("%g"), __toFLOAT(ut, NULL));
			break;
		case CwtType_TIME: {
				CWT_TIME tm;
				__toTIME(ut, &tm, NULL);
				string_ns->sprintf(str, _T("%2u:%2u:%2u"), tm.hour, tm.min, tm.sec);
			}
			break;
		case CwtType_DATE: {
				CWT_TIME tm;
				__toTIME(ut, &tm, NULL);
				string_ns->sprintf(str, _T("%4u-%2u-%2u"), tm.year, tm.mon, tm.day);
			}
			break;
		case CwtType_DATETIME: {
				CWT_TIME tm;
				__toTIME(ut, &tm, NULL);
				string_ns->sprintf(str, _T("%4u-%2u-%2u %2u:%2u:%2u")
					, tm.year, tm.mon, tm.day
					, tm.hour, tm.min, tm.sec);
			}
			break;
		default:
			break;
		}
	}

	return string_ns->cstr(str);
}

/* Helper function.
 * For CwtType_TEXT sz is number of chars.
 * For other types sz is number of bytes.
 *
 * If sz > 0 buffer will be (re)allocated (if previously did not allocated or buffer size is insufficient)
 * */
static void __setBuffer(CwtUniType *ut, CwtTypeEnum cwtType, const void *p, size_t sz)
{
	size_t nbytes;

	CWT_ASSERT(ut);
	nbytes = (CwtType_TEXT == cwtType)
			? sz * sizeof(CWT_CHAR)
			: sz;

	if( ut->capacity < nbytes ) {
		if( ut->capacity > 0 && ut->value.ptr ) {
			CWT_FREE(ut->value.ptr);
			ut->value.ptr = NULL;
			ut->capacity = 0;
			ut->length = 0;
		}
	}

	if( nbytes > 0 ) {

		if( !ut->capacity ) {
			ut->value.ptr = CWT_MALLOCA(char, nbytes);
			ut->capacity = nbytes;
		}

		if( p ) {
			cwt_str_ns()->memcpy(ut->value.ptr, p, nbytes);
			ut->length = sz;
		}
	}

	ut->type = cwtType;
}
