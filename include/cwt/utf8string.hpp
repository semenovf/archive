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
#include <cwt/shared_ptr.hpp>

CWT_NS_BEGIN

// Qt   QString
// C#
// Java

#ifdef CWT_UCS4_CHAR
	typedef uint32_t UChar;
#else
	typedef uint16_t UChar;
#endif

class DLL_API Utf8String
{
	CWT_PIMPL_DECL_COPYABLE(Utf8String);
public:
	static const char ReplacementChar = '?';

    class const_iterator;

	class iterator {
		friend class const_iterator;
		void *impl;
	public:
        iterator();
        explicit iterator(const iterator &o);
        ~iterator();

        UChar value() const;
        UChar operator      * () const;
        bool operator      == (const iterator &o) const;
        bool operator      != (const iterator &o) const;
        iterator& operator ++ ();
        iterator operator  ++ (int);
        iterator &operator -- ();
        iterator operator  -- (int);
        iterator operator   + (int) const;
        iterator operator   - (int) const;
        iterator &operator += (int);
        iterator &operator -= (int);
	};

	class const_iterator {
		friend class iterator;
		void *impl;
	public:
        const_iterator();
        explicit const_iterator(const iterator &o);
        ~const_iterator();

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

public:
	Utf8String();
	Utf8String(const char *latin1);
	Utf8String(const char *latin1, size_t length);
	Utf8String(size_t count, char latin1);

	Utf8String&	 append(const Utf8String & s);
	Utf8String&	 append(const char *latin1, int length);
	Utf8String&	 append(const char *latin1);

#ifdef __NOT_IMPLEMENTED_YET__
	/*
	Utf8String(const UChar *s);
	Utf8String(const UChar *s, size_t size);
	Utf8String(size_t size, UChar ch);
*/

	Utf8String&	 append(const UChar *unicode, int length);
	Utf8String&  append(char latin1);
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
#ifdef __NOT_IMPLEMENTED_YET__
	bool	 endsWith(const String &s, bool cs = true) const;
	bool	 endsWith(UChar ch, bool cs = true) const;
#endif
	bool	 isEmpty() const;
#ifdef __NOT_IMPLEMENTED_YET__
	ssize_t  indexOf(const String &str, int from = 0, bool cs = true) const;
	ssize_t  indexOf(UChar ch, int from = 0, bool cs = true) const;
	size_t   length() const { return size(); }

	String&	 prepend(const String &str);
	String&	 prepend(const UChar *unicode, int size = -1);
	String&	 prepend(UChar ch);
	String&  remove(size_t pos, size_t n);

	String&  replace(const String &before, const String &after, bool cs = true);

	void	 reserve(size_t size);
	void	 resize(size_t size);

	String&  sprintf(const char * cformat, ...);
	String&  vsprintf(const char *cformat, va_list ap);

	size_t   size() const;
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

	friend const Utf8String	operator + (const Utf8String &s, UChar ch);
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

	static String fromUtf8   (const char *utf8);
	static String fromUtf8   (const char *utf8, size_t size);
	static String fromUtf8   (const ByteArray &str);
	static String fromLatin1 (const char * latin1, size_t size);
	static String fromLatin1 (const char * latin1);
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
};

#ifdef __NOT_IMPLEMENTED_YET__
DLL_API uint_t hash_func(const Utf8String &key, uint_t seed);
#endif

CWT_NS_END


#endif /* __CWT_UTF8STRING_HPP__ */
