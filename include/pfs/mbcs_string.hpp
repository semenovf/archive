/*
 * mbcs_string.hpp
 *
 *      Author: wladt
 */

#ifndef __PFS_MBCS_STRING_HPP__
#define __PFS_MBCS_STRING_HPP__

#include <pfs/pimpl.hpp>
#include <pfs/bits/iterator.hpp>
#include <pfs/endian.hpp>
#include <pfs/ucchar.hpp>
#include <pfs/bits/mbcs_string_impl.hpp>
#include <pfs/byte_string.hpp>
#include <pfs/bits/strtointegral.hpp>
#include <pfs/bits/strtoreal.hpp>
#include <pfs/bits/stringlist.hpp>
#include <ostream>

// See http://www.unknownroad.com/rtfm/VisualStudio/warningC4251.html
#ifdef PFS_CC_MSVC
#	pragma warning(push)
#	pragma warning(disable:4251)
#endif

namespace pfs {

template <typename T>
struct mbcs_string_type_trait { typedef T type; };

//TODO implement `class utf16string : protected vector<int16_t,allocator<int16_t, 1> >'

template <typename CodeUnitT>
class mbcs_string : public nullable<mbcs_string_impl<CodeUnitT> >
{
protected:
	typedef nullable<mbcs_string_impl<CodeUnitT> > base_class;
	typedef mbcs_string                            self_class;
	typedef mbcs_string_impl<CodeUnitT>            impl_class;

public:
	typedef ucchar value_type; // Unicode character
	typedef typename impl_class::size_type                size_type;
	typedef typename impl_class::difference_type          difference_type;

	typedef mbcs_string_ptr<CodeUnitT, self_class>       pointer;
	typedef mbcs_string_ptr<CodeUnitT, const self_class> const_pointer;

	typedef pfs::reference<self_class>                    reference;
	typedef pfs::reference<const self_class>              const_reference;
	typedef pfs::random_access_iterator<self_class>       iterator;
	typedef pfs::random_access_iterator<const self_class> const_iterator;
	typedef std::reverse_iterator<iterator>		          reverse_iterator;
	typedef std::reverse_iterator<const_iterator>         const_reverse_iterator;

	typedef value_type char_type;

	typedef typename impl_class::const_pointer const_data_pointer;

public: // static
	static const char TerminatorChar  = '\0';
	static const char EndOfLineChar   = '\n';
	static const char ReplacementChar = '?';

public:
	mbcs_string () : base_class() {}
	explicit mbcs_string (const char * latin1);
	mbcs_string (const char * latin1, size_t n);
	mbcs_string (size_t count, char latin1);
	mbcs_string (size_t count, ucchar ch);
	mbcs_string (const_iterator first, const_iterator last);

	virtual ~mbcs_string () {}

	bool isEmpty () const { return base_class::isNull() || size() == 0; }
	bool empty () const { return isEmpty(); }
	void clear ()
	{
		if (!isEmpty()) {
			base_class::detach();
			base_class::cast()->clear();
		}
	}

	mbcs_string & erase (size_type index = 0) { return erase(index, 1); }
	mbcs_string & erase (size_type index, size_type count);
	iterator erase (const_iterator pos) { return erase(pos, pos + 1); }
	iterator erase (const_iterator first, const_iterator last);

    iterator begin ()                       { return iterator(this, pointer(*this, 0)); }
    iterator end   ()                       { return iterator(this, pointer(*this, size())); }
    const_iterator begin () const           { return cbegin(); }
    const_iterator end   () const           { return cend(); }
    const_iterator cbegin() const           { return const_iterator(this, const_pointer(*this, 0)); }
    const_iterator cend  () const           { return const_iterator(this, const_pointer(*this, size())); }
    reverse_iterator rbegin  ()             { return reverse_iterator(end()); }
    reverse_iterator rend    ()             { return reverse_iterator(begin()); }
    const_reverse_iterator rbegin  () const { return crbegin(); }
    const_reverse_iterator rend    () const { return crend(); }
    const_reverse_iterator crbegin () const { return const_reverse_iterator(cend()); }
    const_reverse_iterator crend   () const { return const_reverse_iterator(cbegin()); }

