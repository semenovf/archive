/**
 * @file   textstream.hpp
 * @author wladt
 * @date   Feb 27, 2013 10:33:44 AM
 *
 * @brief
 */

#ifndef __CWT_TEXTSTREAM_HPP__
#define __CWT_TEXTSTREAM_HPP__

#include <cwt/cwt.h>
#include <cwt/memory.hpp>
#include <cwt/string.hpp>
#include <cwt/bytearray.hpp>
#include <cwt/reader.hpp>

CWT_NS_BEGIN

class TextStream {
public:
	TextStream(Reader *reader);
	String readAll();
private:
    class Impl;
    typedef unique_ptr<Impl> ImplPtr;
    ImplPtr pimpl;
};
CWT_NS_END

#endif /* __CWT_TEXTSTREAM_HPP__ */
