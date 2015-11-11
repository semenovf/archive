/**
 * @file   errorable_ext.cpp
 * @author wladt
 * @date   Feb 25, 2013 3:50:17 PM
 *
 * @brief
 */

#include "pfs/errorable_ext.hpp"
#include "pfs/logger.hpp"
#include "pfs/platform/error_code.hpp"

namespace pfs {

void errorable_ext::addSystemError (int errn, const pfs::string & caption)
{
	pfs::string errstr;
	pfs::lexical_cast(pfs::platform::error_code(errn), errstr);

	if (caption.isEmpty()) {
		addError(_l1("[errno=") << string::toString(errn) << "]: " << errstr);
	} else {
		addError(pfs::string(caption) << " [errno=" << string::toString(errn) << "]: " << errstr);
	}
}

/**
 * @brief Output errors to error logger.
 *
 * @details Errors automatically cleared after output.
 */
void errorable_ext::logErrors ()
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
			pfs::error(r);
		}
	}
	clearErrors();
}

} // pfs
