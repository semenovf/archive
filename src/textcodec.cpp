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

CWT_DEFAULT_MT_POLICY StreamDecoder::g_mutex;
CWT_DEFAULT_MT_POLICY StreamEncoder::g_mutex;

StreamDecoder* StreamDecoder::forName(const char* charset, InputStream &is, Char replacement)
{
	if (!charset)
		return NULL;

	AutoLock<> locker(&g_mutex);

	if (strcasecmp(charset, "system") == 0
			|| strcasecmp(charset, "locale") == 0) {
		;//return new LocaleTextCodec;
	} else if (strcasecmp(charset, "utf8") == 0
			|| strcasecmp(charset, "utf-8") == 0) {
		return new Utf8Decoder(is, replacement);
	}

	return NULL;
}

StreamEncoder* StreamEncoder::forName(
		const char* charset
		, OutputStream &os
		, bool ignoreHeader
		, char replacement)
{
	if (!charset)
		return NULL;

	AutoLock<> locker(&g_mutex);

	if (strcasecmp(charset, "system") == 0
			|| strcasecmp(charset, "locale") == 0) {
		;//return new LocaleTextCodec;
	} else if (strcasecmp(charset, "utf8") == 0
			|| strcasecmp(charset, "utf-8") == 0) {
		return new Utf8Encoder(os, ignoreHeader, replacement);
	}

	return NULL;
}
CWT_NS_END
