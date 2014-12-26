/*
 * bytes_string.cpp
 *
 *  Created on: 23 дек. 2014 г.
 *      Author: wladt
 */

#include <pfs/byte_string.hpp>
#include <cstring>

namespace pfs {

byte_string::byte_string (const char * str)
	: base_class()
{
	if (str)
		base_class::cast()->append(reinterpret_cast<const_data_pointer>(str), strlen(str));
}

byte_string::byte_string (const_data_pointer bytes, size_type n)
	: base_class()
{
	if (bytes && n > 0)
		base_class::cast()->append(bytes, n);
}

byte_string::byte_string (const char * str, size_type n)
	: base_class()
{
	if (str && n > 0)
		base_class::cast()->append(reinterpret_cast<const_data_pointer>(str), n);
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
			d->append(reinterpret_cast<const_data_pointer>(& ch), 1);
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

byte_string & byte_string::erase (size_type index, size_type count)
{
	if (isEmpty() || index >= length())
		return *this;

	base_class::detach();
	impl_class * d = base_class::cast();

	if (index + count >= length()) {
		count = length() - index;
	}

	d->erase(index, count);

	return *this;
}


byte_string::iterator byte_string::erase (const_iterator first, const_iterator last)
{
	PFS_ASSERT(first >= cbegin());
	PFS_ASSERT(last <= cend());

	if (isEmpty())
		iterator(this, pointer(this, 0));

	size_type pos_begin = first.base().index();
	size_type pos_end = last.base().index();

	if (pos_begin < pos_end) {
		base_class::detach();
		base_class::cast()->erase(pos_begin, pos_end - pos_begin);
	}

	return iterator(this, pointer(this, pos_begin));
}


/**
 *
 * @return raw data or @c null if byte string is null.
 */
byte_string::const_data_pointer byte_string::constData () const
{
	return base_class::isNull() ? nullptr : base_class::cast()->constData();
}

/**
 *
 * @return raw data or @c null if byte string is null.
 * @see byte_string::constData()
 */
byte_string::const_data_pointer byte_string::data () const
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


int byte_string::compare (size_type pos1, size_type count1, const_data_pointer bytes, size_type count2) const
{
	PFS_ASSERT(pos1 <= length());
	PFS_ASSERT(pos1 + count1 <= length());

	if (this->isEmpty()) {
		return bytes == nullptr ? 0 : -1;
	}

	if (bytes == nullptr) {
		return this->isEmpty() ? 0 : 1;
	}

	const impl_class * d = base_class::cast();
	return d->compare(constData() + pos1, count1, bytes, count2);
}

int byte_string::compare (size_type pos1, size_type count1, const char * s, size_type count2) const
{
	return compare(pos1, count1, reinterpret_cast<const_data_pointer>(s), count2);
}

int byte_string::compare (size_type pos1, size_type count1, const byte_string & s, size_type pos2, size_type count2) const
{
	PFS_ASSERT(pos1 <= length());
	PFS_ASSERT(pos1 + count1 <= length());
	PFS_ASSERT(pos2 <= s.length());
	PFS_ASSERT(pos2 + count2 <= s.length());

	return compare(pos1, count1, s.constData() + pos2, count2);
}

int byte_string::compare (const_data_pointer bytes, size_type count) const
{
	return compare(0, length(), bytes, count);
}

int byte_string::compare (size_type pos, size_type count, const char * s) const
{
	return compare(pos, count, reinterpret_cast<const_data_pointer>(s), strlen(s));
}

}
