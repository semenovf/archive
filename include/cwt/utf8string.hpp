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
#include <cwt/unicode.hpp>

CWT_NS_BEGIN

// Qt   QString
// C#
// Java

typedef uint32_t UChar;

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
};

class DLL_API Utf8String
{
	CWT_PIMPL_DECL_COPYABLE(Utf8String);
public:
	typedef UChar char_type;
	static const char ReplacementChar = '?';

    class const_iterator;

	class iterator {
		friend class const_iterator;
		friend class Utf8String;
		Utf8Entry m_entry;

	public:
        iterator() : m_entry() {}
    	iterator(const iterator & o) : m_entry(o.m_entry) {}
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
		friend class Utf8String;
		Utf8Entry m_entry;

	public:
        const_iterator() : m_entry() {}
    	const_iterator(const iterator & o) : m_entry(o.m_entry) {}
    	const_iterator(const const_iterator & o) : m_entry(o.m_entry) {}
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

public:
	Utf8String();
	Utf8String(const char *latin1);
	Utf8String(const char *latin1, size_t length);
	Utf8String(size_t count, char latin1);
	Utf8String(const const_iterator & begin, const const_iterator & end);

	const char*	data() const;
	const char*	constData() const { return data(); }
	const char*	c_str() const;
	UChar       charAt(size_t pos) const;
	void        clear();
	bool	    isEmpty() const;

	Utf8String & append(const Utf8String & s) { return insert(s, end()); }
	Utf8String & prepend(const Utf8String & s) { return insert(s, begin()); }
	Utf8String & insert(const Utf8String & s, size_t pos) { return insert(s, begin() + pos); }
	Utf8String & insert(const Utf8String & s, const const_iterator & pos);

    iterator       begin();
    iterator       end();
    const_iterator begin() const;
    const_iterator end() const;
    const_iterator cbegin() const { return begin(); }
    const_iterator cend() const   { return end(); }

    int compare (const Utf8String & s) const;
    int compare (size_t pos, size_t len, const Utf8String & s) const;
    int compare (size_t pos, size_t len, const Utf8String & s, size_t subpos, size_t sublen) const;
    int compare (const char * s) const;
    int compare (size_t pos, size_t len, const char * s) const;
    int compare (size_t pos, size_t len, const char * s, size_t n) const;

	bool contains(const Utf8String & s) const { return find(s, begin()) != end(); }
	bool contains(const char * s) const { return find(s) != end(); }
	bool contains(const char * s, size_t n) const { return find(s, n) != end(); }

	bool endsWith(const Utf8String & s) const;
	bool endsWith(const char * s) const;
	bool endsWith(const char * s, size_t n) const;

	bool startsWith(const Utf8String & s) const;
	bool startsWith(const char * s) const;
	bool startsWith(const char * s, size_t n) const;

	iterator find(const Utf8String & s, iterator from);
	const_iterator find(const Utf8String & s, const_iterator from) const;
	iterator find(const char * s);
	iterator find(const char * s, size_t from);
	const_iterator find(const char * s) const;
	const_iterator find(const char * s, size_t from) const;

#ifdef __NOT_IMPLEMENTED_YET__
	ssize_t  indexOf(const String &str, int from = 0, bool cs = true) const;
	ssize_t  indexOf(UChar ch, int from = 0, bool cs = true) const;

	String&  remove(size_t pos, size_t n);
	String&  replace(const String &before, const String &after, bool cs = true);

#endif

	size_t   length() const;
	size_t   length(const const_iterator & begin, const const_iterator & end) const;
	size_t   size() const;

	void reserve (size_t n = 0);

	Utf8String substr(size_t pos) const { return substr(pos, length()); }
	Utf8String substr(size_t pos, size_t n) const { return substr(begin() + pos, n); }
	Utf8String substr(const const_iterator & from) const { return substr(from, length()); }
	Utf8String substr(const const_iterator & from, size_t n) const;

