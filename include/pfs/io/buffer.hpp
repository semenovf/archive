/*
 * buffer.hpp
 *
 *  Created on: Jul 16, 2013
 *      Author: wladt
 */

#ifndef __PFS_IO_BUFFER_HPP__
#define __PFS_IO_BUFFER_HPP__

#include <pfs/io/device.hpp>

namespace pfs { namespace io {

/**
 * @struct pfs::io::buffer
 * @brief Buffer device implementation.
 * @see pfs::io::device.
 */
struct buffer {};

template <>
struct open_params<buffer>
{
	byte_t * pbytes;
	size_t   size;

	open_params () : pbytes(0), size(0)
	{}

	open_params (size_t n) : pbytes(0), size(n)
	{}

	open_params (byte_t * p, size_t n) : pbytes(p), size(n)
	{}

	open_params (char * p, size_t n) : pbytes(reinterpret_cast<byte_t *>(p)), size(n)
	{}
};

/**
 * @fn bool open_device<buffer> (device & d, const open_params<buffer> & op)
 *
 * @brief Open buffer device.
 *
 * @param d Buffer device to open.
 * @param op Open device parameters.
 *      @li open_params<buffer>()
 *      	Open buffer device with default size. Elements initialized by zeros.
 * 		@li open_params<buffer>(size_t n)
 * 			Open buffer device with @a n size. Elements initialized by zeros.
 * 		@li open_params<buffer>(byte_t * p, size_t n)
 * 			Open buffer device with @a n size. Elements initialized with data from @a p array.
 * 		@li open_params<buffer>(char * p, size_t n)
 * 			Open buffer device with @a n size. Elements initialized with data from @a p array.
 *
 * @return @c true if open is successful, @c false otherwise
 *         (i.e. buffer device is already opened).
 */
template <>
bool open_device<buffer> (device & d, const open_params<buffer> & op, error_code * ex);

}} // pfs::io

#endif /* __PFS_IO_BUFFER_HPP__ */
