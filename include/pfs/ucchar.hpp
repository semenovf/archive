/**
 * ucchar.hpp
 *
 *  Created on: Aug 14, 2013
 *      Author: wladt
 *
 * @brief Unicode code point
 */

#ifndef __PFS_UCHAR_HPP__
#define __PFS_UCHAR_HPP__

#include <pfs.hpp>
#include <pfs/ctype.hpp>

namespace pfs {

class DLL_API ucchar
{
	uint32_t _value;

public:
	// NOTE!!!
	//
	// ucchar.hpp
	// ---
	// static const ucchar MaxCodePoint;
	// ...
	//
	// ---
	// ucchar.cpp
	// ---
	// const ucchar ucchar::MaxCodePoint(0x10ffff);
	// ...
	//
	// Using static member in such manner (see above) can cause in MSVC:
	// error LNK2001: unresolved external symbol "public: static class pfs::ucchar const pfs::ucchar::MaxCodePoint"
	// while linking with dependent library
	//
	// Resolve:
	// 1. Use MaxCodePoint definition as below
	// or
	// 2. Do not use MaxCodePoint in headers
	//

	static const uint32_t MaxCodePoint      = 0x10ffff;
	static const uint32_t Null              = 0x0000;
	static const uint32_t ReplacementChar   = uint32_t(0x0000FFFD);
	static const uint32_t MaxBMP            = uint32_t(0x0000FFFF);
	static const uint32_t BomChar           = uint32_t(0xFEFF);

	static const uint32_t HiSurrogateStart  = uint32_t(0xD800);
	static const uint32_t HiSurrogateEnd    = uint32_t(0xDBFF);
	static const uint32_t LowSurrogateStart = uint32_t(0xDC00);
	static const uint32_t LowSurrogateEnd   = uint32_t(0xDFFF);

public:
	ucchar () : _value(Null) {}

	explicit ucchar (char latin1)   : _value(latin1) { PFS_ASSERT(uint8_t(latin1) <= 127); }
	explicit ucchar (uint8_t latin1): _value(latin1) { PFS_ASSERT(latin1 <= 127); }
	explicit ucchar (int ucs)       : _value(ucs)    { }
	explicit ucchar (uint32_t ucs4) : _value(ucs4)   { }
	ucchar (const ucchar & ucs4)    : _value(ucs4._value) { }
	~ucchar () {}

	operator uint32_t () const { return _value; }
	operator uint32_t & ()     { return _value; }
	uint32_t value () const    { return _value; }
	uint32_t ucs4 () const     { return _value; }
	uint32_t unicode () const  { return _value; }

	void invalidate ();
	bool isValid (const ucchar & min_uc = ucchar()) const;
	bool isSpace () const;
	bool isDigit () const;
	ucchar toLower () const;
	ucchar toUpper () const;

	/* Low Surrogates: DC00-DFFF
	 * Qt implementation: (ucs4 & 0xfffffc00) == 0xdc00;
	 * */
	static bool isLowSurrogate (const ucchar & ucs4) { return ucs4._value >= LowSurrogateStart && ucs4._value <= LowSurrogateEnd; }
	static bool isLowSurrogate (uint32_t ucs4)       { return ucs4 >= LowSurrogateStart && ucs4 <= LowSurrogateEnd; }

	/* High Surrogates: D800-DBFF
	 * Qt implementation: (ucs4 & 0xfffffc00) == 0xd800
	 * */
	static bool isHiSurrogate (const ucchar & ucs4)      { return ucs4._value >= HiSurrogateStart && ucs4._value <= HiSurrogateEnd; }
	static bool isHiSurrogate (uint32_t ucs4)            { return ucs4 >= HiSurrogateStart && ucs4 <= HiSurrogateEnd; }
	static bool isSurrogate (const ucchar & ucs4)        { return (ucs4._value - HiSurrogateStart < 2048u); }
	static bool isSurrogate (uint32_t ucs4)              { return (ucs4 - HiSurrogateStart < 2048u); }
	static bool isValid (const ucchar & ch, const ucchar & min_uc = ucchar());
	static bool isValid (uint32_t ch, uint32_t min_uc);

//	static int decodeUtf8 (const char * units, size_t len, uint32_t & uc, uint32_t & min_uc);

//	template <typename _CodeUnitT>
//	static int decode (const _CodeUnitT * units, size_t len, uint32_t & uc, uint32_t & min_uc);

    size_t encodeUtf8  (uint8_t * buffer, size_t sz);
    size_t encodeUtf16 (uint16_t * buffer, size_t sz);

	// Decode from trusted source
    size_t decodeUtf8  (const uint8_t * units, size_t sz);
    size_t decodeUtf16 (const uint16_t * units, size_t sz);

    template <typename CodeUnitT>
    size_t encode (CodeUnitT * buffer, size_t sz);

    // Decode from trusted source
    template <typename CodeUnitT>
    size_t decode (const CodeUnitT * buffer, size_t sz);
};

inline void ucchar::invalidate ()
{
	PFS_ASSERT(MaxCodePoint < max_type<uint32_t>());
	_value = MaxCodePoint + 1;
}

inline bool ucchar::isValid (const ucchar & min_uc) const
{
	return isValid(_value, min_uc._value);
}

template <>
inline size_t ucchar::encode<uint8_t> (uint8_t * buffer, size_t sz)
{
	return this->encodeUtf8(buffer, sz);
}

template <>
inline size_t ucchar::encode<uint16_t> (uint16_t * buffer, size_t sz)
{
	return this->encodeUtf16(buffer, sz);
}


template <>
inline size_t ucchar::decode<uint8_t> (const uint8_t * begin, size_t sz)
{
	return this->decodeUtf8(begin, sz);
}

template <>
inline size_t ucchar::decode<uint16_t> (const uint16_t * begin, size_t sz)
{
	return this->decodeUtf16(begin, sz);
}

//template <>
//inline int ucchar::decode<char> (const char * units, size_t len, uint32_t & uc, uint32_t & min_uc)
//{
//	return decodeUtf8(units, len, uc, min_uc);
//}


inline bool ucchar::isValid (const ucchar & ch, const ucchar & min_uc)
{
	return isValid(ch._value, min_uc._value);
}

inline bool ucchar::isValid (uint32_t ch, uint32_t min_uc)
{
	return (ch >= min_uc) // overlong
			&& (ch != 0xFEFF)    // the BOM
			&& (!isSurrogate(ch))
			&& (ch <= MaxCodePoint);
}

//
// cctype specializations {
//
template <>
inline bool is_space<ucchar> (ucchar v)
{
	return v.isSpace();
}

template <>
inline ucchar to_upper<ucchar> (ucchar v)
{
	return v.toUpper();
}

template <>
inline bool is_latin1<ucchar> (ucchar v)
{
	return v.value() <= 127;
}

template <>
inline bool eq_latin1<ucchar> (ucchar v, char latin1)
{
	PFS_ASSERT(latin1 >= 0 /*&& latin1 <= 127*/);
	return v.value() == uint32_t(latin1);
}

template <>
inline bool is_digit<ucchar> (ucchar v)
{
	return v.isDigit();
}

template <>
inline int to_digit<ucchar> (ucchar v)
{
	PFS_ASSERT(is_digit(v));
	return v.value() - uint32_t('0');
}
//
// } cctype specializations
//

} // pfs

#endif /* __PFS_UCHAR_HPP__ */
