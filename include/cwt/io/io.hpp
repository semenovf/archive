/*
 * producer.hpp
 *
 *  Created on: Jul 11, 2013
 *      Author: wladt
 */

#ifndef __CWT_IO_HPP__
#define __CWT_IO_HPP__

#include <cwt/cwt.hpp>
#include <cstring>

CWT_NS_BEGIN

namespace io {

template <typename T>
struct NullCodec
{
	typedef T orig_char_type;
	typedef T dest_char_type;
	ssize_t convert(dest_char_type output[], size_t osize, const orig_char_type input[], size_t isize, size_t * remain)
	{
		size_t n = CWT_MIN(osize, isize);
		CWT_ASSERT( n > 0 && n <= CWT_SSIZE_MAX);
		std::memcpy(output, input, sizeof(T) * n);
		*remain = isize > n ? isize - n : 0;
		return ssize_t(n);
	}
};

} // namespace io

CWT_NS_END


#endif /* __CWT_IO_HPP__ */
