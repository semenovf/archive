/**
 * @file   bytearray.hpp
 * @author wladt
 * @date   Jan 24, 2013 4:13:15 PM
 *
 * @brief
 */

#include <cwt/cwt.h>
#include <cwt/pimpl.hpp>

#ifndef __CWT_BYTEARRAY_QT_HPP__
#define __CWT_BYTEARRAY_QT_HPP__


CWT_NS_BEGIN

// Qt   QByteArray
// C#
// Java

class DLL_API ByteArray
{
	CWT_PIMPL_DECL_COPYABLE(ByteArray)

public:
	ByteArray();
	ByteArray(const char *data);
	ByteArray(const char *data, size_t size);
	ByteArray(size_t size, char ch);
	~ByteArray() {}

	ByteArray& append(const ByteArray &bytes);
	ByteArray& append(const char *data);
	ByteArray& append(const char *data, size_t size);
	ByteArray& append(char ch);

	char at(size_t i) const;
	char at(ssize_t i) const;
	bool isEmpty() const;
	void clear();
	char* data();
	const char* data() const;
	size_t size() const;

	ssize_t	indexOf ( const ByteArray & ba, size_t from = 0 ) const;
	ssize_t	indexOf ( const char * str, size_t from = 0 ) const;
	ssize_t	indexOf ( char ch, size_t from = 0 ) const;

	ByteArray &prepend(const ByteArray &ba);
	ByteArray &prepend(const char *str);
	ByteArray &prepend(const char *str, size_t size);
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
	ByteArray toBase64 () const;

	ByteArray&	setNumber(long_t n, int base = 10);
	ByteArray&	setNumber(ulong_t n, int base = 10);
	ByteArray&	setNumber(int_t n, int base = 10);
	ByteArray&	setNumber(uint_t n, int base = 10);
	ByteArray&	setNumber(short_t n, int base = 10);
	ByteArray&	setNumber(ushort_t n, int base = 10);
	ByteArray&	setNumber(sbyte_t n, int base = 10) { return setNumber(long_t(n), base); }
	ByteArray&	setNumber(byte_t n, int base = 10)  { return setNumber(ulong_t(n), base); }
	ByteArray&	setNumber(double n, char f = 'g', int prec = 6);
	ByteArray&	setNumber(float n, char f = 'g', int prec = 6);

	ByteArray& setRawData(const char *data, size_t size);

	bool endsWith    (const ByteArray & ba) const;
	bool endsWith    (const char * s) const;
	bool endsWith    (char c) const;
	bool startsWith  (const ByteArray &ba) const;
	bool startsWith  (const char *s) const;
	bool startsWith  (char c) const;
	ByteArray substr (size_t pos) const;
	ByteArray substr (size_t pos, size_t n) const;

	//ByteArray&	operator = (const ByteArray &other);
	ByteArray&	operator = (const char *str);
	char	operator[] (ssize_t i) const;
	char	operator[] (size_t i) const;

	char &	operator[] (ssize_t i);
	char &	operator[] (size_t i);

	friend bool	operator ==(const ByteArray &s1, const ByteArray &s2);

	static ByteArray number(double n, char format = 'g', int precision = 6);
	static ByteArray number(float n, char format = 'g', int precision = 6);
	static ByteArray number(int_t n, int base = 10);
	static ByteArray number(uint_t n, int base = 10);
	static ByteArray number(long_t n, int base = 10);
	static ByteArray number(ulong_t n, int base = 10);

	static ByteArray fromBase64 (const ByteArray & base64);
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
