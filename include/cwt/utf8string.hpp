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
#include <cwt/hash.hpp>

CWT_NS_BEGIN

// Qt   QString
// C#
// Java

typedef uint32_t UChar;

struct DLL_API Utf8Entry
{
	const char *cursor;
	const char *end;
	//uint32_t    uc;

	Utf8Entry()
		: cursor(nullptr)
		, end(nullptr)
		/*, uc(0) */{}
	Utf8Entry(const Utf8Entry & o)
		: cursor(o.cursor)
		, end(o.end)
		/*, uc(o.uc) */{}

	explicit Utf8Entry(const char *cur, const char *e/*, uint32_t ch = Unicode::Null*/)
		: cursor(cur)
		, end(e)
		/*, uc(ch) */{}

	UChar value() const;
    Utf8Entry & next ();
};

class DLL_API Utf8String
{
	CWT_PIMPL_DECL_COPYABLE(Utf8String);
public:
	static const char ReplacementChar = '?';

    class const_iterator;

	class iterator {
		friend class const_iterator;
		Utf8Entry m_entry;

	public:
        iterator() : m_entry() {}
    	iterator(const iterator & o) : m_entry(o.m_entry) {}
    	explicit iterator(const char *cursor, const char *end/*, uint32_t ch = Unicode::Null*/)
    		: m_entry(cursor, end/*, ch*/) {}

    	UChar value() const { return m_entry.value(); }
        UChar operator  * () const { return m_entry.value(); }
        ssize_t distance(const iterator & o) const { return ssize_t(o.m_entry.cursor - m_entry.cursor); }
        bool operator  == (const iterator & o) const { return m_entry.cursor == o.m_entry.cursor; }
        bool operator  != (const iterator & o) const { return m_entry.cursor != o.m_entry.cursor; }
        iterator & operator ++ () { m_entry.next(); return *this; }
        iterator   operator ++ (int) {
            iterator r(*this);
            this->operator ++();
            return r;
        }

        bool operator  > (const iterator & o) const { return m_entry.cursor > o.m_entry.cursor; }
        bool operator >= (const iterator & o) const { return m_entry.cursor >= o.m_entry.cursor; }
        bool operator  < (const iterator & o) const { return m_entry.cursor < o.m_entry.cursor; }
        bool operator <= (const iterator & o) const { return m_entry.cursor <= o.m_entry.cursor; }

//        iterator & operator -- ();
//        iterator   operator -- (int);
//        iterator   operator  + (int) const;
//        iterator   operator  - (int) const;
//        iterator & operator += (int);
//        iterator & operator -= (int);
	};

	class const_iterator {
		friend class iterator;
		Utf8Entry m_entry;

	public:
        const_iterator() : m_entry() {}
    	const_iterator(const iterator & o) : m_entry(o.m_entry) {}
    	const_iterator(const const_iterator & o) : m_entry(o.m_entry) {}
    	explicit const_iterator(const char *cursor, const char *end/*, uint32_t ch = Unicode::Null*/)
    		: m_entry(cursor, end/*, ch*/) {}

    	UChar value() const { return m_entry.value(); }
        UChar operator  * () const { return m_entry.value(); }
        bool operator  == (const iterator & o) const { return m_entry.cursor == o.m_entry.cursor; }
        bool operator  == (const const_iterator & o) const { return m_entry.cursor == o.m_entry.cursor; }
        bool operator  != (const iterator & o) const { return m_entry.cursor != o.m_entry.cursor; }
        bool operator  != (const const_iterator & o) const { return m_entry.cursor != o.m_entry.cursor; }
        const_iterator & operator ++ () { m_entry.next(); return *this; }
        const_iterator   operator ++ (int) {
            const_iterator r(*this);
            this->operator ++();
            return r;
        }

        bool operator  > (const const_iterator & o) const { return m_entry.cursor > o.m_entry.cursor; }
        bool operator >= (const const_iterator & o) const { return m_entry.cursor >= o.m_entry.cursor; }
        bool operator  < (const const_iterator & o) const { return m_entry.cursor < o.m_entry.cursor; }
        bool operator <= (const const_iterator & o) const { return m_entry.cursor <= o.m_entry.cursor; }

//        iterator & operator -- ();
//        iterator   operator -- (int);
//        iterator   operator  + (int) const;
//        iterator   operator  - (int) const;
//        iterator & operator += (int);
//        iterator & operator -= (int);
	};
/*
	class const_iterator {
		friend class iterator;
		char *cursor;
		char *end;
		UChar ch;
	public:
        const_iterator();
        explicit const_iterator(const iterator &o);

        UChar value() const;
        UChar operator * () const;
        bool operator  == (const const_iterator &o) const;
        bool operator  != (const const_iterator &o) const;
        const_iterator & operator ++ ();
        const_iterator   operator ++ (int);

        const_iterator & operator -- ();
        const_iterator   operator -- (int);
        const_iterator   operator  + (int) const;
        const_iterator   operator  - (int) const;
        const_iterator & operator += (int);
        const_iterator & operator -= (int);
	};
*/

public:
	Utf8String();
	Utf8String(const char *latin1);
	Utf8String(const char *latin1, size_t length);
	Utf8String(size_t count, char latin1);

	Utf8String&	 append(const Utf8String & s);
	Utf8String&	 append(const char *latin1, int length);
	Utf8String&	 append(const char *latin1);
	Utf8String&  append(char latin1);

