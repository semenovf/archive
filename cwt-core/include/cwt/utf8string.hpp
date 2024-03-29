/*
 * utf8string.hpp
 *
 *  Created on: Jul 21, 2013
 *      Author: wladt
 */

#ifndef __CWT_UTF8STRING_HPP__
#define __CWT_UTF8STRING_HPP__

#include <cwt/cwt.hpp>
#include <cwt/pimpl.hpp>
#include <cwt/bytearray.hpp>
#include <cwt/uchar.hpp>
#include <cwt/vector.hpp>
#include <ostream>

CWT_NS_BEGIN

// Qt   QString
// C#
// Java

#define NO_UTF8_CHECK true
#define UTF8_CHECK    false

class Utf8String;
typedef Utf8String utf8string;

struct DLL_API Utf8Entry
{
	typedef const char * pointer;
	pointer cursor;

	Utf8Entry() : cursor(nullptr) {}
	Utf8Entry(const Utf8Entry & o) : cursor(o.cursor) {}
	explicit Utf8Entry(const char *cur) : cursor(cur) {}

	UChar value() const;
    Utf8Entry & next ();
    Utf8Entry & prev ();

	UChar r_value() const;
    Utf8Entry & r_next () { return prev(); }
    Utf8Entry & r_prev () { return next(); }
};

class DLL_API Utf8String
{
	CWT_PIMPL_DECL_COPYABLE(Utf8String)
public:
	typedef UChar char_type;
	static const Utf8String EndOfLine;
	static const bool NumberUppercase = true;

    class const_iterator;
    class reverse_iterator;
    class const_reverse_iterator;

	class iterator {
		friend class const_iterator;
		friend class reverse_iterator;
		friend class Utf8String;
		Utf8Entry m_entry;

	public:
        iterator() : m_entry() {}
    	iterator(const iterator & o) : m_entry(o.m_entry) {}
    	iterator(const reverse_iterator & o);/* : m_entry(o.m_entry) {}*/
    	explicit iterator(Utf8Entry::pointer cursor) : m_entry(cursor) {}

    	UChar value() const                          { return m_entry.value(); }
        UChar operator  * () const                   { return m_entry.value(); }
        size_t distance (const iterator & o) const   { ssize_t d = o.m_entry.cursor - m_entry.cursor; return d > 0 ? d : d * -1; }
        bool operator  == (const iterator & o) const { return m_entry.cursor == o.m_entry.cursor; }
        bool operator  != (const iterator & o) const { return !(m_entry.cursor == o.m_entry.cursor); }
        iterator & operator ++ ()                    { m_entry.next(); return *this; }
        iterator   operator ++ (int) {
            iterator r(*this);
            this->operator ++();
            return r;
        }

        iterator & operator -- ()  { m_entry.prev(); return *this; }
        iterator   operator -- (int) {
            iterator r(*this);
            this->operator --();
            return r;
        }
        iterator   operator  + (int n) const {
        	iterator it(*this);
        	it += n;
        	return it;
        }
        iterator   operator  - (int n ) const {
        	iterator it(*this);
        	it -= n;
        	return it;
        }

        iterator & operator += (int n) {
        	while(n-- > 0)
          		m_entry.next();
          	return *this;
        }

        iterator & operator -= (int n) {
        	while(n-- > 0)
          		m_entry.prev();
          	return *this;
        }

		bool operator  > (const iterator & o) const { return m_entry.cursor > o.m_entry.cursor; }
		bool operator >= (const iterator & o) const { return m_entry.cursor >= o.m_entry.cursor; }
		bool operator  < (const iterator & o) const { return m_entry.cursor < o.m_entry.cursor; }
		bool operator <= (const iterator & o) const { return m_entry.cursor <= o.m_entry.cursor; }
	};

	class const_iterator {
		friend class iterator;
		friend class const_reverse_ietrator;
		friend class Utf8String;
		Utf8Entry m_entry;

	public:
        const_iterator() : m_entry() {}
    	const_iterator(const iterator & o) : m_entry(o.m_entry) {}
    	const_iterator(const const_iterator & o) : m_entry(o.m_entry) {}
    	const_iterator(const const_reverse_iterator & o);/* : m_entry(o.m_entry) {}*/
    	explicit const_iterator(Utf8Entry::pointer cursor)  : m_entry(cursor) {}

