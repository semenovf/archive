/*
 * debby_p.hpp
 *
 *  Created on: Aug 15, 2013
 *      Author: wladt
 */

#ifndef __CWT_DEBBY_P_HPP__
#define __CWT_DEBBY_P_HPP__

#include "../include/cwt/debby.hpp"
#include <cwt/json.hpp>


CWT_NS_BEGIN

class Debby::Impl : public Json
{
public:
	Impl() : Json() {  }
	~Impl() { }
};

CWT_NS_END

#endif /* __CWT_DEBBY_P_HPP__ */
