/**
 * @file   errorable.cpp
 * @author wladt
 * @date   Feb 25, 2013 3:50:17 PM
 *
 * @brief
 */

#include "../include/cwt/safeformat.hpp"
#include "../include/cwt/string.h"
#include "../include/cwt/errorable.hpp"
#include <cstdarg>

CWT_NS_BEGIN

void Errorable::addSystemError(int errn, const String & text)
{
	char errstr[SystemErrorBufLen];
	cwt_strerror(errn, errstr, SystemErrorBufLen);
	if (text.isEmpty()) {
		addError(String::fromUtf8(errstr));
	} else {
		addError(_Fr("%s: %s") % text % errstr);
	}
}

CWT_NS_END
