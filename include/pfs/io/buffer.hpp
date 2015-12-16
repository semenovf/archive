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

struct buffer;

/**
 * @brief Opens buffer device and initializes
 *        it with raw byte array @c a of size @c n
 *
 * @param d Buffer device to open.
 * @param a Raw bytes array to initialize the buffer.
 * @param n Raw bytes array size.
 * @param oflags Open mode flags.
 * @return @c true if open is successful, @c false otherwise
 *         (i.e. buffer device is already opened).
 */
template <>
bool open_device<buffer, byte_t *, size_t, int> (device & d
		, byte_t * a
		, size_t n
		, int oflags);

template <>
inline bool open_device<buffer, byte_t *, size_t, pfs::io::device::OpenMode> (device & d
		, byte_t * a
		, size_t n
		, pfs::io::device::OpenMode oflag)
{
	return open_device<buffer, byte_t *, size_t, int>(d, a, n, int(oflag));
}

template <>
bool open_device<buffer, byte_t *, size_t> (device & d
		, byte_t a[]
		, size_t n)
{
	return open_device<buffer, byte_t *, size_t, int>(d, a, n, io::device::ReadWrite);
}

/**
 * @brief Open buffer device and reserve @a n bytes space.
 *
 * @param d Buffer device to open.
 * @param n Buffer size in bytes.
 * @param oflags Open mode flags.
 * @return @c true if open is successful, @c false otherwise
 */
template <>
bool open_device<buffer, size_t, int> (device & d, size_t n, int oflags);

template <>
inline bool open_device<buffer, size_t, pfs::io::device::OpenMode> (device & d, size_t n, pfs::io::device::OpenMode oflag)
{
	return open_device<buffer, size_t, int>(d, n, int(oflag));
}

template <>
inline bool open_device<buffer, size_t> (device & d, size_t n)
{
	return open_device<buffer, size_t, int>(d, n, io::device::ReadWrite);
}

}} // pfs::io

#endif /* __PFS_IO_BUFFER_HPP__ */
