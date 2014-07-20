/**
 * @file petaloid.cpp
 * @author wladt
 * @date Mar 20, 2014
 */

#include "petaloid.hpp"
#include <pfs/stringlist.hpp>
#include <cwt/logger.hpp>

CWT_PETALOID_API cwt::petaloid * __petaloid_ctor__ (const char *, int /*argc*/, char * /*argv*/[])
{
	hg::booker::logger::petaloid * p = new hg::booker::logger::petaloid;
	return p;
}

CWT_PETALOID_API void  __petaloid_dtor__ (cwt::petaloid * p)
{
	delete p;
}

namespace hg { namespace booker { namespace logger {

petaloid * petaloid::_self = nullptr;

// TODO Where to output/archive this kind of messages?
//
void petaloid::onCritical (const cwt::critical & critical)
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
		cwt::error(_Fr("%s%s") % critical.text() % details);
	else
		cwt::error(critical.text());
}

void petaloid::onInfo  (const pfs::string & msg)
{
	cwt::info(msg);
}

void petaloid::onDebug (const pfs::string & msg)
{
	cwt::debug(msg);
}

void petaloid::onWarn  (const pfs::string & msg)
{
	cwt::warn(msg);
}

void petaloid::onError (const pfs::string & msg)
{
	cwt::error(msg);
}

}}}
