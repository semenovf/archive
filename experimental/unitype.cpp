/*
 * unitype.c
 *
 *  Created on: 08.08.2012
 *      Author: wladt
 */


#include <cwt/unitype.hpp>

CWT_NS_BEGIN
/*
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
*/

/* Helper function */
//static void __setBuffer(CwtUniType *ut, CwtTypeEnum cwtType, const void *p, size_t sz);


/**
 * @brief Identifies the equality of two UniType type values.
 *
 * @details Two UniType type values are equal if they:
 * 		both are integer and have equal real values (without any cast),
 * 		or both are scalar and there values cast to double are equal,
 * 		or both are allocated memory blocks with the same length and content
 * 		(compared with CwtStrNS::memcmp() call).
 *
 * @param ut0 First UniType value.
 * @param ut1 Second UniType value.
 * @return @c true if two values are equal, or @c false.
 */
bool operator == (const UniType &ut0, const UniType &ut1)
{
/*
	if (ut0.isInteger() && ut1.isInteger()) {
		if (ut0.d->value.int_val == ut1.d->value.int_val) {
			if( (__ut_signed_map[ut0.type()] + __ut_signed_map[ut1.type()]) == 2
					|| (__ut_signed_map[ut0.type()] + __ut_signed_map[ut1.type()]) == -2) {
				return true;
			}

			if( __ut_signed_map[ut0.type()] > 0 &&  ut1.d->value.int_val > 0 ) {
				return true;
			}

			if( __ut_signed_map[ut1->type] > 0 &&  ut0.d->value.int_val > 0 ) {
				return true;
			}
		}
	} else if (ut0.isScalar() && ut1.isScalar()) {
		bool ok0, ok1;
		return ut0.toDouble(&ok0) == ut1.toDouble(&ok1) && ok0 && ok1 ? true : false;
	} else {
		if( ut0.type() == ut1.type() ) {
			if( ut0->length == ut1->length ) {
				if( ut0->type == CwtType_TEXT )
					return cwt_str_ns()->memcmp(ut0->value.ptr, ut1->value.ptr, ut0->length * sizeof(CWT_CHAR)) == 0
							? true : false;
				else
					return cwt_str_ns()->memcmp(ut0->value.ptr, ut1->value.ptr, ut0->length) == 0
							? true : false;
			}
		}
	}
*/
	return false;
}


bool UniType::toBool(bool *ok)
{
	bool isOk = true;
	bool val;

	switch(type()) {
	case UniType_Bool:
		val = d->value.bool_val;
		break;
	case UniType_SByte:
	case UniType_Byte:
	case UniType_Short:
	case UniType_UShort:
	case UniType_Int:
	case UniType_UInt:
	case UniType_Long:
	case UniType_ULong:
		val = d->value.long_val != 0 ? true : false;
		break;
	case UniType_Float:
		val = d->value.float_val != 0.0f ? true : false;
		break;
	case UniType_Double:
		val = d->value.double_val != 0.0f ? true : false;
		break;
	case UniType_String: {
		String *s = reinterpret_cast<String*>(d->value.ptr);
		if (s->isEmpty() || *s == _U("false") || *s == _U("no") || *s == _U("0"))
			val = false;
		}
		break;
	case UniType_ByteArray: {
		ByteArray *s = reinterpret_cast<ByteArray*>(d->value.ptr);
		if (s->isEmpty() || *s == _U("false") || *s == _U("no") || *s == _U("0"))
			val = false;
		}
		break;
	default:
		val = false;
		isOk = false;
		break;
	}

	if (ok)
		*ok = isOk;

	return val;
}


template <typename integer_type, typename min_val, typename max_val>
integer_type UniType::toSignedInteger(bool *ok)
{
	bool isOk = false;
	long_t val = 0;

	switch(type()) {
	case UniType_SByte:
	case UniType_Short:
	case UniType_Int:
	case UniType_Long:
		if (d->value.long_val >= min_val && d->value.long_val <= max_val) {
			val = d->value.long_val;
			isOk = true;
		}
		break;
	case UniType_Byte:
	case UniType_UShort:
	case UniType_UInt:
	case UniType_ULong:
		if (((ulong_t)d->value.long_val) <= max_val) {
			val = d->value.long_val;
			isOk = true;
		}
		break;
	case UniType_Float:
		if (d->value.float_val >= (float)min_val && d->value.float_val <= (float)max_val) {
			val = (long_t)d->value.long_val;
			isOk = true;
		}
		break;
	case UniType_Double:
		if (d->value.double_val >= (double)min_val && d->value.double_val <= (double)max_val) {
			val = (long_t)d->value.long_val;
			isOk = true;
		}
		break;
	case UniType_String:
		val = reinterpret_cast<String*>(d->value.ptr)->toLong(&isOk);

		if (isOk) {
			if (val >= min_val && val <= max_val) {
				val = d->value.long_val;
			} else {
				val = 0;
				isOk = false;
			}
		}
		break;
	default:
		val = false;
		isOk = false;
		break;
	}

	if (ok)
		*ok = isOk;

	return (integer_type)val;
}

sbyte_t UniType::toSByte(bool *ok)
{
	return toSignedInteger<sbyte_t, CWT_SBYTE_MIN, CWT_SBYTE_MAX>(ok);
}

byte_t UniType::toByte(bool *ok)
{

}



double UniType::toDouble (bool *ok)
{
	double n = 0.0f;
	bool isOk = true;

	if (UniType_Double == type()) {
		n = d->value.double_val;
	} else if (isInteger()) {
		n = (double)d->value.int_val;
	} else if (UniType_Text == type() && d->value.str_val.length() > 0) {
		n = d->value.str_val.toDouble(&isOk);
	}

	if (ok)
		*ok = isOk;

	return n;
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

CWT_NS_END