    	UChar value() const                                { return m_entry.value(); }
        UChar operator  * () const                         { return m_entry.value(); }
        size_t distance (const const_iterator & o) const   { ssize_t d = o.m_entry.cursor - m_entry.cursor; return d > 0 ? d : d * -1; }
        bool operator  == (const iterator & o) const       { return m_entry.cursor == o.m_entry.cursor; }
        bool operator  == (const const_iterator & o) const { return m_entry.cursor == o.m_entry.cursor; }
        bool operator  != (const iterator & o) const       { return !(m_entry.cursor == o.m_entry.cursor); }
        bool operator  != (const const_iterator & o) const { return !(m_entry.cursor == o.m_entry.cursor); }
        const_iterator & operator ++ ()                    { m_entry.next(); return *this; }
        const_iterator   operator ++ (int) {
            const_iterator r(*this);
            this->operator ++();
            return r;
        }

        const_iterator & operator -- () { m_entry.prev(); return *this; }

        const_iterator   operator -- (int) {
            const_iterator r(*this);
            this->operator --();
            return r;
        }

        const_iterator   operator  + (int n) const {
        	const_iterator it(*this);
        	it += n;
        	return it;
        }

        const_iterator   operator  - (int n) const {
        	const_iterator it(*this);
        	it -= n;
        	return it;
        }

        const_iterator & operator += (int n) {
        	while(n-- > 0)
        		m_entry.next();
        	return *this;
        }

        const_iterator & operator -= (int n) {
        	while(n-- > 0)
          		m_entry.prev();
          	return *this;
        }

        bool operator  > (const const_iterator & o) const { return m_entry.cursor  > o.m_entry.cursor; }
        bool operator >= (const const_iterator & o) const { return m_entry.cursor >= o.m_entry.cursor; }
        bool operator  < (const const_iterator & o) const { return m_entry.cursor  < o.m_entry.cursor; }
        bool operator <= (const const_iterator & o) const { return m_entry.cursor <= o.m_entry.cursor; }
	};

    class const_reverse_iterator;

	class reverse_iterator {
		friend class const_reverse_iterator;
		friend class iterator;
		friend class Utf8String;
		Utf8Entry m_entry;

	public:
		reverse_iterator() : m_entry() {}
		reverse_iterator(const iterator & o) : m_entry(o.m_entry) {}
		reverse_iterator(const reverse_iterator & o) : m_entry(o.m_entry) {}
    	explicit reverse_iterator(Utf8Entry::pointer cursor) : m_entry(cursor) {}

    	UChar value() const                          { return m_entry.r_value(); }
        UChar operator  * () const                   { return m_entry.r_value(); }
        size_t distance (const reverse_iterator & o) const   { ssize_t d = o.m_entry.cursor - m_entry.cursor; return d > 0 ? d : d * -1; }
        bool operator  == (const reverse_iterator & o) const { return m_entry.cursor == o.m_entry.cursor; }
        bool operator  != (const reverse_iterator & o) const { return !(m_entry.cursor == o.m_entry.cursor); }
        reverse_iterator & operator ++ ()                    { m_entry.r_next(); return *this; }
        reverse_iterator   operator ++ (int) {
        	reverse_iterator r(*this);
            this->operator ++();
            return r;
        }

        reverse_iterator & operator -- ()  { m_entry.r_prev(); return *this; }
        reverse_iterator   operator -- (int) {
        	reverse_iterator r(*this);
            this->operator --();
            return r;
        }
        reverse_iterator   operator  + (int n) const {
        	reverse_iterator it(*this);
        	it += n;
        	return it;
        }
        reverse_iterator   operator  - (int n ) const {
        	reverse_iterator it(*this);
        	it -= n;
        	return it;
        }

        reverse_iterator & operator += (int n) {
        	while(n-- > 0)
          		m_entry.r_next();
          	return *this;
        }

        reverse_iterator & operator -= (int n) {
        	while(n-- > 0)
          		m_entry.r_prev();
          	return *this;
        }