    const_data_pointer constData () const { return base_class::isNull() ? nullptr : base_class::cast()->constData(); }
    const_data_pointer data () const      { return base_class::isNull() ? nullptr : base_class::cast()->constData(); }
    const_data_pointer c_str () const     { return base_class::isNull() ? ""/*nullptr*/ : base_class::cast()->constData(); }

    value_type valueAt (size_type index) const { return at(index); }
    value_type charAt (size_type index) const { return at(index); }
    reference at (size_type index) const { pointer p(*const_cast<self_class *>(this), 0); p += index; return p.ref(); }
    reference operator [] (size_type index) const { return at(index); }

    iterator find (const_iterator pos, const mbcs_string & str) const;

    iterator find (const mbcs_string & str) const
    {
    	return find(cbegin(), str);
    }

    iterator find (const_iterator pos, const char * latin1, size_type count) const
    {
    	return find(pos, mbcs_string::fromLatin1(latin1, count));
    }

    iterator find (const_iterator pos, const char * latin1) const
    {
    	return find(pos, mbcs_string::fromLatin1(latin1, strlen(latin1)));
    }

    iterator find (const char * latin1) const
    {
    	return find(cbegin(), latin1);
    }

    iterator find (const_iterator pos, char latin1) const
    {
    	return find(pos, & latin1, 1);
    }

    iterator find (char latin1) const
    {
    	return find(cbegin(), latin1);
    }

    iterator find (const_iterator pos, ucchar ch) const
    {
    	return find(pos, mbcs_string(1, ch));
    }

    iterator find (ucchar ch) const
    {
    	return find(cbegin(), ch);
    }

	bool contains   (const mbcs_string & s) const { return find(s) != cend(); }
	bool contains   (const char * s) const        { return find(s) != cend(); }
	bool contains   (ucchar v) const              { return find(v) != cend(); }
	bool contains   (char v) const                { return find(v) != cend(); }

	// TODO Need tests
	// FIXME need optimization like startsWith (char v)
	bool startsWith (const mbcs_string & s)  const { return find(s) == cbegin(); }
	bool startsWith (const_iterator pos, const mbcs_string & s) const
	{
		return find(pos, s) == pos;
	}
	// FIXME need optimization like startsWith (char v)
	bool startsWith (const char * s) const         { return find(s) == cbegin(); }

	bool startsWith (ucchar v) const               { return isEmpty() ? false : *(cbegin()) == v; }
	bool startsWith (char v) const                 { return isEmpty() ? false : *(cbegin()) == ucchar(v); }

	// TODO Need tests
	bool endsWith   (const mbcs_string & s) const  { return find(s) == cend() - s.length(); }
	bool endsWith   (const char * s) const         { return find(s) == cend() - std::strlen(s); }
	bool endsWith   (ucchar v) const               { return find(v) == cend() - 1; }
	bool endsWith   (char v) const                 { return find(v) == cend() - 1; }

    // Size in bytes
    size_type size () const
    {
    	return base_class::isNull() ? 0 : base_class::cast()->size();
    }

    // Length in unicode chars
    size_type length () const
    {
    	return base_class::isNull() ? 0 : base_class::cast()->length();
    }

    size_type capacity() const
    {
    	return base_class::cast()->capacity();
    }

    size_type max_size() const
    {
    	return base_class::cast()->max_size();
    }

    int compare (size_type pos1, size_type count1, const char * s, size_type count2) const;
    int compare (size_type pos1, size_type count1, const mbcs_string & s) const
    {
    	return compare(pos1, count1, s.constData(), s.size());
    }

    int compare (const mbcs_string & s) const
    {
    	return compare(s.constData());
    }

    int compare (size_type pos1, size_type count1, const mbcs_string & s, size_type pos2, size_type count2) const;
    int compare (const char * s) const;
    int compare (size_type pos, size_type count, const char * s) const;

    void push_back (value_type ch)
    {
    	append(size_type(1), ch);
    }

    void pop_back ()
    {
    	if (!isEmpty())
    		erase(length() - 1);
    }

    mbcs_string & append (const mbcs_string & str, size_type index_str, size_type count)
    {
    	return insert(this->length(), str, index_str, count);
    }

    mbcs_string & append (size_type count, char latin1)
    {
    	return insert(this->length(), count, latin1);
    }

    mbcs_string & append (size_type count, ucchar uc)
    {
    	return insert(this->length(), count, uc);
    }

