/*
 * safeformat.hpp
 *
 *  Created on: Jul 29, 2013
 *      Author: wladt
 */

#ifndef __PFS_SAFEFORMAT_HPP__
#define __PFS_SAFEFORMAT_HPP__

#include <pfs/ctype.hpp>
#include <pfs/ucchar.hpp>
#include <pfs/string.hpp>
#include <pfs/byte_string.hpp>

//#define _Fr(s) pfs::safeformat(_u8(s))

namespace pfs {

struct __sf_base_traits
{
	virtual string asInteger (int /*base*/, bool /*uppercase*/, bool /*isUnsigned*/) const = 0;
	virtual string asFloat ( char f, int prec) const = 0;
	virtual string asChar () const = 0;
	virtual string asString () const = 0;
	virtual ~__sf_base_traits () {}
};

template <typename T>
struct __sf_traits : public __sf_base_traits
{
	T _val;
	__sf_traits (const T & v) : __sf_base_traits(), _val(v) {}
	virtual string asInteger (int /*base*/, bool /*uppercase*/, bool /*isUnsigned*/) const { return string(); }
	virtual string asFloat ( char /*f*/, int /*prec*/) const { return string(); }
	virtual string asChar () const { return string(); }
	virtual string asString () const { return string(); }
};

//template <>
//string __sf_traits<char>::asInteger (int base, bool uppercase, bool isUnsigned) const
//{
//	return isUnsigned
//			? string::toString((unsigned int)_val, base, uppercase)
//		    : string::toString(_val, base, uppercase);
//}


class DLL_API safeformat
{
public:
	typedef string::const_iterator const_iterator;
	typedef string::char_type char_type;

public:
	enum Flag {
		  NoFlag
		, ZeroPadding          = 0x0001 // The value should be zero padded.
		                                // For d, i, o, u, x, X, a, A, e, E, f, F, g, and G conversions,
		                                // the converted value is padded on the left with zeros rather
		                                // than blanks. If the 0 and - flags both appear, the 0 flag is ignored.
		                                // If a precision is given with a numeric conversion (d, i, o, u, x, and X),
		                                // the 0 flag is ignored.  For other conversions, the behavior is undefined.
		, LeftJustify          = 0x0002 // The  converted  value  is  to  be  left adjusted on the field boundary.
		                                // (The default is right justification.)  Except for n conversions,
		                                // the converted value is padded on the right with blanks, rather than on
		                                // the left with blanks or zeros.  A - overrides a 0 if both are given.
		, SpaceBeforePositive  = 0x0004 // Left space before positive number
		, NeedSign             = 0x0008 // A sign (+ or -) should always be placed before a number produced by a signed conversion.
		                                // By default a sign is used only for negative numbers.
		                                // @c NeedSign overrides a @c SpaceBeforePositive if both are used.
		, Alternate            = 0x0010 // The value should be converted to an "alternate form".
										// For o conversions, the first character of the output  string
        								// is  made  zero  (by  prefixing  a 0 if it was not zero already).
										// For x and X conversions, a nonzero result has the
        								// string "0x" (or "0X" for X conversions) prepended to it.
										// For a, A, e, E, f, F, g, and G  conversions,  the  result
        								// will  always contain a decimal point, even if no digits follow it
										// (normally, a decimal point appears in the results
        								// of those conversions only if a digit follows).
										// For g and G conversions, trailing zeros are not removed from the
        								// result as they would otherwise be.  For other conversions, the result is undefined.
	};

	/*       Value     CompatGCC         CompatMSC
	 * 	%p   123ABC     0x123abc           00123ABC
	 *  %+o	  -2875        -5473        37777772305
	 */
	enum Compat {
		  CompatGCC = 0
		, CompatMSC
		, CompatMSVC = CompatMSC
	};

	struct conversion_spec
	{
		int    flags;
		int    width;
		int    prec; // The default precision is 1
		char_type spec_char;
	};

	struct context
	{
		string           format;
		const_iterator   pos; // current position in format string
		string           result;
		conversion_spec  spec;
		Compat           compat;
	};

private:
	// Deny copy
	safeformat (const safeformat & );
	safeformat & operator = (const safeformat & sf);

private:
	void advance ();
	void clearSpec ()
	{
		_ctx.spec.flags     = safeformat::NoFlag;
		_ctx.spec.width     = 0;
		_ctx.spec.prec      = 1;
		_ctx.spec.spec_char = char_type(0);
	}

	bool isDigit (char_type v)
	{
		return is_digit(v);
	}

	bool isDigitExcludeZero (char_type v)
	{
		return (isDigit(v) && v != char_type('0'));
	}

	bool parsePercentChar ();
	bool parseFlags ();
	bool parseFieldWidth ();
	bool parsePrecision ();
	bool parseConvSpec ();
	bool parseSpec ();

	void setZeroPadding ()         { _ctx.spec.flags |= ZeroPadding; }
	void setLeftJustify ()         { _ctx.spec.flags |= LeftJustify; }
	void setRightJustify ()        { _ctx.spec.flags &= ~LeftJustify; }
	void setSpaceBeforePositive () { _ctx.spec.flags |= SpaceBeforePositive; }
	void setNeedSign ()            { _ctx.spec.flags |= NeedSign; }
	void setAlternate ()           { _ctx.spec.flags |= Alternate; }
	void setFieldWidth (int w)     { _ctx.spec.width = w; }
	void setPrecision (int p)      { _ctx.spec.prec = p; }
	void setConvSpecifier (char_type c) { _ctx.spec.spec_char = c; }

