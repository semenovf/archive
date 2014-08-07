/**
 * @file   errorable.cpp
 * @author wladt
 * @date   Feb 25, 2013 3:50:17 PM
 *
 * @brief
 */

#include "pfs/errorable.hpp"
//#include <cstdarg>

namespace pfs {

void errorable::addError (const pfs::string & text)
{
	if (!text.isEmpty()) {
		if (_errors.size() > 0 && _errors.last()._errstr == text) {
			;
		} else {
			_errors.append(erritem(text));
		}
		_errors.last()._ntimes++;
	}
}

/**
 *
 * @brief Output errors to output stream.
 *
 * @details Errors automatically cleared after output.
 *
 * @param out Stream reference to output errors.
 * @param er Errorable object reference.
 * @return Output stream reference.
 *
 */
std::ostream & operator << (std::ostream & out, errorable & er)
{
	if (er._errors.size() > 0) {
		pfs::vector<errorable::erritem>::const_iterator it = er._errors.cbegin();
		pfs::vector<errorable::erritem>::const_iterator itEnd = er._errors.cend();
		for (; it != itEnd; ++it) {
			if (it->_ntimes > 1) {
				out << it->_errstr << _u8("(repeat ") << it->_ntimes << _u8(" times)") << std::endl;
			} else {
				out << it->_errstr << std::endl;
			}
		}
	}
	er.clearErrors();
	return out;
}

} // pfs