    mbcs_string & append (const char * latin1)
    {
    	return insert(this->length(), latin1);
    }

    mbcs_string & append (const char * latin1, size_type n)
    {
    	return insert(this->length(), latin1, n);
    }

    mbcs_string & append (const mbcs_string & str)
    {
    	return insert(this->length(), str);
    }

//    template<class InputIt>
//    mbcs_string & append (InputIt first, InputIt last);

    mbcs_string & prepend (const mbcs_string & str, size_type index_str, size_type count)
    {
    	return insert(0, str, index_str, count);
    }

    mbcs_string & prepend (size_type count, char latin1)
    {
    	return insert(0, count, latin1);
    }

    mbcs_string & prepend (size_type count, ucchar uc)
    {
    	return insert(0, count, uc);
    }

    mbcs_string & prepend (const char * latin1)
    {
    	return insert(0, latin1);
    }

    mbcs_string & prepend (const char * latin1, size_type n)
    {
    	return insert(0, latin1, n);
    }

    mbcs_string & prepend (const mbcs_string & str)
    {
    	return insert(0, str);
    }

//    template<class InputIt>
//    mbcs_string & prepend (InputIt first, InputIt last)


    mbcs_string & insert (size_type index, const mbcs_string & str, size_type index_str, size_type count);

    mbcs_string & insert (size_type index, size_type count, char latin1)
    {
    	return insert(index, self_class(count, latin1));
    }

    mbcs_string & insert (size_type index, size_type count, ucchar uc)
    {
    	return insert(index, mbcs_string(count, uc));
    }

    mbcs_string & insert (size_type index, const char * latin1)
    {
    	return insert(index, mbcs_string(latin1));
    }

    mbcs_string & insert (size_type index, const char * latin1, size_type n)
    {
    	return insert(index, mbcs_string(latin1, n));
    }

    mbcs_string & insert (size_type index, const mbcs_string & str)
    {
    	return insert(index, str, 0, str.length());
    }

    iterator insert (const_iterator pos, char latin1)
    {
    	mbcs_string s(1, latin1);
    	return insert(pos, s.cbegin(), s.cend());
    }

    iterator insert (const_iterator pos, ucchar ch)
    {
    	mbcs_string s(1, ch);
    	return insert(pos, s.cbegin(), s.cend());
    }

    iterator insert (const_iterator pos, size_type count, char latin1)
    {
    	mbcs_string s(count, latin1);
    	return insert(pos, s.cbegin(), s.cend());
    }

    iterator insert (const_iterator pos, size_type count, ucchar ch)
    {
    	mbcs_string s(count, ch);
    	return insert(pos, s.cbegin(), s.cend());
    }

    template<class InputIt>
    iterator insert (const_iterator pos, InputIt first, InputIt last)
    {
    	return insert(pos, first, last, mbcs_string_type_trait<InputIt>());
    }

    mbcs_string & replace (size_type pos1, size_type count1, const mbcs_string & str, size_type pos2, size_type count2);

    mbcs_string & replace (size_type pos, size_type count, const mbcs_string & str)
    {
    	return replace(pos, count, str, 0, str.length());
    }

    mbcs_string & replace (const_iterator first, const_iterator last, const mbcs_string & str)
    {
    	return replace(first, last, str.cbegin(), str.cend(), mbcs_string_type_trait<const_iterator>());
    }

    template< class InputIt >
    mbcs_string & replace (const_iterator first, const_iterator last, InputIt first2, InputIt last2)
    {
    	return replace(first, last, first2, last2, mbcs_string_type_trait<InputIt>());
    }

    mbcs_string & replace (size_type pos, size_type count, size_type count2, ucchar ch)
    {
    	return replace (pos, count, mbcs_string(count2, ch));
    }

    mbcs_string & replace (const_iterator first, const_iterator last, size_type count2, ucchar ch)
    {
    	return replace(first, last, mbcs_string(count2, ch));
    }

	mbcs_string substr (size_type index, size_type count) const;

	stringlist_basic<mbcs_string> split (const mbcs_string & separator, bool keepEmpty = true, ucchar quoteChar = ucchar::Null) const
	{
		return split(false, separator, keepEmpty, quoteChar);
	}

	stringlist_basic<mbcs_string> split (const ucchar & separator, bool keepEmpty = true, ucchar quoteChar = ucchar::Null) const
	{
		return split(false, mbcs_string(1, separator), keepEmpty, quoteChar);
	}

