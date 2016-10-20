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

namespace pfs {

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
//     static SizeType const default_increment_factor = 32; ///<< Default increment factor
//     typedef SizeType      size_type;         ///<< Size type like std::size_t
//     typedef AtomicType    atomic_type;       ///<< Atomic type like std::atomic_size_t since C++11
//     typedef MutexType     mutex_type;        ///<< Mutex type must be satisfied LockGuardType::mutex_type
//     typedef LockGuardType lock_guard_type;   ///<< Lock guard type like std::lock_guard since C++11 (mutex type must be (TODO en?выведен?) from this type)
//     typedef EmptyQueueExceptioType empty_queue_exception_type; ///<< Exception class satisfies DefaultConstructible concept
// };
// 
// MoveFunctor; ///<< Functor with implementation of operator @code void * operator (void *dest, const void *src, size_t n) @endcode
//
template <typename RingQueueTraits, typename MoveFunctor>
class ring_queue
{
public: // Typedefs
    typedef typename RingQueueTraits::size_type          size_type;
    typedef typename RingQueueTraits::atomic_type        atomic_type;
    typedef typename RingQueueTraits::lock_guard_type    lock_guard_type;
    typedef typename RingQueueTraits::mutex_type         mutex_type;
    typedef typename RingQueueTraits::empty_queue_exception_type empty_queue_exception_type;
    typedef MoveFunctor                                  move_functor_type;
    
public: // Constants
    static size_type const default_increment_factor = RingQueueTraits::default_increment_factor;

private:
	char *      _begin;
	size_type   _end;   // Logic end (not a real end of buffer)
	size_type   _head;
	size_type   _tail;
    atomic_type _count;
    atomic_type _capacity;
    size_type   _max_capacity;
    size_type   _increment_factor;
    
    mutex_type & _pop_mutex;
    mutex_type   _push_mutex;

private:
    // Check for overflow
    //
    bool overflow (size_type increment) const
    {
        return !(_max_capacity >= increment 
                && _max_capacity - increment > _capacity);
    }

    template <typename T>
    T * allocate ();

    template <typename T>
    T & front_helper ()
    {
        if (empty())
            throw empty_queue_exception_type();

        if (_head == _end) { // Head exceeds Tail, but Queue is not empty
            _head = 0;
            _end = _capacity;
        }
        
        return *reinterpret_cast<T *>(_begin + _head);
    }

    template <typename T>
    T & back_helper ()
    {
        if (empty())
            throw empty_queue_exception_type();

        return _tail == 0 
                ? *reinterpret_cast<T *>(_end - sizeof(T))
                : *reinterpret_cast<T *>(_tail - sizeof(T));
    }

public:
	ring_queue (mutex_type & pop_mutex
            , size_type initial
            , size_type max_capacity
            , size_type increment_factor = default_increment_factor)
        : _begin(new char[initial])
		, _end(initial)
		, _head(0)
		, _tail(0)
		, _count(0)
        , _capacity(initial)
        , _max_capacity(max_capacity)
        , _increment_factor(increment_factor)
        , _pop_mutex(pop_mutex)
	{}

    ~ring_queue ()
    {
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
        
        _head += sizeof(T);
        --_count;
    }
    
