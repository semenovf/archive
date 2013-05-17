/**
 * @file   bytearray.cpp
 * @author wladt
 * @date   May 15, 2013 4:37:44 PM
 *
 * @brief
 */


#include "../include/cwt/bytearray.hpp"
#include "../include/cwt/hash.hpp"
#include <cstdlib>
#include <cerrno>
#include <cmath>

CWT_NS_BEGIN

DLL_API uint_t hash_func(const ByteArray &key, uint_t seed)
{
	return hash_bytes(reinterpret_cast<const byte_t *>(key.constData()), key.size(), seed);
}

#ifdef CWT_BYTEARRAY_SELF_IMPL

ByteArray::ByteArray() : BaseClass() {}
ByteArray::ByteArray(const char *data, int size = -1) : BaseClass() { append(data, size); }
ByteArray::ByteArray(const ByteArray &other) : BaseClass() { m_d = other.m_d; }
ByteArray::~ByteArray() {}

ByteArray&	ByteArray::operator = (const ByteArray &other) { m_d = other.m_d; return *this; }
ByteArray&	ByteArray::operator = (const char *str) { ByteArray other(str); swap(other); return *this; }


const char* ByteArray::constData() const { return data(); }
size_t ByteArray::length() const { return BaseClass::size(); }
ByteArray& ByteArray::remove(size_t pos, size_t len) { BaseClass::remove(pos, len); return *this; }


ByteArray& ByteArray::append(const ByteArray &bytes)
{
	return append(bytes.constData(), bytes.size());
}

ByteArray& ByteArray::append(const char *data, int n)
{
	CWT_ASSERT(!m_d->data.isRaw());

	n = n < 0 ? strlen(data) : n;
	size_t sz = size() + n;
	BaseClass::reserve(sz + 1);
	Array<data_type>::copy(m_d->data, data, size(), 0, n);
	m_d->data[sz] = '\x0';
	m_d->count += n;

	return *this;
}

ByteArray& ByteArray::append(char ch)
{
	CWT_ASSERT(!m_d->data.isRaw());

	size_t sz = size();
	BaseClass::reserve(sz + 2);
	m_d->data[sz] = ch;
	m_d->data[sz+1] = '\x0';
	++m_d->count;

	return *this;
}

void ByteArray::reserve(size_t size)
{
	CWT_ASSERT(!m_d->data.isRaw());
	BaseClass::reserve(size+1);
}

void ByteArray::resize(size_t size)
{
	CWT_ASSERT(!m_d->data.isRaw());
	BaseClass::resize(size+1);
	m_d->data[size] = '\x0';
}

ByteArray& ByteArray::setRawData(const char *data, size_t size)
{
	Array<ByteArray::data_type> a(const_cast<char*>(data), size);
	m_d->data.swap(a);
	return *this;
}


double ByteArray::toDouble(bool *ok) const
{
	double val;
	char *endptr;

	const char *s = constData();

   /* Need to reset errno because it will not reset */
   /* in the subsequent call if error will not occurred */
	errno = 0;
	val = strtod(s, &endptr);

	if((errno == ERANGE	&& (val == HUGE_VAL || val == -HUGE_VAL))
            || (errno != 0 && val == (double)0.0)
            || endptr == s
            || *endptr != '\x0' ) {

		if( ok ) {
			*ok = false;
		}

		val = (double)0.0;
     } else {
    	 if( ok ) {
    		 *ok = true;
    	 }
     }

	return val;
}

float ByteArray::toFloat(bool *ok) const
{
	bool okk = true;
    double d = toDouble(&okk);

    if(!okk || d > CWT_FLOAT_MAX || d < -CWT_FLOAT_MAX) {
        if( ok != 0 )
            *ok = false;
        return 0.0;
    }
    if( ok != 0 )
        *ok = true;

    return (float)d;
}


template <typename strtol_func>
static long_t to_long_helper(const char *str, bool *ok, int base)
{
	char *endptr;
	long_t i = strtol_func()(str, &endptr, base);

	if((errno == ERANGE && (i == CWT_LONG_MAX || i == CWT_LONG_MIN))
			|| (errno != 0 && i == long_t(0))
			|| endptr == str
			|| *endptr != '\x0' ) {
		if( ok ) {
			*ok = false;
		}
		i = long_t(0);
	} else {
		if( ok ) {
			*ok = true;
		}
	}
	return i;
}

