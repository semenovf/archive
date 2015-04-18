/*
 * uchar.cpp
 *
 *  Created on: Aug 14, 2013
 *      Author: wladt
 */

#include "pfs/ucchar.hpp"

namespace pfs {

const ucchar ucchar::MaxCodePoint      (0x10ffff);
const ucchar ucchar::Null              (0x0000);
const ucchar ucchar::ReplacementChar   (uint32_t(0x0000FFFD));
const ucchar ucchar::MaxBMP            (uint32_t(0x0000FFFF));
const ucchar ucchar::BomChar           (uint32_t(0xFEFF));
const ucchar ucchar::HiSurrogateStart  (uint32_t(0xD800));
const ucchar ucchar::HiSurrogateEnd    (uint32_t(0xDBFF));
const ucchar ucchar::LowSurrogateStart (uint32_t(0xDC00));
const ucchar ucchar::LowSurrogateEnd   (uint32_t(0xDFFF));


bool ucchar::isSpace() const
{
// TODO need to use Unicode Database (UCD) to extend the notion of 'space' symbol.
	return _value == uint32_t(' ')
		|| _value == uint32_t('\t')
		|| _value == uint32_t('\n')
		|| _value == uint32_t('\v')
		|| _value == uint32_t('\f')
		|| _value == uint32_t('\r')
		? true : false;
}


bool ucchar::isDigit () const
{
	if (_value < 128)
		return (char(_value) >= '0' && char(_value) <= '9');
	return false;
}

// FIXME need full support of Unicode standard
ucchar ucchar::toLower () const
{
	if (_value < 128) {
		if (char(_value) >= 'A' && char(_value) <= 'Z')
			return ucchar(_value + 0x20);
	}
	return ucchar(_value);
}

// FIXME need full support of Unicode standard
ucchar ucchar::toUpper () const
{
	if (_value < 128) {
		if (char(_value) >= 'a' && char(_value) <= 'z')
			return ucchar(_value - 0x20);
	}
	return ucchar(_value);
}

} // pfs

