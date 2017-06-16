/* 
 * File:   active_queue.hpp
 * Author: wladt
 *
 * Created on September 22, 2016, 4:05 PM
 */

#ifndef __PFS_ACTIVE_QUEUE_HPP__
#define __PFS_ACTIVE_QUEUE_HPP__

#include <pfs/atomic.hpp>
#include <pfs/mutex.hpp>
#include <pfs/exception.hpp>
#include <pfs/tl/active_queue.hpp>

namespace pfs {

struct empty_exception : public length_error
{
    empty_exception () : length_error("empty container") {}
};

struct active_queue_traits_st
{
    static size_t const default_increment_factor = 10;
    
    typedef size_t          size_type; 
    typedef size_t          atomic_type; 
    typedef pfs::fake_mutex mutex_type;
    typedef empty_exception empty_exception_type;
};

struct active_queue_traits_mt
{
    static size_t const default_increment_factor = 10;
    
    typedef size_t             size_type; 
    typedef pfs::atomic_size_t atomic_type; 
    typedef pfs::mutex         mutex_type;
    typedef empty_exception    empty_exception_type;
};

/**
 * @brief Single thread version of active_queue
 */
template <typename ReturnType = void>
class active_queue_st : public tl::active_queue<ReturnType, active_queue_traits_st>
{
    typedef tl::active_queue<ReturnType, active_queue_traits_st> base_class;
    typedef typename base_class::size_type size_type;
    
public:
	active_queue_st (size_type initial
            , size_type max_capacity
            , size_type increment_factor)
        : base_class(initial, max_capacity, increment_factor)
    {}

   	active_queue_st (size_type initial, size_type max_capacity)
        : base_class(initial, max_capacity)
	{}
    
   	active_queue_st (size_type max_capacity)
        : base_class(max_capacity)
	{}

   	active_queue_st ()
        : base_class(pfs::max_value<size_t>())
	{}
};

/**
 * @brief Multi-thread version of active_queue
 */
template <typename ReturnType = void>
class active_queue_mt : public tl::active_queue<ReturnType, active_queue_traits_mt>
{
    typedef tl::active_queue<ReturnType, active_queue_traits_mt> base_class;
    typedef typename base_class::size_type size_type;
    
public:
	active_queue_mt (size_type initial
            , size_type max_capacity
            , size_type increment_factor)
        : base_class(initial, max_capacity, increment_factor)
    {}

   	active_queue_mt (size_type initial, size_type max_capacity)
        : base_class(initial, max_capacity)
	{}
    
   	active_queue_mt (size_type max_capacity)
        : base_class(max_capacity)
	{}
    
    active_queue_mt ()
        : base_class(pfs::max_value<size_t>())
	{}
};

} // pfs

#endif /* __PFS_ACTIVE_QUEUE_HPP__ */

