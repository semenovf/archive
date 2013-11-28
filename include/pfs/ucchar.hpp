/**
 * uchar.hpp
 *
 *  Created on: Aug 14, 2013
 *      Author: wladt
 *
 * @brief Unicode code point
 */

#ifndef __PFS_UCHAR_HPP__
#define __PFS_UCHAR_HPP__

#include <pfs.hpp>

namespace pfs {

class DLL_API ucchar
{
public:
	static const uint32_t MaxCodePoint    = 0x10ffff;
	static const uint32_t Null            = 0x0000;
	static const uint32_t ReplacementChar = uint32_t(0x0000FFFD);
	static const uint32_t MaxBMP          = uint32_t(0x0000FFFF);

	static const uint32_t HiSurrogateStart  = uint32_t(0xD800);
	static const uint32_t HiSurrogateEnd    = uint32_t(0xDBFF);
	static const uint32_t LowSurrogateStart = uint32_t(0xDC00);
	static const uint32_t LowSurrogateEnd   = uint32_t(0xDFFF);

public:
	ucchar() : _value(Null) {}
	ucchar(char latin1) : _value(latin1) { PFS_ASSERT(byte_t(latin1) <= 127); }
	ucchar(uchar_t latin1) : _value(latin1) { PFS_ASSERT(latin1 <= 127); }
	ucchar(int ucs) : _value(ucs) { PFS_ASSERT(uint32_t(ucs)); }
	ucchar(uint32_t ucs4) : _value(ucs4) { }
	ucchar(const ucchar & ucs4) : _value(ucs4._value) { }
	operator uint32_t () const { return _value; }
	operator uint32_t & () { return _value; }
	uint32_t value () const { return _value; }
	uint32_t ucs4 () const { return _value; }
	uint32_t unicode () const { return _value; }
	size_t  encodeUtf8 (char utf8[7]);

	bool isSpace() const;
	ucchar toLower () const;
	ucchar toUpper () const;


	/* Low Surrogates: DC00-DFFF
	 * Qt implementation: (ucs4 & 0xfffffc00) == 0xdc00;
	 * */
	static bool isLowSurrogate(const ucchar & ucs4) { return ucs4 >= LowSurrogateStart && ucs4 <= LowSurrogateEnd; }

	/* High Surrogates: D800-DBFF
	 * Qt implementation: (ucs4 & 0xfffffc00) == 0xd800
	 * */
	static bool isHiSurrogate(const ucchar & ucs4)      { return ucs4 >= HiSurrogateStart && ucs4 <= HiSurrogateEnd; }
	static bool isSurrogate(const ucchar & ucs4)        { return (ucs4 - HiSurrogateStart < 2048u); }
	static bool isValid(const ucchar & ch, const ucchar & min_uc = ucchar());

	static int decodeUtf8(const char * units, size_t len, uint32_t & uc, uint32_t & min_uc);

private:
	uint32_t _value;
};

inline bool ucchar::isValid(const ucchar & ch, const ucchar & min_uc)
{
	return (ch >= min_uc)     // overlong
			&& (ch != 0xFEFF) // the BOM
			&& (!isSurrogate(ch))
			&& (ch <= MaxCodePoint);
}

} // pfs

#endif /* __PFS_UCHAR_HPP__ */
