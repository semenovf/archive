/**
 * @file   atomic.hpp
 * @author wladt
 * @date   Mar 26, 2013 12:24:30 PM
 *
 * @brief
 */

#ifndef __PFS_ATOMIC_HPP__
#define __PFS_ATOMIC_HPP__

#include <pfs/cxxversion.hpp>
#include <pfs/compiler.hpp>
#include <pfs.hpp>

#if __PFS_CXX_VERSION__ >= 2011
#   include PFS_CXX_HEADER(atomic)
#else
#include PFS_CC_HEADER(atomic_intrinsics)

namespace pfs {

template <typename Integral>
struct atomic
{
    typedef Integral value_type;

private:    
    value_type value;
    
private:
    atomic (atomic const &);              // Is not CopyConstructible
    atomic & operator = (atomic const &); // Is not CopyAssignable
        
public:
    atomic () 
        : value(0) 
    {}
    
    atomic (value_type v)
        : value(v)
    {}
    
    value_type operator = (value_type v)
    {
        store(v);
        return v;
    }
    
    operator value_type () const
    {
        return load();
    }
    
    void store (value_type v)
    {
        atomic_store(& value, v);
    }
    
    value_type load () const
    {
        return atomic_load((value_type *)& value);
    }

    value_type operator ++ ()
    {
        return atomic_add_fetch(& value, static_cast<value_type>(1));
    }

    value_type operator ++ (int)
    {
        return atomic_fetch_add(& value, static_cast<value_type>(1));
    }
    
    value_type operator -- ()
    {
        return atomic_sub_fetch(& value, static_cast<value_type>(1));
    }

    value_type operator -- (int)
    {
        return fetch_sub(& value, static_cast<value_type>(1));
    }
    
    value_type operator += (value_type n)
    {
        return atomic_add_fetch(& value, n); 
    }

    value_type operator -= (value_type n)
    {
        return atomic_sub_fetch(& value, n);
    }

    value_type operator &= (value_type n)
    {
        return atomic_and_fetch(& value, n);
    }

    value_type operator |= (value_type n)
    {
        return atomic_or_fetch(& value, n);
    }

    value_type operator ^= (value_type n)
    { 
        return atomic_xor_fetch(& value, n);
    }
    
    value_type fetch_add (value_type n)
    {
        return atomic_fetch_add(& value, n);
    }

    value_type fetch_sub (value_type n)
    {
        return atomic_fetch_sub(& value, n);
    }
    
    value_type fetch_and (value_type n)
    {
        return atomic_fetch_and(& value, n); 
    }
    
    value_type fetch_or (value_type n)
    {
        return atomic_fetch_or(& value, n);
    }

    value_type fetch_xor (value_type n)
    {
        return atomic_fetch_xor(& value, n);
    }

    value_type exchange (value_type v)
    {
        return atomic_exchange (& value, v);
    }
    
    //bool is_lock_free() const; // TODO Implement
};        

// Partial specialization for pointer types.
//
template <typename T>
struct atomic <T *>
{
private:
    typedef T * pointer_type;

    pointer_type _p;

private:    
    ptrdiff_t type_size (ptrdiff_t d) const 
    { 
        return d * sizeof(T);
    }

private:
    atomic (atomic const &);
    atomic & operator = (atomic const &);
    
public:
    atomic () 
        : _p(0)  
    {}
        
    ~atomic ()
    {}

    atomic (pointer_type p) 
        : _p (p) 
    {}

    operator pointer_type () const
    {
        return load(); 
    }

    pointer_type operator = (pointer_type p)
    {
        store(p);
        return p;
    }

    pointer_type operator ++ (int)
    {
        return fetch_add(1);
    }

    pointer_type operator -- (int)
    { 
        return fetch_sub(1);
    }

    pointer_type operator ++ ()
    {
        return atomic_add_fetch(& _p, type_size(1)); 
    }

    pointer_type operator -- ()
    { 
        return atomic_sub_fetch(& _p, type_size(1));
    }

    pointer_type operator += (ptrdiff_t d)
    {
        return atomic_add_fetch(& _p, type_size(d)); 
    }

    pointer_type operator -= (ptrdiff_t d)
    {
        return atomic_sub_fetch(& _p, type_size(d)); 
    }

//    bool is_lock_free () const
//    { 
//        return __atomic_is_lock_free(sizeof(__pointer_type), nullptr);
//    }

    void store (pointer_type p)
    {
        atomic_store(& _p, p);
    }

    pointer_type load () const
    {
        return atomic_load((pointer_type *)& _p);
    }

    pointer_type exchange (pointer_type p)
    {
        return atomic_exchange(& _p, p);
    }

    pointer_type fetch_add (ptrdiff_t d)
    {
        return atomic_fetch_add(& _p, type_size(d));
    }

    pointer_type fetch_sub (ptrdiff_t d)
    {
        return atomic_fetch_sub(& _p, type_size(d));
    }
};
      
template <typename SourceType, typename DestType>
inline DestType atomic_cast_helper (SourceType const & from)
{
    union { SourceType from; DestType to; } cast_data = {};
    cast_data.from = from;
    return cast_data.to;
}

typedef atomic<char>               atomic_char;
typedef atomic<signed char>        atomic_schar;
typedef atomic<unsigned char>      atomic_uchar;
typedef atomic<short>              atomic_short;
typedef atomic<unsigned short>     atomic_ushort;
typedef atomic<int>                atomic_int;
typedef atomic<unsigned int>       atomic_uint;
typedef atomic<long>               atomic_long;
typedef atomic<unsigned long>      atomic_ulong;
typedef atomic<long long>          atomic_llong;
typedef atomic<unsigned long long> atomic_ullong;

typedef atomic<int8_t>             atomic_int8;
typedef atomic<uint8_t>            atomic_uint8;
typedef atomic<int16_t>            atomic_int16;
typedef atomic<uint16_t>           atomic_uint16;
typedef atomic<int32_t>            atomic_int32;
typedef atomic<uint32_t>           atomic_uint32;

#if PFS_HAVE_INT64
typedef atomic<int64_t>            atomic_int64;
typedef atomic<uint64_t>           atomic_uint64;
#endif

typedef atomic<intptr_t>           atomic_intptr_t;
typedef atomic<uintptr_t>          atomic_uintptr_t;
typedef atomic<size_t>             atomic_size_t;
typedef atomic<intmax_t>           atomic_intmax_t;
typedef atomic<uintmax_t>          atomic_uintmax_t;
typedef atomic<ptrdiff_t>          atomic_ptrdiff_t;

} // pfs

#endif // __PFS_CXX_VERSION__ >= 2011

#endif /* __PFS_ATOMIC_HPP__ */
