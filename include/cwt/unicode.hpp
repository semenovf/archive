/*
 * unicode.hpp
 *
 *  Created on: Jul 18, 2013
 *      Author: wladt
 */

#ifndef __CWT_UNICODE_HPP__
#define __CWT_UNICODE_HPP__

#include <cwt/cwt.hpp>

CWT_NS_BEGIN

#ifdef __COMMENT__
class DLL_API Unicode
{
public:
	static const uint32_t MaxCodePoint = 0x10ffff;
	static const uint32_t Null = 0x0000;
public:
	/* Low Surrogates: DC00-DFFF
	 * Qt implementation: (ucs4 & 0xfffffc00) == 0xdc00;
	 * */
	static bool isLowSurrogate(uint32_t ucs4) { return ucs4 >= 0xdc00 && ucs4 <= 0xdfff; }

	/* High Surrogates: D800-DBFF
	 * Qt implementation: (ucs4 & 0xfffffc00) == 0xd800
	 * */
	static bool isHiSurrogate(uint32_t ucs4)      { return ucs4 >= 0xd800 && ucs4 <= 0xdbff; }

	//static bool isNonCharacter(uint32_t ucs4)     { return ucs4 >= 0xfdd0 && (ucs4 <= 0xfdef || (ucs4 & 0xfffe) == 0xfffe); }
	static bool isSurrogate(uint32_t ucs4)        { return (ucs4 - 0xd800u < 2048u); }
	//static uint16_t hiSurrogate(uint32_t ucs4)    { return uint16_t((ucs4 >> 10) + 0xd7c0); }
	//static uint16_t lowSurrogate(uint32_t ucs4)   { return uint16_t(ucs4 % 0x400 + 0xdc00); }
	//static bool requiresSurrogates(uint32_t ucs4) { return (ucs4 >= 0x10000); }

	/* ucs4 = (hi-0xD800)*400 + (lo-0xDC00) + 10000
	 * Qt implementation: (uint_t(hi)<<10) + lo - 0x35fdc00
	 * */
	//static uint32_t surrogatePairToUcs4(uint16_t hi, uint16_t lo) { return (hi-0xd800)*400 + (lo-0xdc00) + 10000; }
	static bool isValid(uint32_t ch, uint32_t min_uc = 0)
	{
		return (ch >= min_uc) // overlong
				&& (ch != 0xFEFF) // the BOM
				&& (!isSurrogate(ch))
				&& (ch <= MaxCodePoint);
	}

};
#endif
CWT_NS_END

#endif /* __CWT_UNICODE_HPP__ */
