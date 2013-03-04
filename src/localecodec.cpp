/**
 * @file   localecodec.cpp
 * @author wladt
 * @date   Mar 4, 2013 12:26:36
 *
 * @brief
 */

#include "localecodec_p.hpp"

CWT_NS_BEGIN

String LocaleTextCodec::decode(const char *bytes, size_t len, CodecState *state)
{
	CWT_UNUSED2(bytes, state);

	if(!len)
		return String();

	String result;
	return result;
}

ByteArray LocaleTextCodec::encode(const Char *chars, size_t len, CodecState *state)
{
	ByteArray bytes;

	CWT_UNUSED2(chars, state);

	if(!len)
		return ByteArray();

	return bytes;
}

CWT_NS_END
