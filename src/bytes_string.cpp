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


byte_string & byte_string::insert (size_type index, const byte_string & str, size_type index_str, size_type count)
{
	if (str.length() == 0)
		return *this;

	base_class::detach();
	PFS_ASSERT(index <= length());
	PFS_ASSERT(index_str <= str.length());

	impl_class * d = base_class::cast();

	// Append
	if (index == this->length() && index_str == 0 && count == str.length()) {
		d->append(str.constData(), str.size());
		return *this;
	}

	if (index_str + count > str.length())
		count = str.length() - index_str;

	d->insert(index, str.constData() + index_str, count);

	return *this;
}

byte_string::iterator byte_string::find (const_iterator pos, const byte_string & str) const
{
	return find(pos, str.c_str(), str.length());
}

byte_string::iterator byte_string::find (const_iterator pos, const char * s, size_type count) const
{
	return find(pos, reinterpret_cast<const_data_pointer>(s), count);
}

byte_string::iterator byte_string::find (const_iterator pos, const char * s) const
{
	return find(pos, s, strlen(s));
}

byte_string::iterator byte_string::find (const_iterator pos, const_data_pointer bytes, size_type count) const
{
	PFS_ASSERT(pos >= cbegin());
	PFS_ASSERT(pos <= cend());

	size_type r = 0;
	size_type index = pos.base().index();

	byte_string * self = const_cast<byte_string *>(this);

	return base_class::cast()->find(index, bytes, count, r)
		? iterator(self, pointer(self, r))
		: iterator(self, pointer(self, size())); // end()
}

byte_string & byte_string::replace (size_type pos1, size_type count1, const byte_string & str, size_type pos2, size_type count2)
{
	PFS_ASSERT(pos2 <= str.length());
	PFS_ASSERT(pos2 + count2 <= str.length());
	return replace(pos1, count1, str.constData() + pos2, count2);
}

byte_string & byte_string::replace (size_type pos1, size_type count1, const_data_pointer bytes, size_type count2)
{
	PFS_ASSERT(pos1 <= this->length());
	PFS_ASSERT(pos1 + count1 <= this->length());

	base_class::detach();

	if (this->isEmpty()) {
		// Target is empty, assign to replacement string
		byte_string r(bytes, count2);
		this->swap(r);
	} else {
		base_class::cast()->replace(constData() + pos1, count1, bytes, count2);
	}

	return *this;
}

byte_string & byte_string::replace (size_type pos1, size_type count1, const char * s)
{
	return replace(pos1, count1, s, strlen(s));
}

byte_string & byte_string::replace (
		  const_iterator first
		, const_iterator last
		, const_iterator first2
		, const_iterator last2
		, extra_trait<const_iterator>)
{
	PFS_ASSERT(first.holder() == this);
	PFS_ASSERT(last.holder() == this);
	PFS_ASSERT(first2.holder() == last2.holder());
	PFS_ASSERT(first <= last);
	PFS_ASSERT(first2 <= last2);

	const_data_pointer ptr1_begin = first.base().base();
	const_data_pointer ptr1_end   = last.base().base();

	const_data_pointer ptr2_begin = first2.base().base();
	const_data_pointer ptr2_end   = last2.base().base();

	base_class::detach();

	impl_class * d = base_class::cast();
	d->replace(ptr1_begin, ptr1_end - ptr1_begin, ptr2_begin, ptr2_end - ptr2_begin);

	return *this;
}

byte_string byte_string::substr (size_type index, size_type count) const
{
	PFS_ASSERT(index <= length());

	byte_string r;

	if (index < length()) {
		if (index + count > length()) {
			count = length() - index;
		}
		r.append (*this, index, count);
	}
	return r;
}

} // pfs
