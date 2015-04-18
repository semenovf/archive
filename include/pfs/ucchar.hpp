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
	static const ucchar MaxCodePoint;
	static const ucchar Null;
	static const ucchar ReplacementChar;
	static const ucchar MaxBMP;
	static const ucchar BomChar;

	static const ucchar HiSurrogateStart;
	static const ucchar HiSurrogateEnd;
	static const ucchar LowSurrogateStart;
	static const ucchar LowSurrogateEnd;

public:
	ucchar() : _value(Null) {}
	explicit ucchar(char latin1)   : _value(latin1) { PFS_ASSERT(byte_t(latin1) <= 127); }
	explicit ucchar(uchar_t latin1): _value(latin1) { PFS_ASSERT(latin1 <= 127); }
	explicit ucchar(int ucs)       : _value(ucs)    { }
	explicit ucchar(uint32_t ucs4) : _value(ucs4)   { }
	ucchar(const ucchar & ucs4)    : _value(ucs4._value) { }
	~ucchar () {}

	operator uint32_t () const { return _value; }
	operator uint32_t & ()     { return _value; }
	uint32_t value () const    { return _value; }
	uint32_t ucs4 () const     { return _value; }
	uint32_t unicode () const  { return _value; }

	size_t encodeUtf8  (char * utf8, size_t sz);
	size_t encodeUtf16 (uint16_t * utf16, size_t sz);

	int decodeUtf8  (const char * units, size_t len); // TODO reimplement using backward_utf8_char()
	int decodeUtf16 (const uint16_t * units, size_t len);

	template <typename _CodeUnitT>
	size_t encode (_CodeUnitT * units, size_t sz);

	template <typename _CodeUnitT>
	int decode (const _CodeUnitT * units, size_t len);

	void invalidate ();
	bool isValid (const ucchar & min_uc = ucchar()) const;
	bool isSpace () const;
	bool isDigit () const;
	ucchar toLower () const;
	ucchar toUpper () const;

	/* Low Surrogates: DC00-DFFF
	 * Qt implementation: (ucs4 & 0xfffffc00) == 0xdc00;
	 * */
	static bool isLowSurrogate (const ucchar & ucs4) { return ucs4._value >= LowSurrogateStart._value && ucs4._value <= LowSurrogateEnd._value; }
	static bool isLowSurrogate (uint32_t ucs4) { return ucs4 >= LowSurrogateStart._value && ucs4 <= LowSurrogateEnd._value; }

	/* High Surrogates: D800-DBFF
	 * Qt implementation: (ucs4 & 0xfffffc00) == 0xd800
	 * */
	static bool isHiSurrogate (const ucchar & ucs4)      { return ucs4._value >= HiSurrogateStart._value && ucs4._value <= HiSurrogateEnd._value; }
	static bool isHiSurrogate (uint32_t ucs4)            { return ucs4 >= HiSurrogateStart._value && ucs4 <= HiSurrogateEnd._value; }
	static bool isSurrogate (const ucchar & ucs4)        { return (ucs4._value - HiSurrogateStart._value < 2048u); }
	static bool isSurrogate (uint32_t ucs4)              { return (ucs4 - HiSurrogateStart._value < 2048u); }
	static bool isValid (const ucchar & ch, const ucchar & min_uc = ucchar());
	static bool isValid (uint32_t ch, uint32_t min_uc);

	static int decodeUtf8 (const char * units, size_t len, uint32_t & uc, uint32_t & min_uc);

//	template <typename _CodeUnitT>
//	static int decode (const _CodeUnitT * units, size_t len, uint32_t & uc, uint32_t & min_uc);
};

inline void ucchar::invalidate ()
{
	PFS_ASSERT(MaxCodePoint._value < max_type<uint32_t>());
	_value = MaxCodePoint._value + 1;
}

inline bool ucchar::isValid (const ucchar & min_uc) const
{
	return isValid(_value, min_uc._value);
}

template <>
inline size_t ucchar::encode<char> (char * units, size_t sz)
{
	return this->encodeUtf8(units, sz);
}

template <>
inline size_t ucchar::encode<uint16_t> (uint16_t * units, size_t sz)
{
	return this->encodeUtf16(units, sz);
}


template <>
inline int ucchar::decode<char> (const char * units, size_t len)
{
	return this->decodeUtf8(units, len);
}

template <>
inline int ucchar::decode<uint16_t> (const uint16_t * units, size_t len)
{
	return this->decodeUtf16(units, len);
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
			&& (ch <= MaxCodePoint._value);
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
