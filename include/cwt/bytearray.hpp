/**
 * @file   bytearray.hpp
 * @author wladt
 * @date   Jan 24, 2013 4:13:15 PM
 *
 * @brief
 */

#include <cwt/cwt.h>
#include <cwt/vector.hpp>

#ifndef __CWT_BYTEARRAY_QT_HPP__
#define __CWT_BYTEARRAY_QT_HPP__


CWT_NS_BEGIN

// Qt   QByteArray
// C#
// Java

class DLL_API ByteArray : public Vector<char>
{
	typedef char   data_type;
	typedef Vector<data_type> BaseClass;

public:
	ByteArray() : BaseClass() {}
	ByteArray(const char *data, int size = -1) : BaseClass() { append(data, size); }
	//ByteArray(int size, char ch);
	ByteArray(const ByteArray &other) : BaseClass() { m_d = other.m_d; }
	~ByteArray() {}

	ByteArray& append(const ByteArray &bytes);
	ByteArray& append(const char *data, int size = -1);
	ByteArray& append(char ch);

	// void clear(); // inherited
	// char* data(); // inherited
	// const char* data() const; // inherited
	const char* constData() const { return data(); }
	size_t length() const { return BaseClass::size(); }
	ByteArray& remove(size_t pos, size_t len) { BaseClass::remove(pos, len); return *this; }
	void reserve(size_t size);
	void resize(size_t size);

	double	 toDouble(bool *ok = 0) const;
	float	 toFloat(bool *ok = 0) const;
	int_t	 toInt(bool *ok = 0, int base = 10) const;
	long_t   toLong(bool *ok = 0, int base = 10) const;
	short_t	 toShort(bool *ok = 0, int base = 10) const;
	uint_t	 toUInt(bool *ok = 0, int base = 10) const;
	ulong_t	 toULong(bool *ok = 0, int base = 10) const;
	ushort_t toUShort(bool *ok = 0, int base = 10) const;
	byte_t	 toByte(bool *ok = 0, int base = 10) const;
	sbyte_t  toSByte(bool *ok = 0, int base = 10) const;

	ByteArray&	setNumber(long_t n, int base = 10);
	ByteArray&	setNumber(ulong_t n, int base = 10);
	ByteArray&	setNumber(int_t n, int base = 10)   { return setNumber(long_t(n), base); }
	ByteArray&	setNumber(uint_t n, int base = 10)  { return setNumber(ulong_t(n), base); }
	ByteArray&	setNumber(short_t n, int base = 10) { return setNumber(long_t(n), base); }
	ByteArray&	setNumber(ushort_t n, int base = 10){ return setNumber(ulong_t(n), base); }
	ByteArray&	setNumber(sbyte_t n, int base = 10) { return setNumber(long_t(n), base); }
	ByteArray&	setNumber(byte_t n, int base = 10)  { return setNumber(ulong_t(n), base); }
	ByteArray&	setNumber(double n, char f = 'g', int prec = 6);
	ByteArray&	setNumber(float n, char f = 'g', int prec = 6) { return setNumber(double(n), f, prec); }

	ByteArray& setRawData(const char *data, size_t size);
	// size_t   size() const; // inherited

	ByteArray&	operator = (const ByteArray &other) { m_d = other.m_d; return *this; }
	ByteArray&	operator = (const char *str) { ByteArray other(str); swap(other); return *this; }

	friend bool	operator ==(const ByteArray &s1, const ByteArray &s2);
};

DLL_API uint_t hash_func(const ByteArray &key, uint seed = 0);

inline bool operator ==(const ByteArray &s1, const ByteArray &s2)
{
	return s1.m_d->data.eq(s2.m_d->data);
}

CWT_NS_END

#endif /* __CWT_BYTEARRAY_QT_HPP__ */
