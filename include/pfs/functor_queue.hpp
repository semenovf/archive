/*
 * functor_queue.hpp
 *
 *  Created on: Nov 16, 2015
 *      Author: wladt
 */

#ifndef __PFS_FUNCTOR_QUEUE_HPP__
#define __PFS_FUNCTOR_QUEUE_HPP__

#include <pfs/functor.hpp>

namespace pfs {

template <size_t Size, typename Return>
class functor_queue_base
{
public:
	typedef Return return_type;
	typedef functor_base<Return> functor_base_type;

protected:
	char   _q[Size];
	char * _begin;
	char * _end;
	char * _head;
	char * _tail;
	size_t _count;

protected:
	void pop (functor_base<Return> * fr);
	bool prepare_push (size_t frsize);

public:
	functor_queue_base ()
		: _begin(_q)
		, _end(_begin + Size)
		, _head(_begin)
		, _tail(_begin)
		, _count(0)
	{}

	virtual ~functor_queue_base ()
	{
		while (_head != _tail)
			pop();
	}

	bool empty () const
	{
		return _count == 0;
	}

	bool count () const
	{
		return _count;
	}

	size_t capacity () const
	{
		return Size;
	}

	bool push (return_type (* f) ())
	{
		if (prepare_push(sizeof(functor0<return_type>))) {
			functor_base_type * fr = new (_tail) functor0<return_type>(f);
			_tail += fr->size();
			return true;
		}
		return false;
	}

	template <typename Arg1>
	bool push (return_type (*f) (Arg1), Arg1 a1)
	{
		if (prepare_push(sizeof(functor1<return_type, Arg1>))) {
			functor_base_type * fr = new (_tail) functor1<return_type, Arg1>(f, a1);
			_tail += fr->size();
			return true;
		}
		return false;
	}

	template <typename Arg1, typename Arg2>
	bool push (return_type (*f) (Arg1, Arg2), Arg1 a1, Arg2 a2)
	{
		if (prepare_push(sizeof(functor2<return_type, Arg1, Arg2>))) {
			functor_base_type * fr = new (_tail) functor2<return_type, Arg1, Arg2>(f, a1, a2);
			_tail += fr->size();
			return true;
		}
		return false;
	}

	void pull (functor_base_type * & fr)
	{
		fr = 0;
		if (!empty()) {
			 fr = reinterpret_cast<functor_base_type *>(_head);
		}
	}

//	return_type call () = 0;
//	return_type call_all () = 0;
	void pop ();
};

template <size_t Size, typename Return>
void functor_queue_base<Size, Return>::pop ()
{
	functor_base<Return> * fr;

	pull(fr);

	if (fr)
		pop(fr);
}

template <size_t Size, typename Return>
bool functor_queue_base<Size, Return>::prepare_push (size_t frsize)
{
	if (_tail >= _head) {                   // Tail is at the right side of Head
		char * end  = _begin + Size;

		if (_tail + frsize <= end) {        // There is enough space before the real end of queue
			_end = end;                     // Logic End must be moved to real end of queue
			++_count;
			return true;
		} else {                            // There is no enough space before the real end of queue
			end  = _tail;                   // Save Tail position

			if (_begin + frsize <= _head) { // There is enough space before Head from Begin
				_tail = _begin;             // Move Tail to Begin
				_end = end;                 // Move Logic End
				++_count;
				return true;
			}
		}
	} else {                                // Tail is at the left side of Head
		if (_tail + frsize <= _head) {      // There is enough space before Head
			++_count;
			return true;
		}
	}

	// Threre is no enough space to push function
	return false;
}

template <size_t Size, typename Return>
void functor_queue_base<Size, Return>::pop (functor_base<Return> * fr)
{
	if (fr) {
    	fr->~functor_base();
    	_head += fr->size();        // Supposed Head position

    	if (_head == _end) {        // Head exceeds login End (supposed position is equivalent to real position)
    		_head  = _begin;        // Can move Head to Begin
    		_end   = _begin + Size; // Can move logic End to the real end of queue
    	} else if (_head > _end) {  // Head is at the right side if logic End (supposed position is not equivalent to real position)
    		_head = _begin;         // So functor really was placed from Begin
    		_head += fr->size();    // Move Head to real position (against supposed position)
    	}

    	if (_head == _tail) {       // Head exceeds Tail, need to optimize pointers
    		_head = _begin;         // Move Head and Tail to the begin of queue
    		_tail = _begin;
    	}

    	--_count;
	}
}

template <size_t Size, typename Return>
class functor_queue : public functor_queue_base<Size, Return>
{
public:
	typedef functor_queue_base<Size, Return> base_class;
	typedef typename base_class::return_type return_type;
	typedef typename base_class::functor_base_type functor_base_type;

	functor_queue () : base_class() {}

	return_type call ();
	return_type call_all ();
};

template <size_t Size>
class functor_queue<Size, void> : public functor_queue_base<Size, void>
{
public:
	typedef functor_queue_base<Size, void> base_class;
	typedef typename base_class::return_type return_type;
	typedef typename base_class::functor_base_type functor_base_type;

	functor_queue () : base_class() {}

	return_type call ();
	return_type call_all ();
};

template <size_t Size, typename Return>
typename functor_queue<Size, Return>::return_type functor_queue<Size, Return>::call ()
{
	return_type r;
	functor_base<Return> * fr;

	this->pull(fr);

	if (fr) {
		r = (*fr)();
		this->pop(fr);
	}

	return r;
}

template <size_t Size, typename Return>
typename functor_queue<Size, Return>::return_type functor_queue<Size, Return>::call_all ()
{
	return_type r;

	while (!this->empty())
		r = call();
	return r;
}

template <size_t Size>
typename functor_queue<Size, void>::return_type functor_queue<Size, void>::call ()
{
	functor_base<void> * fr;

	this->pull(fr);

	if (fr) {
		(*fr)();
		this->pop(fr);
	}
}

template <size_t Size>
inline typename functor_queue<Size, void>::return_type functor_queue<Size, void>::call_all ()
{
	while (!this->empty())
		call();
}


} // pfs

#endif /* __PFS_FUNCTOR_QUEUE_HPP__ */
