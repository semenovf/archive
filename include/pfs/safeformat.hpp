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
#include <pfs/variant.hpp>

//#define _Fr(s) pfs::safeformat(_u8(s))

namespace pfs {

class safeformat
{
public:
	typedef typename string::const_iterator const_iterator;
	typedef typename string::char_type char_type;
	typedef pfs::variant<real_t, long_t, string, ucchar> variant_type;

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
	void setSpaceBeforePositive () { _ctx.spec.flags |= SpaceBeforePositive; }
	void setNeedSign ()            { _ctx.spec.flags |= NeedSign; }
	void setAlternate ()           { _ctx.spec.flags |= Alternate; }
	void setFieldWidth (int w)     { _ctx.spec.width = w; }
	void setPrecision (int p)      { _ctx.spec.prec = p; }
	void setConvSpecifier (char_type c) { _ctx.spec.spec_char = c; }

	void prependSign (string & r);
	void doPadding (string & r);

	safeformat & operator () (const variant_type & v);

public:
	safeformat () {}

	explicit safeformat (const string & format)
	{
		_ctx.format = format;
		_ctx.pos = _ctx.format.cbegin();
		clearSpec();
	}

	explicit safeformat (const char * latin1Format)
	{
		_ctx.format = string::fromLatin1(latin1Format);
		_ctx.pos = _ctx.format.cbegin();
		clearSpec();
	}

	safeformat & operator () (char c)           { return operator () (variant_type(long_t(c))); }
	safeformat & operator () (unsigned char c)  { return operator () (variant_type(ulong_t(c))); }
	safeformat & operator () (short n)          { return operator () (variant_type(long_t(n))); }
	safeformat & operator () (unsigned short n) { return operator () (variant_type(ulong_t(n))); }
	safeformat & operator () (int n)            { return operator () (variant_type(long_t(n))); }
	safeformat & operator () (unsigned int n)   { return operator () (variant_type(ulong_t(n))); }
	safeformat & operator () (long n)           { return operator () (variant_type(long_t(n))); }
	safeformat & operator () (unsigned long n)  { return operator () (variant_type(ulong_t(n))); }

#ifdef PFS_HAVE_LONGLONG
	safeformat & operator () (long long n)      { return operator () (variant_type(long_t(n))); }
	safeformat & operator () (unsigned long long n) { return operator () (variant_type(ulong_t(n))); }
#endif

	safeformat & operator () (float n)          { return operator () (variant_type(real_t(n))); }
	safeformat & operator () (double n)         { return operator () (variant_type(real_t(n))); }

#ifdef PFS_HAVE_LONG_DOUBLE
	safeformat & operator () (long double n)    { return operator () (variant_type(real_t(n))); }
#endif

	safeformat & operator () (ucchar ch)        { return operator () (variant_type(ch)); }
	safeformat & operator () (const char * s)   { return operator () (variant_type(string(s))); }
	safeformat & operator () (const pfs::string & s) { return operator () (variant_type(s)); }
	safeformat & operator () (const pfs::byte_string & s) { return operator () (variant_type(string::fromUtf8(s))); }
	safeformat & operator () (void * p)         { return operator () (variant_type(ulong_t(p))); }

	string & operator () () { return _ctx.result; }

//	template <typename Arg>
//	safeformat & operator () (const Arg & a) { return operator () (a.toString()); }

//	safeformat & operator () (int_t n)              { return operator () (pfs::unitype(n)); }
//	safeformat & operator () (uint_t n)             { return operator () (pfs::unitype(n)); }
//	safeformat & operator () (long_t n)             { return operator () (pfs::unitype(n)); }
//	safeformat & operator () (ulong_t n)            { return operator () (pfs::unitype(n)); }
//	safeformat & operator () (real_t n)             { return operator () (pfs::unitype(n)); }
//	safeformat & operator () (const char * latin1)  { return operator () (pfs::unitype(pfs::string(latin1))); }
//	safeformat & operator () (const void * ptr)     { return operator () (pfs::unitype(uintptr_t(ptr))); }
//	safeformat & operator () (const pfs::string & s)     { return operator () (pfs::unitype(s)); }
//	safeformat & operator () (const pfs::bytearray & ba) { return operator () (pfs::unitype(ba)); }
//
//	// boost-like operators
//	safeformat & operator % (const pfs::unitype & ut)   { return operator () (ut); }
//	safeformat & operator % (char c)               { return operator () (c); }
//	safeformat & operator % (int_t n)              { return operator () (n); }
//	safeformat & operator % (uint_t n)             { return operator () (n); }
//	safeformat & operator % (long_t n)             { return operator () (n); }
//	safeformat & operator % (ulong_t n)            { return operator () (n); }
//	safeformat & operator % (real_t n)             { return operator () (n); }
//	safeformat & operator % (const char * latin1)  { return operator () (pfs::unitype(pfs::string(latin1))); }
//	safeformat & operator % (const void * ptr)     { return operator () (pfs::unitype(uintptr_t(ptr))); }
//	safeformat & operator % (const pfs::string & s)     { return operator () (pfs::unitype(s)); }
//	safeformat & operator % (const pfs::bytearray & ba) { return operator () (pfs::unitype(ba)); }
//
//	// Qt-like methods
//	safeformat & arg (const pfs::unitype & ut)   { return operator () (ut); }
//	safeformat & arg (char c)               { return operator () (c); }
//	safeformat & arg (int_t n)              { return operator () (n); }
//	safeformat & arg (uint_t n)             { return operator () (n); }
//	safeformat & arg (long_t n)             { return operator () (n); }
//	safeformat & arg (ulong_t n)            { return operator () (n); }
//	safeformat & arg (real_t n)             { return operator () (n); }
//	safeformat & arg (const char * latin1)  { return operator () (pfs::unitype(pfs::string(latin1))); }
//	safeformat & arg (const void * ptr)     { return operator () (pfs::unitype(uintptr_t(ptr))); }
//	safeformat & arg (const pfs::string & s)     { return operator () (pfs::unitype(s)); }
//	safeformat & arg (const pfs::bytearray & ba) { return operator () (pfs::unitype(ba)); }

private:
	context _ctx;
};

} // pfs

#endif /* __PFS_SAFEFORMAT_HPP__ */
