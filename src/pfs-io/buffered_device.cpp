/*
 * buffered_device.cpp
 *
 *  Created on: Mar 5, 2016
 *      Author: wladt
 */

#include <cstdlib>
#include <pfs/io/buffered_device.hpp>

namespace pfs { namespace io {

buffered_device::buffered_device (device & d, size_t initialSize)
    : _d (d)
    , _cursor(0)
    , _count(0)
{
    _buffer = static_cast<byte_t *>(std::malloc(initialSize * sizeof(byte_t)));
    _bufsz = initialSize;
}

buffered_device::~buffered_device ()
{
    if (_buffer)
        std::free(_buffer);
}
    
ssize_t buffered_device::upload_bytes (size_t max_size, error_code & ex)
{
    if (max_size == 0)
        return 0;

    if (max_size > _bufsz - _count) {
        _bufsz += max_size - (_bufsz - _count);
        _buffer = static_cast<byte_t *>(std::realloc(_buffer, _bufsz));
    }
    
    ssize_t r = _d.read(_buffer + _count, max_size, & ex);
        
    if (r > 0) {
        _count += r;
    }
    
    return r;
}
    
bool buffered_device::can_read (size_t count, error_code & ex)
{
	ex.reset();

	if (_cursor + count < _count) {
		return true;
	}

	if (_cursor > 0 and _cursor == _count) {
		_cursor = 0;
	}

	ssize_t n = upload_bytes(count < 256 ? 256 : count, ex);

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

//void buffered_device::unread_byte (byte_t c)
//{
//	if (_cursor > 0) {
//		_buffer[--_cursor] = c;
//	} else {
//		_buffer.insert(_buffer.begin(), c);
//	}
//}

error_code buffered_device::read_line (byte_string & line, size_t maxSize)
{
	size_t n = 0;
	error_code ex;
	byte_t c;

	while (n < maxSize && read_byte(c, ex)) {
		++n;
		line.append(1, c);

		if (c == byte_t('\n')) { // FIXME for Windows and MacOS
			break;
		}
	}

	return ex;
}

}}
