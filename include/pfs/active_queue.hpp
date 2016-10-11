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

#ifndef __PFS_ACTIVE_QUEUE_HPP__
#define __PFS_ACTIVE_QUEUE_HPP__

#include <cstring> // for std::memcpy
#include <pfs/atomic.hpp>
#include <pfs/binder.hpp>
#include <pfs/mutex.hpp>

namespace pfs {

template <typename Return, typename Mutex = pfs::fake_mutex>
class active_queue_base
{
public:
	typedef Return return_type;
	typedef binder_base<Return> binder_base_type;

protected:
	Mutex  _mutex;
	char * _begin;
	char * _end;
	char * _head;
	char * _tail;
	atomic_integer<size_t> _count;
    atomic_integer<size_t> _capacity;
    atomic_integer<size_t> _max_capacity;
    size_t _increment_factor;

protected:
	void pop (binder_base<Return> & fr);
	bool prepare_push (size_t frsize);

	void reset ()
	{
		_head = _begin;   // Move Head and Tail to the begin of queue
		_tail = _begin;
		_end  = _begin + _capacity.load();
	}

public:
	active_queue_base (size_t initial, size_t max_capacity, size_t increment_factor)
        : _begin(initial > 0 ? new char[initial] : 0)
		, _end(_begin + initial)
		, _head(_begin)
		, _tail(_begin)
		, _count(0)
        , _capacity(initial)
        , _max_capacity(max_capacity)
        , _increment_factor(increment_factor)
	{}

   	active_queue_base (size_t initial)
        : _begin(initial > 0 ? new char[initial] : 0)
		, _end(_begin + initial)
		, _head(_begin)
		, _tail(_begin)
		, _count(0)
        , _capacity(initial)
        , _max_capacity(pfs::max_value<size_t>())
        , _increment_factor(10)
	{}

   	active_queue_base ()
        : _begin(0)
		, _end(_begin)
		, _head(_begin)
		, _tail(_begin)
		, _count(0)
        , _capacity(0)
        , _max_capacity(pfs::max_value<size_t>())
        , _increment_factor(10)
	{}

	virtual ~active_queue_base ()
	{
		while (_head != _tail)
			pop();
        delete [] _begin;
	}

	bool empty () const
	{
		return _count.load() == 0;
	}

	size_t count () const
	{
		return _count.load();
	}

	size_t capacity () const
	{
		return _capacity.load();
	}

	bool push_function (return_type (* f) ())
	{
		lock_guard<Mutex> locker(_mutex);

		if (prepare_push(sizeof(binder_function0<return_type>))) {
			binder_base_type * fr = new (_tail) binder_function0<return_type>(f);
			_tail += fr->size();
			_count.ref(); //++_count;
			return true;
		}
		return false;
	}

	template <typename Class>
	bool push_method (Class * p, return_type (Class::* f) ())
	{
		lock_guard<Mutex> locker(_mutex);

		if (prepare_push(sizeof(binder_method0<Class, return_type>))) {
			binder_base_type * fr = new (_tail) binder_method0<Class, return_type>(p, f);
			_tail += fr->size();
			_count.ref(); //++_count;
			return true;
		}
		return false;
	}

	template <typename Arg1>
	bool push_function (return_type (*f) (Arg1), Arg1 a1)
	{
		lock_guard<Mutex> locker(_mutex);

		if (prepare_push(sizeof(binder_function1<return_type, Arg1>))) {
			binder_base_type * fr = new (_tail) binder_function1<return_type, Arg1>(f, a1);
			_tail += fr->size();
			_count.ref(); //++_count;
			return true;
		}
		return false;
	}

	template <typename Class, typename Arg1>
	bool push_method (Class * p, return_type (Class::*f) (Arg1), Arg1 a1)
	{
		lock_guard<Mutex> locker(_mutex);

		if (prepare_push(sizeof(binder_method1<Class, return_type, Arg1>))) {
			binder_base_type * fr = new (_tail) binder_method1<Class, return_type, Arg1>(p, f, a1);
			_tail += fr->size();
			_count.ref(); //++_count;
			return true;
		}
		return false;
	}

	template <typename Arg1, typename Arg2>
	bool push_function (return_type (*f) (Arg1, Arg2), Arg1 a1, Arg2 a2)
	{
		lock_guard<Mutex> locker(_mutex);

		if (prepare_push(sizeof(binder_function2<return_type, Arg1, Arg2>))) {
			binder_base_type * fr = new (_tail) binder_function2<return_type, Arg1, Arg2>(f, a1, a2);
			_tail += fr->size();
			_count.ref(); //++_count;
			return true;
		}
		return false;
	}

	template <typename Class, typename Arg1, typename Arg2>
	bool push_method (Class * p, return_type (Class::*f) (Arg1, Arg2), Arg1 a1, Arg2 a2)
	{
		lock_guard<Mutex> locker(_mutex);

		if (prepare_push(sizeof(binder_method2<Class, return_type, Arg1, Arg2>))) {
			binder_base_type * fr = new (_tail) binder_method2<Class, return_type, Arg1, Arg2>(p, f, a1, a2);
			_tail += fr->size();
			_count.ref(); //++_count;
			return true;
		}
		return false;
	}

