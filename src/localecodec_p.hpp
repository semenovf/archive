/**
 * @file   localecodec_p.hpp
 * @author wladt
 * @date   Mar 4, 2013 12:25:20
 *
 * @brief
 */

#ifndef __CWT_LOCALECODEC_P_HPP__
#define __CWT_LOCALECODEC_P_HPP__

#include "../include/cwt/textcodec.hpp"

CWT_NS_BEGIN

class LocaleTextCodec : public TextCodec {
public:
	LocaleTextCodec() {}
	virtual ~LocaleTextCodec() {}

protected:
	virtual String decode(const char *bytes, size_t len, CodecState *state);
	virtual ByteArray encode(const Char *chars, size_t len, CodecState *state);
};


CWT_NS_END

#endif /* __CWT_LOCALECODEC_P_HPP__ */
