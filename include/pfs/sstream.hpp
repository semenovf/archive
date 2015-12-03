/*
 * sstream.hpp
 *
 *  Created on: Dec 3, 2015
 *      Author: wladt
 */

#ifndef __PFS_SSTREAM_HPP__
#define __PFS_SSTREAM_HPP__

#include <sstream>
#include <pfs/string.hpp>

namespace pfs {

template <typename String>
struct ostringstream;

template <typename String>
struct istringstream;

template <typename String>
struct stringstream;

template <>
struct ostringstream<utf8_string>
{
	typedef ::std::ostringstream type;
};

template <>
struct istringstream<utf8_string>
{
	typedef ::std::istringstream type;
};

template <>
struct stringstream<utf8_string>
{
	typedef ::std::stringstream type;
};

} // pfs

#endif /* __PFS_SSTREAM_HPP__ */
