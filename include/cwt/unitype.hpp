/*
 * unitype.hpp
 *
 *  Created on: 02.08.2012
 *      Author: wladt
 *
 *  Derived from unitype.h
 */

#ifndef __CWT_UNITYPE_H__
#define __CWT_UNITYPE_H__

#include <cwt/cwt.h>
#include <cwt/atomic.hpp>
#include <cwt/string.hpp>
#include <cwt/bytearray.hpp>
#include <cwt/date.hpp>
#include <cwt/datetime.hpp>
#include <cwt/time.hpp>

QVariant v;
CWT_NS_BEGIN

enum UniTypeEnum {
	  UniType_Null
	, UniType_Bool
	, UniType_SByte
	, UniType_Byte
	, UniType_Short
	, UniType_UShort
	, UniType_Int
	, UniType_UInt
	, UniType_Long
	, UniType_ULong
	, UniType_Float
	, UniType_Double
	, UniType_String
	, UniType_ByteArray
	, UniType_Time
	, UniType_Date
	, UniType_DateTime
};


struct UniTypeData : AtomicInt {
	UniTypeData() : m_type(UniType_Null) { ref(); }
	~UniTypeData() { if (m_type >= UniType_String && value.ptr) delete value.ptr; }
	UniTypeEnum m_type;

	union {
		bool      bool_val;
		long_t    long_val;
		float     float_val;
        double    double_val;
        void      *ptr;
	} value;
};

class UniType
{
public:
	UniType () : d(new UniType) {}
	UniType (const UniType &ut) { ut.d->ref(); d = ut.d; }
	UniType (bool b)       : d(new UniType) { d->m_type = UniType_Bool; d->value.bool_val = b; }
	UniType (byte_t n)     : d(new UniType) { d->m_type = UniType_Byte; d->value.long_val = n; }
	UniType (sbyte_t n)    : d(new UniType) { d->m_type = UniType_SByte; d->value.long_val = n; }
	UniType (short_t n)    : d(new UniType) { d->m_type = UniType_Short; d->value.long_val = n; }
	UniType (ushort_t n)   : d(new UniType) { d->m_type = UniType_UShort; d->value.long_val = n; }
	UniType (int_t n)      : d(new UniType) { d->m_type = UniType_Int; d->value.long_val = n; }
	UniType (uint_t n)     : d(new UniType) { d->m_type = UniType_UInt; d->value.long_val = (int_t)n; }
	UniType (long_t n)     : d(new UniType) { d->m_type = UniType_Long; d->value.long_val = n; }
	UniType (ulong_t n)    : d(new UniType) { d->m_type = UniType_ULong; d->value.long_val = (long_t)n; }
	UniType (float f)      : d(new UniType) { d->m_type = UniType_Float; d->value.float_val = f; }
	UniType (double f)     : d(new UniType) { d->m_type = UniType_Double; d->value.double_val = f; }
	UniType (const String &str) : d(new UniType) { d->m_type = UniType_String; d->value.ptr = new String(str); }
	UniType (const ByteArray &bytes) : d(new UniType) { d->m_type = UniType_ByteArray; d->value.ptr = new ByteArray(bytes); }
	UniType (const char *bytes, int size) : d(new UniType) { d->m_type = UniType_ByteArray; d->value.ptr = new ByteArray(bytes, size); }
	UniType (const Time &t);
	UniType (const Date &d);
	UniType (const DateTime &d);
	~UniType() { if(!d->deref()) delete d; }

	UniType& operator=(const UniType &other);
	UniTypeEnum type () const { return d->m_type; }

	bool     toBool(bool *ok);
	sbyte_t  toSByte(bool *ok);
	byte_t   toByte(bool *ok);
	short_t  toShort(bool *ok);
	ushort_t toUShort(bool *ok);
	int_t    toInt(bool *ok);
	uint_t   toUInt(bool *ok);
	long_t   toLong(bool *ok);
	ulong_t  toULong(bool *ok);
	float    toFloat(bool *ok);
	double   toDouble(bool *ok);

/*	BOOL        (*canCast)       (CwtUniType *ut, CwtTypeEnum type);

	BOOL        (*set)           (CwtUniType *ut, CwtTypeEnum type, const void *copy, size_t sz);
	BOOL        (*setFromString) (CwtUniType *ut, CwtTypeEnum type, const CWT_CHAR *s);
	CWT_CHAR*   (*toTEXT)        (CwtUniType *ut, BOOL *ok);
	const void* (*toBLOB)        (CwtUniType *ut, size_t *sz);
	CWT_TIME*   (*toTIME)        (CwtUniType *ut, CWT_TIME *tm, BOOL *ok);
	CWT_TIME*   (*toDATE)        (CwtUniType *ut, CWT_TIME *tm, BOOL *ok);
	CWT_TIME*   (*toDATETIME)    (CwtUniType *ut, CWT_TIME *tm, BOOL *ok);
	const CWT_CHAR* (*toString)  (CwtUniType *ut, CwtString *str);
*/

	friend bool operator == (const UniType &ut0, const UniType &ut1);
private:
	template <typename result_type, typename min_val, typename max_val>
	result_type toSignedInteger(bool *ok);

private:
	UniTypeData *d;
};


inline UniType& UniType::operator=(const UniType &other)
{
    (void) other.d->ref();
    if (!d->deref()) {
        delete d;
    }
    d = other.d;
    return *this;
}

CWT_NS_END

#endif /* __CWT_UNITYPE_H__ */