		bool operator  > (const reverse_iterator & o) const { return m_entry.cursor < o.m_entry.cursor; }
		bool operator >= (const reverse_iterator & o) const { return m_entry.cursor <= o.m_entry.cursor; }
		bool operator  < (const reverse_iterator & o) const { return m_entry.cursor > o.m_entry.cursor; }
		bool operator <= (const reverse_iterator & o) const { return m_entry.cursor >= o.m_entry.cursor; }
	};

	class const_reverse_iterator {
		friend class reverse_iterator;
		friend class const_iterator;
		friend class Utf8String;
		Utf8Entry m_entry;

	public:
		const_reverse_iterator() : m_entry() {}
		const_reverse_iterator(const reverse_iterator & o) : m_entry(o.m_entry) {}
		const_reverse_iterator(const const_reverse_iterator & o) : m_entry(o.m_entry) {}
		const_reverse_iterator(const const_iterator & o) : m_entry(o.m_entry) {}
    	explicit const_reverse_iterator(Utf8Entry::pointer cursor)  : m_entry(cursor) {}

    	UChar value() const                                { return m_entry.value(); }
        UChar operator  * () const                         { return m_entry.value(); }
        size_t distance (const const_reverse_iterator & o) const   { ssize_t d = o.m_entry.cursor - m_entry.cursor; return d > 0 ? d : d * -1; }
        bool operator  == (const reverse_iterator & o) const       { return m_entry.cursor == o.m_entry.cursor; }
        bool operator  == (const const_reverse_iterator & o) const { return m_entry.cursor == o.m_entry.cursor; }
        bool operator  != (const reverse_iterator & o) const       { return !(m_entry.cursor == o.m_entry.cursor); }
        bool operator  != (const const_reverse_iterator & o) const { return !(m_entry.cursor == o.m_entry.cursor); }
        const_reverse_iterator & operator ++ ()                    { m_entry.r_next(); return *this; }
        const_reverse_iterator   operator ++ (int) {
        	const_reverse_iterator r(*this);
            this->operator ++();
            return r;
        }

        const_reverse_iterator & operator -- () { m_entry.r_prev(); return *this; }

        const_reverse_iterator   operator -- (int) {
        	const_reverse_iterator r(*this);
            this->operator --();
            return r;
        }

        const_reverse_iterator   operator  + (int n) const {
        	const_reverse_iterator it(*this);
        	it += n;
        	return it;
        }

        const_reverse_iterator   operator  - (int n) const {
        	const_reverse_iterator it(*this);
        	it -= n;
        	return it;
        }

        const_reverse_iterator & operator += (int n) {
        	while(n-- > 0)
        		m_entry.r_next();
        	return *this;
        }

        const_reverse_iterator & operator -= (int n) {
        	while(n-- > 0)
          		m_entry.r_prev();
          	return *this;
        }

        bool operator  > (const const_reverse_iterator & o) const { return m_entry.cursor  < o.m_entry.cursor; }
        bool operator >= (const const_reverse_iterator & o) const { return m_entry.cursor <= o.m_entry.cursor; }
        bool operator  < (const const_reverse_iterator & o) const { return m_entry.cursor  > o.m_entry.cursor; }
        bool operator <= (const const_reverse_iterator & o) const { return m_entry.cursor >= o.m_entry.cursor; }
	};

public:
	Utf8String ();
	Utf8String (const char * latin1);
	Utf8String (const char * latin1, size_t size);
	Utf8String (size_t count, char latin1);
	Utf8String (const const_iterator & begin, const const_iterator & end);
	Utf8String (size_t count, UChar c);
	Utf8String (bool no_ut8_check, const char * utf8);
	Utf8String (bool no_ut8_check, const char * utf8, size_t size);

	const char*	data () const;
	const char*	constData () const { return data(); }
	const char*	c_str () const;
	const char*	utf8 () const { return c_str(); }
	UChar       charAt (size_t pos) const;
	void        clear ();
	bool	    isEmpty () const;

