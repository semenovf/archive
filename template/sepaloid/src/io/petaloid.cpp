/**
 * @file petaloid.cpp
 * @author
 * @date
 */

#include "petaloid.hpp"

CWT_PETALOID_API cwt::petaloid * __petaloid_ctor__ (const char *, int /*argc*/, char * /*argv*/[])
{
	io::petaloid * p = new io::petaloid;
	return p;
}

CWT_PETALOID_API void  __petaloid_dtor__ (cwt::petaloid * p)
{
	delete p;
}

namespace io {

petaloid * petaloid::_self = nullptr;

}
