/*
 * mbcs_string_inc.hpp
 *
 *  Created on: 13 нояб. 2014 г.
 *      Author: wladt
 */

#ifndef __PFS_MBCS_STRING_HPP__
#	error Do not include this file
#endif

namespace pfs {

template <typename _CodeUnitT>
mbcs_string<_CodeUnitT>::mbcs_string (const char * latin1)
	: base_class()
{
	if (latin1) {
		*this = fromLatin1(latin1, strlen(latin1));
	}
}

template <typename _CodeUnitT>
mbcs_string<_CodeUnitT>::mbcs_string (const char * latin1, size_t n)
{
	if (latin1) {
		*this = fromLatin1(latin1, n);
	}
}

template <typename _CodeUnitT>
mbcs_string<_CodeUnitT>::mbcs_string (size_t count, char latin1)
{
	if (byte_t(latin1) > 127)
		latin1 = ReplacementChar;

	impl_class * d = base_class::cast();
	d->reserve(count);
	for (size_t i = 0; i < count; ++i) {
		d->append(& latin1, 1);
		//(*d)[i] = latin1;
	}
	d->_length = count;
}

template <typename _CodeUnitT>
mbcs_string<_CodeUnitT>::mbcs_string (size_t count, ucchar ch)
{
	_CodeUnitT utf[10]; // 6 max need for UTF-8 character encoding

	if (!ucchar::isValid(ch))
		ch = ucchar(ucchar::ReplacementChar);

	size_t sz = ch.encode<_CodeUnitT>(utf, 10);
	impl_class * d = base_class::cast();
	d->reserve(count * 4);

	for (size_t i = 0; i < count; ++i) {
		d->append(utf, size_t(sz));
	}

	d->_length = count;
}

template <typename _CodeUnitT>
mbcs_string<_CodeUnitT>::mbcs_string (const_iterator first, const_iterator last)
{
	impl_class * d = base_class::cast();

	// Convert `first' to offset in bytes
	size_type index_begin = first.base().index();

	// Convert `last' to offset in bytes
	size_type index_end = last.base().index();

	if (index_end >= index_begin) {
		const char * p = first.holder()->constData();
		d->insert(0, p + index_begin, index_end - index_begin);
		d->_length = impl_class::length(p + index_begin, p + index_end);
	}
}

/**
 *
 * @param pos1
 * @param count1
 * @param s
 * @param count2
 * @return
 */
template <typename _CodeUnitT>
int mbcs_string<_CodeUnitT>::compare (size_type pos1, size_type count1, const char * latin1, size_type count2) const
{
	PFS_ASSERT(pos1 <= this->length());
	PFS_ASSERT(pos1 + count1 <= this->length());

	if (this->isEmpty()) {
		return latin1 == nullptr ? 0 : -1;
	}

	if (latin1 == nullptr) {
		return this->isEmpty() ? 0 : 1;
	}

	const impl_class * d = base_class::cast();
	const char * ptr_begin = impl_class::increment(constData(), pos1);
	const char * ptr_end = impl_class::increment(ptr_begin, count1);
	return d->compare(ptr_begin, ptr_end - ptr_begin, latin1, count2);
}

/**
 *
 * @param pos1
 * @param count1
 * @param s
 * @param pos2
 * @param count2
 * @return
 */
template <typename _CodeUnitT>
int mbcs_string<_CodeUnitT>::compare (size_type pos1, size_type count1
		, const mbcs_string<_CodeUnitT> & s, size_type pos2, size_type count2) const
{
	PFS_ASSERT(pos1 <= this->length());
	PFS_ASSERT(pos1 + count1 <= this->length());
	PFS_ASSERT(pos2 <= s.length());
	PFS_ASSERT(pos2 + count2 <= s.length());

	if (this->isEmpty()) {
		return s.isEmpty() ? 0 : -1;
	}

	if (s.isEmpty()) {
		return this->isEmpty() ? 0 : 1;
	}

	const impl_class * d = base_class::cast();
	const char * ptr1_begin = impl_class::increment(constData(), pos1);
	const char * ptr1_end   = impl_class::increment(ptr1_begin, count1);

	const char * ptr2_begin = impl_class::increment(s.constData(), pos2);
	const char * ptr2_end   = impl_class::increment(ptr2_begin, count2);

	return d->compare(ptr1_begin, ptr1_end - ptr1_begin, ptr2_begin, ptr2_end - ptr2_begin);
}

/**
 *
 * @param s
 * @return
 */
template <typename _CodeUnitT>
int mbcs_string<_CodeUnitT>::compare (const char * latin1) const
{
	if (this->isEmpty()) {
		return latin1 == nullptr ? 0 : -1;
	}

	if (latin1 == nullptr) {
		return this->isEmpty() ? 0 : 1;
	}

	return base_class::cast()->compare(this->constData(), this->size(), latin1, strlen(latin1));
}


template <typename _CodeUnitT>
inline int mbcs_string<_CodeUnitT>::compare (size_type pos, size_type count, const char * latin1) const
{
	return compare(pos, count, latin1, strlen(latin1));
}

/**
 *
 * @param index
 * @param count
 * @return
 */
template <typename _CodeUnitT>
mbcs_string<_CodeUnitT> & mbcs_string<_CodeUnitT>::erase (size_type index, size_type count)
{
	if (isEmpty() || index >= length())
		return *this;

	base_class::detach();
	impl_class * d = base_class::cast();

	// Convert `index' to offset in bytes
	size_type pos = impl_class::size(this->constData(), difference_type(index));
	size_type n = 0;

	if (index + count >= length()) {
		n = size() - pos;
		count = length() - index;
	} else {
		size_type pos_end = pos + impl_class::size(this->constData() + pos, difference_type(count));
		n = pos_end - pos;
	}

	d->erase(pos, n);
	d->_length -= count;

	return *this;
}

template <typename _CodeUnitT>
typename mbcs_string<_CodeUnitT>::iterator mbcs_string<_CodeUnitT>::erase (const_iterator first, const_iterator last)
{
	PFS_ASSERT(first.holder() == this);
	PFS_ASSERT(first.holder() == last.holder());
	PFS_ASSERT(first >= cbegin());
	PFS_ASSERT(last <= cend());

	if (isEmpty())
		return iterator(this, pointer(*this, 0));

	size_type pos_begin = first.base().index();
	size_type pos_end = last.base().index();

	if (pos_begin < pos_end) {
		base_class::detach();
		impl_class * d = base_class::cast();
		size_type count = last - first;
		d->erase(pos_begin, pos_end - pos_begin);
		d->_length -= count;
	}

	return iterator(this, pointer(*this, pos_begin));
}


template <typename _CodeUnitT>
mbcs_string<_CodeUnitT> & mbcs_string<_CodeUnitT>::insert (size_type index, const mbcs_string<_CodeUnitT> & str, size_type index_str, size_type count)
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
		d->_length += str.length();
		return *this;
	}


	if (index_str + count > str.length())
		count = str.length() - index_str;

	// Convert `index' to offset in bytes
	size_type pos = impl_class::size(this->constData(), difference_type(index));

	// Convert `index_str' to offset in bytes
	size_type pos_str_begin = impl_class::size(str.constData(), difference_type(index_str));

	// Convert `index_str' + `count' to offset in bytes
	size_type pos_str_end = impl_class::size(str.constData(), difference_type(index_str + count));


	d->insert(pos, str.constData() + pos_str_begin, pos_str_end - pos_str_begin);
	d->_length = this->length() + count;

	return *this;
}

