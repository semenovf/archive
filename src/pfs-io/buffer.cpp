/*
 * buffer.cpp
 *
 *  Created on: Jul 16, 2013
 *      Author: wladt
 */

#include "pfs/io/buffer.hpp"
#include <pfs/array.hpp>

namespace pfs { namespace io {

struct buffer_impl : public device_impl
{
    typedef array<byte_t> buffer_type;

    buffer_type _buffer;
    size_t _pos;

    buffer_impl () : _pos(0) {}
    buffer_impl (byte_t a[], size_t n)
        : _buffer(a, n)
        , _pos(0)
    {}

    buffer_impl (size_t sz)
        : _buffer(sz, true)
        , _pos(0)
    {}


    virtual size_t  bytesAvailable () const { return _buffer.size() - _pos; }
    virtual ssize_t readBytes      (byte_t [] /*bytes*/, size_t /*n*/, errorable_ext &);
    virtual ssize_t writeBytes     (const byte_t [] /*bytes*/, size_t /*n*/, errorable_ext &);
    virtual bool    closeDevice    (errorable_ext &) { return true; }
    virtual bool    deviceIsOpened () const { return true; }
    virtual void    flushDevice    () {}
    virtual bool    setNonBlocking () { return true; }
};

ssize_t buffer_impl::readBytes (byte_t bytes[], size_t n, errorable_ext &)
{
    if (_pos >= _buffer.size())
        return 0;

    n = pfs::min(n, _buffer.size() - _pos);
    buffer_type::copy(bytes, _buffer,  _pos, n);
    _pos += n;

    return ssize_t(n);
}

ssize_t buffer_impl::writeBytes (const byte_t bytes[], size_t n, errorable_ext &)
{
    PFS_ASSERT(max_type<size_t>() - _pos >= n);

    size_t sizeAvailable = _buffer.size() - _pos;

    if (sizeAvailable < n)
        _buffer.realloc(_buffer.size() + n - sizeAvailable);

    buffer_type::copy(_buffer, bytes, _pos, n);

    return integral_cast_check<ssize_t>(n);
}


/**
 * @brief Opens buffer device and initializes
 *        it with raw byte array @c a of size @c n
 *
 * @param a Raw byte array.
 * @param n Raw byte array size
 * @param oflags Open mode flags.
 *
 * @return @c true if open is successful, @c false otherwise
 *         (i.e. buffer device is already opened).
 */
bool buffer::open (byte_t a[], size_t n, uint32_t oflags)
{
    if (!checkNotOpened())
        return false;

    if (_d)
        delete _d;

    _d = new buffer_impl(a, n);
    this->setFlags(oflags);

    return _d != nullptr;
}

/**
 * @brief Opens buffer device and initializes it byte array @c a of size @c sz.
 *
 * @param sz Raw byte array size.
 * @param oflags Open mode flags.
 *
 * @return @c true if open is successful, @c false otherwise
 *         (i.e. buffer device is already opened).
 */
bool buffer::open (size_t sz, uint32_t oflags)
{
    if (!checkNotOpened())
        return false;

    if (_d)
        delete _d;

    _d = new buffer_impl(sz);
    _oflags = oflags;

    return _d != nullptr;
}

size_t buffer::size () const
{
    if (isNull())
        return 0;

    const buffer_impl * d = dynamic_cast<buffer_impl *>(_d);
    return d->_buffer.size();
}

}} // pfs::io
