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

#ifndef __PFS_TL_ACTIVE_QUEUE_HPP__
#define __PFS_TL_ACTIVE_QUEUE_HPP__

#include <pfs/tl/ring_queue.hpp>
#include <pfs/tl/binder.hpp>

namespace pfs {
namespace tl {

//
// RingQueueTraits::move_functor_type must be active_queue_move_functor<RingQueueTraits::size_type>
//
template <typename ReturnType, typename RingQueueTraits>
class active_queue_base
{
protected:
    typedef ReturnType                                return_type;
    typedef binder_base<ReturnType>                   binder_base_type;

    struct move_functor
    {
        void * operator () (void * dest, void * src, size_t n)
        {
            return binder_base_type::move(dest, src, n);
        }
    };

    typedef ring_queue<RingQueueTraits, move_functor> ring_queue_type;
    typedef typename ring_queue_type::size_type       size_type;
    typedef typename ring_queue_type::lock_guard_type lock_guard_type;
    typedef typename ring_queue_type::mutex_type      mutex_type;

protected:
	mutex_type      _pop_mutex;
    ring_queue_type _queue;

public:
	active_queue_base (size_type initial
            , size_type max_capacity
            , size_type increment_factor)
        : _queue(_pop_mutex, initial, max_capacity, increment_factor)
	{}

	virtual ~active_queue_base ()
	{
		while (!_queue.empty())
			this->pop();
	}
    
	bool empty () const
	{
		return _queue.empty();
	}

	size_type count () const
	{
		return _queue.count();
	}

	size_type capacity () const
	{
		return _queue.capacity();
	}

	bool push_function (return_type (* f) ())
	{
        return _queue.template push<binder_function0<return_type> >(f);
	}

	template <typename Arg1>
	bool push_function (return_type (* f) (Arg1), Arg1 a1)
	{
        return _queue.template push<binder_function1<return_type, Arg1> >(f, a1);
	}
    
	template <typename Arg1, typename Arg2>
	bool push_function (return_type (* f) (Arg1, Arg2), Arg1 a1, Arg2 a2)
	{
        return _queue.template push<binder_function2<return_type, Arg1, Arg2> >(f, a1, a2);
	}

	template <typename Arg1, typename Arg2, typename Arg3>
	bool push_function (return_type (* f) (Arg1, Arg2, Arg3), Arg1 a1, Arg2 a2, Arg3 a3)
	{
        return _queue.template push<binder_function3<return_type, Arg1, Arg2, Arg3> >(f, a1, a2, a3);
	}

  	template <typename Arg1, typename Arg2, typename Arg3, typename Arg4>
	bool push_function (return_type (* f) (Arg1, Arg2, Arg3, Arg4), Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4)
	{
        return _queue.template push<binder_function4<return_type, Arg1, Arg2, Arg3, Arg4> >(f, a1, a2, a3, a4);
	}

  	template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
	bool push_function (return_type (* f) (Arg1, Arg2, Arg3, Arg4, Arg5), Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5)
	{
        return _queue.template push<binder_function5<return_type, Arg1, Arg2, Arg3, Arg4, Arg5> >(f, a1, a2, a3, a4, a5);
	}

  	template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
	bool push_function (return_type (* f) (Arg1, Arg2, Arg3, Arg4, Arg5, Arg6), Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6)
	{
        return _queue.template push<binder_function6<return_type, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6> >(f, a1, a2, a3, a4, a5, a6);
	}

  	template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
	bool push_function (return_type (* f) (Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7), Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7)
	{
        return _queue.template push<binder_function7<return_type, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7> >(f, a1, a2, a3, a4, a5, a6, a7);
	}

  	template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8>
	bool push_function (return_type (* f) (Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8), Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8)
	{
        return _queue.template push<binder_function8<return_type, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8> >(f, a1, a2, a3, a4, a5, a6, a7, a8);
	}
    
    template <typename C>
	bool push_method (C * c, return_type (C::* f) ())
	{
        return _queue.template push<binder_method0<C, return_type> >(c, f);
	}

	template <typename C, typename Arg1>
	bool push_method (C * c, return_type (C::* f) (Arg1), Arg1 a1)
	{
        return _queue.template push<binder_method1<C, return_type, Arg1> >(c, f, a1);
	}
    
	template <typename C, typename Arg1, typename Arg2>
	bool push_method (C * c, return_type (C::* f) (Arg1, Arg2), Arg1 a1, Arg2 a2)
	{
        return _queue.template push<binder_method2<C, return_type, Arg1, Arg2> >(c, f, a1, a2);
	}

	template <typename C, typename Arg1, typename Arg2, typename Arg3>
	bool push_method (C * c, return_type (C::* f) (Arg1, Arg2, Arg3), Arg1 a1, Arg2 a2, Arg3 a3)
	{
        return _queue.template push<binder_method3<C, return_type, Arg1, Arg2, Arg3> >(c, f, a1, a2, a3);
	}

  	template <typename C, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
	bool push_method (C * c, return_type (C::* f) (Arg1, Arg2, Arg3, Arg4), Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4)
	{
        return _queue.template push<binder_method4<C, return_type, Arg1, Arg2, Arg3, Arg4> >(c, f, a1, a2, a3, a4);
	}

