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
#include <cwt/mt.hpp>
#include <cwt/hash.hpp>
#include <cwt/string.h>

CWT_NS_BEGIN

static CWT_DEFAULT_MT_POLICY g_decoder_mutex;
static CWT_DEFAULT_MT_POLICY g_encoder_mutex;

typedef Hash<ByteArray, Decoder*> DecodersTable;
typedef Hash<ByteArray, Encoder*> EncodersTable;
static DecodersTable g_decoders;
static EncodersTable g_encoders;

Decoder* Decoder::forName(const char* charset, Char replacement)
{
	if (!charset)
		return NULL;

	AutoLock<> locker(&g_decoder_mutex);
	Decoder *decoder = NULL;

	if (strcasecmp(charset, "system") == 0
			|| strcasecmp(charset, "locale") == 0) {
		;//return new LocaleTextCodec;
	} else if (strcasecmp(charset, "utf8") == 0
			|| strcasecmp(charset, "utf-8") == 0) {
		decoder = g_decoders.contains("utf8")
				? g_decoders["utf8"]
				: g_decoders.insert("utf8", new Utf8Decoder(replacement)).value();
	}

	if (decoder) {
		decoder->m_replacement = replacement;
	}

	return decoder;
}

Encoder* Encoder::forName(
		const char* charset
		, bool ignoreHeader
		, char replacement)
{
	if (!charset)
		return NULL;

	AutoLock<> locker(&g_encoder_mutex);
	Encoder *encoder = NULL;

	if (strcasecmp(charset, "system") == 0
			|| strcasecmp(charset, "locale") == 0) {
		;//return new LocaleTextCodec;
	} else if (strcasecmp(charset, "utf8") == 0
			|| strcasecmp(charset, "utf-8") == 0) {
		encoder = g_encoders.contains("utf8")
				? g_encoders["utf8"]
				: g_encoders.insert("utf8", new Utf8Encoder(ignoreHeader, replacement)).value();
	}

	if (encoder) {
		encoder->m_replacement = replacement;
		encoder->m_ignoreHeader = ignoreHeader;
	}

	return encoder;
}

void Decoder::cleanup()
{
	AutoLock<> locker(&g_decoder_mutex);
	DecodersTable::iterator it = g_decoders.begin();
	DecodersTable::iterator itEnd = g_decoders.end();
	for (; it != itEnd; ++it) {
		delete it.value();
		it.value() = NULL;
	}
	g_decoders.clear();
}

void Encoder::cleanup()
{
	AutoLock<> locker(&g_encoder_mutex);
	EncodersTable::iterator it = g_encoders.begin();
	EncodersTable::iterator itEnd = g_encoders.end();
	for (; it != itEnd; ++it) {
		delete it.value();
		it.value() = NULL;
	}
	g_encoders.clear();
}

CWT_NS_END
