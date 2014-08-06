/**
 * @file petaloid.cpp
 * @author
 * @date
 */

#include "petaloid.hpp"

CWT_PETALOID_API cwt::petaloid * __petaloid_ctor__ (const char *, int /*argc*/, char * /*argv*/[])
{
	core::petaloid * p = new core::petaloid;
	return p;
}

CWT_PETALOID_API void  __petaloid_dtor__ (cwt::petaloid * p)
{
	delete p;
}

namespace core {

petaloid * petaloid::_self = nullptr;

}
