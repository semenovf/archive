/*
 * nullcodec.hpp
 *
 *  Created on: Aug 12, 2013
 *      Author: wladt
 */

#ifndef __CWT_IO_NULLCODEC_HPP__
#define __CWT_IO_NULLCODEC_HPP__

CWT_NS_BEGIN

namespace io {

struct NullByteCodec : public NullCodec<char>
{
	typedef NullCodec<char>::orig_char_type orig_char_type;
	typedef NullCodec<char>::dest_char_type dest_char_type;
	typedef ByteArray vector_type;
};

} // namespace io

CWT_NS_END

#endif /* __CWT_IO_NULLCODEC_HPP__ */