	double	 toDouble(bool *ok = 0) const;
	float	 toFloat(bool *ok = 0) const;
	long_t   toLong(bool *ok = 0, int base = 10) const;
	ulong_t	 toULong(bool *ok = 0, int base = 10) const;
	int_t	 toInt(bool *ok = 0, int base = 10) const;
	uint_t	 toUInt(bool *ok = 0, int base = 10) const;
	short_t	 toShort(bool *ok = 0, int base = 10) const;
	ushort_t toUShort(bool *ok = 0, int base = 10) const;
	sbyte_t  toSByte(bool *ok = 0, int base = 10) const;
	byte_t	 toByte(bool *ok = 0, int base = 10) const;

	Utf8String & setNumber (long_t n, int base = 10);
	Utf8String & setNumber (ulong_t n, int base = 10);
	Utf8String & setNumber (int_t n, int base = 10)    { return setNumber(long_t(n), base); }
	Utf8String & setNumber (uint_t n, int base = 10)   { return setNumber(ulong_t(n), base); }
	Utf8String & setNumber (short_t n, int base = 10)  { return setNumber(long_t(n), base); }
	Utf8String & setNumber (ushort_t n, int base = 10) { return setNumber(ulong_t(n), base); }
	Utf8String & setNumber (sbyte_t n, int base = 10)  { return setNumber(long_t(n), base); }
	Utf8String & setNumber (byte_t n, int base = 10)   { return setNumber(ulong_t(n), base); }
	Utf8String & setNumber (float n, char f = 'g', int prec = 6) { return setNumber(double(n), f, prec); }
	Utf8String & setNumber (double n, char f = 'g', int prec = 6);

	Utf8String & sprintf(const char * cformat, ...);
	Utf8String & vsprintf(const char *cformat, va_list ap);

#ifdef __NOT_IMPLEMENTED_YET__
	void     truncate(size_t count);
	const UChar*	unicode() const;
#endif
	Utf8String & operator += (const Utf8String & other) { return append(other); }
	UChar operator[](size_t pos) const { return charAt(pos); }

	friend Utf8String operator + (const Utf8String &s1, const Utf8String &s2);

	friend bool	operator != (const Utf8String &s1, const Utf8String &s2);
	friend bool	operator <  (const Utf8String &s1, const Utf8String &s2);
	friend bool	operator <= (const Utf8String &s1, const Utf8String &s2);
	friend bool	operator == (const Utf8String &s1, const Utf8String &s2);
	friend bool	operator >  (const Utf8String &s1, const Utf8String &s2);
	friend bool	operator >= (const Utf8String &s1, const Utf8String &s2);

	static Utf8String fromUtf8   (const ByteArray &str, bool * ok = nullptr);
	static Utf8String fromUtf8   (const char *utf8, bool * ok = nullptr);
	static Utf8String fromUtf8   (const char *utf8, size_t size, bool * ok = nullptr);
	static Utf8String fromLatin1 (const char * latin1, size_t size, bool * ok = nullptr);
	static Utf8String fromLatin1 (const char * latin1, bool * ok = nullptr);

	static Utf8String number (double n, char fmt = 'g', int prec = 6) { return Utf8String().setNumber(n, fmt, prec); }
	static Utf8String number (float n, char fmt = 'g', int prec = 6)  { return Utf8String().setNumber(n, fmt, prec); }
	static Utf8String number (int_t n, int base = 10)                 { return Utf8String().setNumber(n, base); }
	static Utf8String number (uint_t n, int base = 10)                { return Utf8String().setNumber(n, base); }
	static Utf8String number (long_t n, int base = 10)                { return Utf8String().setNumber(n, base); }
	static Utf8String number (ulong_t n, int base = 10)               { return Utf8String().setNumber(n, base); }

#ifdef __NOT_IMPLEMENTED_YET__
	static String fromUtf16  (const ushort_t * unicode, size_t size);
	static String fromUtf16  (const ushort_t * unicode);

	static const String& constNull();
#endif

	friend void swap (Utf8String& x, Utf8String& y) { x.pimpl.swap(y.pimpl); }

protected:
	int compare (const const_iterator & from, size_t len, const char * s, size_t subpos, size_t sublen) const;
	size_t calculateLength();

public:
	static int decodeBytes(const char * bytes, size_t len, uint32_t & uc, uint32_t & min_uc);
	static ssize_t encodeUcs4(char *utf8, size_t size, uint32_t ucs4);
};

CWT_NS_END


#endif /* __CWT_UTF8STRING_HPP__ */
