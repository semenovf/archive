/**
 * @file petaloid.cpp
 * @author
 * @date
 */

#include "petaloid.hpp"
#include <pfs/platform.hpp>
#include <pfs/safeformat.hpp>

PFS_PETALOID_API pfs::petaloid * __petaloid_ctor__ (const char *, int /*argc*/, char * /*argv*/[])
{
	prefs::petaloid * p = new prefs::petaloid;
	return p;
}

PFS_PETALOID_API void  __petaloid_dtor__ (pfs::petaloid * p)
{
	delete p;
}

namespace prefs {

petaloid * petaloid::_self = nullptr;

//pfs::unitype petaloid::pref (const pfs::string & key) const
//{
//	pfs::unitype r = _prefs.value(key, pfs::unitype());
//	PFS_VERIFY_X(!r.isNull(), pfs::string(_Fr("%s: preference not found") % key).c_str());
//	return r;
//}
//
//void petaloid::setPref (const pfs::string & key, const pfs::unitype & value)
//{
//	prefs_map_type::iterator it = _prefs.find(key);
//	if (PFS_VERIFY_X(it != _prefs.cend(), pfs::string(_Fr("%s: preference not found") % key).c_str())) {
//		it->second = value;
//	}
//}

}
