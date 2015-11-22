/*
 * bytes_string.cpp
 *
 *  Created on: 23 дек. 2014 г.
 *      Author: wladt
 */

#include <pfs/byte_string.hpp>
//#include <pfs/string.hpp>
//#include <pfs/bits/strtointegral.hpp>
//#include <pfs/bits/strtoreal.hpp>
//#include <cstring>

namespace pfs {

byte_string & base64_encode (const byte_string & src, byte_string & result)
{
    static const char alphabet[] = "ABCDEFGH" "IJKLMNOP" "QRSTUVWX" "YZabcdef"
		            "ghijklmn" "opqrstuv" "wxyz0123" "456789+/";
    static const char padchar = '=';
    int padlen = 0;
    size_t sz = src.size();

    result.clear();
    result.reserve((sz * 4) / 3 + 3);

    size_t i = 0;
    byte_string::const_pointer d = src.data();

    while (i < sz) {
		int chunk = 0;
			chunk |= int(d[i++]) << 16;
		if (i == sz) {
			padlen = 2;
		} else {
			chunk |= int(d[i++]) << 8;

			if (i == sz)
				padlen = 1;
			else
				chunk |= int(d[i++]);
		}

		int j = (chunk & 0x00fc0000) >> 18;
		int k = (chunk & 0x0003f000) >> 12;
		int l = (chunk & 0x00000fc0) >> 6;
		int m = (chunk & 0x0000003f);

		result.append(1, alphabet[j]);
		result.append(1, alphabet[k]);

		if (padlen > 1)
			result.append(1, padchar);
		else
			result.append(1, alphabet[l]);
		if (padlen > 0)
			result.append(1, padchar);
		else
			result.append(1, alphabet[m]);
    }

    return result;
}

byte_string & base64_decode (const byte_string & src, byte_string & result)
{
    size_t sz = src.size();

    result.clear();
    result.reserve((sz * 3) / 4);

    unsigned int buf = 0;
    int nbits = 0;

    for (size_t i = 0; i < sz; ++i) {
		int ch = src.at(i);
		int d = -1;

		if (ch >= 'A' && ch <= 'Z')
			d = ch - 'A';
		else if (ch >= 'a' && ch <= 'z')
			d = ch - 'a' + 26;
		else if (ch >= '0' && ch <= '9')
			d = ch - '0' + 52;
		else if (ch == '+')
			d = 62;
		else if (ch == '/')
			d = 63;

		if (d != -1) {
			buf = (buf << 6) | d;
			nbits += 6;

			if (nbits >= 8) {
				nbits -= 8;
				result.push_back(buf >> nbits);
				buf &= (1 << nbits) - 1;
			}
		}
    }

    result.resize(result.size());
    return result;
}


#if __COMMENT__
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

// pfs::reference class requirement
void byte_string::detach_and_assign (pointer & p, const value_type & value)
{
	const_data_pointer first = constData();
	difference_type offset = p.base() - first;
	base_class::detach();
	base_class::cast()->replace(offset, value);
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
		return (bytes == nullptr) ? 0 : -1;
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

integral_t byte_string::toIntegral (bool * ok, int base) const
{
	if (isNull()) {
		if (ok) *ok = false;
		return 0;
	}
	const char * begin = reinterpret_cast<const char *>(constData());
	const char * end = reinterpret_cast<const char *>(constData() + size());

	return strtointegral_helper<char, const char *>
		(begin, end, ok, base
		, integral_t(pfs::min_type<integral_t>())
		, uintegral_t(pfs::max_type<integral_t>()));
}

uintegral_t byte_string::toUIntegral (bool * ok, int base) const
{
	if (isNull()) {
		if (ok) *ok = false;
		return 0;
	}

	const char * begin = reinterpret_cast<const char *>(constData());
	const char * end = reinterpret_cast<const char *>(constData() + size());

	return strtouintegral_helper<char, const char *>
		(begin, end, ok, base
		, uintegral_t(pfs::max_type<unsigned long long>()));
}

short byte_string::toShort (bool * ok, int base) const
{
	if (isNull()) {
		if (ok) *ok = false;
		return 0;
	}

	const char * begin = reinterpret_cast<const char *>(constData());
	const char * end = reinterpret_cast<const char *>(constData() + size());
	return (short)strtointegral_helper<char, const char *>
		(begin, end, ok, base, integral_t(pfs::min_type<short>()), uintegral_t(pfs::max_type<short>()));
}

unsigned short byte_string::toUShort (bool * ok, int base) const
{
	if (isNull()) {
		if (ok) *ok = false;
		return 0;
	}

	const char * begin = reinterpret_cast<const char *>(constData());
	const char * end = reinterpret_cast<const char *>(constData() + size());
	return (unsigned short)strtouintegral_helper<char, const char *>
		(begin, end, ok, base, uintegral_t(pfs::max_type<unsigned short>()));
}

int	byte_string::toInt (bool * ok, int base) const
{
	if (isNull()) {
		if (ok) *ok = false;
		return 0;
	}

	const char * begin = reinterpret_cast<const char *>(constData());
	const char * end = reinterpret_cast<const char *>(constData() + size());
	return (int)strtointegral_helper<char, const char *>
		(begin, end, ok, base
		, integral_t(pfs::min_type<int>())
		, uintegral_t(pfs::max_type<int>()));
}

unsigned int byte_string::toUInt (bool * ok, int base) const
{
	if (isNull()) {
		if (ok) *ok = false;
		return 0;
	}

	const char * begin = reinterpret_cast<const char *>(constData());
	const char * end = reinterpret_cast<const char *>(constData() + size());
	return (unsigned int)strtouintegral_helper<char, const char *>
		(begin, end, ok, base
		, uintegral_t(pfs::max_type<unsigned int>()));
}

long byte_string::toLong (bool * ok, int base) const
{
	if (isNull()) {
		if (ok) *ok = false;
		return 0;
	}

	const char * begin = reinterpret_cast<const char *>(constData());
	const char * end = reinterpret_cast<const char *>(constData() + size());

	return (long)strtointegral_helper<char, const char *>
		(begin, end, ok, base
		, integral_t(pfs::min_type<long>()), uintegral_t(pfs::max_type<long>()));
}

unsigned long byte_string::toULong (bool * ok, int base) const
{
	if (isNull()) {
		if (ok) *ok = false;
		return 0;
	}

	const char * begin = reinterpret_cast<const char *>(constData());
	const char * end = reinterpret_cast<const char *>(constData() + size());

	return (unsigned long)strtouintegral_helper<char, const char *>
		(begin, end, ok, base
		, uintegral_t(pfs::max_type<unsigned long>()));
}

#ifdef PFS_HAVE_LONGLONG
long long byte_string::toLongLong (bool * ok, int base) const
{
	if (isNull()) {
		if (ok) *ok = false;
		return 0;
	}

	const char * begin = reinterpret_cast<const char *>(constData());
	const char * end = reinterpret_cast<const char *>(constData() + size());

	return (long long)strtointegral_helper<char, const char *>
		(begin, end, ok, base
		, integral_t(pfs::min_type<long long>())
		, uintegral_t(pfs::max_type<long long>()));
}

unsigned long long byte_string::toULongLong (bool * ok, int base) const
{
	if (isNull()) {
		if (ok) *ok = false;
		return 0;
	}

	const char * begin = reinterpret_cast<const char *>(constData());
	const char * end = reinterpret_cast<const char *>(constData() + size());

	return (unsigned long long)strtouintegral_helper<char, const char *>
		(begin, end, ok, base
		, uintegral_t(pfs::max_type<unsigned long long>()));
}
#endif


/**
 * @brief Converts the byte string to a real value.
 *
 * @param ok If a conversion error occurs, *ok is set to false; otherwise *ok is set to true.
 * @param decimalPoint The separator between integral and fractal parts.
 * @return The byte string converted to a real value. Returns 0.0 if the conversion fails.
 */
real_t byte_string::toReal (bool * ok, char decimalPoint) const
{
	if (isNull()) {
		if (ok) *ok = false;
		return real_t(.0);
	}

	const char * endptr;
	const char * begin = reinterpret_cast<const char *>(constData());
	const char * end = begin + size();
	bool ok1 = true;

	real_t r = pfs::strtoreal<char, const char *>(begin, end, decimalPoint, & endptr);
	if (errno || endptr != end) {
		ok1 = false;
		r = real_t(0.0f);
	}

	if (ok) *ok = ok1;
	return r;
}

float byte_string::toFloat (bool * ok, char decimalPoint) const
{
	if (isNull()) {
		if (ok) *ok = false;
		return float(.0);
	}

	bool ok1;
	real_t r = toReal(& ok1, decimalPoint);
	if (!ok1 || r < pfs::min_type<float>() || r > pfs::max_type<float>()) {
		ok1 = false;
		r = float(0.0f);
	}
	if (ok) *ok = ok1;
	return float(r);
}

double byte_string::toDouble (bool * ok, char decimalPoint) const
{
	if (isNull()) {
		if (ok) *ok = false;
		return double(.0);
	}

#ifndef PFS_HAVE_LONG_DOUBLE
	return toReal(ok, decimalPoint);
#else
	bool ok1;
	real_t r = toReal(& ok1, decimalPoint);
	if (!ok1 || r < pfs::min_type<double>() || r > pfs::max_type<double>()) {
		ok1 = false;
		r = double(0.0f);
	}
	if (ok) *ok = ok1;
	return double(r);
#endif
}


//
// TODO
//
template <>
byte_string::size_type byte_string::readNumber<float> (float & v, size_t pos, endian::type_enum order) const
{
	PFS_UNUSED3(v, pos, order);
	PFS_ASSERT_TODO();
	return 0;
}

//
// TODO
//
template <>
byte_string::size_type byte_string::readNumber<double> (double & v, size_t pos, endian::type_enum order) const
{
	PFS_UNUSED3(v, pos, order);
	PFS_ASSERT_TODO();
	return 0;
}

#ifdef PFS_HAVE_LONG_DOUBLE
//
// TODO
//
template <>
byte_string::size_type byte_string::readNumber<long double> (long double & v, size_t pos, endian::type_enum order) const
{
	PFS_UNUSED3(v, pos, order);
	PFS_ASSERT_TODO();
	return 0;
}
#endif // PFS_HAVE_LONG_DOUBLE

#endif // __COMMENT__

} // pfs
