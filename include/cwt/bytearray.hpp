/**
 * @file   bytearray.hpp
 * @author wladt
 * @date   Jan 24, 2013 4:13:15 PM
 *
 * @brief
 */

#include <cwt/cwt.h>

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
	~ByteArray();

	char* data();
	const char* data() const;

	double	 toDouble(bool *ok = 0) const;
	float	 toFloat(bool *ok = 0) const;
	int_t	 toInt(bool *ok = 0, int base = 10) const;
	long_t   toLong(bool *ok = 0, int base = 10) const;
	short_t	 toShort(bool *ok = 0, int base = 10) const;
	uint_t	 toUInt(bool *ok = 0, int base = 10) const;
	ulong_t	 toULong(bool *ok = 0, int base = 10) const;
	ushort_t toUShort(bool *ok = 0, int base = 10) const;

	ByteArray& setRawData(const char * data, uint size);

	friend bool	operator==(const ByteArray &s1, const ByteArray &s2);

public:
	void *__impl;
};

CWT_NS_END

#endif /* __CWT_BYTEARRAY_HPP__ */
