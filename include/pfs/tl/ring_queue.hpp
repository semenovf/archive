/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ring_queue.hpp
 * Author: wladt
 *
 * Created on October 20, 2016
 */

#ifndef __PFS_TL_RING_QUEUE_HPP__
#define __PFS_TL_RING_QUEUE_HPP__

#include <pfs/debug.hpp>

namespace pfs {
namespace tl {

//
//    ---------------------------- 
//    | | | |x|x|x|x| | | | | | |
//    ---------------------------- 
//     ^     ^       ^           ^   
//     |     |       |           |
//  _begin _head   _tail     _capacity
//                             _end
//
//    ---------------------------- 
//    |y|y|y|y| | | | | |x|x|x|x|
//    ---------------------------- 
//     ^       ^         ^       ^
//     |       |         |       |
//  _begin   _tail    _head  _capacity
//                             _end
//
//    ---------------------------- 
//    |y|y|y|y| | | |x|x|x|x| | |
//    ---------------------------- 
//     ^       ^     ^       ^   ^   
//     |       |     |       |   |
//  _begin   _tail _head   _end _capacity
//

//
// struct RingQueueTraits
// {
//     typedef SizeType size_type; ///<< Size type like std::size_t
//     typedef EmptyExceptionType empty_exception_type; ///<< Exception class satisfies DefaultConstructible concept
// };
//

//
// Ring queue item is a ring queue itself.
// It is a helper class for use in ring queue.
//
template <typename Traits>
class ring_queue
{
public:
    typedef typename Traits::size_type            size_type;
    typedef typename Traits::atomic_type          atomic_type;
    typedef typename Traits::empty_exception_type empty_exception_type;

private:
    char *      _begin;
   	size_type   _end;      // Logic end (not a real end of buffer)
	size_type   _head;     // Head of data
	size_type   _tail;     // Tail of data
    atomic_type _count;    // Count in bytes
    size_type const _capacity; // Capacity in bytes

private:
    template <typename T>
    T * allocate ();

    template <typename T>
    T & front_helper ()
    {
        if (empty())
            throw empty_exception_type();

        return _head == _end // Head exceeds End, but Queue is not empty
                ? *reinterpret_cast<T *>(_begin)
                : *reinterpret_cast<T *>(_begin + _head);
    }
    
    template <typename T>
    T & back_helper ()
    {
        if (empty())
            throw empty_exception_type();

        return _tail == 0 
                ? *reinterpret_cast<T *>(_begin + (_end - sizeof(T)))
                : *reinterpret_cast<T *>(_begin + (_tail - sizeof(T)));
    }

    void pop_helper (size_type nsize)
    {
        if (empty())
            throw empty_exception_type();

        _head += nsize;
        
        if (_head == _end) { // Head exceeds End, but Queue is not empty
            _head = 0;
            _end = _capacity;
        }
        
        --_count;
    }
        
public:
	ring_queue (size_type capacity)
        : _begin(new char[capacity])
		, _end(capacity)
		, _head(0)
		, _tail(0)
		, _count(0)
        , _capacity(capacity)
	{
        // PFS_DEBUG(printf("ring_queue::ring_queue(%lu)\n", size_type(_capacity)));
    }
        
    ~ring_queue ()
    {
        //PFS_DEBUG(printf("ring_queue::~ring_queue(%lu)\n", size_type(_capacity)));
        delete [] _begin;
    }

    bool empty () const
    {
        return _count == 0;
    }

    size_type capacity () const
    {
        return size_type(_capacity);
    }
    
    size_type count () const
    {
        return size_type(_count);
    }

    bool ensure_capacity (size_type nsize);
    
    template <typename T>
    T & front ()
    {
        return front_helper<T>();
    }
    
    template <typename T>
    T const & front () const
    {
        return front_helper<T>();
    }

    template <typename T>
    T & back ()
    {
        return back_helper<T>();
    }

    template <typename T>
    T const & back () const
    {
        return back_helper<T>();
    }

    /**
     *  @brief  Removes first element.
     */
    template <typename T>
    void pop ()
    {
        front<T>().~T();
        pop_helper(size_type(sizeof(T)));
    }

    void pop (size_type nsize)
    {
        pop_helper(nsize);
    }
    
    template <typename T>
    bool push ();
    
    template <typename T, typename Arg1>
    bool push (Arg1 a1);

