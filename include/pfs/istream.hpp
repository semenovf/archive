/*
 * istream.hpp
 *
 *  Created on: Dec 3, 2015
 *      Author: wladt
 */

#ifndef __PFS_ISTREAM_HPP__
#define __PFS_ISTREAM_HPP__

#include <istream>
#include <pfs/string.hpp>

namespace pfs {

template <typename String>
struct istream;

template <>
struct istream<utf8_string>
{
	typedef ::std::istream type;
};

} // pfs

#endif /* __PFS_ISTREAM_HPP__ */
