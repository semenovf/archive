/*
 * ostream.hpp
 *
 *  Created on: Dec 3, 2015
 *      Author: wladt
 */

#ifndef __PFS_OSTREAM_HPP__
#define __PFS_OSTREAM_HPP__

#include <ostream>
#include <pfs/string.hpp>

namespace pfs {

template <typename String>
struct ostream;

template <>
struct ostream<utf8_string>
{
	typedef ::std::ostream type;
};

} // pfs

#endif /* __PFS_OSTREAM_HPP__ */
