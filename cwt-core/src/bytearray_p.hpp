/*
 * bytearray_p.hpp
 *
 *  Created on: Jul 29, 2013
 *      Author: wladt
 */

#ifndef __CWT_BYTEARRAY_P_HPP__
#define __CWT_BYTEARRAY_P_HPP__

#include "../../include/cwt/bytearray.hpp"
#include <string>
#include <cstring>

CWT_NS_BEGIN

struct ByteArray::Impl : public std::string
{
	static const size_t npos = std::string::npos;
	Impl() : std::string() {}
	Impl(const Impl & other) : std::string(other) { }
	Impl(const std::string & other) : std::string(other) { }
	Impl(const char * s) : std::string(s) { }
	Impl(const char * s, size_t n) : std::string(s, n) { }
	Impl(size_t size, char ch) : std::string(size, ch) { }
};

CWT_NS_END

#endif /* __CWT_BYTEARRAY_P_HPP__ */
