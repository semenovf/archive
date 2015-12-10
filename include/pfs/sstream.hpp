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

#if PFS_STRING_UTF16

typedef ::std::wostringstream ostringstream;
typedef ::std::wistringstream istringstream;
typedef ::std::wstringstream  stringstream;

#elif PFS_STRING_UTF32

typedef ::std::wostringstream ostringstream;
typedef ::std::wistringstream istringstream;
typedef ::std::wstringstream  stringstream;

#else

typedef ::std::ostringstream ostringstream;
typedef ::std::istringstream istringstream;
typedef ::std::stringstream  stringstream;

#endif

} // pfs

#endif /* __PFS_SSTREAM_HPP__ */
