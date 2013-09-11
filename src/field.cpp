/*
 * field.cpp
 *
 *  Created on: Sep 11, 2013
 *      Author: wladt
 */

#include "../include/cwt/debby/scheme.hpp"
#include <cwt/logger.hpp>

CWT_NS_BEGIN

namespace debby {

void Field::setBoolean ()
{
	m_type = Debby::TypeBool;
}

void Field::setNumber (ulong_t max)
{
	m_type = Debby::TypeInteger;
}

void Field::setNumber (long_t min, long_t max)
{
	m_type = Debby::TypeInteger;
}

void Field::setNumber (double min, double max)
{
	if (min < CWT_FLOAT_MIN || max > CWT_FLOAT_MAX)
		m_type = Debby::TypeDouble;
	else
		m_type = Debby::TypeFloat;
}

} // namespace debby

}
CWT_NS_END



