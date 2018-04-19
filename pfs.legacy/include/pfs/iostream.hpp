/*
 * iostream.hpp
 *
 *  Created on: Dec 3, 2015
 *      Author: wladt
 */

#ifndef __PFS_IOSTREAM_HPP__
#define __PFS_IOSTREAM_HPP__

#include <iostream>
#include <pfs/ostream.hpp>
#include <pfs/istream.hpp>

namespace pfs {

#if PFS_STRING_UTF16

typedef ::std::wiostream iostream;

#elif PFS_STRING_UTF32

typedef ::std::wiostream iostream;

#else

typedef ::std::iostream iostream;

#endif

extern istream & cin;  /// Linked to platform specific standard input
extern ostream & cout; /// Linked to platform specific standard output
extern ostream & cerr; /// Linked to platform specific standard error (unbuffered)
extern ostream & clog; /// Linked to platform specific standard error (buffered)

} // pfs

#endif /* __PFS_IOSTREAM_HPP__ */
