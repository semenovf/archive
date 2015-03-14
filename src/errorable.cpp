/**
 * @file   errorable.cpp
 * @author wladt
 * @date   Feb 25, 2013 3:50:17 PM
 *
 * @brief
 */

#include "cwt/errorable.hpp"
#include "cwt/logger.hpp"
//#include <cstdarg>

extern pfs::string __cwt_strerror (int_t errn);

namespace cwt {

void errorable::addSystemError (int errn, const pfs::string & caption)
{
	pfs::string errstr(__cwt_strerror(errn));

	if (caption.isEmpty()) {
		addError(errstr);
	} else {
		pfs::string s(caption);
		s << ": ";
		s << errstr;
		addError(s);
	}
}

/**
 * @brief Output errors to error logger.
 *
 * @details Errors automatically cleared after output.
 */
void errorable::logErrors ()
{
	if (_errors.size() > 0) {
		pfs::vector<errorable::erritem>::const_iterator it = _errors.cbegin();
		pfs::vector<errorable::erritem>::const_iterator itEnd = _errors.cend();
		for (; it != itEnd; ++it) {
			pfs::string r;
			if (it->_ntimes > 1) {
				r << it->_errstr << _Tr("(repeat ") << pfs::string::toString(it->_ntimes) << _Tr(" times)");
			} else {
				r << it->_errstr;
			}
			cwt::error(r);
		}
	}
	clearErrors();
}

} // cwt
