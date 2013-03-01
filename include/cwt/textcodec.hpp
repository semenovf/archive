/**
 * @file   textcodec.hpp
 * @author wladt
 * @date   Mar 1, 2013 10:59:37 AM
 *
 * @brief
 */

#ifndef __CWT_TEXTCODEC_HPP__
#define __CWT_TEXTCODEC_HPP__

#include <cwt/cwt.h>
#include <cwt/bytearray.hpp>
#include <cwt/string.hpp>

CWT_NS_BEGIN


class TextCodec {
public:
	enum CodecFlag {
		  NullReplacement = 0x0001  // replace invalid characters with null character.
		, IgnoreHeader    = 0x0002  //
	};

	struct CodecState {
		CodecState() : flags(0), remain_chars(0), invalid_chars(0) { data[0] = data[1] = data[2] = 0; }
		uint_t flags;
		int    remain_chars;
		int    invalid_chars;
		uint_t data[3];
	};
protected:
	TextCodec() {}
public:
	virtual ~TextCodec() {}

protected:
	virtual String decode(const char *bytes, size_t len, CodecState *state) = 0;
	virtual ByteArray encode(const Char *chars, size_t len, CodecState *state) = 0;

public:
	static TextCodec* forName(const char* name);
	static TextCodec* forName(const ByteArray &name) { return forName(name.data()); }
};

CWT_NS_END

#endif /* __CWT_TEXTCODEC_HPP__ */
