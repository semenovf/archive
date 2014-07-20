/**
 * @file petaloid.cpp
 * @author wladt
 * @date Jul 20, 2014
 */

#include "petaloid.hpp"

CWT_PETALOID_API cwt::petaloid * __petaloid_ctor__ (const char *, int /*argc*/, char * /*argv*/[])
{
	hg::booker::io::petaloid * p = new hg::booker::io::petaloid;
	return p;
}

CWT_PETALOID_API void  __petaloid_dtor__ (cwt::petaloid * p)
{
	delete p;
}

namespace hg { namespace booker { namespace io {

petaloid * petaloid::_self = nullptr;

}}}
