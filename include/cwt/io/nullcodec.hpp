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

template <typename string_type>
class NullCodec
{
public:
	typedef string_type istring_type;
	typedef string_type ostring_type;

	bool convert(ostring_type & output, const istring_type & input, size_t & remain)
	{
		output.append(input);
		remain = 0;
		return true;
	}
};


class NullByteCodec
{
public:
	typedef ByteArray istring_type;
	typedef ByteArray ostring_type;

	bool convert(ostring_type & output, const istring_type & input, size_t & remain)
	{
		output.append(input);
		remain = 0;
		return true;
	}
};

} // namespace io

CWT_NS_END

#endif /* __CWT_IO_NULLCODEC_HPP__ */