	Utf8String & append  (const Utf8String & s) { return insert(s, end()); }
	Utf8String & append  (const char * s, size_t size) { return append(Utf8String(s, size)); }
	Utf8String & append  (size_t n, UChar ch) { return append(Utf8String(n, ch)); }
	Utf8String & prepend (const Utf8String & s) { return insert(s, begin()); }
	Utf8String & prepend (size_t n, UChar ch) { return insert(Utf8String(n, ch), begin()); }
	Utf8String & insert  (const Utf8String & s, size_t pos) { return insert(s, begin() + pos); }
	Utf8String & insert  (const Utf8String & s, const const_iterator & pos);
	Utf8String & insert  (size_t n, UChar ch, size_t pos) { return insert(Utf8String(n, ch), pos); }
	Utf8String & insert  (size_t n, UChar ch, const const_iterator & pos) { return insert(Utf8String(n, ch), pos); }

    iterator       begin ();
    iterator       end   ();
    const_iterator begin () const;
    const_iterator end   () const;
    const_iterator cbegin() const { return begin(); }
    const_iterator cend  () const { return end(); }

    reverse_iterator       rbegin();
    reverse_iterator       rend();
    const_reverse_iterator rbegin() const;
    const_reverse_iterator rend() const;
    const_reverse_iterator crbegin() const { return rbegin(); }
    const_reverse_iterator crend() const   { return rend(); }

    int compare (const Utf8String & s) const;
    int compare (size_t pos, size_t len, const Utf8String & s) const;
    int compare (size_t pos, size_t len, const Utf8String & s, size_t subpos, size_t sublen) const;
    int compare (const char * s) const;
    int compare (size_t pos, size_t len, const char * s) const;
    int compare (size_t pos, size_t len, const char * s, size_t n) const;

	bool contains(const Utf8String & s) const { return find(s, begin()) != end(); }
	bool contains(UChar ch) const { return find(Utf8String(1, ch), begin()) != end(); }
	bool contains(const char * s) const { return find(s, 0, strlen(s)) != end(); }
	bool contains(const char * s, size_t n) const { return find(s, 0, n) != end(); }

	bool endsWith(const Utf8String & s) const;
	bool endsWith(const char * s) const;
	bool endsWith(const char * s, size_t n) const;

	bool startsWith(const Utf8String & s, const Utf8String::iterator & from) const
	{
		return startsWith(s, Utf8String::const_iterator(from));
	}
	bool startsWith(const Utf8String & s, const Utf8String::const_iterator & from) const;
	bool startsWith(const Utf8String & s) const { return startsWith(s, cbegin()); }
	bool startsWith(const char * s) const;
	bool startsWith(const char * s, size_t n) const;

	const_iterator find(const Utf8String & s, const_iterator from) const;
	const_iterator find(const Utf8String & s) const { return find(s, cbegin()); }
	const_iterator find(const char * s, size_t from, size_t n) const;
	const_iterator find(const char * s, size_t from) const;

#ifdef __NOT_IMPLEMENTED_YET__
	ssize_t  indexOf(const String &str, int from = 0, bool cs = true) const;
	ssize_t  indexOf(UChar ch, int from = 0, bool cs = true) const;

#endif

	size_t   length() const;
	static size_t length(const const_iterator & begin, const const_iterator & end);
	static size_t length(const const_reverse_iterator & begin, const const_reverse_iterator & end);
	size_t   size() const;
	size_t   calculateLength();

	/* TODO need to implement 'replace' methods
		Utf8String & replace (size_t pos,  size_t len,  const Utf8String & str);
		Utf8String & replace (iterator i1, iterator i2, const Utf8String & str);
		Utf8String & replace (size_t pos,  size_t len,  const Utf8String & str, size_t subpos, size_t sublen);
		Utf8String & replace (iterator i1, iterator i2, const Utf8String & str, size_t subpos, size_t sublen);
		Utf8String & replace (size_t pos,  size_t len,  size_t n, UChar c);
		Utf8String & replace (iterator i1, iterator i2, size_t n, UChar c);
	*/
	Utf8String & replace(const Utf8String & before, const Utf8String & after);

	void reserve (size_t n = 0);

	Utf8String & remove(size_t pos)                  { return remove(pos, length()); }
	Utf8String & remove(size_t pos, size_t n)        { return remove(begin() + pos, n); }
	Utf8String & remove(const const_iterator & from) { return remove(from, length()); }
	Utf8String & remove(const const_iterator & from, size_t n);

