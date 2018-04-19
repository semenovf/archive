/*
 * ostream.hpp
 *
 *  Created on: Dec 3, 2015
 *      Author: wladt
 */

#ifndef __PFS_OSTREAM_HPP__
#define __PFS_OSTREAM_HPP__

#include <ostream>

namespace pfs {

#if PFS_STRING_UTF16

typedef ::std::wostream ostream;

#elif PFS_STRING_UTF32

typedef ::std::wostream ostream;

#else

typedef ::std::ostream ostream;

#endif

/**
 * @brief Write a newline and flush the stream.
 */
inline ostream & endl (ostream & os)
{
	return std::endl(os);
}

/**
 * @brief Write a null character into the output sequence.
 */
inline ostream & ends (ostream & os)
{
	return std::ends(os);
}

/**
 * @brief Flushes the output stream.
 */
inline ostream & flush (ostream & os)
{
	return std::flush(os);
}

} // pfs

#endif /* __PFS_OSTREAM_HPP__ */
