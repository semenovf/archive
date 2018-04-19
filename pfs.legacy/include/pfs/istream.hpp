/*
 * istream.hpp
 *
 *  Created on: Dec 3, 2015
 *      Author: wladt
 */

#ifndef __PFS_ISTREAM_HPP__
#define __PFS_ISTREAM_HPP__

#include <istream>

namespace pfs {

#if PFS_STRING_UTF16

typedef ::std::wistream istream;

#elif PFS_STRING_UTF32

typedef ::std::wistream istream;

#else

typedef ::std::istream istream;

#endif

} // pfs

#endif /* __PFS_ISTREAM_HPP__ */