	Utf8String substr(size_t pos) const                  { return substr(pos, length()); }
	Utf8String substr(size_t pos, size_t n) const        { return substr(begin() + pos, n); }
	Utf8String substr(const const_iterator & from) const { return substr(from, length()); }
	Utf8String substr(const const_iterator & from, size_t n) const;
	Utf8String substr(const const_iterator & begin, const const_iterator & end) const;
	Utf8String mid(size_t pos, size_t n) const           { return substr(pos, n); }
	Utf8String left(size_t n) const                      { return substr(0, n); }
	Utf8String right(size_t n) const                     { return substr(length() - n, n); }

	Utf8String ltrim() const;
	Utf8String rtrim() const;
	Utf8String trim() const;

	double	 toDouble(bool * ok = 0) const;
	float	 toFloat(bool * ok = 0) const;
	long_t   toLong(bool * ok = 0, int base = 10) const;
	ulong_t	 toULong(bool * ok = 0, int base = 10) const;
	int_t	 toInt(bool * ok = 0, int base = 10) const;
	uint_t	 toUInt(bool * ok = 0, int base = 10) const;
	short_t	 toShort(bool * ok = 0, int base = 10) const;
	ushort_t toUShort(bool * ok = 0, int base = 10) const;
	sbyte_t  toSByte(bool * ok = 0, int base = 10) const;
	byte_t	 toByte(bool * ok = 0, int base = 10) const;
	Vector<uint16_t> toUtf16 () const;

	Utf8String toLower () const;
	Utf8String toUpper () const;

	Utf8String & setNumber (long_t n, int base = 10, bool uppercase = false);
	Utf8String & setNumber (ulong_t n, int base = 10, bool uppercase = false);
	Utf8String & setNumber (int_t n, int base = 10, bool uppercase = false)    { return setNumber(long_t(n), base, uppercase); }
	Utf8String & setNumber (uint_t n, int base = 10, bool uppercase = false)   { return setNumber(ulong_t(n), base, uppercase); }
	Utf8String & setNumber (short_t n, int base = 10, bool uppercase = false)  { return setNumber(long_t(n), base, uppercase); }
	Utf8String & setNumber (ushort_t n, int base = 10, bool uppercase = false) { return setNumber(ulong_t(n), base, uppercase); }
	Utf8String & setNumber (sbyte_t n, int base = 10, bool uppercase = false)  { return setNumber(long_t(n), base, uppercase); }
	Utf8String & setNumber (byte_t n, int base = 10, bool uppercase = false)   { return setNumber(ulong_t(n), base, uppercase); }
	Utf8String & setNumber (float n, char f = 'g', int prec = 6) { return setNumber(double(n), f, prec); }
	Utf8String & setNumber (double n, char f = 'g', int prec = 6);

	Vector<Utf8String> split(const Utf8String & separator, bool keepEmpty = true, UChar quoteChar = UChar(UChar::Null)) const
	{
		return split(false, separator, keepEmpty, quoteChar);
	}
	Vector<Utf8String> split(const UChar & separator, bool keepEmpty = true, UChar quoteChar = UChar(UChar::Null)) const
	{
		return split(false, Utf8String(1, separator), keepEmpty, quoteChar);
	}

	Vector<Utf8String> splitOneOf(const Utf8String & separators, bool keepEmpty = true, UChar quoteChar = UChar(UChar::Null)) const
	{
		return split(true, Utf8String(separators), keepEmpty, quoteChar);
	}


#ifdef __NOT_IMPLEMENTED_YET__
	const Vector<UChar>	unicode() const;
#endif

	UChar operator [] (size_t pos) const { return charAt(pos); }

	Utf8String & operator += (const Utf8String & other) { return append(other); }
	Utf8String & operator += (const char * latin1) { return append(Utf8String(latin1)); }
	Utf8String & operator += (UChar ch) { return append(1, ch); }
	Utf8String & operator += (char latin1) { return append(1, UChar(latin1)); }

	Utf8String & operator << (const Utf8String & other) { return append(other); }
	Utf8String & operator << (const char * latin1) { return append(Utf8String(latin1)); }
	Utf8String & operator << (UChar ch) { return append(1, ch); }
	Utf8String & operator << (char latin1) { return append(1, UChar(latin1)); }

