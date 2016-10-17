/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   active_queue.hpp
 * Author: wladt
 *
 * Created on September 22, 2016, 4:05 PM
 */

#ifndef __PFS_ACTIVE_QUEUE_1_HPP__
#define __PFS_ACTIVE_QUEUE_1_HPP__

#include <cstring> // for std::memcpy
#include <pfs/atomic.hpp>
#include <pfs/binder.hpp>
#include <pfs/mutex.hpp>

#include <pfs/debug.hpp>

namespace pfs {

namespace details {

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

class ring_queue
{
public:
    static size_t const default_increment_factor = 10;

private:
	char * _begin;
	size_t _end;   // Logic end (not a real end of buffer)
	size_t _head;
	size_t _tail;
    size_t _count;
    size_t _capacity;
    size_t _max_capacity;
    size_t _increment_factor;

private:
    // Check for overflow
    //
    bool overflow (size_t increment) const
    {
        return !(_max_capacity >= increment 
                && _max_capacity - increment > _capacity);
    }

	void reset ()
	{
		_head = 0;   // Move Head and Tail to the begin of queue
		_tail = 0;
		_end  = _capacity;
	}
    
    template <typename T>
    T * allocate ();

    template <typename T>
    T & front_helper ()
    {
        PFS_ASSERT(!empty());

        if (_head == _end) { // Head exceeds Tail, but Queue is not empty
            _head = 0;
            _end = _capacity;
        }
        
        return *reinterpret_cast<T *>(_begin + _head);
    }

    template <typename T>
    T & back_helper ()
    {
        PFS_ASSERT(!empty());

        return _tail == 0 
                ? *reinterpret_cast<T *>(_end - sizeof(T))
                : *reinterpret_cast<T *>(_tail - sizeof(T));
    }

    
public:
	ring_queue (size_t initial = 0
            , size_t max_capacity = pfs::max_value<size_t>()
            , size_t increment_factor = default_increment_factor)
        : _begin(initial > 0 ? new char[initial] : 0)
		, _end(initial)
		, _head(0)
		, _tail(0)
		, _count(0)
        , _capacity(initial)
        , _max_capacity(max_capacity)
        , _increment_factor(increment_factor)
	{}

    ~ring_queue ()
    {
        delete _begin;
    }
        
    bool empty () const
    {
        return _count == 0;
    }

    size_t capacity () const
    {
        return _capacity;
    }
    
    size_t count () const
    {
        return _count;
    }

    bool ensure_capacity (size_t nsize);

    template <typename T>
    bool push ()
    {
        T * ptr = allocate<T>();
        
        if (ptr) {
            (void *)new (ptr) T;
            return true;
        }
        
        return false;
    }

    template <typename T, typename Arg1>
    bool push (Arg1 const & a1)
    {
        T * ptr = allocate<T>();
        
        if (ptr) {
            (void *)new (ptr) T(a1);
            return true;
        }
        
        return false;
    }

    template <typename T, typename Arg1, typename Arg2>
    bool push (Arg1 const & a1, Arg2 const & a2)
    {
        T * ptr = allocate<T>();
        if (ptr) {
            (void *)new (ptr) T(a1, a2);
            return true;
        }
        return false;
    }

    template <typename T, typename Arg1, typename Arg2, typename Arg3>
    bool push (Arg1 const & a1, Arg2 const & a2, Arg3 const & a3)
    {
        T * ptr = allocate<T>();
        if (ptr) {
            (void *)new (ptr) T(a1, a2, a3);
            return true;
        }
        return false;
    }

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
        PFS_ASSERT(!empty());
        front<T>().~T();
        _head += sizeof(T); // Supposed Head position
        --_count;
    }
    