	template <typename Arg1, typename Arg2, typename Arg3>
	bool push_function (return_type (*f) (Arg1, Arg2, Arg3), Arg1 a1, Arg2 a2, Arg3 a3)
	{
		lock_guard<Mutex> locker(_mutex);

		if (prepare_push(sizeof(binder_function3<return_type, Arg1, Arg2, Arg3>))) {
			binder_base_type * fr = new (_tail) binder_function3<return_type, Arg1, Arg2, Arg3>(f, a1, a2, a3);
			_tail += fr->size();
			_count.ref(); //++_count;
			return true;
		}
		return false;
	}

	template <typename Class, typename Arg1, typename Arg2, typename Arg3>
	bool push_method (Class * p, return_type (Class::*f) (Arg1, Arg2, Arg3), Arg1 a1, Arg2 a2, Arg3 a3)
	{
		lock_guard<Mutex> locker(_mutex);

		if (prepare_push(sizeof(binder_method3<Class, return_type, Arg1, Arg2, Arg3>))) {
			binder_base_type * fr = new (_tail) binder_method3<Class, return_type, Arg1, Arg2, Arg3>(p, f, a1, a2, a3);
			_tail += fr->size();
			_count.ref(); //++_count;
			return true;
		}
		return false;
	}

	template <typename Arg1, typename Arg2, typename Arg3, typename Arg4>
	bool push_function (return_type (*f) (Arg1, Arg2, Arg3, Arg4), Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4)
	{
		lock_guard<Mutex> locker(_mutex);

		if (prepare_push(sizeof(binder_function4<return_type, Arg1, Arg2, Arg3, Arg4>))) {
			binder_base_type * fr = new (_tail) binder_function4<return_type, Arg1, Arg2, Arg3, Arg4>(f, a1, a2, a3, a4);
			_tail += fr->size();
			_count.ref(); //++_count;
			return true;
		}
		return false;
	}

	template <typename Class, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
	bool push_method (Class * p, return_type (Class::*f) (Arg1, Arg2, Arg3, Arg4), Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4)
	{
		lock_guard<Mutex> locker(_mutex);

		if (prepare_push(sizeof(binder_method4<Class, return_type, Arg1, Arg2, Arg3, Arg4>))) {
			binder_base_type * fr = new (_tail) binder_method4<Class, return_type, Arg1, Arg2, Arg3, Arg4>(p, f, a1, a2, a3, a4);
			_tail += fr->size();
			_count.ref(); //++_count;
			return true;
		}
		return false;
	}

	template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
	bool push_function (return_type (*f) (Arg1, Arg2, Arg3, Arg4, Arg5), Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5)
	{
		lock_guard<Mutex> locker(_mutex);

		if (prepare_push(sizeof(binder_function5<return_type, Arg1, Arg2, Arg3, Arg4, Arg5>))) {
			binder_base_type * fr = new (_tail) binder_function5<return_type, Arg1, Arg2, Arg3, Arg4, Arg5>(f, a1, a2, a3, a4, a5);
			_tail += fr->size();
			_count.ref(); //++_count;
			return true;
		}
		return false;
	}

	template <typename Class, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
	bool push_method (Class * p, return_type (Class::*f) (Arg1, Arg2, Arg3, Arg4, Arg5), Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5)
	{
		lock_guard<Mutex> locker(_mutex);

		if (prepare_push(sizeof(binder_method5<Class, return_type, Arg1, Arg2, Arg3, Arg4, Arg5>))) {
			binder_base_type * fr = new (_tail) binder_method5<Class, return_type, Arg1, Arg2, Arg3, Arg4, Arg5>(p, f, a1, a2, a3, a4, a5);
			_tail += fr->size();
			_count.ref(); //++_count;
			return true;
		}
		return false;
	}

