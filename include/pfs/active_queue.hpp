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

#include <pfs/atomic.hpp>
#include <pfs/binder.hpp>
#include <pfs/mutex.hpp>

namespace pfs {

template <size_t Size, typename Return, typename Mutex = pfs::fake_mutex>
class active_queue_base
{
public:
	typedef Return return_type;
	typedef binder_base<Return> binder_base_type;

protected:
	Mutex  _mutex;
	char   _begin[Size];
	char * _end;
	char * _head;
	char * _tail;
	atomic_integer<size_t> _count;

protected:
	void pop (binder_base<Return> & fr);
	bool prepare_push (size_t frsize);

	void reset ()
	{
		_head = _begin;   // Move Head and Tail to the begin of queue
		_tail = _begin;
		_end  = _begin + Size;
	}

public:
	active_queue_base ()
		: _end(_begin + Size)
		, _head(_begin)
		, _tail(_begin)
		, _count(0)
	{}

	virtual ~active_queue_base ()
	{
		while (_head != _tail)
			pop();
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
		return Size;
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

template <size_t Size, typename Return, typename Mutex>
void active_queue_base<Size, Return, Mutex>::pop ()
{
	binder_base<Return> * fr = 0;

	pull(fr);

	if (fr)
		pop(*fr);
}

template <size_t Size, typename Return, typename Mutex>
bool active_queue_base<Size, Return, Mutex>::prepare_push (size_t frsize)
{
	if (_tail == _head) {
		if (empty()) {
			reset();   // Move Head and Tail to the begin of queue
		} else {              // Queue is full
			return false;
		}
	}

	if (_tail >= _head) {                   // Tail is at the right side of Head or Queue is empty (_tail == _head)
		char * end  = _begin + Size;

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

	// There is no enough space to push function
	return false;
}

template <size_t Size, typename Return, typename Mutex>
void active_queue_base<Size, Return, Mutex>::pull (binder_base_type * & fr)
{
	if (!empty()) {
		if (_head == _end) { // Head exceeds Tail, but Queue is not empty
			_head = _begin;
			_end = _begin + Size;
		}

		fr = reinterpret_cast<binder_base_type *>(_head);
	}
}


template <size_t Size, typename Return, typename Mutex>
void active_queue_base<Size, Return, Mutex>::pop (binder_base<Return> & fr)
{
	//lock_guard<Mutex> locker(_mutex);

#if __COMMENT__
	if (_head == _end) {        // Head exceeds logic End (supposed position is equivalent to real position)
		_head  = _begin;        // Need move Head to Begin
	}
#endif
	_head += fr.size(); // Supposed Head position
	_count.deref();     //--_count;

#if __COMMENT__
	if (_head == _tail && empty()) {  // Head exceeds Tail, need to optimize pointers
		reset();                      // Move Head and Tail to the begin of queue
	}
#endif

//	if (_head == _end) {        // Head exceeds logic End (supposed position is equivalent to real position)
//		_head  = _begin;        // Can move Head to Begin
////		_end   = _begin + Size; // Can move logic End to the real end of queue
//	} else if (_head > _end) {  // Head is at the right side if logic End (supposed position is not equivalent to real position)
////		_head = _begin;         // So binder really was placed from Begin
////		_head += fr.size();     // Move Head to real position (against supposed position)
//	}

	fr.~binder_base();
}

template <size_t Size, typename Return, typename Mutex = pfs::fake_mutex>
class active_queue : public active_queue_base<Size, Return, Mutex>
{
public:
	typedef active_queue_base<Size, Return, Mutex> base_class;
	typedef typename base_class::return_type return_type;
	typedef typename base_class::binder_base_type binder_base_type;

	active_queue () : base_class() {}

	return_type call ();
	return_type call_all ();
};

template <size_t Size, typename Mutex>
class active_queue<Size, void, Mutex> : public active_queue_base<Size, void, Mutex>
{
public:
	typedef active_queue_base<Size, void, Mutex> base_class;
	typedef typename base_class::return_type return_type;
	typedef typename base_class::binder_base_type binder_base_type;

	active_queue () : base_class() {}

	return_type call ();
	return_type call_all ();
};

template <size_t Size, typename Return, typename Mutex>
typename active_queue<Size, Return, Mutex>::return_type
	active_queue<Size, Return, Mutex>::call ()
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

template <size_t Size, typename Return, typename Mutex>
typename active_queue<Size, Return, Mutex>::return_type
	active_queue<Size, Return, Mutex>::call_all ()
{
	return_type r;

	while (!this->empty())
		r = call();
	return r;
}

template <size_t Size, typename Mutex>
typename active_queue<Size, void, Mutex>::return_type active_queue<Size, void, Mutex>::call ()
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

template <size_t Size, typename Mutex>
inline typename active_queue<Size, void, Mutex>::return_type active_queue<Size, void, Mutex>::call_all ()
{
	while (!this->empty())
		call();
}


} // pfs


#endif /* __PFS_ACTIVE_QUEUE_HPP__ */