    template <typename T, typename Arg1, typename Arg2>
    bool push (Arg1 a1, Arg2 a2);

    template <typename T, typename Arg1, typename Arg2, typename Arg3>
    bool push (Arg1 a1, Arg2 a2, Arg3 a3);

    template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
    bool push (Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4);
    
    template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
    bool push (Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5);

    template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
    bool push (Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6);

    template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
    bool push (Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7);

    template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8>
    bool push (Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8);
};

template <typename Traits>
bool ring_queue<Traits>::ensure_capacity (size_type nsize)
{
    if (empty()) {
   		_head = 0;                          // Move Head ...
		_tail = 0;                          // .. and Tail to the begin of queue
		_end  = _capacity;

        return nsize <= _capacity;
    } else if (_head < _tail) {             // Tail is at the right side of Head
        size_type end  = _capacity;

        if (_tail + nsize <= end) {         // There is enough space before the real end of queue
            _end = end;                     // Logic End must be moved to real end of queue
            return true;
        } else {                            // There is no enough space before the real end of queue
            end = _tail;                    // Save Tail position

            if (nsize <= _head) {           // There is enough space before Head from Begin
                _tail = 0;                  // Move Tail to Begin
                _end = end;                 // Move Logic End
                return true;
            }
         }

        return nsize <= _capacity - _tail;
    } else if (_head > _tail) {
        return _tail + nsize <= _head; 
    }

    return false;
}

template <typename Traits>
template <typename T>
T * ring_queue<Traits>::allocate ()
{
    T * result = 0;

    if (ensure_capacity(sizeof(T))) {
        result = reinterpret_cast<T *>(_begin + _tail);
        _tail += sizeof(T);
    }

    return result;
}

template <typename Traits>
template <typename T>
bool ring_queue<Traits>::push ()
{
    T * ptr = allocate<T>();

    if (ptr) {
        (void *)new (ptr) T;
        ++_count;
        return true;
    }

    return false;
}

template <typename Traits>
template <typename T, typename Arg1>
bool ring_queue<Traits>::push (Arg1 a1)
{
    T * ptr = allocate<T>();

    if (ptr) {
        (void *)new (ptr) T(a1);
        ++_count;
        return true;
    }

    return false;
}

template <typename Traits>
template <typename T, typename Arg1, typename Arg2>
bool ring_queue<Traits>::push (Arg1 a1, Arg2 a2)
{
    T * ptr = allocate<T>();

    if (ptr) {
        (void *)new (ptr) T(a1, a2);
        ++_count;
        return true;
    }
    return false;
}

template <typename Traits>
template <typename T, typename Arg1, typename Arg2, typename Arg3>
bool ring_queue<Traits>::push (Arg1 a1, Arg2 a2, Arg3 a3)
{
    T * ptr = allocate<T>();

    if (ptr) {
        (void *)new (ptr) T(a1, a2, a3);
        ++_count;
        return true;
    }
    return false;
}

template <typename Traits>
template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
bool ring_queue<Traits>::push (Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4)
{
    T * ptr = allocate<T>();

    if (ptr) {
        (void *)new (ptr) T(a1, a2, a3, a4);
        ++_count;
        return true;
    }
    return false;
}

template <typename Traits>
template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
bool ring_queue<Traits>::push (Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5)
{
    T * ptr = allocate<T>();

    if (ptr) {
        (void *)new (ptr) T(a1, a2, a3, a4, a5);
        ++_count;
        return true;
    }
    return false;
}

template <typename Traits>
template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
bool ring_queue<Traits>::push (Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6)
{
    T * ptr = allocate<T>();

    if (ptr) {
        (void *)new (ptr) T(a1, a2, a3, a4, a5, a6);
        ++_count;
        return true;
    }
    return false;
}

template <typename Traits>
template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
bool ring_queue<Traits>::push (Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7)
{
    T * ptr = allocate<T>();

    if (ptr) {
        (void *)new (ptr) T(a1, a2, a3, a4, a5, a6, a7);
        ++_count;
        return true;
    }
    return false;
}

template <typename Traits>
template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8>
bool ring_queue<Traits>::push (Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8)
{
    T * ptr = allocate<T>();

    if (ptr) {
        (void *)new (ptr) T(a1, a2, a3, a4, a5, a6, a7, a8);
        ++_count;
        return true;
    }
    return false;
}

}} // pfs::tl

#endif /* __PFS_TL_RING_QUEUE_HPP__ */