	stringlist_basic<mbcs_string> splitOneOf (const mbcs_string & separators, bool keepEmpty = true, ucchar quoteChar = ucchar::Null) const
	{
		return split(true, mbcs_string(separators), keepEmpty, quoteChar);
	}

	integral_t toIntegral (bool * ok = 0, int base = 10) const;
	uintegral_t toUIntegral (bool * ok = 0, int base = 10) const;

	short          toShort  (bool * ok = 0, int base = 10) const;
	unsigned short toUShort (bool * ok = 0, int base = 10) const;
	int	           toInt    (bool * ok = 0, int base = 10) const;
	unsigned int   toUInt   (bool * ok = 0, int base = 10) const;
	long           toLong   (bool * ok = 0, int base = 10) const;
	unsigned long  toULong  (bool * ok = 0, int base = 10) const;

#ifdef PFS_HAVE_LONGLONG
	long long toLongLong (bool * ok = 0, int base = 10) const;
	unsigned long long toULongLong (bool * ok = 0, int base = 10) const;
#endif

	real_t toReal (bool * ok = 0, ucchar decimalPoint = ucchar('.')) const;
	float toFloat (bool * ok = 0, ucchar decimalPoint = ucchar('.')) const;
	double toDouble (bool * ok = 0, ucchar decimalPoint = ucchar('.')) const;

#ifdef PFS_HAVE_LONG_DOUBLE
	long double toLongDouble (bool * ok = 0, ucchar decimalPoint = ucchar('.')) const { return toReal(ok, decimalPoint); }
#endif

	mbcs_string & operator += (const mbcs_string & other) { return append(other); }
	mbcs_string & operator += (const char * latin1) { return append(mbcs_string(latin1)); }
	mbcs_string & operator += (const ucchar & ch) { return append(mbcs_string(1, ch)); }
	mbcs_string & operator += (char latin1) { return append(mbcs_string(1, latin1)); }

	mbcs_string & operator << (const mbcs_string & other) { return append(other); }
	mbcs_string & operator << (const char * latin1) { return append(mbcs_string(latin1)); }
	mbcs_string & operator << (const ucchar & ch) { return append(mbcs_string(1, ch)); }
	mbcs_string & operator << (char latin1) { return append(mbcs_string(1, latin1)); }

private:
    // This private methods need to fix "error: explicit specialization in non-namespace scope ‘class pfs::mbcs_string’"
    template <typename InputIt>
    iterator insert (const_iterator pos, InputIt first, InputIt last, mbcs_string_type_trait<InputIt>);
    iterator insert (const_iterator pos, const_iterator first, const_iterator last, mbcs_string_type_trait<const_iterator>);

    // This private methods need to fix "error: explicit specialization in non-namespace scope ‘class pfs::mbcs_string’"
    template <typename InputIt>
    mbcs_string & replace (const_iterator first, const_iterator last, InputIt first2, InputIt last2, mbcs_string_type_trait<InputIt>);
    mbcs_string & replace (const_iterator first, const_iterator last, const_iterator first2, const_iterator last2, mbcs_string_type_trait<const_iterator>);

    stringlist_basic<mbcs_string> split (bool isOneSeparatorChar, const mbcs_string & separator, bool keepEmpty = true, ucchar quoteChar = ucchar::Null) const;

public:
	struct ConvertState
	{
		ConvertState() : nremain(0), invalidChars(0), replacementChar(ucchar::ReplacementChar) {}
		size_type nremain;
		size_type invalidChars;
		ucchar    replacementChar;
	};

	static DLL_API mbcs_string fromLatin1 (const char * latin1, size_t n, ConvertState * state = nullptr);
	static DLL_API mbcs_string fromLatin1 (const char * latin1, ConvertState * state = nullptr);
	static mbcs_string fromLatin1 (const pfs::byte_string & latin1, ConvertState * state = nullptr)
	{
		return fromLatin1(latin1.constData(), latin1.length(), state);
	}

	static DLL_API mbcs_string fromUtf8 (const char * utf8, size_t size, ConvertState * state = nullptr);
	static DLL_API mbcs_string fromUtf8 (const char * utf8, ConvertState * state = nullptr);
	static mbcs_string fromUtf8 (const byte_string & utf8, ConvertState * state = nullptr)
	{
		return fromUtf8(reinterpret_cast<const char *>(utf8.constData()), utf8.length(), state);
	}

