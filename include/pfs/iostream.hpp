/*
 * iostream.hpp
 *
 *  Created on: Dec 3, 2015
 *      Author: wladt
 */

#ifndef __PFS_IOSTREAM_HPP__
#define __PFS_IOSTREAM_HPP__

#include <iostream>

namespace pfs {

template <typename String>
struct iostream;

template <>
struct iostream<utf8_string>
{
	typedef ::std::iostream type;
};

} // pfs

#endif /* __PFS_IOSTREAM_HPP__ */
