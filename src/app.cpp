/*
 * app.cpp
 *
 *  Created on: Aug 15, 2013
 *      Author: wladt
 */


#include "../include/cwt/app.hpp"

CWT_NS_BEGIN

App * App::self = nullptr;

App::App(const String & progname)
	: _program(progname.isEmpty() ? "<anonymous>" : progname)
{
	CWT_CHECK_SIZEOF_TYPES;
	CWT_ASSERT(self == nullptr);
	self = this;
}

CWT_NS_END
