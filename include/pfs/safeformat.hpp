/*
 * safeformat.hpp
 *
 *  Created on: Jul 29, 2013
 *      Author: wladt
 */

#ifndef __PFS_SAFEFORMAT_HPP__
#define __PFS_SAFEFORMAT_HPP__

#include <pfs/unicode.hpp>
#include <pfs/string.hpp>
#include <pfs/byte_string.hpp>

#define _Sf(x) pfs::safeformat(_u8(x))

namespace pfs {

struct __sf_base_traits
{
	virtual string stringify_int (int /*base*/, bool /*uppercase*/, bool /*is_unsigned*/) const = 0;
	virtual string stringify_float ( char f, int prec) const = 0;
	virtual string stringify_char () const = 0;
	virtual string stringify_string () const = 0;
	virtual ~__sf_base_traits () {}
};

template <typename T>
struct __sf_traits : public __sf_base_traits
{
	T _val;
	__sf_traits (const T & v) : __sf_base_traits(), _val(v)
	{}

	virtual string stringify_int (int /*base*/, bool /*uppercase*/, bool /*is_unsigned*/) const
	{
		return string();
	}

	virtual string stringify_float ( char /*f*/, int /*prec*/) const
	{
		return string();
	}

	virtual string stringify_char () const
	{
		return string();
	}

	virtual string stringify_string () const
	{
		return string();
	}
};

class DLL_API safeformat
{
public:
	typedef string::const_iterator const_iterator;
	typedef string::value_type char_type;

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

	/*       Value     сompat_gcc         compat_msc
	 * 	%p   123ABC     0x123abc           00123ABC
	 *  %+o	  -2875        -5473        37777772305
	 */
	enum compat_enum {
		  compat_gcc = 0
		, compat_msc
		, compat_msvc = compat_msc
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
		compat_enum      compat;
	};

private:
	context _ctx;

private:
	// Deny copy
	safeformat (const safeformat & );
	safeformat & operator = (const safeformat & sf);

private:
	void advance ();
	void clear_spec ()
	{
		_ctx.spec.flags     = safeformat::NoFlag;
		_ctx.spec.width     = 0;
		_ctx.spec.prec      = 1;
		_ctx.spec.spec_char = char_type(char(0));
	}

	bool is_digit (char_type v)
	{
		return unicode::is_decimal_digit(v);
	}

	bool is_digit_exclude_zero (char_type v)
	{
		return (is_digit(v) && v != '0');
	}

	bool parse_percent_char ();
	bool parse_flags ();
	bool parse_field_width ();
	bool parse_precision ();
	bool parse_conv_spec ();
	bool parse_spec ();

	void set_zero_padding ()              { _ctx.spec.flags |= ZeroPadding; }
	void set_left_justify ()              { _ctx.spec.flags |= LeftJustify; }
	void set_right_justify ()             { _ctx.spec.flags &= ~LeftJustify; }
	void set_space_before_positive ()     { _ctx.spec.flags |= SpaceBeforePositive; }
	void set_need_sign ()                 { _ctx.spec.flags |= NeedSign; }
	void set_alternate ()                 { _ctx.spec.flags |= Alternate; }
	void set_field_width (int w)          { _ctx.spec.width = w; }
	void set_precision (int p)            { _ctx.spec.prec = p; }
	void set_conv_specifier (char_type c) { _ctx.spec.spec_char = c; }

	void prepend_sign (string & r);
	void do_padding (string & r);

	safeformat & arg (const __sf_base_traits * v);

private:    
    safeformat ();
    
public:
	explicit safeformat (const string & format)
	{
		_ctx.format = format;
		_ctx.pos = _ctx.format.cbegin();
		set_compat((compat_enum)global_compat());
		clear_spec();
	}

	explicit safeformat (const char * latin1Format)
	{
		_ctx.format = string(latin1Format);
		_ctx.pos = _ctx.format.cbegin();
		set_compat((compat_enum)global_compat());
		clear_spec();
	}

	/**
	 * @brief Set compatibility flag.
	 *
	 * @param c Compatibility flag to set.
	 * @return Instance itself.
	 */
	safeformat & set_compat (compat_enum c) { _ctx.compat = c; return *this; }

	/**
	 * @brief Set compatibility flag globally
	 *
	 * @param c Compatibility flag to set.
	 */
	static void set_global_compat (compat_enum c);

	/**
	 * @brief Set global compatibility flag.
	 *
	 * @return Compatibility flag.
	 */
	static int global_compat ();

//	template <typename T>
//	safeformat & operator () (T const & v)
//	{
//		__sf_traits<T> t(v);
//		return arg(& t);
//	}
    
//    template <typename T>
//  	safeformat & operator () (T const * p)
//    {
//        return operator () (static_cast<void const *>(p));
//    }

//    template <typename T>
//  	safeformat & operator ()<T *> (T * p)
//    {
//        return operator () (static_cast<void const *>(p));
//    }

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

	safeformat & operator () (string::value_type c);
	safeformat & operator () (string const & s);
	safeformat & operator () (char const * s)
	{
		string ss(s);
		return operator () (ss);
	}
    
    safeformat & operator () (void const * p);

	const string & operator () ()
	{
		return str();
	}

	const string & str ()
	{
		advance();
		return _ctx.result;
	}

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

	safeformat & operator % (string::value_type c) { return operator () (c); }
	safeformat & operator % (const string & s)     { return operator () (s); }
	safeformat & operator % (const char * s)       { return operator () (s); }
	safeformat & operator % (void const * p)       { return operator () (p); }

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

	safeformat & arg (string::value_type c) { return operator () (c); }
	safeformat & arg (const string & s)     { return operator () (s); }
	safeformat & arg (const char * s)       { return operator () (s); }
	safeformat & arg (void const * p)       { return operator () (p); }
};

} // pfs

#endif /* __PFS_SAFEFORMAT_HPP__ */