	void pop ();

protected:
	void pull (binder_base_type * & fr);
};

template <typename Return, typename Mutex>
void active_queue_base<Return, Mutex>::pop ()
{
	binder_base<Return> * fr = 0;

	pull(fr);

	if (fr)
		pop(*fr);
}

template <typename Return, typename Mutex>
bool active_queue_base<Return, Mutex>::prepare_push (size_t frsize)
{
    if (! _begin) {
        size_t capacity = _increment_factor * frsize;
        _begin = new char[capacity];
        _end = _begin + capacity;
        _head = _begin;
        _tail = _begin;
        return true;
    }
    
    if (_tail == _head && empty()) {
        reset();   // Move Head and Tail to the begin of queue
    }

    if (_tail == _head && ! empty()) {      // Queue is full
        ;
    } if (_tail >= _head) {                 // Tail is at the right side of Head or Queue is empty (_tail == _head)
        char * end  = _begin + _capacity.load();

        if (_tail + frsize <= end) {        // There is enough space before the real end of queue
            _end = end;                     // Logic End must be moved to real end of queue
            return true;
        } else {                            // There is no enough space before the real end of queue
            end  = _tail;                   // Save Tail position

            if (_begin + frsize <= _head) { // There is enough space before Head from Begin
                _tail = _begin;             // Move Tail to Begin
                _end = end;                 // Move Logic End
                return true;
            }
        }
    } else {                                // Tail is at the left side of Head
        if (_tail + frsize <= _head) {      // There is enough space before Head
            return true;
        }
    }

    //
	// There is no enough space to push function
    //
    
    size_t capacity = _capacity.load();
    size_t increment = _increment_factor * frsize;
            
    // Check for overflow
    //
    if (_max_capacity.load() - increment > capacity) {
        capacity += increment;
        char * begin = new char[capacity];
        
        if (_tail >= _head) {
            //
            //          |----n----|              |----n----|
            //    ------------------------       ------------------------------
            //    | | | |x|x|x|x|x| | | |   -->  |x|x|x|x|x| | | | | | | | | |
            //    ------------------------       ------------------------------
            //     ^     ^         ^     ^        ^         ^                 ^
            //     |     |         |     |        |         |                 |
            //  _begin _head     _tail _end    _begin     _tail             _end
            //                                 _head
            //
            size_t n = _tail - _head;
            std::memcpy(begin, _head, n);
            _head = begin;
            _tail = begin + n;
        } else {
            //
            //    |n1-|         |-n2--|        |n1-|                   |-n2--|
            //    ----------------------       --------------------------------
            //    |x|x| | | | | |x|x|x|   -->  |x|x| | | | | | | | | | |x|x|x|
            //    ----------------------       --------------------------------
            //     ^   ^         ^     ^        ^   ^                   ^     ^
            //     |   |         |     |        |   |                   |     |
            // _begin _tail  _head  _end    _begin _tail             _head  _end
            //
            size_t n1 = _tail - _begin;
            size_t n2 = _end - _head;
            
            std::memcpy(begin, _begin, n1);
            std::memcpy(begin + capacity - n2, _head, n2);
            
            _head = begin + capacity - n2;
            _tail = begin + n1;
        }
        
        delete [] _begin;
        
      	_begin = begin;
        _end = _begin + capacity;
        _capacity.store(capacity);
        
        return true;
    }
    
	return false;
}

template <typename Return, typename Mutex>
void active_queue_base<Return, Mutex>::pull (binder_base_type * & fr)
{
	if (!empty()) {
		if (_head == _end) { // Head exceeds Tail, but Queue is not empty
			_head = _begin;
			_end = _begin + _capacity.load();
		}

		fr = reinterpret_cast<binder_base_type *>(_head);
	}
}

template <typename Return, typename Mutex>
void active_queue_base<Return, Mutex>::pop (binder_base<Return> & fr)
{
	_head += fr.size(); // Supposed Head position
	_count.deref();     //--_count;
	fr.~binder_base();
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
	active_queue (size_t initial, size_t max_capacity, size_t increment_factor)
        : base_class(initial, max_capacity, increment_factor)
    {}

    /**
     * @brief Constructs instance of @c active_queue with the @a initial capacity,
     *        unlimited maximum capacity (actually this value is a maximum value 
     *        of @c size_t) and increment factor equals to 10. 
     * 
     * @param initial Initial capacity of internal buffer in bytes.
     */
  	active_queue (size_t initial)
        : base_class(initial)
    {}

    /**
     * @brief Constructs instance of empty @c active_queue,
     *        unlimited maximum capacity (actually this value is a maximum value 
     *        of @c size_t) and increment factor equals to 10. 
     */
   	active_queue ()
        : base_class()
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

	active_queue (size_t initial, size_t max_capacity, size_t increment_factor)
        : base_class(initial, max_capacity, increment_factor) 
    {}

  	active_queue (size_t initial)
        : base_class(initial)
    {}
    
   	active_queue ()
        : base_class()
    {}


	return_type call ();
	return_type call_all ();
};

template <typename Return, typename Mutex>
typename active_queue<Return, Mutex>::return_type
	active_queue<Return, Mutex>::call ()
{
	return_type r;
	binder_base<Return> * fr = 0;

	unique_lock<Mutex> locker(this->_mutex);

	this->pull(fr);

	locker.unlock();

	// To avoid this assert need to check for empty of queue before this call
	PFS_ASSERT(fr);

	r = (*fr)();

	locker.lock();
	this->pop(*fr);
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
	binder_base<void> * fr = 0;

	unique_lock<Mutex> locker(this->_mutex);

	this->pull(fr);

	locker.unlock();

	// To avoid this assert need to check for empty of queue before this call
	PFS_ASSERT(fr);

	(*fr)();

	locker.lock();
	this->pop(*fr);
	locker.unlock();
}

template <typename Mutex>
inline typename active_queue<void, Mutex>::return_type active_queue<void, Mutex>::call_all ()
{
	while (!this->empty())
		call();
}


} // pfs


#endif /* __PFS_ACTIVE_QUEUE_HPP__ */

