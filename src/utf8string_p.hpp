/*
 * utf8string_p.hpp
 *
 *  Created on: Jul 22, 2013
 *      Author: wladt
 */

#ifndef __CWT_UTF8STRING_P_HPP__
#define __CWT_UTF8STRING_P_HPP__

#include "../../include/cwt/utf8string.hpp"
#include "../../include/cwt/unicode.hpp"
#include <string>
#include <cstring>

CWT_NS_BEGIN

struct Utf8String::Impl : public std::string
{
	typedef std::string::iterator iterator;
	typedef std::string::const_iterator const_iterator;
	static const size_t npos = std::string::npos;

	Impl() : std::string(), m_length(0) {}
	Impl(const Impl & other) : std::string(other), m_length(other.m_length) { }
	Impl(const std::string & other) : std::string(other), m_length(0) { } // need recalculation of length after use this constructor
	size_t m_length; // length in UChars
};

CWT_NS_END

#endif /* __CWT_UTF8STRING_P_HPP__ */

