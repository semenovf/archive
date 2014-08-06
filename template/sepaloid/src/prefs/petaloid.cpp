/**
 * @file petaloid.cpp
 * @author
 * @date
 */

#include "petaloid.hpp"
#include <cwt/platform.hpp>
#include <cwt/safeformat.hpp>

CWT_PETALOID_API cwt::petaloid * __petaloid_ctor__ (const char *, int /*argc*/, char * /*argv*/[])
{
	prefs::petaloid * p = new prefs::petaloid;
	return p;
}

CWT_PETALOID_API void  __petaloid_dtor__ (cwt::petaloid * p)
{
	delete p;
}

namespace prefs {

petaloid * petaloid::_self = nullptr;

pfs::unitype petaloid::pref (const pfs::string & key) const
{
	pfs::unitype r = _prefs.value(key, pfs::unitype());
	PFS_VERIFY_X(!r.isNull(), pfs::string(_Fr("%s: preference not found") % key).c_str());
	return r;
}

void petaloid::setPref (const pfs::string & key, const pfs::unitype & value)
{
	prefs_map_type::iterator it = _prefs.find(key);
	if (PFS_VERIFY_X(it != _prefs.cend(), pfs::string(_Fr("%s: preference not found") % key).c_str())) {
		it->second = value;
	}
}

}