    iterator       begin();
    iterator       end();
    const_iterator begin() const;
    const_iterator end() const;
    const_iterator cbegin() const { return begin(); }
    const_iterator cend() const   { return end(); }

#ifdef __NOT_IMPLEMENTED_YET__
	/*
	Utf8String(const UChar *s);
	Utf8String(const UChar *s, size_t size);
	Utf8String(size_t size, UChar ch);
*/
	Utf8String&	 append(const UChar *unicode, int length);
	Utf8String&  append(UChar ch);

	const UChar at(int pos) const;
#endif
	void     clear();
#ifdef __NOT_IMPLEMENTED_YET__
	bool     contains(const Utf8String & s, bool cs = true) const;
	bool     contains(UChar ch, bool cs = true) const;
	int      compare(const String & other, bool cs = true) const;
#endif
	const char*	data() const;
	const char*	c_str() const;
#ifdef __NOT_IMPLEMENTED_YET__
	bool	 endsWith(const String &s, bool cs = true) const;
	bool	 endsWith(UChar ch, bool cs = true) const;
#endif

	bool	 isEmpty() const;
	iterator find(const Utf8String & s, iterator from = begin());
	const_iterator find(const Utf8String & s, const_iterator from = cbegin()) const;

#ifdef __NOT_IMPLEMENTED_YET__
	ssize_t  indexOf(const String &str, int from = 0, bool cs = true) const;
	ssize_t  indexOf(UChar ch, int from = 0, bool cs = true) const;

	String&	 prepend(const String &str);
	String&	 prepend(const UChar *unicode, int size = -1);
	String&	 prepend(UChar ch);
	String&  remove(size_t pos, size_t n);

	String&  replace(const String &before, const String &after, bool cs = true);

	String&  sprintf(const char * cformat, ...);
	String&  vsprintf(const char *cformat, va_list ap);
#endif

	size_t   length() const;
	size_t   size() const;

	void reserve (size_t n = 0);
	void resize (size_t n);
	void resize (size_t n, char c);

#ifdef __NOT_IMPLEMENTED_YET__
	bool	 startsWith(const String &s, bool cs = true) const;
	bool	 startsWith(UChar c, bool cs = true) const;
	String   substr(size_t pos) const;
	String   substr(size_t pos, size_t n) const;

	double	 toDouble(bool *ok = 0) const;
	float	 toFloat(bool *ok = 0) const;
	int_t	 toInt(bool *ok = 0, int base = 10) const;
	long_t   toLong(bool *ok = 0, int base = 10) const;
	short_t	 toShort(bool *ok = 0, int base = 10) const;
	uint_t	 toUInt(bool *ok = 0, int base = 10) const;
	ulong_t	 toULong(bool *ok = 0, int base = 10) const;
	ushort_t toUShort(bool *ok = 0, int base = 10) const;

	void     truncate(size_t count);
	const UChar*	unicode() const;

	String&	operator += (const String & other);
	String&	operator += (UChar ch);
	String&	operator = (UChar ch);
	UChar operator[](int pos) const;

#endif
	friend Utf8String operator + (const Utf8String &s1, const Utf8String &s2);

	friend bool	operator != (const Utf8String &s1, const Utf8String &s2);
	friend bool	operator <  (const Utf8String &s1, const Utf8String &s2);
	friend bool	operator <= (const Utf8String &s1, const Utf8String &s2);
	friend bool	operator == (const Utf8String &s1, const Utf8String &s2);
	friend bool	operator >  (const Utf8String &s1, const Utf8String &s2);
	friend bool	operator >= (const Utf8String &s1, const Utf8String &s2);

#ifdef __NOT_IMPLEMENTED_YET__
	String&	setNumber (long_t n, int base = 10);
	String&	setNumber (ulong_t n, int base = 10);
	String&	setNumber (int_t n, int base = 10);
	String&	setNumber (uint_t n, int base = 10);
	String&	setNumber (short_t n, int base = 10);
	String&	setNumber (ushort_t n, int base = 10);
	String&	setNumber (sbyte_t n, int base = 10) { return setNumber(long_t(n), base); }
	String&	setNumber (byte_t n, int base = 10)  { return setNumber(ulong_t(n), base); }
	String&	setNumber (float n, char f = 'g', int prec = 6);
	String&	setNumber (double n, char f = 'g', int prec = 6);
#endif
	static Utf8String fromUtf8   (const ByteArray &str, bool * ok);
	static Utf8String fromUtf8   (const char *utf8, bool * ok);
	static Utf8String fromUtf8   (const char *utf8, size_t size, bool * ok);
	static Utf8String fromLatin1 (const char * latin1, size_t size, bool * ok);
	static Utf8String fromLatin1 (const char * latin1, bool * ok);

#ifdef __NOT_IMPLEMENTED_YET__
	static String fromUtf16  (const ushort_t * unicode, size_t size);
	static String fromUtf16  (const ushort_t * unicode);

	static String number (double n, char format = 'g', int precision = 6);
	static String number (float n, char format = 'g', int precision = 6);
	static String number (int_t n, int base = 10);
	static String number (uint_t n, int base = 10);
	static String number (long_t n, int base = 10);
	static String number (ulong_t n, int base = 10);

	static const String& constNull();
#endif

	friend void swap (Utf8String& x, Utf8String& y) { x.pimpl.swap(y.pimpl); }

public:
	static int decodeBytes(const char * bytes, size_t len, uint32_t & uc, uint32_t & min_uc);
	static ssize_t encodeUcs4(char *utf8, size_t size, uint32_t ucs4);
};

inline uint_t hash_func(const Utf8String &key, uint_t seed)
{
	return hash_bytes(reinterpret_cast<const byte_t*>(key.data()), key.size(), seed);
}

CWT_NS_END


#endif /* __CWT_UTF8STRING_HPP__ */
