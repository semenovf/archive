/**
 * @file   utf8codec_p.hpp
 * @author wladt
 * @date   Mar 1, 2013 11:14:20 AM
 *
 * @brief
 */

#ifndef __CWT_UTF8CODEC_P_HPP__
#define __CWT_UTF8CODEC_P_HPP__

#include "../include/cwt/textcodec.hpp"

CWT_NS_BEGIN

class Utf8TextCodec : public TextCodec {
public:
	Utf8TextCodec() {}
	virtual ~Utf8TextCodec() {}

protected:
	virtual String decode(const char *bytes, size_t len, CodecState *state);
	virtual ByteArray encode(const Char *chars, size_t len, CodecState *state);
};


CWT_NS_END

#endif /* __CWT_UTF8CODEC_P_HPP__ */
