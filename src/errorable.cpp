/**
 * @file   errorable.cpp
 * @author wladt
 * @date   Feb 25, 2013 3:50:17 PM
 *
 * @brief
 */

#include "../include/cwt/errorable.hpp"
#include <cwt/safeformat.hpp>
#include <cstdarg>

extern pfs::string __strerror (int_t errn);

namespace cwt {

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
				r << (_Fr("%s (repeat %d times)") % it->_errstr % it->_ntimes);
			} else {
				r << it->_errstr;
			}
			log::error(r);
		}
	}
	clearErrors();
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
				out << it->_errstr << "( " << _Tr("repeat") << ' ' << it->_ntimes << _Tr("times") << ") " << std::endl;
			} else {
				out << it->_errstr << std::endl;
			}
		}
	}
	er.clearErrors();
	return out;
}

} // cwt