    void pop (size_type nsize) 
    {
        if (empty())
            throw empty_queue_exception_type();

        _head += nsize;
        --_count;
    }
};

template <typename RingQueueTraits, typename MoveFunctor>
bool ring_queue<RingQueueTraits, MoveFunctor>::ensure_capacity (size_type nsize)
{
    lock_guard_type locker(_pop_mutex);
    
//    if (! _begin) {
//        size_type increment = _increment_factor * nsize;
//        
//        if (overflow(increment))
//            return false;
//        
//        _capacity += increment;
//        _begin = new char[_capacity];
//        _end = _capacity;
//        _head = 0;
//        _tail = 0;
//        _count = 0;
//        
//        return true;
//    }

    if (empty()) {
        // Move Head and Tail to the begin of queue
   		_head = 0;
		_tail = 0;
		_end  = _capacity;

        if (nsize <= _capacity)
            return true;
    } else {                                    // Queue is not empty
        if (_tail == _head) {                   // Queue is full, need expand the queue
            ;
        } else if (_tail > _head) {             // Tail is at the right side of Head
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
        } else {                                // Tail is at the left side of Head
            if (_tail + nsize <= _head) {       // There is enough space before Head
                return true;
            }
        }
    }
    
    //
	// There is no enough space to push function
    //
    
    size_type capacity = _capacity;
    size_type increment = _increment_factor * nsize;

    // Check for overflow
    //
    if (! overflow(increment)) {
        capacity += increment;
        char * begin = new char[capacity];
        
        if (_tail > _head || empty()) {
            //
            //          |---n---|                |---n---|
            //    -------------------------       -----------------------------
            //    | | | |x|x|x|x| | | | | | -->  |x|x|x|x| | | | | | | | | | |
            //    -------------------------       -----------------------------
            //     ^     ^       ^     ^          ^       ^                   ^
            //     |     |       |     |          |       |                   |
            //  _begin _head   _tail _end      _begin   _tail               _end
            //                                 _head
            //
            size_type n = _tail - _head;
            
            move_functor_type()(begin, _begin + _head, n);
            
            _head = 0;
            _tail = n;
            _end  = capacity;
        } else {
            //
            //    |n1-|         |-n2--|          |-n2--|n1-|
            //    -------------------------       ------------------------------
            //    |y|y| | | | | |x|x|x| | | -->  |x|x|x|y|y| | | | | | | | | | 
            //    -------------------------       ------------------------------
            //     ^   ^         ^     ^          ^         ^                 ^
            //     |   |         |     |          |         |                 |
            // _begin _tail  _head  _end       _begin     _tail             _end
            //                                 _head
            //
            size_type n1 = _tail;
            size_type n2 = _end - _head;
            
            move_functor_type()(begin, _begin + _head, n2);
            move_functor_type()(begin + n2, _begin, n1);
            
            _head = 0;
            _tail = n2 + n1;
            _end  = capacity;
        }
        
        delete [] _begin;
        
      	_begin = begin;
        _capacity = capacity;
        
        return true;
    }
    
    return false;
}

template <typename RingQueueTraits, typename MoveFunctor>
template <typename T>
T * ring_queue<RingQueueTraits, MoveFunctor>::allocate ()
{
    T * result = 0;

    if (ensure_capacity(sizeof(T))) {
        result = reinterpret_cast<T *>(_begin + _tail);
        _tail += sizeof(T);
    }

    return result;
}

template <typename RingQueueTraits, typename MoveFunctor>
template <typename T>
bool ring_queue<RingQueueTraits, MoveFunctor>::push ()
{
    lock_guard_type locker(_push_mutex);

    T * ptr = allocate<T>();

    if (ptr) {
        (void *)new (ptr) T;
        ++_count;
        return true;
    }

    return false;
}

template <typename RingQueueTraits, typename MoveFunctor>
template <typename T, typename Arg1>
bool ring_queue<RingQueueTraits, MoveFunctor>::push (Arg1 a1)
{
    lock_guard_type locker(_push_mutex);

    T * ptr = allocate<T>();

    if (ptr) {
        (void *)new (ptr) T(a1);
        ++_count;
        return true;
    }

    return false;
}

template <typename RingQueueTraits, typename MoveFunctor>
template <typename T, typename Arg1, typename Arg2>
bool ring_queue<RingQueueTraits, MoveFunctor>::push (Arg1 a1, Arg2 a2)
{
    lock_guard_type locker(_push_mutex);

    T * ptr = allocate<T>();

    if (ptr) {
        (void *)new (ptr) T(a1, a2);
        ++_count;
        return true;
    }
    return false;
}

template <typename RingQueueTraits, typename MoveFunctor>
template <typename T, typename Arg1, typename Arg2, typename Arg3>
bool ring_queue<RingQueueTraits, MoveFunctor>::push (Arg1 a1, Arg2 a2, Arg3 a3)
{
    lock_guard_type locker(_push_mutex);

    T * ptr = allocate<T>();

    if (ptr) {
        (void *)new (ptr) T(a1, a2, a3);
        ++_count;
        return true;
    }
    return false;
}

template <typename RingQueueTraits, typename MoveFunctor>
template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
bool ring_queue<RingQueueTraits, MoveFunctor>::push (Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4)
{
    lock_guard_type locker(_push_mutex);

    T * ptr = allocate<T>();

    if (ptr) {
        (void *)new (ptr) T(a1, a2, a3, a4);
        ++_count;
        return true;
    }
    return false;
}

template <typename RingQueueTraits, typename MoveFunctor>
template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
bool ring_queue<RingQueueTraits, MoveFunctor>::push (Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5)
{
    lock_guard_type locker(_push_mutex);

    T * ptr = allocate<T>();

    if (ptr) {
        (void *)new (ptr) T(a1, a2, a3, a4, a5);
        ++_count;
        return true;
    }
    return false;
}

template <typename RingQueueTraits, typename MoveFunctor>
template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
bool ring_queue<RingQueueTraits, MoveFunctor>::push (Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6)
{
    lock_guard_type locker(_push_mutex);

    T * ptr = allocate<T>();

    if (ptr) {
        (void *)new (ptr) T(a1, a2, a3, a4, a5, a6);
        ++_count;
        return true;
    }
    return false;
}

template <typename RingQueueTraits, typename MoveFunctor>
template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
bool ring_queue<RingQueueTraits, MoveFunctor>::push (Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7)
{
    lock_guard_type locker(_push_mutex);

    T * ptr = allocate<T>();

    if (ptr) {
        (void *)new (ptr) T(a1, a2, a3, a4, a5, a6, a7);
        ++_count;
        return true;
    }
    return false;
}

template <typename RingQueueTraits, typename MoveFunctor>
template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8>
bool ring_queue<RingQueueTraits, MoveFunctor>::push (Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8)
{
    lock_guard_type locker(_push_mutex);

    T * ptr = allocate<T>();

    if (ptr) {
        (void *)new (ptr) T(a1, a2, a3, a4, a5, a6, a7, a8);
        ++_count;
        return true;
    }
    return false;
}

} // pfs

#endif /* __PFS_ACTIVE_QUEUE_HPP__ */
