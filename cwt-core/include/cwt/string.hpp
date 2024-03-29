/**
 * @file   string.hpp
 * @author wladt
 * @date   Jan 24, 2013 2:31:19 PM
 *
 * @brief
 */

#ifndef __CWT_STRING_HPP__
#define __CWT_STRING_HPP__

#include <cwt/utf8string.hpp>

CWT_NS_BEGIN

typedef Utf8String String; // NOTE obsolete type name
typedef utf8string string;

#define _u8(s) cwt::string::fromUtf8(s)

CWT_NS_END

#endif /* __CWT_STRING_HPP__ */
