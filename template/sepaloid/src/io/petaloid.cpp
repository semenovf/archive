/**
 * @file petaloid.cpp
 * @author
 * @date
 */

#include "petaloid.hpp"

PFS_PETALOID_API pfs::petaloid * __petaloid_ctor__ (const char *, int /*argc*/, char * /*argv*/[])
{
	io::petaloid * p = new io::petaloid;
	return p;
}

PFS_PETALOID_API void  __petaloid_dtor__ (pfs::petaloid * p)
{
	delete p;
}

namespace io {

petaloid * petaloid::_self = nullptr;

}
