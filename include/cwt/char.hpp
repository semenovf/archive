/**
 * @file   Char.hpp
 * @author wladt
 * @date   Jan 24, 2013 3:51:54 PM
 *
 * @brief
 */

#ifndef __CWT_CHAR_HPP__
#define __CWT_CHAR_HPP__

#include <cwt/cwt.h>

CWT_NS_BEGIN

// Qt   QString
// C#
// Java

class DLL_API Char {
public:
	enum SpecialChar {
		  Null = 0x0000
		, ReplacementChar = 0xfffd
		, MaxCodePoint = 0x10ffff
	};

public:
	Char() : ucs(ushort_t(0)) {}
	Char(ushort_t ch) : ucs(ch) {}
	Char(short_t ch)  : ucs(ushort_t(ch)) {}
	Char(uint_t ch)   : ucs(ushort_t(ch & 0xffff)) {}
	Char(int_t ch)    : ucs(ushort_t(ch & 0xffff)) {}
	~Char() {}

	bool isLowSurrogate() const { return isLowSurrogate(ucs); }
	bool isHiSurrogate() const { return isHiSurrogate(ucs); }
	bool isSurrogate() const { return isSurrogate(ucs); }

	char toLatin1() const { return ucs > 0xff ? '\x0' : char(ucs); }
	ushort_t unicode() const { return ucs; }
	ushort_t& unicode() { return ucs; }

	static Char fromLatin1(char ch) { return Char(ushort_t(uchar_t(ch))); }

	/* Low Surrogates: DC00-DFFF
	 * Qt implementation: (ucs4 & 0xfffffc00) == 0xdc00;
	 * */
	static bool isLowSurrogate(uint_t ucs4) { return ucs4 >= 0xdc00 && ucs4 <= 0xdfff; }

	/* High Surrogates: D800-DBFF
	 * Qt implementation: (ucs4 & 0xfffffc00) == 0xd800
	 * */
	static bool isHiSurrogate(uint_t ucs4)  { return ucs4 >= 0xd800 && ucs4 <= 0xdbff; }
	static bool isNonCharacter(uint_t ucs4) { return ucs4 >= 0xfdd0 && (ucs4 <= 0xfdef || (ucs4 & 0xfffe) == 0xfffe); }
	static bool isSurrogate(uint_t ucs4)    { return (ucs4 - 0xd800u < 2048u); }
	static ushort_t hiSurrogate(uint_t ucs4)  { return ushort_t((ucs4>>10) + 0xd7c0); }
	static ushort_t lowSurrogate(uint_t ucs4)   { return ushort_t(ucs4%0x400 + 0xdc00); }
	static bool requiresSurrogates(uint_t ucs4) { return (ucs4 >= 0x10000); }

	/* ucs4 = (hi-0xD800)*400 + (lo-0xDC00) + 10000
	 * Qt implementation: (uint_t(hi)<<10) + lo - 0x35fdc00
	 * */
	static uint_t surrogatePairToUcs4(ushort_t hi, ushort_t lo) { return (hi-0xd800)*400 + (lo-0xdc00) + 10000; }

	friend bool	operator != (Char ch1, Char ch2) { return ch1.ucs != ch2.ucs; }
	friend bool	operator  < (Char ch1, Char ch2) { return ch1.ucs  < ch2.ucs; }
	friend bool	operator <= (Char ch1, Char ch2) { return ch1.ucs <= ch2.ucs; }
	friend bool	operator == (Char ch1, Char ch2) { return ch1.ucs == ch2.ucs; }
	friend bool	operator >  (Char ch1, Char ch2) { return ch1.ucs  > ch2.ucs; }
	friend bool	operator >= (Char ch1, Char ch2) { return ch1.ucs >= ch2.ucs; }

private:
	ushort_t ucs;
};

CWT_NS_END

#endif /* __CWT_CHAR_HPP__ */