    void pop (size_t nsize) 
    {
        PFS_ASSERT(!empty());
        _head += nsize;   // Supposed Head position
        --_count;
    }
};

// TODO Move to .cpp file
//
inline bool ring_queue::ensure_capacity (size_t nsize)
{
    if (! _begin) {
        size_t increment = _increment_factor * nsize;
        
        if (overflow(increment))
            return false;
        
        _capacity += increment;
        _begin = new char[_capacity];
        _end = _capacity;
        _head = 0;
        _tail = 0;
        _count = 0;
        
        return true;
    }

    if (_tail == _head && empty()) {
        reset();   // Move Head and Tail to the begin of queue
    }

    if (_tail == _head && ! empty()) {      // Queue is full
        ;
    } if (_tail >= _head) {                 // Tail is at the right side of Head or Queue is empty (_tail == _head)
        size_t end  = _capacity;

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

    //
	// There is no enough space to push function
    //
    
    size_t capacity = _capacity;
    size_t increment = _increment_factor * nsize;

    // Check for overflow
    //
    if (! overflow(increment)) {
        capacity += increment;
        char * begin = new char[capacity];
        
        if (_tail >= _head) {
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
            size_t n = _tail - _head;
            std::memcpy(begin, _begin + _head, n);
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
            size_t n1 = _tail;
            size_t n2 = _end - _head;
            
            std::memcpy(begin, _begin + _head, n2);
            std::memcpy(begin + n2, _begin, n1);
            
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

template <typename T>
T * ring_queue::allocate ()
{
    T * result = 0;

    if (ensure_capacity(sizeof(T))) {
        result = reinterpret_cast<T *>(_begin + _tail);
        _tail += sizeof(T);
        ++_count;
    }

    return result;
}

} // details

template <typename Return, typename Mutex = pfs::fake_mutex>
class active_queue_base
{
public:
	typedef Return return_type;
	typedef binder_base<Return> binder_base_type;

protected:
	mutable Mutex  _mutex;
    details::ring_queue _queue;

public:
	active_queue_base (size_t initial = 0
            , size_t max_capacity = pfs::max_value<size_t>()
            , size_t increment_factor = details::ring_queue::default_increment_factor)
        : _queue(initial, max_capacity, increment_factor)
	{}

	virtual ~active_queue_base ()
	{
		while (!_queue.empty())
			this->pop();
	}

	bool empty () const
	{
        lock_guard<Mutex> locker(_mutex);
		return _queue.empty();
	}

	size_t count () const
	{
        lock_guard<Mutex> locker(_mutex);
		return _queue.count();
	}

	size_t capacity () const
	{
        lock_guard<Mutex>(_mutex);
		return _queue.capacity;
	}

	bool push_function (return_type (* f) ())
	{
		lock_guard<Mutex> locker(_mutex);
        return _queue.push<binder_function0<return_type> >(f);
	}

	template <typename Arg1>
	bool push_function (return_type (* f) (Arg1), Arg1 a1)
	{
		lock_guard<Mutex> locker(_mutex);
        return _queue.push<binder_function1<return_type, Arg1> >(f, a1);
	}
    
	template <typename Arg1, typename Arg2>
	bool push_function (return_type (* f) (Arg1, Arg2), Arg1 a1, Arg2 a2)
	{
		lock_guard<Mutex> locker(_mutex);
        return _queue.push<binder_function2<return_type, Arg1, Arg2> >(f, a1, a2);
	}
    
	void pop ();
};

template <typename Return, typename Mutex>
void active_queue_base<Return, Mutex>::pop ()
{
    lock_guard<Mutex> locker(_mutex);
	binder_base<Return> & bb = _queue.front<binder_base<Return> >();
    
    bb.~binder_base_type();
	_queue.pop(bb.size());
}

template <typename Return, typename Mutex = pfs::fake_mutex>
class active_queue : public active_queue_base<Return, Mutex>
{
public:
	typedef active_queue_base<Return, Mutex> base_class;
	typedef typename base_class::return_type return_type;
	typedef typename base_class::binder_base_type binder_base_type;

    /**
     * @brief Constructs instance of @c active_queue with the given traits. 
     * 
     * @param initial Initial capacity of internal buffer in bytes.
     * @param max_capacity Maximum capacity of internal buffer in bytes.
     * @param increment_factor Increment factor. Multiplier value for increment . 
     *        This value used to increment capacity of internal buffer while pushing
     *        new binder and no room to store it. Real increment value calculated
     *        from @a increment_factor multiplying by size of pushing binder.
     */
	active_queue (size_t initial = 0
            , size_t max_capacity = pfs::max_value<size_t>()
            , size_t increment_factor = details::ring_queue::default_increment_factor)
        : base_class(initial, max_capacity, increment_factor)
    {}

	return_type call ();
	return_type call_all ();
};

template <typename Mutex>
class active_queue<void, Mutex> : public active_queue_base<void, Mutex>
{
public:
	typedef active_queue_base<void, Mutex>   base_class;
	typedef typename base_class::return_type return_type;
	typedef typename base_class::binder_base_type binder_base_type;

	active_queue (size_t initial = 0
            , size_t max_capacity = pfs::max_value<size_t>()
            , size_t increment_factor = details::ring_queue::default_increment_factor)
        : base_class(initial, max_capacity, increment_factor) 
    {}

	return_type call ();
	return_type call_all ();
};

template <typename Return, typename Mutex>
typename active_queue<Return, Mutex>::return_type
	active_queue<Return, Mutex>::call ()
{
    unique_lock<Mutex> locker(this->_mutex);
	return_type r;
    
    details::ring_queue & rq = this->_queue;
    
	binder_base_type & bb = rq.front<binder_base_type>();

	locker.unlock();

	r = bb();

	locker.lock();

	bb.~binder_base_type();
	this->_queue.pop(bb.size());

	locker.unlock();

	return r;
}

template <typename Return, typename Mutex>
typename active_queue<Return, Mutex>::return_type
	active_queue<Return, Mutex>::call_all ()
{
	return_type r;

	while (!this->empty())
		r = call();
	return r;
}


template <typename Mutex>
typename active_queue<void, Mutex>::return_type active_queue<void, Mutex>::call ()
{
    //unique_lock<Mutex> locker(this->_mutex);
    lock_guard<Mutex> locker(this->_mutex);
    
	binder_base_type & bb = this->_queue.template front<binder_base_type>();

//	locker.unlock();

	bb();

//	locker.lock();

	bb.~binder_base_type();
	this->_queue.pop(bb.size());

//	locker.unlock();
}

template <typename Mutex>
inline typename active_queue<void, Mutex>::return_type active_queue<void, Mutex>::call_all ()
{
	while (!this->empty())
		call();
}

} // pfs

#endif /* __PFS_ACTIVE_QUEUE_HPP__ */
