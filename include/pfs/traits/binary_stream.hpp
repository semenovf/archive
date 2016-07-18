/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   binary_stream.hpp
 * Author: wladt
 *
 * Created on July 15, 2016, 7:04 PM
 */

#ifndef __PFS_TRAITS_BINARY_STREAM_HPP__
#define __PFS_TRAITS_BINARY_STREAM_HPP__

#include <pfs/endian.hpp>

namespace pfs {

//template <typename Device>
//struct binary_writer
//{
//    ssize_t write (char const * data, size_t n);
//};

//template <typename Device>
//struct binary_reader
//{
//    ssize_t read(char * chars, size_t n)
//};


template <typename Device, typename T>
ssize_t write_binary (Device & dev, endian order, T const & v);

template <typename Device, typename T>
ssize_t read_binary (Device & dev, endian order, T & v);

namespace details {
namespace integral {

template <typename Device, typename Integral>
ssize_t write_binary (Device & dev, endian order, Integral const & v)
{
	Integral a = order.convert(v);
	union u { Integral v; char b[sizeof(Integral)]; } d;
	d.v = a;
	return dev.write(d.b, sizeof(Integral));
}

template <typename Device, typename Integral>
ssize_t read_binary (Device & dev, endian order, Integral & v)
{
    union u
    {
        Integral const v;
        char const b[sizeof(Integral)];
    } d;

    ssize_t result = dev.read(d.b, sizeof(Integral));
    
    if (result < 0)
        return result;
    
    v = order.convert(d.v);
    
	return result;
}

}} // integral

namespace details {
namespace fp {

template <typename Device, typename Float>
ssize_t write_binary (Device & dev, endian order, Float const & v)
{
#ifdef PFS_HAVE_INT64    
    if (sizeof(Float) == 8) {
        return details::integral::write_binary(dev, order, *reinterpret_cast<uint64_t const *>(& v));
    } else
#endif        
    if (sizeof(Float) == 4) {
        return details::integral::write_binary(dev, order, *reinterpret_cast<uint32_t const *>(& v));
    } else if (sizeof(Float) == 2) {
        return details::integral::write_binary(dev, order, *reinterpret_cast<uint16_t const *>(& v));
    } else {
        union { Float v; char b[sizeof(Float)]; } d;
        
        if (order != endian::native_order()) {
            int8_t b[sizeof(Float)];
            
            for (int i = 0, j = sizeof(Float) - 1; j >= 0; ++i, --j) {
                b[i] = d.b[j];
            }
            
            return dev.write(b, sizeof(Float));
        } else {
            return dev.write(d.b, sizeof(Float));
        }
    }
    
    return ssize_t(-1);
}

template <typename Device, typename Float>
ssize_t read_binary (Device & dev, endian order, Float & v)
{
    ssize_t result = -1;
            
#ifdef PFS_HAVE_INT64    
    if (sizeof(Float) == 8) {
        uint64_t d = 0;
        result = details::integral::read_binary(dev, order, d);
        v = *reinterpret_cast<Float *>(& d);
        return result;
    } else
#endif        
    if (sizeof(Float) == 4) {
        uint32_t d = 0;
        result = details::integral::read_binary(dev, order, d);
        v = *reinterpret_cast<Float *>(& d);
        return result;
    } else if (sizeof(Float) == 2) {
        uint16_t d = 0;
        result = details::integral::read_binary(dev, order, d);
        v = *reinterpret_cast<Float *>(& d);
        return result;
    } else {
        // FIXME
//        byte_string::const_iterator pos(ctx.b);
//        std::advance(pos, sizeof(Float));
//
//        if (pos <= ctx.e) {
//            u * b = reinterpret_cast<u *>(ctx.b.base());
//            
//            if (order != endian::native_order()) {
//                byte_string::value_type b[sizeof(Float)];
//            
//            for (int i = 0, j = sizeof(Float) - 1; j >= 0; ++i, --j) {
//                b[i] = d.b[j];
//            }
//            
//            appender.append(byte_string(b, sizeof(Float)));
//        } else {
//            appender.append(byte_string(d.b, sizeof(Float)));
//        }
//            
//            v = ctx.o.convert(d->v);
//            ctx.b = pos;
//            ctx.fail = false;
//        } else {
//            ctx.fail = true;
//        }
    }
    
	return result;
}

}} // details::fp

namespace details {
namespace sequence {

template <typename Device>
ssize_t write_binary (Device & dev, endian order, char const * data, size_t n)
{
    ssize_t result = write_binary(dev, order, n);
    
    if (result > 0) { // pack size of byte_string
        ssize_t r1 = dev.write(data, n);
        
        if (r1 >= 0)
            result += r1;
        else
            result = r1;
    }
    
    return result;
}

// TODO Reimplement using Allocator as template parameter
//
template <typename Device>
ssize_t read_binary (Device & dev, endian order, char ** buffer, size_t * n)
{
    size_t size = 0;
    ssize_t result = read_binary(dev, order, size);
    
    *buffer = 0;
    
    if (result > 0) {
        *buffer = new byte_t[size];
        
        ssize_t r1 = dev.read(*buffer, size);
        
        if (r1 >= 0) {
            result += r1;
        } else {
            result = r1;
        }
    }
    
    return result;
}


}} // details::sequence

template <typename Device>
inline ssize_t write_binary (Device & dev, endian order, bool const & v)
{
    return details::integral::write_binary(dev, order, v);
}

template <typename Device>
inline ssize_t write_binary (Device & dev, endian order, char const & v)
{
    return details::integral::write_binary(dev, order, v);
}

template <typename Device>
inline ssize_t write_binary (Device & dev, endian order, signed char const & v)
{
    return details::integral::write_binary(dev, order, v);
}

template <typename Device>
inline ssize_t write_binary (Device & dev, endian order, unsigned char const & v)
{
    return details::integral::write_binary(dev, order, v);
}

template <typename Device>
inline ssize_t write_binary (Device & dev, endian order, wchar_t const & v)
{
    return details::integral::write_binary(dev, order, v);
}

template <typename Device>
inline ssize_t write_binary (Device & dev, endian order, short const & v)
{
    return details::integral::write_binary(dev, order, v);
}

template <typename Device>
inline ssize_t write_binary (Device & dev, endian order, unsigned short const & v)
{
    return details::integral::write_binary(dev, order, v);
}

template <typename Device>
inline ssize_t write_binary (Device & dev, endian order, int const & v)
{
    return details::integral::write_binary(dev, order, v);
}

template <typename Device>
inline ssize_t write_binary (Device & dev, endian order, unsigned int const & v)
{
    return details::integral::write_binary(dev, order, v);
}

template <typename Device>
inline ssize_t write_binary (Device & dev, endian order, long const & v)
{
    return details::integral::write_binary(dev, order, v);
}

template <typename Device>
inline ssize_t write_binary (Device & dev, endian order, unsigned long const & v)
{
    return details::integral::write_binary(dev, order, v);
}

#if PFS_HAVE_LONGLONG

template <typename Device>
inline ssize_t write_binary (Device & dev, endian order, long long const & v)
{
    return details::integral::write_binary(dev, order, v);
}

template <typename Device>
inline ssize_t write_binary (Device & dev, endian order, unsigned long long const & v)
{
    return details::integral::write_binary(dev, order, v);
}

#endif

template <typename Device>
inline ssize_t write_binary (Device & dev, endian order, float const & v)
{
//    //return details::pack_ieee754(appender, real64_t(v), order, 32, 8);
    return details::fp::write_binary(dev, order, v);
}

template <typename Device>
inline ssize_t write_binary (Device & dev, endian order, double const & v)
{
//    //return details::pack_ieee754(appender, real64_t(v), order, 64, 11);
    return details::fp::write_binary(dev, order, v);
}

#if PFS_HAVE_LONG_DOUBLE

template <typename Device>
inline ssize_t write_binary (Device & dev, endian order, long double const & v)
{
    return details::fp::write_binary(dev, order, v);
}

#endif

template <typename Device>
inline ssize_t read_binary (Device & dev, endian order, bool & v)
{
    return details::integral::read_binary(dev, order, v);
}

template <typename Device>
inline ssize_t read_binary (Device & dev, endian order, char & v)
{
    return details::integral::read_binary(dev, order, v);
}

template <typename Device>
inline ssize_t read_binary (Device & dev, endian order, signed char & v)
{
    return details::integral::read_binary(dev, order, v);
}

template <typename Device>
inline ssize_t read_binary (Device & dev, endian order, unsigned char & v)
{
    return details::integral::read_binary(dev, order, v);
}

template <typename Device>
inline ssize_t read_binary (Device & dev, endian order, wchar_t & v)
{
    return details::integral::read_binary(dev, order, v);
}

template <typename Device>
inline ssize_t read_binary (Device & dev, endian order, short & v)
{
    return details::integral::read_binary(dev, order, v);
}

template <typename Device>
inline ssize_t read_binary (Device & dev, endian order, unsigned short & v)
{
    return details::integral::read_binary(dev, order, v);
}

template <typename Device>
inline ssize_t read_binary (Device & dev, endian order, int & v)
{
    return details::integral::read_binary(dev, order, v);
}

template <typename Device>
inline ssize_t read_binary (Device & dev, endian order, unsigned int & v)
{
    return details::integral::read_binary(dev, order, v);
}

template <typename Device>
inline ssize_t read_binary (Device & dev, endian order, long & v)
{
    return details::integral::read_binary(dev, order, v);
}

template <typename Device>
inline ssize_t read_binary (Device & dev, endian order, unsigned long & v)
{
    return details::integral::read_binary(dev, order, v);
}

#if PFS_HAVE_LONGLONG

template <typename Device>
inline ssize_t read_binary (Device & dev, endian order, long long & v)
{
    return details::integral::read_binary(dev, order, v);
}

template <typename Device>
inline ssize_t read_binary (Device & dev, endian order, unsigned long long & v)
{
    return details::integral::read_binary(dev, order, v);
}

#endif

template <typename Device>
inline ssize_t read_binary (Device & dev, endian order, float & v)
{
    return details::fp::read_binary(dev, order, v);
}

template <typename Device>
inline ssize_t read_binary (Device & dev, endian order, double & v)
{
    return details::fp::read_binary(dev, order, v);
}

#if PFS_HAVE_LONG_DOUBLE

template <typename Device>
inline ssize_t read_binary (Device & dev, endian order, long double & v)
{
    return details::fp::read_binary(dev, order, v);
}

#endif


}

#endif /* __PFS_TRAITS_BINARY_STREAM_HPP__ */

