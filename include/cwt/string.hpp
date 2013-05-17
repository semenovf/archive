/**
 * @file   string.hpp
 * @author wladt
 * @date   Jan 24, 2013 2:31:19 PM
 *
 * @brief
 */

#ifndef __CWT_STRING_HPP__
#define __CWT_STRING_HPP__

#include <cwt/cwt.h>
#include <cwt/char.hpp>
#include <cwt/bytearray.hpp>
#include <cwt/memory.hpp>
#include <cstdarg>

#ifdef CWT_STRING_SELF_IMPL
#	include <cwt/vector.hpp>
#endif

/*#define StringLiteral(s) String::fromUtf8("" s "", sizeof(s)-1)*/

/* Unicode characters array */
#define _U(s)  cwt::String::fromUtf8("" s "", sizeof(s)-1)
#define _Ur(s) _U(s)

CWT_NS_BEGIN

//class StringDataPtr;

// Qt   QString
// C#
// Java

/*
class TextStream;
class FileReader;
*/

class DLL_API String
#ifdef CWT_STRING_SELF_IMPL
	: public Vector<Char::char_type>
#endif
{
#ifdef CWT_STRING_SELF_IMPL
	typedef Char::char_type   data_type;
	typedef Char::char_type   char_type;
	typedef Vector<char_type> BaseClass;
#endif

public:
	String();
	String(const Char *unicode, int size = -1);
	String(Char ch);
	String(int size, Char ch);
	String(const String &other);
	~String() { /*printf("{%p}: String::~String('%s')\n", this, this->toUtf8().constData());*/ }

	String&	 append(const String &str);
	String&	 append(const Char *unicode, int size);
	String&  append(Char ch);

	const Char at(int pos) const;
#ifndef CWT_STRING_SELF_IMPL // already defined in base class (Vector)
	void     clear();
#endif
	bool     contains(const String & str, bool cs = true) const;
	bool     contains(Char ch, bool cs = true) const;
	Char*    data();
	const Char*	data() const;
	bool	 endsWith(const String &s, bool cs = true) const;
	bool	 endsWith(Char c, bool cs = true) const;
#ifndef CWT_STRING_SELF_IMPL // already defined in base class (Vector)
	bool	 isEmpty() const;
#endif
#ifndef CWT_STRING_SELF_IMPL // obsolete
	bool	 isNull() const;
#endif
	int	     indexOf(const String &str, int from = 0, bool cs = true) const;
	int	     indexOf(Char ch, int from = 0, bool cs = true) const;
	int	     length() const;

	String&	 prepend(const String &str);
	String&	 prepend(const Char *unicode, int size = -1);
	String&	 prepend(Char ch);

	String&  sprintf(const char * cformat, ...);
	String&  vsprintf(const char *cformat, va_list ap);

#ifndef CWT_STRING_SELF_IMPL // already defined in base class (Vector)
	int	     size() const;
#endif
	bool	 startsWith(const String &s, bool cs = true) const;
	bool	 startsWith(Char c, bool cs = true) const;
	String   substr(int pos, int n = -1) const;

	double	 toDouble(bool *ok = 0) const;
	float	 toFloat(bool *ok = 0) const;
	int_t	 toInt(bool *ok = 0, int base = 10) const;
	long_t   toLong(bool *ok = 0, int base = 10) const;
	short_t	 toShort(bool *ok = 0, int base = 10) const;
	uint_t	 toUInt(bool *ok = 0, int base = 10) const;
	ulong_t	 toULong(bool *ok = 0, int base = 10) const;
	ushort_t toUShort(bool *ok = 0, int base = 10) const;

	void     truncate(int count);
	const Char*	unicode() const;

	ByteArray toUtf8() const;
	const ushort_t* utf16() const;

	String&	operator+=(const String & other);
	String&	operator+=(Char ch);
	String&	operator=(const String & other);
	String&	operator=(Char ch);
	Char    operator[](int pos);
	const Char operator[](int pos) const;

	friend bool	operator!=(const String &s1, const String &s2);
	friend const String	operator+(const String &s1, const String &s2);
	friend const String	operator+(const String &s, Char ch);
	friend bool	operator<(const String &s1, const  String &s2);
	friend bool	operator<=(const String &s1, const String &s2);
	friend bool	operator==(const String &s1, const String &s2);
	friend bool	operator>(const String &s1, const  String &s2);
	friend bool	operator>=(const String &s1, const String &s2);

	static String fromUtf8(const char *str, int size = -1);
	static String fromUtf8(const ByteArray &str);

	static String number(double n, char format = 'g', int precision = 6);
	static String number(int_t n, int base = 10);
	static String number(uint_t n, int base = 10);
	static String number(long_t n, int base = 10);
	static String number(ulong_t n, int base = 10);

	static const String& null();

private:
#ifndef CWT_STRING_SELF_IMPL
    class Impl;
    typedef unique_ptr<Impl> ImplPtr;
    ImplPtr pimpl;
#endif

    friend class FileReader;
    friend class TextStream;
};

DLL_API uint_t hash_func(const String &key, uint_t seed);

CWT_NS_END

#endif /* __CWT_STRING_HPP__ */
