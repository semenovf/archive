/**
 * @file   bytearray.hpp
 * @author wladt
 * @date   Jan 24, 2013 4:13:15 PM
 *
 * @brief
 */

#include <cwt/cwt.h>
#include <cwt/memory.hpp>

#ifndef __CWT_BYTEARRAY_QT_HPP__
#define __CWT_BYTEARRAY_QT_HPP__


CWT_NS_BEGIN

// Qt   QByteArray
// C#
// Java

class DLL_API ByteArray
{
/*
#ifdef CWT_BYTEARRAY_SELF_IMPL
	typedef char   data_type;
	typedef Vector<data_type> BaseClass;
#endif
*/

public:
	ByteArray();
	ByteArray(const char *data, int size = -1);
	//ByteArray(int size, char ch);
	ByteArray(const ByteArray &other);
	~ByteArray();

	ByteArray& append(const ByteArray &bytes);
	ByteArray& append(const char *data, int size = -1);
	ByteArray& append(char ch);

	void clear();
	char* data();
	const char* data() const;
	size_t size() const;

	ByteArray &prepend(const ByteArray &ba);
	ByteArray &prepend(const char *str, int len = -1);
	ByteArray &prepend(char ch);

	const char* constData() const;
	size_t length() const;
	ByteArray& remove(size_t pos, size_t len);
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

	ByteArray&	operator = (const ByteArray &other);
	ByteArray&	operator = (const char *str);

	friend bool	operator ==(const ByteArray &s1, const ByteArray &s2);

private:
    class Impl;
    typedef unique_ptr<Impl> ImplPtr;
    ImplPtr pimpl;
};

DLL_API uint_t hash_func(const ByteArray &key, uint seed = 0);

/*
#ifdef CWT_BYTEARRAY_SELF_IMPL
inline bool operator ==(const ByteArray &s1, const ByteArray &s2)
{
	return s1.m_d->data.eq(s2.m_d->data);
}
#endif
*/

CWT_NS_END

#endif /* __CWT_BYTEARRAY_QT_HPP__ */