	void prependSign (string & r);
	void doPadding (string & r);

	safeformat & arg (const __sf_base_traits * v);

public:
	safeformat () {}

	explicit safeformat (const string & format)
	{
		_ctx.format = format;
		_ctx.pos = _ctx.format.cbegin();
		setCompat((Compat)globalCompat());
		clearSpec();
	}

	explicit safeformat (const char * latin1Format)
	{
		_ctx.format = string::fromLatin1(latin1Format);
		_ctx.pos = _ctx.format.cbegin();
		setCompat((Compat)globalCompat());
		clearSpec();
	}

	/**
	 * @brief Set compatibility flag.
	 *
	 * @param c Compatibility flag to set.
	 * @return Instance itself.
	 */
	safeformat & setCompat (Compat c) { _ctx.compat = c; return *this; }

	/**
	 * @brief Set compatibility flag globally
	 *
	 * @param c Compatibility flag to set.
	 */
	static void setGlobalCompat (Compat c);

	/**
	 * @brief Set global compatibility flag.
	 *
	 * @return Compatibility flag.
	 */
	static int globalCompat ();

	template <typename T>
	safeformat & operator () (const T & v)
	{
		__sf_traits<T> t(v);
		return arg(& t);
	}

	safeformat & operator () (char c);
	safeformat & operator () (signed char n);
	safeformat & operator () (unsigned char n);
	safeformat & operator () (short n);
	safeformat & operator () (unsigned short n);
	safeformat & operator () (int n);
	safeformat & operator () (unsigned int n);
	safeformat & operator () (long n);
	safeformat & operator () (unsigned long n);

#ifdef PFS_HAVE_LONGLONG
	safeformat & operator () (long long n);
	safeformat & operator () (unsigned long long n);
#endif

	safeformat & operator () (float n);
	safeformat & operator () (double n);

#ifdef PFS_HAVE_LONG_DOUBLE
	safeformat & operator () (long double n);
#endif

	safeformat & operator () (ucchar c);
	safeformat & operator () (const string & s);
	safeformat & operator () (const char * s)
	{
		string ss(s);
		return operator () (ss);
	}

	safeformat & operator () (void * p);

	string & operator () () { return _ctx.result; }

//--- boost-like operators
	safeformat & operator % (char c)               { return operator () (c); }
	safeformat & operator % (signed char n)        { return operator () (n); }
	safeformat & operator % (unsigned char n)      { return operator () (n); }
	safeformat & operator % (short n)              { return operator () (n); }
	safeformat & operator % (unsigned short n)     { return operator () (n); }
	safeformat & operator % (int n)                { return operator () (n); }
	safeformat & operator % (unsigned int n)       { return operator () (n); }
	safeformat & operator % (long n)               { return operator () (n); }
	safeformat & operator % (unsigned long n)      { return operator () (n); }

#ifdef PFS_HAVE_LONGLONG
	safeformat & operator % (long long n)          { return operator () (n); }
	safeformat & operator % (unsigned long long n) { return operator () (n); }
#endif

	safeformat & operator % (float n)              { return operator () (n); }
	safeformat & operator % (double n)             { return operator () (n); }

#ifdef PFS_HAVE_LONG_DOUBLE
	safeformat & operator % (long double n)        { return operator () (n); }
#endif

	safeformat & operator % (ucchar c)             { return operator () (c); }
	safeformat & operator % (const string & s)     { return operator () (s); }
	safeformat & operator % (const char * s)       { return operator () (s); }
	safeformat & operator % (void * p)             { return operator () (p); }

//--- Qt-like methods
	safeformat & arg (char c)               { return operator () (c); }
	safeformat & arg (signed char n)        { return operator () (n); }
	safeformat & arg (unsigned char n)      { return operator () (n); }
	safeformat & arg (short n)              { return operator () (n); }
	safeformat & arg (unsigned short n)     { return operator () (n); }
	safeformat & arg (int n)                { return operator () (n); }
	safeformat & arg (unsigned int n)       { return operator () (n); }
	safeformat & arg (long n)               { return operator () (n); }
	safeformat & arg (unsigned long n)      { return operator () (n); }

#ifdef PFS_HAVE_LONGLONG
	safeformat & arg (long long n)          { return operator () (n); }
	safeformat & arg (unsigned long long n) { return operator () (n); }
#endif

	safeformat & arg (float n)              { return operator () (n); }
	safeformat & arg (double n)             { return operator () (n); }

#ifdef PFS_HAVE_LONG_DOUBLE
	safeformat & arg (long double n)        { return operator () (n); }
#endif

	safeformat & arg (ucchar c)             { return operator () (c); }
	safeformat & arg (const string & s)     { return operator () (s); }
	safeformat & arg (const char * s)       { return operator () (s); }
	safeformat & arg (void * p)             { return operator () (p); }

private:
	context _ctx;
};

} // pfs

#endif /* __PFS_SAFEFORMAT_HPP__ */
