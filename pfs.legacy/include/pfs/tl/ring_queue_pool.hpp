/* 
 * File:   ring_queue.hpp
 * Author: wladt
 *
 * Created on October 20, 2016
 */

#ifndef __PFS_TL_RING_QUEUE_POOL_HPP__
#define __PFS_TL_RING_QUEUE_POOL_HPP__

#include <pfs/tl/trivial_lock_guard.hpp>
#include <pfs/tl/trivial_list.hpp>
#include <pfs/tl/ring_queue.hpp>

namespace pfs {
namespace tl {

//
// Glossary
//      Item    - instance of ring_queue_item.
//      Element - type instance added to queue.
//
//
// struct RingQueuePoolTraits
// {
//     static  SizeType const default_increment_factor = 32; ///<< Default increment factor
//     typedef SizeType      size_type;         ///<< Size type like std::size_t
//     typedef MutexType     mutex_type;        ///<< Mutex type must be satisfied LockGuardType::mutex_type
//     typedef EmptyQueueExceptioType empty_exception_type; ///<< Exception class satisfies DefaultConstructible concept
// };
//
// Pop Item                                       Push Item
// ----------   ----------   ----------           ----------   
// | Item-0 |-->| Item-1 |-->| Item-2 |-->     -->| Item-0 |
// ----------   ----------   ----------   . . .   ----------

template <typename Traits>
class ring_queue_pool
{
public: // Typedefs
    typedef typename Traits::size_type            size_type;
    typedef typename Traits::empty_exception_type empty_exception_type;
    typedef ring_queue<Traits>                    item_type;
    typedef trivial_list<item_type>               list_type;
    typedef typename list_type::iterator          iterator;
    typedef typename list_type::const_iterator    const_iterator;
    typedef typename Traits::mutex_type           mutex_type;
    typedef trivial_lock_guard<mutex_type>        lock_guard_type;
    
public: // Constants
    static size_type const default_increment_factor = Traits::default_increment_factor;

private:
    //
    // Sequence contains Items
    //
    list_type _list;
    
    //
    // Actual capacity for new items.
    // Incremented when added new item according to formula:
    //
    // actual_item_capacity = actual_item_capacity + <Element size> * increment_factor
    //
    size_type _actual_item_capacity;
    
    //
    // Limit of total capacity in bytes (Constant from instantiation).
    //
    size_type const _max_capacity;
    
    //
    // Total bytes occupied.
    //
    size_type _occupied;
    
    //
    // Increment factor while adding new Item
    //
    size_type _increment_factor;
    
    //
    // Total count of Elements
    //
    size_type _count;
    
    mutex_type _mutex;
    
private:
//    void erase_empty_items () 
//    {
//        typename list_type::iterator it = _list.begin();
//        typename list_type::iterator it_end = _list.end();
//        
//        while (it != it_end && it->next != it_end && it->value->empty()) {
//            it = _list.erase(it);
//        }
//    }
    
    bool ensure_capacity (size_type nsize);
    
public:
	ring_queue_pool (size_type initial_capacity
            , size_type max_capacity
            , size_type increment_factor = default_increment_factor)
        : _actual_item_capacity(initial_capacity)
        , _max_capacity(max_capacity)
        , _occupied(0)
        , _increment_factor(increment_factor)
        , _count(0)
	{
        _list.emplace_back(initial_capacity);
    }

    bool empty () const
    {
        return _count == 0;
    }

    size_type count () const
    {
        return _count;
    }

    template <typename T>
    T & front ()
    {
        lock_guard_type locker(_mutex); // Below list can be modified
        if (_count == 0)
            throw empty_exception_type();
        return _list.front().front<T>();
    }

//    template <typename T>
//    T const & front () const
//    {
//        if (_count == 0)
//            throw empty_exception_type();
//        return _list.front().front<T>();
//    }

    template <typename T>
    T & back ()
    {
        lock_guard_type locker(_mutex); // Below list can be modified
        if (_count == 0)
            throw empty_exception_type();
        return _list.back().back<T>();
    }

//    template <typename T>
//    T const & back () const
//    {
//        if (_count == 0)
//            throw empty_exception_type();
//        return _list.back().back<T>();
//    }

    template <typename T>
    bool push ()
    {
        lock_guard_type locker(_mutex); // Below list can be modified

        if (ensure_capacity(sizeof(T)) && _list.back().push<T>()) {
            ++_count;
            _occupied += sizeof(T);
            return true;
        }
        return false;
    }

