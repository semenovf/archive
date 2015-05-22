/**
 * @file petaloid.cpp
 * @author
 * @date
 */

#include "petaloid.hpp"
#include <pfs/stringlist.hpp>
#include <pfs/logger.hpp>

PFS_PETALOID_API pfs::petaloid * __petaloid_ctor__ (const char *, int /*argc*/, char * /*argv*/[])
{
	logger::petaloid * p = new logger::petaloid;
	return p;
}

PFS_PETALOID_API void  __petaloid_dtor__ (pfs::petaloid * p)
{
	delete p;
}

namespace logger {

petaloid * petaloid::_self = nullptr;

// TODO Where to output/archive this kind of messages?
//
void petaloid::onCritical (const pfs::critical & critical)
{
	pfs::string details;

	if (critical.details().size() > 0) {
		pfs::stringlist::const_iterator it = critical.details().cbegin();
		pfs::stringlist::const_iterator itEnd = critical.details().cend();

		for (; it != itEnd; ++it) {
			details << "\n\t" << *it;
		}
	}

	if (details.length() > 0)
		pfs::error(_Fr("%s%s") % critical.text() % details);
	else
		pfs::error(critical.text());
}

void petaloid::onInfo  (const pfs::string & msg)
{
	pfs::info(msg);
}

void petaloid::onDebug (const pfs::string & msg)
{
	pfs::debug(msg);
}

void petaloid::onWarn  (const pfs::string & msg)
{
	pfs::warn(msg);
}

void petaloid::onError (const pfs::string & msg)
{
	pfs::error(msg);
}

}
