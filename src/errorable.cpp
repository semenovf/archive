/**
 * @file   errorable.cpp
 * @author wladt
 * @date   Feb 25, 2013 3:50:17 PM
 *
 * @brief
 */

#include "../include/cwt/errorable.hpp"
#include <cstdarg>

extern pfs::string __strerror (int_t errn);

namespace cwt {

void errorable::addSystemError (int errn, const pfs::string & caption)
{
	pfs::string errstr(__strerror(errn));

	if (caption.isEmpty()) {
		addError(errstr);
	} else {
		pfs::string s(caption);
		s << ": ";
		s << errstr;
		addError(s);
	}
}

} // cwt
