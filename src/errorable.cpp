/**
 * @file   errorable.cpp
 * @author wladt
 * @date   Feb 25, 2013 3:50:17 PM
 *
 * @brief
 */

#include "../include/cwt/errorable.hpp"
#include <cstdarg>

CWT_NS_BEGIN

void Errorable::addSystemError(int errn, const String &prefix)
{
	char errstr[SystemErrorBufLen];
	cwt_strerror(errn, errstr, SystemErrorBufLen);
	if (prefix.isEmpty()) {
		addError(String::fromUtf8(errstr));
	} else {
		addError(String().sprintf("%s: %s", prefix.c_str(), errstr));
	}
}

void Errorable::addSystemError(int errn, const char *prefix, ...)
{
	if (!prefix) {
		addSystemError(errn, String());
	} else {
		va_list args;
		va_start(args, prefix);
		String s = String().vsprintf(prefix, args);
		va_end(args);
		addSystemError(errn, s);
	}
}

void Errorable::logErrors(bool clear)
{
	Vector<ErrorData>::const_iterator itEnd = m_errors.cend();
	for (Vector<ErrorData>::const_iterator it = m_errors.cbegin()
			; it != itEnd; ++it) {
		if (it->ntimes > 1) {
			Logger::error(_Tr("%s <occured %d times>"), it->errstr.c_str(), it->ntimes);

		} else {
			Logger::error("%s", it->errstr.c_str());
		}
	}

	if (clear)
		clearErrors();
}


void Errorable::addError(const char * cformat, ...)
{
	va_list args;
	va_start(args, cformat);
	String s = String().vsprintf(cformat, args);
	va_end(args);
	addError(s);
}


CWT_NS_END
