/*
 * bytes_string.cpp
 *
 *  Created on: 23 дек. 2014 г.
 *      Author: wladt
 */

#include <pfs/byte_string.hpp>
#include "byte_string_impl.hpp"
#include <cstring>

namespace pfs {

byte_string::byte_string (const byte_t * bytes)
	: base_class()
{
	if (bytes)
		base_class::cast()->append(bytes, strlen(reinterpret_cast<const char *>(bytes)));
}

byte_string::byte_string (const char * str)
	: base_class()
{
	if (str)
		base_class::cast()->append(reinterpret_cast<const byte_t *>(str), strlen(str));
}

byte_string::byte_string (const byte_t * bytes, size_type n)
	: base_class()
{
	if (bytes && n > 0)
		base_class::cast()->append(bytes, n);
}

byte_string::byte_string (const char * str, size_type n)
	: base_class()
{
	if (str && n > 0)
		base_class::cast()->append(reinterpret_cast<const byte_t *>(str), n);
}

byte_string::byte_string (size_type count, byte_t ch)
	: base_class()
{
	if (count > 0) {
		impl_class * d = base_class::cast();
		d->reserve(count);
		for (size_t i = 0; i < count; ++i) {
			d->append(& ch, 1);
		}
	}
}

byte_string::byte_string (size_type count, char ch)
	: base_class()
{
	if (count > 0) {
		impl_class * d = base_class::cast();
		d->reserve(count);
		for (size_t i = 0; i < count; ++i) {
			d->append(reinterpret_cast<byte_t *>(& ch), 1);
		}
	}
}

bool byte_string::isEmpty () const
{
	return base_class::isNull() || base_class::cast()->isEmpty();
}

void byte_string::clear ()
{
	base_class::detach();
	base_class::cast()->clear();
}


/**
 *
 * @return raw data or @c null if byte string is null.
 */
const byte_t * byte_string::constData () const
{
	return base_class::isNull() ? nullptr : base_class::cast()->constData();
}

/**
 *
 * @return raw data or @c null if byte string is null.
 * @see byte_string::constData()
 */
const byte_t * byte_string::data () const
{
	return base_class::isNull() ? nullptr : base_class::cast()->constData();
}

/**
 *
 * @return C-style character string representation or "" (empty) if byte string is null or empty
 */
const char * byte_string::c_str () const
{
	return base_class::isNull()
		? ""
		: reinterpret_cast<const char *>(base_class::cast()->constData());
}

byte_string::size_type byte_string::size () const
{
	return base_class::isNull() ? 0 : base_class::cast()->size();
}

byte_string::size_type byte_string::length () const
{
	return base_class::isNull() ? 0 : base_class::cast()->length();
}

byte_string::size_type byte_string::capacity() const
{
	return base_class::cast()->capacity();
}

byte_string::size_type byte_string::max_size() const
{
	return base_class::cast()->max_size();
}

}
