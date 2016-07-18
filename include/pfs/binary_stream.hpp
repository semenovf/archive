/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   binary_stream.hpp
 * Author: wladt
 *
 * Created on July 15, 2016, 6:03 PM
 */

#ifndef __PFS_BINARY_STREAM_HPP__
#define __PFS_BINARY_STREAM_HPP__

#include <pfs/traits/binary_stream.hpp>

namespace pfs {

template <typename Device>
class binary_stream
{
public:
    typedef Device device_type;
    
    enum status_enum {
          no_error
        , write_failed
        , read_failed
    };
    
private:
    status_enum _status;
    device_type _dev;
    endian      _order;
    
    // pack context
    //byte_string buffer;
    
    
    // unpack context
//    byte_string::const_iterator b;
//    byte_string::const_iterator e;

private:
    binary_stream (binary_stream const & other);
    binary_stream & operator = (binary_stream const & other);
    
public:
    binary_stream (device_type dev, endian order)
            : _status(no_error)
            , _dev(dev)
            , _order(order)
    {}
            
    virtual ~binary_stream () {}

    status_enum status () const
    {
        return _status;
    }
    
    device_type & device ()
    {
        return _dev;
    }
    
public:

template <typename T>
void write (T const & v)
{
    if (_status != write_failed) {
        _status = write_binary(_dev, _order, v) < 0 
                ? write_failed
                : no_error;
    }
}

template <typename T>
void read (T & v)
{
    if (_status != read_failed) {
        _status = read_binary(_dev, _order, v) < 0 
                ? read_failed
                : no_error;
    }
}

template <typename T>
binary_stream & operator << (T const & v)
{
    write<T>(v);
    return *this;
}

template <typename T>
binary_stream & operator >> (T & v)
{
    read<T>(v);
    return *this;
}

};

} // pfs

#endif /* __PFS_BINARY_STREAM_HPP__ */

