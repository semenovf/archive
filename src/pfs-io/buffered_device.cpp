/*
 * buffered_device.cpp
 *
 *  Created on: Mar 5, 2016
 *      Author: wladt
 */

#include <pfs/io/buffered_device.hpp>

namespace pfs { namespace io {

bool buffered_device::can_read (size_t count, error_code & ex)
{
	ex.reset();

	if (_cursor + count < _buffer.size()) {
		return true;
	}

	if (_cursor > 0 and _cursor == _buffer.size()) {
		_cursor = 0;
		_buffer.clear();
	}

	ssize_t n = upload_bytes(count, ex);

	if (n < count) {
		return false;
	}

	return true;
}

bool buffered_device::read_byte (byte_t & c, error_code & ex)
{
	if (can_read(1, ex)) {
		c = _buffer[_cursor++];
		return true;
	}
	return false;
}

bool buffered_device::peek_byte (byte_t & c, error_code & ex)
{
	if (can_read(1, ex)) {
		c = _buffer[_cursor];
		return true;
	}
	return false;
}

void buffered_device::unread_byte (byte_t c)
{
	if (_cursor > 0) {
		_buffer[--_cursor] = c;
	} else {
		_buffer.insert(_buffer.begin(), c);
	}
}

error_code buffered_device::read_line (byte_string & line, size_t maxSize)
{
	size_t n = 0;
	error_code ex;
	byte_t c;

	while (n < maxSize && read_byte(c, ex)) {
		++n;
		line.append(1, c);

		if (c == byte_t('\n')) { // FIXME for Windows and MacOS
			line.append(1, c);
			break;
		}
	}

	return ex;
}


}}
