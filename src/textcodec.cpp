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
#include <cstring>

CWT_NS_BEGIN

CWT_DEFAULT_MT_POLICY Decoder::g_mutex;
CWT_DEFAULT_MT_POLICY Encoder::g_mutex;

Decoder* Decoder::forName(const char* charset, Char replacement)
{
	if (!charset)
		return NULL;

	CWT_UNUSED(replacement); // TODO remove
	AutoLock<> locker(&g_mutex);

	if (strcasecmp(charset, "system") == 0
			|| strcasecmp(charset, "locale") == 0) {
		;//return new LocaleTextCodec;
	} else if (strcasecmp(charset, "utf8") == 0
			|| strcasecmp(charset, "utf-8") == 0) {
		;//return new Utf8Decoder(replacement);
	}

	return NULL;
}

Encoder* Encoder::forName(
		const char* charset
		, bool ignoreHeader
		, char replacement)
{
	if (!charset)
		return NULL;

	CWT_UNUSED2(ignoreHeader,replacement); // TODO remove
	AutoLock<> locker(&g_mutex);

	if (strcasecmp(charset, "system") == 0
			|| strcasecmp(charset, "locale") == 0) {
		;//return new LocaleTextCodec;
	} else if (strcasecmp(charset, "utf8") == 0
			|| strcasecmp(charset, "utf-8") == 0) {
		;//return new Utf8Encoder(ignoreHeader, replacement);
	}

	return NULL;
}
CWT_NS_END