  	template <typename C, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
	bool push_method (C * c, return_type (C::* f) (Arg1, Arg2, Arg3, Arg4, Arg5), Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5)
	{
        return _queue.template push<binder_method5<C, return_type, Arg1, Arg2, Arg3, Arg4, Arg5> >(c, f, a1, a2, a3, a4, a5);
	}

  	template <typename C, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
	bool push_method (C * c, return_type (C::* f) (Arg1, Arg2, Arg3, Arg4, Arg5, Arg6), Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6)
	{
        return _queue.template push<binder_method6<C, return_type, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6> >(c, f, a1, a2, a3, a4, a5, a6);
	}

  	template <typename C, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
	bool push_method (C * c, return_type (C::* f) (Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7), Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7)
	{
        return _queue.template push<binder_method7<C, return_type, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7> >(c, f, a1, a2, a3, a4, a5, a6, a7);
	}

  	template <typename C, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8>
	bool push_method (C * c, return_type (C::* f) (Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8), Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8)
	{
        return _queue.template push<binder_method8<C, return_type, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8> >(c, f, a1, a2, a3, a4, a5, a6, a7, a8);
	}    
    
	void pop ();
};

template <typename ReturnType, typename RingQueueTraits>
void active_queue_base<ReturnType, RingQueueTraits>::pop ()
{
    lock_guard_type locker(_pop_mutex);
	binder_base<return_type> & bb = _queue.template front<binder_base<return_type> >();
    
    bb.~binder_base_type();
	_queue.pop(bb.size());
}

template <typename ReturnType, typename RingQueueTraits>
class active_queue : public active_queue_base<ReturnType, RingQueueTraits>
{
protected:
	typedef active_queue_base<ReturnType, RingQueueTraits> base_class;
	typedef typename base_class::return_type           return_type;
	typedef typename base_class::binder_base_type      binder_base_type;
    typedef typename base_class::ring_queue_type       ring_queue_type;
    typedef typename base_class::size_type             size_type;
    typedef typename base_class::lock_guard_type       lock_guard_type;

public:
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
	active_queue (size_type initial
            , size_type max_capacity
            , size_type increment_factor)
        : base_class(initial, max_capacity, increment_factor)
    {}

   	active_queue (size_type initial, size_type max_capacity)
        : base_class(initial, max_capacity, ring_queue_type::default_increment_factor)
	{}
    
   	active_queue (size_type max_capacity)
        : base_class(0, max_capacity, ring_queue_type::default_increment_factor)
	{}

	return_type call ();
	return_type call_all ();
};

template <typename RingQueueTraits>
class active_queue<void, RingQueueTraits> : public active_queue_base<void, RingQueueTraits>
{
protected:
	typedef active_queue_base<void, RingQueueTraits>   base_class;
	typedef typename base_class::return_type           return_type;
	typedef typename base_class::binder_base_type      binder_base_type;
    typedef typename base_class::ring_queue_type       ring_queue_type;
    typedef typename base_class::size_type             size_type;
    typedef typename base_class::lock_guard_type       lock_guard_type;

public:
	active_queue (size_type initial
            , size_type max_capacity
            , size_type increment_factor)
        : base_class(initial, max_capacity, increment_factor)
    {}

   	active_queue (size_type initial, size_type max_capacity)
        : base_class(initial, max_capacity, ring_queue_type::default_increment_factor)
	{}
    
   	active_queue (size_type max_capacity)
        : base_class(0, max_capacity, ring_queue_type::default_increment_factor)
	{}

	return_type call ();
	return_type call_all ();
};

template <typename ReturnType, typename RingQueueTraits>
typename active_queue<ReturnType, RingQueueTraits>::return_type
	active_queue<ReturnType, RingQueueTraits>::call ()
{
    lock_guard_type locker(this->_pop_mutex);
    
	binder_base_type & bb = this->_queue.template front<binder_base_type>();
    
	return_type r = bb();

	bb.~binder_base_type();
	this->_queue.pop(bb.size());

	return r;
}

template <typename RingQueueTraits>
typename active_queue<void, RingQueueTraits>::return_type 
    active_queue<void, RingQueueTraits>::call ()
{
    lock_guard_type locker(this->_pop_mutex);

	binder_base_type & bb = this->_queue.template front<binder_base_type>();
    
	bb();
	bb.~binder_base_type();
	this->_queue.pop(bb.size());
}

template <typename ReturnType, typename RingQueueTraits>
inline typename active_queue<ReturnType, RingQueueTraits>::return_type
	active_queue<ReturnType, RingQueueTraits>::call_all ()
{
	return_type r;

	while (!this->empty())
		r = call();
	return r;
}

template <typename RingQueueTraits>
inline typename active_queue<void, RingQueueTraits>::return_type 
    active_queue<void, RingQueueTraits>::call_all ()
{
	while (!this->empty())
		call();
}

}} // pfs::tl

#endif /* __PFS_ACTIVE_QUEUE_HPP__ */