	friend Utf8String operator + (const Utf8String & s1, const Utf8String & s2);
	friend Utf8String operator + (const Utf8String & s, const char * latin1) { return s + Utf8String(latin1); }
	friend Utf8String operator + (const Utf8String & s, UChar ch) { return s + Utf8String(1, ch); }
	friend Utf8String operator + (const Utf8String & s, char latin1) { return s + Utf8String(1, UChar(latin1)); }

	friend bool	operator != (const Utf8String & s1, const Utf8String & s2);
	friend bool	operator <  (const Utf8String & s1, const Utf8String & s2);
	friend bool	operator <= (const Utf8String & s1, const Utf8String & s2);
	friend bool	operator == (const Utf8String & s1, const Utf8String & s2);
	friend bool	operator >  (const Utf8String & s1, const Utf8String & s2);
	friend bool	operator >= (const Utf8String & s1, const Utf8String & s2);

	struct ConvertState {
		ConvertState() : nremain(0), invalidChars(0), replacementChar(UChar::ReplacementChar) {}
		size_t nremain;
		size_t invalidChars;
		UChar  replacementChar;
	};

	Utf8String escape            () const { return escape(*this); }
	Utf8String escape            (const UChar escaped[], size_t nescaped) const { return escape(*this, escaped, nescaped); }
	static Utf8String escape     (const Utf8String & str, const UChar escaped[], size_t nescaped);
	static Utf8String escape     (const Utf8String & str);

	static Utf8String fromUtf8   (const ByteArray & str, bool * ok = nullptr, ConvertState * state = nullptr);
	static Utf8String fromUtf8   (const char * utf8, bool * ok = nullptr, ConvertState * state = nullptr);
	static Utf8String fromUtf8   (const char * utf8, size_t size, bool * ok = nullptr, ConvertState * state = nullptr);

	static Utf8String fromUtf16  (const uint16_t * utf16, size_t size, bool * ok = nullptr, ConvertState * state = nullptr);

	static Utf8String fromLatin1 (const ByteArray & latin1, bool * ok = nullptr, ConvertState * state = nullptr);
	static Utf8String fromLatin1 (const char * latin1, size_t size, bool * ok = nullptr, ConvertState * state = nullptr);
	static Utf8String fromLatin1 (const char * latin1, bool * ok = nullptr, ConvertState * state = nullptr);

	static Utf8String number (double n, char fmt = 'g', int prec = 6)           { return Utf8String().setNumber(n, fmt, prec); }
	static Utf8String number (float n, char fmt = 'g', int prec = 6)            { return Utf8String().setNumber(n, fmt, prec); }
	static Utf8String number (int_t n, int base = 10, bool uppercase = false)   { return Utf8String().setNumber(n, base, uppercase); }
	static Utf8String number (uint_t n, int base = 10, bool uppercase = false)  { return Utf8String().setNumber(n, base, uppercase); }
	static Utf8String number (long_t n, int base = 10, bool uppercase = false)  { return Utf8String().setNumber(n, base, uppercase); }
	static Utf8String number (ulong_t n, int base = 10, bool uppercase = false) { return Utf8String().setNumber(n, base, uppercase); }

	friend void swap (Utf8String& x, Utf8String& y) { x.pimpl.swap(y.pimpl); }

protected:
	int compare (const const_iterator & from, size_t len, const char * s, size_t subpos, size_t sublen) const;
	Vector<Utf8String> split(bool isOneSeparatorChar, const Utf8String & separator, bool keepEmpty = true, UChar quoteChar = UChar(UChar::Null)) const;

public:
	static int decodeBytes(const char * bytes, size_t len, uint32_t & uc, uint32_t & min_uc);
	static ssize_t encodeUcs4(char * utf8, size_t size, uint32_t ucs4);
};

inline Utf8String::iterator::iterator(const reverse_iterator & o) : m_entry(o.m_entry) {}
inline Utf8String::const_iterator::const_iterator(const const_reverse_iterator & o) : m_entry(o.m_entry) {}


inline Utf8String operator + (const Utf8String & s1, const Utf8String & s2)
{
	Utf8String s(s1);
	s.append(s2);
	return s;
}

inline std::ostream & operator << (std::ostream & os, const Utf8String & o) { os << o.c_str(); return os; }

CWT_NS_END


#endif /* __CWT_UTF8STRING_HPP__ */
