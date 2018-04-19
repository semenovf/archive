/*
 * uchar.cpp
 *
 *  Created on: Aug 14, 2013
 *      Author: wladt
 */


#include "../include/cwt/uchar.hpp"

CWT_NS_BEGIN

bool UChar::isSpace() const
{
// TODO need to use Unicode Database (UCD) to extend the notion of 'space' symbol.
	return m_value == uint32_t(' ')
		|| m_value == uint32_t('\t')
		|| m_value == uint32_t('\n')
		|| m_value == uint32_t('\v')
		|| m_value == uint32_t('\f')
		|| m_value == uint32_t('\r')
		? true : false;
}

CWT_NS_END