    template <typename T, typename Arg1>
    bool push (Arg1 a1)
    {
        lock_guard_type locker(_mutex); // Below list can be modified

        if (ensure_capacity(sizeof(T)) && _list.back().push<T>(a1)) {
            ++_count;
            _occupied += sizeof(T);
            return true;
        }
        return false;
    }

    template <typename T, typename Arg1, typename Arg2>
    bool push (Arg1 a1, Arg2 a2)
    {
        lock_guard_type locker(_mutex); // Below list can be modified

        if (ensure_capacity(sizeof(T)) && _list.back().push<T>(a1, a2)) {
            ++_count;
            _occupied += sizeof(T);
            return true;
        }
        return false;
    }

    template <typename T, typename Arg1, typename Arg2, typename Arg3>
    bool push (Arg1 a1, Arg2 a2, Arg3 a3)
    {
        lock_guard_type locker(_mutex); // Below list can be modified

        if (ensure_capacity(sizeof(T)) && _list.back().push<T>(a1, a2, a3)) {
            ++_count;
            _occupied += sizeof(T);
            return true;
        }
        return false;
    }

    template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
    bool push (Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4)
    {
        lock_guard_type locker(_mutex); // Below list can be modified

        if (ensure_capacity(sizeof(T)) && _list.back().push<T>(a1, a2, a3, a4)) {
            ++_count;
            _occupied += sizeof(T);
            return true;
        }
        return false;
    }
    
    template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
    bool push (Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5)
    {
        lock_guard_type locker(_mutex); // Below list can be modified

        if (ensure_capacity(sizeof(T)) && _list.back().push<T>(a1, a2, a3, a4, a5)) {
            ++_count;
            _occupied += sizeof(T);
            return true;
        }
        return false;
    }

    template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
    bool push (Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6)
    {
        lock_guard_type locker(_mutex); // Below list can be modified

        if (ensure_capacity(sizeof(T)) && _list.back().push<T>(a1, a2, a3, a4, a5, a6)) {
            ++_count;
            _occupied += sizeof(T);
            return true;
        }
        return false;
    }

    template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
    bool push (Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7)
    {
        lock_guard_type locker(_mutex); // Below list can be modified

        if (ensure_capacity(sizeof(T)) && _list.back().push<T>(a1, a2, a3, a4, a5, a6, a7)) {
            ++_count;
            _occupied += sizeof(T);
            return true;
        }
        return false;
    }

    template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8>
    bool push (Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8)
    {
        lock_guard_type locker(_mutex); // Below list can be modified

        if (ensure_capacity(sizeof(T)) && _list.back().push<T>(a1, a2, a3, a4, a5, a6, a7, a8)) {
            ++_count;
            _occupied += sizeof(T);
            return true;
        }
        return false;
    }

    /**
     *  @brief  Removes first element.
     */
    template <typename T>
    void pop ();
    
    /**
     * @brief Removes first @a nsize bytes.
     * @param nsize
     * 
     * @note Using this method is unsafe. 
     *       It must be used with manually call of destructor.
     */
    void pop (size_type nsize);
};

template <typename Traits>
bool ring_queue_pool<Traits>::ensure_capacity (size_type nsize)
{
    if (! _list.empty()) {
        if (_list.back().ensure_capacity(nsize))
            return true;
    }
    
    size_type increment = _increment_factor * nsize;
    size_type item_capacity = _actual_item_capacity + increment;

    if (_max_capacity < _occupied + nsize)
        return false;
    
    _actual_item_capacity = item_capacity;

    _list.emplace_back(size_type(_actual_item_capacity));
    
    if (& _list.front() != & _list.back() // List has mor than one element
            && _list.front().empty())
        _list.erase(_list.begin());

    return true;
}

template <typename Traits>
template <typename T>
void ring_queue_pool<Traits>::pop ()
{
    lock_guard_type locker(_mutex); // Below list can be modified
    
    if (! empty()) {
        _list.front().pop<T>();
        --_count;
        _occupied -= sizeof(T);
    
        if (& _list.front() != & _list.back() // List has more than one element
                && _list.front().empty())
            _list.erase(_list.begin());
    }
}

template <typename Traits>
void ring_queue_pool<Traits>::pop (size_type nsize) 
{
    lock_guard_type locker(_mutex); // Below list can be modified

    if (! empty()) {
        _list.front().pop(nsize);
        --_count;
        _occupied -= nsize;

        if (& _list.front() != & _list.back()  // List has mor than one element
                && _list.front().empty())
            _list.erase(_list.begin());
    }
}

}} // pfs::tl

#endif /* __PFS_TL_RING_QUEUE_POOL_HPP__ */
