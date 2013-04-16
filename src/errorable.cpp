/**
 * @file   errorable.cpp
 * @author wladt
 * @date   Feb 25, 2013 3:50:17 PM
 *
 * @brief
 */

#include "../include/cwt/errorable.hpp"

CWT_NS_BEGIN

void Errorable::addSystemError(int errn, const String &prefix)
{
	char errstr[SystemErrorBufLen];
	cwt_strerror(errn, errstr, SystemErrorBufLen);
	if (prefix.isEmpty()) {
		addError(String::fromUtf8(errstr));
	} else {
		addError(String().sprintf("%ls: %s", prefix.unicode(), errstr));
	}
}

void Errorable::addSystemError(int errn, const char *prefix)
{
	if (prefix)
		addSystemError(errn, String::null());
	else
		addSystemError(errn, String::fromUtf8(prefix));
}

void Errorable::logErrors() const
{
	Vector<ErrorData>::const_iterator itEnd = m_errors.end();
	for (Vector<ErrorData>::const_iterator it = m_errors.begin()
			; it != itEnd; ++it) {
		if (it->ntimes > 1) {
			Logger::error(_Tr("%ls <occured %d times>"), it->errstr.unicode(), it->ntimes);

		} else {
			Logger::error("%ls", it->errstr.unicode());
		}
	}
}

CWT_NS_END