template <typename _CodeUnitT>
typename mbcs_string<_CodeUnitT>::iterator mbcs_string<_CodeUnitT>::insert (
		  const_iterator pos
		, const_iterator first
		, const_iterator last
		, mbcs_string_type_trait<const_iterator>)
{
	PFS_ASSERT(first.holder() == last.holder());
	PFS_ASSERT(pos.holder() == this);

	mbcs_string<_CodeUnitT> s(first, last);
	size_type index = pos.base().index();

	if (first >= last)
		return iterator(this, pointer(*this, index));

	base_class::detach();

	if (this->isEmpty()) {
		(*this) = s;
		return iterator(this, pointer(*this, 0));
	}

	impl_class * d = base_class::cast();
	d->insert(index, s.constData(), s.size());
	d->_length += s.length();
	return iterator(this, pointer(*this, index + 1));
}


/**
 * @brief
 *
 * @param index
 * @param count
 * @return
 */
template <typename _CodeUnitT>
mbcs_string<_CodeUnitT> mbcs_string<_CodeUnitT>::substr (size_type index, size_type count) const
{
	PFS_ASSERT(index <= length());

	mbcs_string<_CodeUnitT> r;

	if (index < length()) {
		if (index + count > length()) {
			count = length() - index;
		}
		r.append (*this, index, count);
	}
	return r;
}