template <typename strtoul_func>
ulong_t to_ulong_helper(const char *str, bool *ok, int base)
{
	char *endptr;
	ulong_t i = strtoul_func()(str, &endptr, base);

	if((errno == ERANGE && i == CWT_ULONG_MAX)
			|| (errno != 0 && i == ulong_t(0))
			|| endptr == str
			|| *endptr != '\x0' ) {
		if( ok ) {
			*ok = false;
		}
		i = ulong_t(0);
	} else {
		if( ok ) {
			*ok = true;
		}
	}
	return i;
}

#ifdef CWT_HAS_INT64
class strtol_func {
	public:
	long_t operator ()(const char *nptr, char **endptr, int base) const throw() { return strtoll(nptr, endptr, base); }
};
class strtoul_func {
	public:
	ulong_t operator ()(const char *nptr, char **endptr, int base) const throw() { return strtoull(nptr, endptr, base); }
};

long_t ByteArray::toLong(bool *ok, int base) const
{
	return to_long_helper<strtol_func>(constData(), ok, base);
}

ulong_t	 ByteArray::toULong(bool *ok, int base) const
{
	return to_ulong_helper<strtoul_func>(constData(), ok, base);
}
#else
long_t ByteArray::toLong(bool *ok, int base) const
{
	return to_long_helper<strtol>(constData(), ok, base);
}

ulong_t ByteArray::toULong(bool *ok, int base) const
{
	return to_ulong_helper<strtoul>(constData(), ok, base);
}
#endif


template <typename int_type>
int_type to_int_type_helper(const ByteArray &ba, long_t max_value, long_t min_value, bool *ok = 0, int base = 10)
{
	bool okk = true;
	long_t i = ba.toLong(&okk, base);
	if( !okk || i > max_value || i < min_value) {
		okk = false;
	}
	if( ok )
		*ok = okk;
	return int_type(i);
}

template <typename uint_type>
uint_type to_uint_type_helper(const ByteArray &ba, ulong_t max_value, bool *ok = 0, int base = 10)
{
	bool okk = true;
	ulong_t i = ba.toULong(&okk, base);
	if( !okk || i > max_value) {
		okk = false;
	}
	if( ok )
		*ok = okk;
	return uint_type(i);
}


int_t ByteArray::toInt(bool *ok, int base) const
{
	return to_int_type_helper<int_t>(*this, long_t(CWT_INT_MAX), long_t(CWT_INT_MIN), ok, base);
}

short_t	ByteArray::toShort(bool *ok, int base) const
{
	return to_int_type_helper<short_t>(*this, long_t(CWT_SHORT_MAX), long_t(CWT_SHORT_MIN), ok, base);
}

sbyte_t	ByteArray::toSByte(bool *ok, int base) const
{
	return to_int_type_helper<sbyte_t>(*this, long_t(CWT_SBYTE_MAX), long_t(CWT_SBYTE_MIN), ok, base);
}

uint_t ByteArray::toUInt(bool *ok, int base) const
{
	return to_uint_type_helper<uint_t>(*this, ulong_t(CWT_UINT_MAX), ok, base);
}

ushort_t ByteArray::toUShort(bool *ok, int base) const
{
	return to_uint_type_helper<ushort_t>(*this, ulong_t(CWT_USHORT_MAX), ok, base);
}

byte_t ByteArray::toByte(bool *ok, int base) const
{
	return to_uint_type_helper<byte_t>(*this, ulong_t(CWT_BYTE_MAX), ok, base);
}

ByteArray& ByteArray::setNumber(long_t n, int base)
{
	char buf[64];
CWT_UNUSED(base);
#ifdef CWT_HAS_INT64
	sprintf(buf, "%lld", n);
#else
	sprintf(buf, "%ld", n);
#endif
	detach();
	ByteArray other(buf);
	swap(other);
	return *this;
}

ByteArray& ByteArray::setNumber(ulong_t n, int base)
{
	char buf[64];
	CWT_UNUSED(base);
#ifdef CWT_HAS_INT64
	sprintf(buf, "%llu", n);
#else
	sprintf(buf, "%lu", n);
#endif
	detach();
	ByteArray other(buf);
	swap(other);
	return *this;
}

ByteArray& ByteArray::setNumber(double n, char f, int prec)
{
	char format[32];
	char buf[64];
	if (f == 'g')
		sprintf(format, "%%%c", f);
	else
		sprintf(format, "%%.%d%c", prec, f);
	sprintf(buf, format, n);

	detach();
	ByteArray other(buf);
	swap(other);

	return *this;
}
#endif

CWT_NS_END
