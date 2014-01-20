/*
 * nullcodec.hpp
 *
 *  Created on: Aug 12, 2013
 *      Author: wladt
 */

#ifndef __CWT_IO_NULLCODEC_HPP__
#define __CWT_IO_NULLCODEC_HPP__

namespace cwt { namespace io {

template <typename string_type>
class null_codec
{
public:
	typedef string_type istring_type;
	typedef string_type ostring_type;

	bool convert (ostring_type & output, const istring_type & input, size_t & remain)
	{
		output.append(input);
		remain = 0;
		return true;
	}
};


class nullbyte_codec
{
public:
	typedef pfs::bytearray istring_type;
	typedef pfs::bytearray ostring_type;

	bool convert(ostring_type & output, const istring_type & input, size_t & remain)
	{
		output.append(input);
		remain = 0;
		return true;
	}
};

}} // cwt::io

#endif /* __CWT_IO_NULLCODEC_HPP__ */
