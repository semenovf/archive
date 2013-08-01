/*
 * safeformat.hpp
 *
 *  Created on: Jul 29, 2013
 *      Author: wladt
 */

#ifndef __CWT_SAFEFORMAT_HPP__
#define __CWT_SAFEFORMAT_HPP__

#include <cwt/fsm.hpp>
#include <cwt/unitype.hpp>
#include <cwt/shared_ptr.hpp>

#define _F(s)  cwt::SafeFormat(s)
#define _Fr(s) cwt::SafeFormat(_Tr(s))

CWT_NS_BEGIN

struct SafeFormatContext;

class SafeFormat
{
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
	};

public:
	SafeFormat() {}
	SafeFormat(const String & format);

	SafeFormat(const SafeFormat & sf) : m_context(sf.m_context) { }
	SafeFormat & operator = (const SafeFormat & sf) { m_context = sf.m_context; return *this; }

	operator String & ();

	SafeFormat & operator () (const UniType & ut);
	SafeFormat & operator () (char c)               { return operator () (UniType(c)); }
	SafeFormat & operator () (int_t n)              { return operator () (UniType(n)); }
	SafeFormat & operator () (uint_t n)             { return operator () (UniType(n)); }
	SafeFormat & operator () (long_t n)             { return operator () (UniType(n)); }
	SafeFormat & operator () (ulong_t n)            { return operator () (UniType(n)); }
	SafeFormat & operator () (float n)              { return operator () (UniType(n)); }
	SafeFormat & operator () (double n)             { return operator () (UniType(n)); }
	SafeFormat & operator () (const char * latin1)  { return operator () (UniType(String(latin1))); }
	SafeFormat & operator () (const void * ptr)     { return operator () (UniType(uintptr_t(ptr))); }
	SafeFormat & operator () (const String & s)     { return operator () (UniType(s)); }
	SafeFormat & operator () (const ByteArray & ba) { return operator () (UniType(ba)); }

	// boost-like operators
	SafeFormat & operator % (const UniType & ut)   { return operator () (ut); }
	SafeFormat & operator % (char c)               { return operator () (c); }
	SafeFormat & operator % (int_t n)              { return operator () (n); }
	SafeFormat & operator % (uint_t n)             { return operator () (n); }
	SafeFormat & operator % (long_t n)             { return operator () (n); }
	SafeFormat & operator % (ulong_t n)            { return operator () (n); }
	SafeFormat & operator % (float n)              { return operator () (n); }
	SafeFormat & operator % (double n)             { return operator () (n); }
	SafeFormat & operator % (const char * latin1)  { return operator () (UniType(String(latin1))); }
	SafeFormat & operator % (const void * ptr)     { return operator () (UniType(uintptr_t(ptr))); }
	SafeFormat & operator % (const String & s)     { return operator () (UniType(s)); }
	SafeFormat & operator % (const ByteArray & ba) { return operator () (UniType(ba)); }

	// Qt-like methods
	SafeFormat & arg (const UniType & ut)   { return operator () (ut); }
	SafeFormat & arg (char c)               { return operator () (c); }
	SafeFormat & arg (int_t n)              { return operator () (n); }
	SafeFormat & arg (uint_t n)             { return operator () (n); }
	SafeFormat & arg (long_t n)             { return operator () (n); }
	SafeFormat & arg (ulong_t n)            { return operator () (n); }
	SafeFormat & arg (float n)              { return operator () (n); }
	SafeFormat & arg (double n)             { return operator () (n); }
	SafeFormat & arg (const char * latin1)  { return operator () (UniType(String(latin1))); }
	SafeFormat & arg (const void * ptr)     { return operator () (UniType(uintptr_t(ptr))); }
	SafeFormat & arg (const String & s)     { return operator () (UniType(s)); }
	SafeFormat & arg (const ByteArray & ba) { return operator () (UniType(ba)); }

private:
	shared_ptr<SafeFormatContext> m_context;
};

CWT_NS_END

#endif /* __CWT_SAFEFORMAT_HPP__ */
