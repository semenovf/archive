/**
 * @file critical.cpp
 * @author wladt
 * @date Jan 28, 2014
 */

#include "pfs/critical.hpp"

namespace pfs {

void critical::addDetails (const errorable_ext & details)
{
	size_t n = details.errorCount();

	for (size_t i = 0; i < n; ++i) {
		_detailList.append(details.errorTextAt(i));
	}
}

} // pfs
