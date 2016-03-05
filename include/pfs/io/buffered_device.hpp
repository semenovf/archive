/*
 * buffered_device.hpp
 *
 *  Created on: Mar 5, 2016
 *      Author: wladt
 */

#ifndef __PFS_IO_BUFFERED_DEVICE_HPP__
#define __PFS_IO_BUFFERED_DEVICE_HPP__

#include <pfs/vector.hpp>
#include <pfs/error_code.hpp>
#include <pfs/io/device.hpp>

namespace pfs { namespace io {

class buffered_device
{
	device &       _d;
	vector<byte_t> _buffer;
	size_t         _cursor;

private:
	bool can_read (size_t count, error_code & ex);

	ssize_t upload_bytes (size_t max_size, error_code & ex)
	{
		_buffer.reserve(_buffer.size() + max_size);
		return _d.read(_buffer.data() + _buffer.size(), max_size, & ex);
	}

public:
	buffered_device (device & d, size_t initialSize = 256)
		: _d (d)
		, _cursor(0)
	{
		_buffer.reserve(initialSize);
	}

    /**
     * @brief Reads byte from the buffered device.
     * @details Reads one character from the device and stores it in @a c.
     *
     * @param c Reference to byte to store value read from device.
     * @param ex Reference to store error code.
     * @return @c true if byte read successfully,
     *         or @c false if error occurred or no more bytes available (i.e. at end of device).
     */
    bool read_byte (byte_t & c, error_code & ex);

    bool read_byte (byte_t & c)
    {
    	error_code ex;
    	return read_byte(c, ex);
    }

    bool peek_byte (byte_t & c, error_code & ex);

    bool peek_byte (byte_t & c)
    {
    	error_code ex;
    	return peek_byte(c, ex);
    }

    void unread_byte (byte_t c);

    error_code read_line (byte_string & line, size_t maxSize);
};

}}

#endif /* __PFS_IO_BUFFERED_DEVICE_HPP__ */
