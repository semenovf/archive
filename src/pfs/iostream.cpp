/*
 * iostream.cpp
 *
 *  Created on: Dec 10, 2015
 *      Author: wladt
 */

#include "pfs/iostream.hpp"

namespace pfs {

#if PFS_STRING_UTF16

istream & cin  = std::wcin;
ostream & cout = std::wcout;
ostream & cerr = std::wcerr;
ostream & clog = std::wclog;

#elif PFS_STRING_UTF32

istream & cin  = std::wcin;
ostream & cout = std::wcout;
ostream & cerr = std::wcerr;
ostream & clog = std::wclog;

#else

istream & cin  = std::cin;
ostream & cout = std::cout;
ostream & cerr = std::cerr;
ostream & clog = std::clog;

#endif

} // pfs
