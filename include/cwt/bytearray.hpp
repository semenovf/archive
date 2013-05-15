/**
 * @file   bytearray.hpp
 * @author wladt
 * @date   Jan 24, 2013 4:13:15 PM
 *
 * @brief
 */

#include <cwt/cwt.h>
#include <cwt/memory.hpp>

#ifndef __CWT_BYTEARRAY_HPP__
#define __CWT_BYTEARRAY_HPP__


CWT_NS_BEGIN

// Qt   QString
// C#
// Java

class DLL_API ByteArray
{
public:
	ByteArray();
	ByteArray(const char *data, int size = -1);
	ByteArray(int size, char ch);
	ByteArray(const ByteArray &other);
	~ByteArray() {}

	ByteArray& append(const ByteArray &bytes);
	ByteArray& append(const char *data, int size = -1);
	ByteArray& append(char ch);

	void clear();
	char* data();
	const char* data() const;
	const char* constData() const;
	size_t length() const;
	ByteArray& remove(int pos, int len);
	void reserve(int size);
	void resize(int size);

	double	 toDouble(bool *ok = 0) const;
	float	 toFloat(bool *ok = 0) const;
	int_t	 toInt(bool *ok = 0, int base = 10) const;
	long_t   toLong(bool *ok = 0, int base = 10) const;
	short_t	 toShort(bool *ok = 0, int base = 10) const;
	uint_t	 toUInt(bool *ok = 0, int base = 10) const;
	ulong_t	 toULong(bool *ok = 0, int base = 10) const;
	ushort_t toUShort(bool *ok = 0, int base = 10) const;

	ByteArray& setRawData(const char * data, uint size);
	size_t   size() const;

	ByteArray &	operator=(const ByteArray &other);
	ByteArray &	operator=(const char *str);

	friend bool	operator==(const ByteArray &s1, const ByteArray &s2);

private:
    class Impl;
    typedef unique_ptr<Impl> ImplPtr;
    ImplPtr pimpl;
};

DLL_API uint_t hash_func(const ByteArray &key, uint seed = 0);

CWT_NS_END

#endif /* __CWT_BYTEARRAY_HPP__ */