template <typename _CodeUnitT>
mbcs_string<_CodeUnitT> & mbcs_string<_CodeUnitT>::replace (
	  size_type pos1
	, size_type count1
	, const mbcs_string & s
	, size_type pos2
	, size_type count2)
{
	PFS_ASSERT(pos1 <= this->length());
	PFS_ASSERT(pos1 + count1 <= this->length());
	PFS_ASSERT(pos2 <= s.length());
	PFS_ASSERT(pos2 + count2 <= s.length());

	base_class::detach();

	if (this->isEmpty()) {
		// Target is empty, assign to replacement string
		mbcs_string<_CodeUnitT> r(s.cbegin() + pos2, s.cbegin() + (pos2 + count2));
		this->swap(r);
	} else {
		const_data_pointer ptr1_begin = impl_class::increment(constData(), pos1);
		const_data_pointer ptr1_end   = impl_class::increment(ptr1_begin, count1);

		const_data_pointer ptr2_begin = impl_class::increment(s.constData(), pos2);
		const_data_pointer ptr2_end   = impl_class::increment(ptr2_begin, count2);

		impl_class * d = base_class::cast();
		d->replace(ptr1_begin, ptr1_end - ptr1_begin, ptr2_begin, ptr2_end - ptr2_begin);

		if (count2 > count1)
			d->_length += count2 - count1;
		else
			d->_length -= count1 - count2;
	}

	return *this;
}


template <typename _CodeUnitT>
mbcs_string<_CodeUnitT> & mbcs_string<_CodeUnitT>::replace (
		  const_iterator first
		, const_iterator last
		, const_iterator first2
		, const_iterator last2
		, mbcs_string_type_trait<const_iterator>)
{
	PFS_ASSERT(first.holder() == this);
	PFS_ASSERT(first.holder() == last.holder());
	PFS_ASSERT(first2.holder() == last2.holder());
	PFS_ASSERT(first <= last);
	PFS_ASSERT(first2 <= last2);

	const_data_pointer ptr1_begin = first.base().base();
	const_data_pointer ptr1_end   = last.base().base();

	const_data_pointer ptr2_begin = first2.base().base();
	const_data_pointer ptr2_end   = last2.base().base();

	size_type count1 = last - first;
	size_type count2 = last2 - first2;

	base_class::detach();

	impl_class * d = base_class::cast();
	d->replace(ptr1_begin, ptr1_end - ptr1_begin, ptr2_begin, ptr2_end - ptr2_begin);

	if (count2 > count1)
		d->_length += count2 - count1;
	else
		d->_length -= count1 - count2;

	return *this;
}

template <typename _CodeUnitT>
typename mbcs_string<_CodeUnitT>::iterator mbcs_string<_CodeUnitT>::find (const_iterator pos, const mbcs_string & str) const
{
	PFS_ASSERT(pos >= cbegin());
	PFS_ASSERT(pos <= cend());

	size_type r = 0;
	size_type index = pos.base().index();

	mbcs_string * self = const_cast<mbcs_string *>(this);

	return base_class::cast()->find(index, str.constData(), str.size(), r)
		? iterator(self, pointer(*self, r))
		: iterator(self, pointer(*self, size())); // end()
}

template <typename _CodeUnitT>
stringlist_basic<mbcs_string<_CodeUnitT> > mbcs_string<_CodeUnitT>::split (
		  bool isOneSeparatorChar
		, const mbcs_string & separator
		, bool keepEmpty
		, ucchar quoteChar) const
{
	stringlist_basic<mbcs_string> r;
	const_iterator it = cbegin();
	const_iterator itBegin = it;
	const_iterator itEnd = cend();
	size_t separatorLength = separator.length();
	bool quote = false;

	while (it != itEnd) {
		if (quoteChar != ucchar::Null && *it == quoteChar) {
			if (quote) {
				quote = false;
			} else {
				quote = true;
			}
			++it;
		} else if (!quote
				&& ((isOneSeparatorChar && separator.contains(*it))
					|| (!isOneSeparatorChar && this->startsWith(it, separator)))) {
			mbcs_string s(itBegin, it);
			if (!s.isEmpty()) {
				r.append(s);
			} else {
				if (keepEmpty)
					r.append(s);
			}
			it += isOneSeparatorChar ? 1 : separatorLength;
			itBegin = it;
		} else {
			++it;
		}
	}

	if (itBegin != it || itBegin == itEnd) {
		mbcs_string s(itBegin, it);
		if (!s.isEmpty()) {
			r.append(s);
		} else {
			if (keepEmpty)
				r.append(s);
		}
	}

	if (r.isEmpty())
		r.append(*this);


	PFS_VERIFY_X(!quote, "Unbalanced quote");
	if (quote)
		r.clear();

	return r;
}


} // pfs
