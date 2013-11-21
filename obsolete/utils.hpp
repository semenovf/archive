/*
 * utils.hpp
 *
 *  Created on: Nov 11, 2013
 *      Author: wladt
 */

#ifndef __CWT_DEBBY_UTILS_HPP__
#define __CWT_DEBBY_UTILS_HPP__

#include <cwt/safeformat.hpp>

CWT_NS_BEGIN

namespace debby
{

inline cwt::String buildSqlite3Uri(const cwt::String & name, const cwt::String & dir, const cwt::String & opts)
{
	return _F("sqlite3:%s/%s.db?%s") % dir % name % opts;
}

} // namespace debby

CWT_NS_END

#endif /* __CWT_DEBBY_UTILS_HPP__ */
