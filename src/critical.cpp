/**
 * @file critical.cpp
 * @author wladt
 * @date Jan 28, 2014
 */

#include "../../include/cwt/critical.hpp"

namespace cwt {

void critical::addDetails (const cwt::errorable & details)
{
	size_t n = details.errorCount();

	for (size_t i = 0; i < n; ++i) {
		_detailList.append(details.errorTextAt(i));
	}
}

} // cwt
