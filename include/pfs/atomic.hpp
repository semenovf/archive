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
    typedef Integral int_type;

private:    
    int_type value;
    
private:
    atomic (atomic const &);              // Is not CopyConstructible
    atomic & operator = (atomic const &); // Is not CopyAssignable
        
public:
    atomic () 
        : value(0) 
    {}
    
    atomic (int_type v)
        : value(v)
    {}
    
    int_type operator = (int_type v)
    {
        store(v);
        return v;
    }
    
    operator int_type () const
    {
        return load();
    }
    
    void store (int_type v)
    {
        atomic_store(& value, v);
    }
    
    int_type load () const
    {
        return atomic_load((int_type *)& value);
    }

    int_type operator ++ ()
    {
        return atomic_add_fetch(& value, 1);
    }

//    int_type operator ++ (int)
//    {
//        return atomic_fetch_add(1);
//    }
    
    int_type operator -- ()
    {
        return atomic_sub_fetch(& value, 1);
    }

//    int_type operator -- (int)
//    {
//        return fetch_sub(1);
//    }
    
    int_type operator += (int_type n)
    {
        return atomic_add_fetch(& value, n); 
    }

    int_type operator -= (int_type n)
    {
        return atomic_sub_fetch(& value, n);
    }

    int_type operator &= (int_type n)
    {
        return atomic_and_fetch(& value, n);
    }

    int_type operator |= (int_type n)
    {
        return atomic_or_fetch(& value, n);
    }

    int_type operator ^= (int_type n)
    { 
        return atomic_xor_fetch(& value, n);
    }
    
    int_type fetch_add (int_type n)
    {
        return atomic_fetch_add(& value, n);
    }

    int_type fetch_sub (int_type n)
    {
        return atomic_fetch_sub(& value, n);
    }
    
    int_type fetch_and (int_type n)
    {
        return atomic_fetch_and(& value, n); 
    }
    
    int_type fetch_or (int_type n)
    {
        return atomic_fetch_or(& value, n);
    }

    int_type fetch_xor (int_type n)
    {
        return atomic_fetch_xor(& value, n);
    }

    int_type exchange (int_type v)
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
        atomic_store(_p, p);
    }

    pointer_type load () const
    {
        return atomic_load(_p);
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

typedef atomic<intptr_t>           atomic_intptr_t;
typedef atomic<uintptr_t>          atomic_uintptr_t;
typedef atomic<size_t>             atomic_size_t;
typedef atomic<intmax_t>           atomic_intmax_t;
typedef atomic<uintmax_t>          atomic_uintmax_t;
typedef atomic<ptrdiff_t>          atomic_ptrdiff_t;

} // pfs

#endif

#if __COMMENT__


#if defined(PFS_CC_MSVC)
//#	pragma message("Using MSVC implementation for atomic variables")
#	include <pfs/bits/atomic_msvc.hpp>
#	define HAVE_ATOMIC_MSVC 1
#elif defined(PFS_CPP_CXX11)
//#	pragma message("Using C++11 implementation for atomic variables")
#	include <pfs/bits/atomic_cxx11.hpp>
#	define HAVE_ATOMIC_CX11 1
#elif defined(PFS_CC_GNUC)
//#	pragma message("Using GCC implementation for atomic variables")
#	include <pfs/bits/atomic_gcc.hpp>
#	define HAVE_ATOMIC_GCC 1
#elif defined(PFS_TARGET_CPU) && (defined(PFS_TARGET_CPU_X86) || defined(PFS_TARGET_CPU_x86))
//#	pragma message("Using x86 implementation for atomic variables")
#	include <pfs/bits/atomic_x86.hpp>
#	define HAVE_ATOMIC_X86 1
#else
//#	pragma message("Using implementation with locking (worst case) for atomic variables")
#	include <pfs/bits/atomic_mt.hpp>
#	define HAVE_ATOMIC_MT 1
#endif

#endif

#endif /* __PFS_ATOMIC_HPP__ */
