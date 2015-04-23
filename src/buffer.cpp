/*
 * buffer.cpp
 *
 *  Created on: Jul 16, 2013
 *      Author: wladt
 */

#include "pfs/io/buffer.hpp"

namespace pfs { namespace io {

struct buffer_impl : public device_impl
{
    byte_string _buffer;

    virtual ssize_t readBytes (byte_t bytes[], size_t n, errorable_ext & ex);
    virtual ssize_t writeBytes (const byte_t bytes[], size_t n, errorable_ext &)
    {
        PFS_ASSERT(n <= PFS_SSIZE_MAX);
        _buffer.append(bytes, n);
        return ssize_t(n);
    }

    virtual size_t bytesAvailable () const
    {
        return _buffer.size();
    }

    virtual bool closeDevice (errorable_ext & /*ex*/)
    {
        return true;
    }

    virtual bool deviceIsOpened () const
    {
        return true;
    }

    virtual void flushDevice ()
    { ; }
};

ssize_t buffer_impl::readBytes (byte_t bytes[], size_t n, errorable_ext &)
{
    size_t nbytes = 0;
    if (n > 0 && _buffer.size() > 0) {
        nbytes = pfs::min(n, _buffer.size());
        PFS_ASSERT(nbytes <= PFS_SSIZE_MAX);
        memcpy(bytes, _buffer.constData(), nbytes);
        _buffer.remove(0, nbytes);
    }
    return ssize_t(nbytes);
}

const byte_t * buffer::constData () const
{
    if (isNull())
        return nullptr;

    const buffer_impl * d = _d.cast<buffer_impl>();
    return d->_buffer.constData();
}

size_t buffer::size () const
{
    if (isNull())
        return 0;

    const buffer_impl * d = _d.cast<buffer_impl>();
    return d->_buffer.size();
}

byte_string buffer::data () const
{
    if (isNull())
        return byte_string();

    const buffer_impl * d = _d.cast<buffer_impl>();
    return d->_buffer;
}

}} // pfs::io