	static mbcs_string toString (signed char value, int base = 10, bool uppercase = false);
	static mbcs_string toString (short value, int base = 10, bool uppercase = false);
	static mbcs_string toString (int value, int base = 10, bool uppercase = false);
	static mbcs_string toString (long value, int base = 10, bool uppercase = false);
	static mbcs_string toString (unsigned char value, int base = 10, bool uppercase = false);
	static mbcs_string toString (unsigned short value, int base = 10, bool uppercase = false);
	static mbcs_string toString (unsigned int value, int base = 10, bool uppercase = false);
	static mbcs_string toString (unsigned long value, int base = 10, bool uppercase = false);

#ifdef PFS_HAVE_LONGLONG
	static mbcs_string toString (long long value, int base = 10, bool uppercase = false);
	static mbcs_string toString (unsigned long long value, int base = 10, bool uppercase = false);
#endif

	static mbcs_string toString (float value, char f = 'f', int prec = 6);
	static mbcs_string toString (double value, char f = 'f', int prec = 6);

#ifdef PFS_HAVE_LONG_DOUBLE
	static mbcs_string toString (long double value, char f = 'f', int prec = 6);
#endif
};

template <typename CodeUnitT>
template <typename InputIt>
typename mbcs_string<CodeUnitT>::iterator mbcs_string<CodeUnitT>::insert (const_iterator pos, InputIt first, InputIt last, mbcs_string_type_trait<InputIt>)
{
	PFS_ASSERT(pos.holder() == this);

	size_type index = pos.base().index();

	if (first >= last)
		return iterator(this, pointer(*this, index));

	for (InputIt it = first; it < last; ++it) {
		this->insert(pos++, *it);
	}

	return iterator(this, pointer(*this, index + 1));
}


template <typename CodeUnitT>
template <typename InputIt>
mbcs_string<CodeUnitT> & mbcs_string<CodeUnitT>::replace (
		  const_iterator first
		, const_iterator last
		, InputIt first2
		, InputIt last2
		, mbcs_string_type_trait<InputIt>)
{
	PFS_ASSERT(first.holder() == this);
	PFS_ASSERT(last.holder() == this);

	mbcs_string<CodeUnitT> s;

	for (InputIt it = first2; it < last2; ++it) {
		s.append(*it);
	}

	return replace(first, last, s);
}

template <typename CodeUnitT>
inline integral_t mbcs_string<CodeUnitT>::toIntegral (bool * ok, int base) const
{
	return (short)strtointegral_helper<mbcs_string<CodeUnitT>::char_type, mbcs_string<CodeUnitT>::const_iterator >
		(cbegin(), cend(), ok, base
		, integral_t(pfs::min_type<integral_t>())
		, uintegral_t(pfs::max_type<integral_t>()));
}

template <typename CodeUnitT>
inline uintegral_t mbcs_string<CodeUnitT>::toUIntegral (bool * ok, int base) const
{
	return (unsigned short)strtouintegral_helper<mbcs_string<CodeUnitT>::char_type, mbcs_string<CodeUnitT>::const_iterator >
		(cbegin(), cend(), ok, base
		, uintegral_t(pfs::max_type<uintegral_t>()));
}


template <typename CodeUnitT>
inline short mbcs_string<CodeUnitT>::toShort (bool * ok, int base) const
{
	return (short)strtointegral_helper<mbcs_string<CodeUnitT>::char_type, mbcs_string<CodeUnitT>::const_iterator >
		(cbegin(), cend(), ok, base
		, integral_t(pfs::min_type<short>())
		, uintegral_t(pfs::max_type<short>()));
}

template <typename CodeUnitT>
inline unsigned short mbcs_string<CodeUnitT>::toUShort (bool * ok, int base) const
{
	return (unsigned short)strtouintegral_helper<mbcs_string<CodeUnitT>::char_type, mbcs_string<CodeUnitT>::const_iterator >
		(cbegin(), cend(), ok, base
		, uintegral_t(pfs::max_type<unsigned short>()));
}

template <typename CodeUnitT>
inline int	mbcs_string<CodeUnitT>::toInt (bool * ok, int base) const
{
	return (int)strtointegral_helper<mbcs_string<CodeUnitT>::char_type, mbcs_string<CodeUnitT>::const_iterator >
		(cbegin(), cend(), ok, base
		, integral_t(pfs::min_type<int>())
		, uintegral_t(pfs::max_type<int>()));
}

template <typename CodeUnitT>
inline unsigned int mbcs_string<CodeUnitT>::toUInt (bool * ok, int base) const
{
	return (unsigned int)strtouintegral_helper<mbcs_string<CodeUnitT>::char_type, mbcs_string<CodeUnitT>::const_iterator >
		(cbegin(), cend(), ok, base
		, uintegral_t(pfs::max_type<unsigned int>()));
}

template <typename CodeUnitT>
inline long mbcs_string<CodeUnitT>::toLong (bool * ok, int base) const
{
	return (long)strtointegral_helper<mbcs_string<CodeUnitT>::char_type, mbcs_string<CodeUnitT>::const_iterator >
		(cbegin(), cend(), ok, base
		, integral_t(pfs::min_type<long>())
		, uintegral_t(pfs::max_type<long>()));
}

template <typename CodeUnitT>
inline unsigned long mbcs_string<CodeUnitT>::toULong (bool * ok, int base) const
{
	return (unsigned long)strtouintegral_helper<mbcs_string<CodeUnitT>::char_type, mbcs_string<CodeUnitT>::const_iterator >
		(cbegin(), cend(), ok, base
		, uintegral_t(pfs::max_type<unsigned long>()));
}

#ifdef PFS_HAVE_LONGLONG
template <typename CodeUnitT>
inline long long mbcs_string<CodeUnitT>::toLongLong (bool * ok, int base) const
{
	return (long long)strtointegral_helper<mbcs_string<CodeUnitT>::char_type, mbcs_string<CodeUnitT>::const_iterator >
		(cbegin(), cend(), ok, base
		, integral_t(pfs::min_type<long long>())
		, uintegral_t(pfs::max_type<long long>()));
}

template <typename CodeUnitT>
inline unsigned long long mbcs_string<CodeUnitT>::toULongLong (bool * ok, int base) const
{
	return (unsigned long long)strtouintegral_helper<mbcs_string<CodeUnitT>::char_type, mbcs_string<CodeUnitT>::const_iterator >
		(cbegin(), cend(), ok, base
		, uintegral_t(pfs::max_type<unsigned long long>()));
}
#endif


/**
 * @brief Converts the string to a real value.
 *
 * @param ok If a conversion error occurs, *ok is set to false; otherwise *ok is set to true.
 * @param decimalPoint The separator between integral and fractal parts.
 * @return The string converted to a real value. Returns 0.0 if the conversion fails.
 */
template <typename CodeUnitT>
real_t mbcs_string<CodeUnitT>::toReal (bool * ok, ucchar decimalPoint) const
{
	const_iterator begin(cbegin());
	const_iterator end(cend());
	const_iterator endptr(end);
	bool ok1 = true;

	real_t r = pfs::strtoreal<ucchar, const_iterator>(begin, end, decimalPoint, & endptr);
	if (errno || endptr != end) {
		ok1 = false;
		r = real_t(0.0f);
	}

	if (ok) *ok = ok1;
	return r;
}

template <typename CodeUnitT>
float mbcs_string<CodeUnitT>::toFloat (bool * ok, ucchar decimalPoint) const
{
	bool ok1;
	real_t r = toReal(& ok1, decimalPoint);
	if (!ok1 || r < PFS_FLOAT_MIN || r > PFS_FLOAT_MAX) {
		ok1 = false;
		r = float(0.0f);
	}
	if (ok) *ok = ok1;
	return float(r);
}

template <typename CodeUnitT>
double mbcs_string<CodeUnitT>::toDouble (bool * ok, ucchar decimalPoint) const
{
#ifndef PFS_HAVE_LONG_DOUBLE
	return toReal(ok, decimalPoint);
#else
	bool ok1;
	real_t r = toReal(& ok1, decimalPoint);
	if (!ok1 || r < PFS_DOUBLE_MIN || r > PFS_DOUBLE_MAX) {
		ok1 = false;
		r = double(0.0f);
	}
	if (ok) *ok = ok1;
	return double(r);
#endif
}

template <typename CodeUnitT>
inline mbcs_string<CodeUnitT> mbcs_string<CodeUnitT>::toString (signed char value, int base, bool uppercase)
{
	char buf[65];
	return mbcs_string<CodeUnitT>::fromLatin1(
			pfs_integral_to_string(integral_t(value), base, int(uppercase), buf, 65));
}

template <typename CodeUnitT>
inline mbcs_string<CodeUnitT> mbcs_string<CodeUnitT>::toString (short value, int base, bool uppercase)
{
	char buf[65];
	return mbcs_string<CodeUnitT>::fromLatin1(
			pfs_integral_to_string(integral_t(value), base, int(uppercase), buf, 65));
}

template <typename CodeUnitT>
inline mbcs_string<CodeUnitT> mbcs_string<CodeUnitT>::toString (int value, int base, bool uppercase)
{
	char buf[65];
	return mbcs_string<CodeUnitT>::fromLatin1(
			pfs_integral_to_string(integral_t(value), base, int(uppercase), buf, 65));
}

template <typename CodeUnitT>
inline mbcs_string<CodeUnitT> mbcs_string<CodeUnitT>::toString (long value, int base, bool uppercase)
{
	char buf[65];
	return mbcs_string<CodeUnitT>::fromLatin1(
			pfs_integral_to_string(integral_t(value), base, int(uppercase), buf, 65));
}

template <typename CodeUnitT>
inline mbcs_string<CodeUnitT> mbcs_string<CodeUnitT>::toString (unsigned char value, int base, bool uppercase)
{
	char buf[65];
	return mbcs_string<CodeUnitT>::fromLatin1(
			pfs_uintegral_to_string(uintegral_t(value), base, int(uppercase), buf, 65));
}

template <typename CodeUnitT>
inline mbcs_string<CodeUnitT> mbcs_string<CodeUnitT>::toString (unsigned short value, int base, bool uppercase)
{
	char buf[65];
	return mbcs_string<CodeUnitT>::fromLatin1(
			pfs_uintegral_to_string(uintegral_t(value), base, int(uppercase), buf, 65));
}

template <typename CodeUnitT>
inline mbcs_string<CodeUnitT> mbcs_string<CodeUnitT>::toString (unsigned int value, int base, bool uppercase)
{
	char buf[65];
	return mbcs_string<CodeUnitT>::fromLatin1(
			pfs_uintegral_to_string(uintegral_t(value), base, int(uppercase), buf, 65));
}

template <typename CodeUnitT>
inline mbcs_string<CodeUnitT> mbcs_string<CodeUnitT>::toString (unsigned long value, int base, bool uppercase)
{
	char buf[65];
	return mbcs_string<CodeUnitT>::fromLatin1(
			pfs_uintegral_to_string(uintegral_t(value), base, int(uppercase), buf, 65));
}

#ifdef PFS_HAVE_LONGLONG
template <typename CodeUnitT>
inline mbcs_string<CodeUnitT> mbcs_string<CodeUnitT>::toString (long long value, int base, bool uppercase)
{
	char buf[65];
	return mbcs_string<CodeUnitT>::fromLatin1(
			pfs_integral_to_string(integral_t(value), base, int(uppercase), buf, 65));
}

template <typename CodeUnitT>
inline mbcs_string<CodeUnitT> mbcs_string<CodeUnitT>::toString (unsigned long long value, int base, bool uppercase)
{
	char buf[65];
	return mbcs_string<CodeUnitT>::fromLatin1(
			pfs_uintegral_to_string(uintegral_t(value), base, int(uppercase), buf, 65));
}
#endif

template <typename CodeUnitT>
inline mbcs_string<CodeUnitT> mbcs_string<CodeUnitT>::toString (float value, char f, int prec)
{
	char buf[65];
	return mbcs_string<CodeUnitT>::fromLatin1(
			pfs_real_to_string(real_t(value), f, prec, buf, 65));
}

template <typename CodeUnitT>
inline mbcs_string<CodeUnitT> mbcs_string<CodeUnitT>::toString (double value, char f, int prec)
{
	char buf[129];
	return mbcs_string<CodeUnitT>::fromLatin1(
			pfs_real_to_string(real_t(value), f, prec, buf, 129));
}

#ifdef PFS_HAVE_LONG_DOUBLE
template <typename CodeUnitT>
inline mbcs_string<CodeUnitT> mbcs_string<CodeUnitT>::toString (long double value, char f, int prec)
{
	char buf[129];
	return mbcs_string<CodeUnitT>::fromLatin1(
			pfs_real_to_string(real_t(value), f, prec, buf, 129));
}
#endif


template <typename CodeUnitT>
inline mbcs_string<CodeUnitT> operator + (const mbcs_string<CodeUnitT> & lhs, const mbcs_string<CodeUnitT> & rhs)
{
	mbcs_string<CodeUnitT> r(lhs);
	return r += rhs;
}

template <typename CodeUnitT>
inline mbcs_string<CodeUnitT> operator + (const mbcs_string<CodeUnitT> & lhs, const char * latin1)
{
	mbcs_string<CodeUnitT> r(lhs);
	return r += latin1;
}

template <typename CodeUnitT>
inline mbcs_string<CodeUnitT> operator + (const mbcs_string<CodeUnitT> & lhs, const ucchar & ch)
{
	mbcs_string<CodeUnitT> r(lhs);
	return r += ch;
}

template <typename CodeUnitT>
inline mbcs_string<CodeUnitT> operator + (const mbcs_string<CodeUnitT> & lhs, char latin1)
{
	mbcs_string<CodeUnitT> r(lhs);
	return r += latin1;
}

template <typename CodeUnitT>
inline bool operator == ( const mbcs_string<CodeUnitT> & lhs
		, const mbcs_string<CodeUnitT> & rhs )
{
	return lhs.compare(rhs) == 0;
}

template <typename CodeUnitT>
inline bool operator != ( const mbcs_string<CodeUnitT> & lhs
		, const mbcs_string<CodeUnitT> & rhs )
{
	return lhs.compare(rhs) != 0;
}

template <typename CodeUnitT>
inline bool operator < ( const mbcs_string<CodeUnitT> & lhs
		, const mbcs_string<CodeUnitT> & rhs )
{
	return lhs.compare(rhs) < 0;
}

template <typename CodeUnitT>
inline bool operator <= ( const mbcs_string<CodeUnitT> & lhs
		, const mbcs_string<CodeUnitT> & rhs )
{
	return lhs.compare(rhs) <= 0;
}

template <typename CodeUnitT>
inline bool operator > ( const mbcs_string<CodeUnitT> & lhs
		, const mbcs_string<CodeUnitT> & rhs )
{
	return lhs.compare(rhs) > 0;
}

template <typename CodeUnitT>
inline bool operator >= ( const mbcs_string<CodeUnitT> & lhs
		, const mbcs_string<CodeUnitT> & rhs )
{
	return lhs.compare(rhs) >= 0;
}

template <typename CodeUnitT>
inline bool operator == (const mbcs_string<CodeUnitT> & lhs, const char * rhs)
{
	return lhs.compare(rhs) == 0;
}

template <typename CodeUnitT>
inline bool operator != (const mbcs_string<CodeUnitT> & lhs, const char * rhs)
{
	return lhs.compare(rhs) != 0;
}

template <typename CodeUnitT>
inline bool operator < (const mbcs_string<CodeUnitT> & lhs, const char * rhs)
{
	return lhs.compare(rhs) < 0;
}

template <typename CodeUnitT>
inline bool operator <= (const mbcs_string<CodeUnitT> & lhs, const char * rhs)
{
	return lhs.compare(rhs) <= 0;
}

template <typename CodeUnitT>
inline bool operator > (const mbcs_string<CodeUnitT> & lhs, const char * rhs)
{
	return lhs.compare(rhs) > 0;
}

template <typename CodeUnitT>
inline bool operator >= (const mbcs_string<CodeUnitT> & lhs, const char * rhs)
{
	return lhs.compare(rhs) >= 0;
}

template <typename CodeUnitT>
inline std::ostream & operator << (std::ostream & os, const mbcs_string<CodeUnitT> & o)
{
// FIXME is c_str() that function to use in this case?
	os << o.c_str();
	return os;
}

} // pfs

#include <pfs/bits/mbcs_string_impl_inc.hpp>
#include <pfs/bits/mbcs_string_inc.hpp>

#ifdef PFS_CC_MSVC
#	pragma warning(pop)
#endif

#endif /* __PFS_MBCS_STRING_HPP__ */
