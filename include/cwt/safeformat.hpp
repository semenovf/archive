/*
 * safeformat.hpp
 *
 *  Created on: Jul 29, 2013
 *      Author: wladt
 */

#ifndef __CWT_SAFEFORMAT_HPP__
#define __CWT_SAFEFORMAT_HPP__

#include <pfs/unitype.hpp>
#include <pfs/string.hpp>
#include <pfs/bytearray.hpp>
#include <pfs/shared_ptr.hpp>

#define _Fr(s) cwt::safeformat(_u8(s))
#define _F(s) cwt::safeformat(_u8(s))

namespace cwt {

struct safeformatcontext;

class safeformat
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
	safeformat () {}
	safeformat (const pfs::string & format);
	safeformat (const char * latin1Format);

	safeformat(const safeformat & sf) : m_context(sf.m_context) { }
	safeformat & operator = (const safeformat & sf) { m_context = sf.m_context; return *this; }

	operator pfs::string & ();
	const pfs::string & operator () ();

	safeformat & operator () (const pfs::unitype & ut);
	safeformat & operator () (char c)               { return operator () (pfs::unitype(c)); }
	safeformat & operator () (int_t n)              { return operator () (pfs::unitype(n)); }
	safeformat & operator () (uint_t n)             { return operator () (pfs::unitype(n)); }
	safeformat & operator () (long_t n)             { return operator () (pfs::unitype(n)); }
	safeformat & operator () (ulong_t n)            { return operator () (pfs::unitype(n)); }
	safeformat & operator () (float n)              { return operator () (pfs::unitype(n)); }
	safeformat & operator () (double n)             { return operator () (pfs::unitype(n)); }
	safeformat & operator () (const char * latin1)  { return operator () (pfs::unitype(pfs::string(latin1))); }
	safeformat & operator () (const void * ptr)     { return operator () (pfs::unitype(uintptr_t(ptr))); }
	safeformat & operator () (const pfs::string & s)     { return operator () (pfs::unitype(s)); }
	safeformat & operator () (const pfs::bytearray & ba) { return operator () (pfs::unitype(ba)); }

	// boost-like operators
	safeformat & operator % (const pfs::unitype & ut)   { return operator () (ut); }
	safeformat & operator % (char c)               { return operator () (c); }
	safeformat & operator % (int_t n)              { return operator () (n); }
	safeformat & operator % (uint_t n)             { return operator () (n); }
	safeformat & operator % (long_t n)             { return operator () (n); }
	safeformat & operator % (ulong_t n)            { return operator () (n); }
	safeformat & operator % (float n)              { return operator () (n); }
	safeformat & operator % (double n)             { return operator () (n); }
	safeformat & operator % (const char * latin1)  { return operator () (pfs::unitype(pfs::string(latin1))); }
	safeformat & operator % (const void * ptr)     { return operator () (pfs::unitype(uintptr_t(ptr))); }
	safeformat & operator % (const pfs::string & s)     { return operator () (pfs::unitype(s)); }
	safeformat & operator % (const pfs::bytearray & ba) { return operator () (pfs::unitype(ba)); }

	// Qt-like methods
	safeformat & arg (const pfs::unitype & ut)   { return operator () (ut); }
	safeformat & arg (char c)               { return operator () (c); }
	safeformat & arg (int_t n)              { return operator () (n); }
	safeformat & arg (uint_t n)             { return operator () (n); }
	safeformat & arg (long_t n)             { return operator () (n); }
	safeformat & arg (ulong_t n)            { return operator () (n); }
	safeformat & arg (float n)              { return operator () (n); }
	safeformat & arg (double n)             { return operator () (n); }
	safeformat & arg (const char * latin1)  { return operator () (pfs::unitype(pfs::string(latin1))); }
	safeformat & arg (const void * ptr)     { return operator () (pfs::unitype(uintptr_t(ptr))); }
	safeformat & arg (const pfs::string & s)     { return operator () (pfs::unitype(s)); }
	safeformat & arg (const pfs::bytearray & ba) { return operator () (pfs::unitype(ba)); }

#if CWT_OS_BITS == 32
	safeformat & operator () (long n)             { return operator () (pfs::unitype(n)); }
	safeformat & operator () (unsigned long n)    { return operator () (pfs::unitype(n)); }
	safeformat & operator %  (long n)             { return operator () (n); }
	safeformat & operator %  (unsigned long n)    { return operator () (n); }
	safeformat & arg         (long n)             { return operator () (n); }
	safeformat & arg         (unsigned long n)    { return operator () (n); }
#endif

private:
	pfs::shared_ptr<safeformatcontext> m_context;
};

} // cwt

#endif /* __CWT_SAFEFORMAT_HPP__ */
