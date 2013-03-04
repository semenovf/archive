/**
 * @file   textcodec.cpp
 * @author wladt
 * @date   Mar 1, 2013 11:08:46 AM
 *
 * @brief
 */

#include "../include/cwt/textcodec.hpp"
#include "utfcodec_p.hpp"
#include "localecodec_p.hpp"
#include <cwt/mt.h>
#include <cstring>

CWT_NS_BEGIN

static LocaleTextCodec *__localeTextCodec = NULL;
static Utf8TextCodec *__utf8TextCodec = NULL;

TextCodec* TextCodec::forName(const char* name)
{
	if (!name)
		return NULL;

	AutoLock<> locker(&g_mutex);

	// Search in list of standard codecs
	if (strcasecmp(name, "system") == 0
			|| strcasecmp(name, "locale") == 0) {
		if (!__localeTextCodec)
			__localeTextCodec = new LocaleTextCodec;
		return __localeTextCodec;
	} else if (strcasecmp(name, "utf8") == 0
			|| strcasecmp(name, "utf-8") == 0) {
		if (!__utf8TextCodec)
			__utf8TextCodec = new Utf8TextCodec;
		return __utf8TextCodec;
	}

	return NULL;
}

TextCodec* TextCodec::forLocale()
{
	AutoLock<> locker(&g_mutex);
	if (!__localeTextCodec)
		__localeTextCodec = new LocaleTextCodec;
	return __localeTextCodec;
}

CWT_NS_END
