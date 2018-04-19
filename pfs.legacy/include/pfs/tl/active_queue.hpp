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

#include <pfs/tl/ring_queue_pool.hpp>
#include <pfs/tl/binder.hpp>
#include <pfs/tl/trivial_lock_guard.hpp>

// struct Traits
// {
//     static  SizeType const default_increment_factor = 32; ///<< Default increment factor
//
//     typedef SizeType          size_type;            ///<< Size type like std::size_t
//     typedef AtomicType        atomic_type;          ///<< Atomic type like std::atomic_size_t since C++11
//     typedef MutexType         mutex_type;           ///<< Mutex type must be satisfied LockGuardType::mutex_type
//     typedef EmptyExceptioType empty_exception_type; ///<< Exception class satisfies DefaultConstructible concept
// };


namespace pfs {
namespace tl {

template <typename ReturnType, typename Traits>
class active_queue_base
{
protected:
    typedef ReturnType              return_type;
    typedef binder_base<ReturnType> binder_base_type;

    typedef Traits                                traits_type;
    typedef typename Traits::size_type            size_type;
    typedef typename Traits::atomic_type          atomic_type;
    typedef typename Traits::mutex_type           mutex_type;
    typedef typename Traits::empty_exception_type empty_exception_type;
    typedef ring_queue_pool<Traits>               queue_type;
    typedef trivial_lock_guard<mutex_type>        lock_guard_type;

protected:
	mutex_type _call_mutex;
    mutex_type _push_mutex;
    queue_type _queue;

public:
    static size_type const default_increment_factor = traits_type::default_increment_factor;
        
public:
	active_queue_base (size_type initial
            , size_type max_capacity
            , size_type increment_factor)
        : _queue(initial, max_capacity, increment_factor)
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

	bool push_function (return_type (* f) ())
	{
        lock_guard_type locker(_push_mutex);
        return _queue.template push<binder_function0<return_type> >(f);
	}

	template <typename Arg1>
	bool push_function (return_type (* f) (Arg1), Arg1 a1)
	{
        lock_guard_type locker(_push_mutex);
        return _queue.template push<binder_function1<return_type, Arg1> >(f, a1);
	}
    
	template <typename Arg1, typename Arg2>
	bool push_function (return_type (* f) (Arg1, Arg2), Arg1 a1, Arg2 a2)
	{
        lock_guard_type locker(_push_mutex);
        return _queue.template push<binder_function2<return_type, Arg1, Arg2> >(f, a1, a2);
	}

	template <typename Arg1, typename Arg2, typename Arg3>
	bool push_function (return_type (* f) (Arg1, Arg2, Arg3), Arg1 a1, Arg2 a2, Arg3 a3)
	{
        lock_guard_type locker(_push_mutex);
        return _queue.template push<binder_function3<return_type, Arg1, Arg2, Arg3> >(f, a1, a2, a3);
	}

  	template <typename Arg1, typename Arg2, typename Arg3, typename Arg4>
	bool push_function (return_type (* f) (Arg1, Arg2, Arg3, Arg4), Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4)
	{
        lock_guard_type locker(_push_mutex);
        return _queue.template push<binder_function4<return_type, Arg1, Arg2, Arg3, Arg4> >(f, a1, a2, a3, a4);
	}

  	template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
	bool push_function (return_type (* f) (Arg1, Arg2, Arg3, Arg4, Arg5), Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5)
	{
        lock_guard_type locker(_push_mutex);
        return _queue.template push<binder_function5<return_type, Arg1, Arg2, Arg3, Arg4, Arg5> >(f, a1, a2, a3, a4, a5);
	}

  	template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
	bool push_function (return_type (* f) (Arg1, Arg2, Arg3, Arg4, Arg5, Arg6), Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6)
	{
        lock_guard_type locker(_push_mutex);
        return _queue.template push<binder_function6<return_type, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6> >(f, a1, a2, a3, a4, a5, a6);
	}

  	template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
	bool push_function (return_type (* f) (Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7), Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7)
	{
        lock_guard_type locker(_push_mutex);
        return _queue.template push<binder_function7<return_type, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7> >(f, a1, a2, a3, a4, a5, a6, a7);
	}

  	template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8>
	bool push_function (return_type (* f) (Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8), Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8)
	{
        lock_guard_type locker(_push_mutex);
        return _queue.template push<binder_function8<return_type, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8> >(f, a1, a2, a3, a4, a5, a6, a7, a8);
	}
    
    template <typename C>
	bool push_method (C * c, return_type (C::* f) ())
	{
        lock_guard_type locker(_push_mutex);
        return _queue.template push<binder_method0<C, return_type> >(c, f);
	}

	template <typename C, typename Arg1>
	bool push_method (C * c, return_type (C::* f) (Arg1), Arg1 a1)
	{
        lock_guard_type locker(_push_mutex);
        return _queue.template push<binder_method1<C, return_type, Arg1> >(c, f, a1);
	}
    
	template <typename C, typename Arg1, typename Arg2>
	bool push_method (C * c, return_type (C::* f) (Arg1, Arg2), Arg1 a1, Arg2 a2)
	{
        lock_guard_type locker(_push_mutex);
        return _queue.template push<binder_method2<C, return_type, Arg1, Arg2> >(c, f, a1, a2);
	}

	template <typename C, typename Arg1, typename Arg2, typename Arg3>
	bool push_method (C * c, return_type (C::* f) (Arg1, Arg2, Arg3), Arg1 a1, Arg2 a2, Arg3 a3)
	{
        lock_guard_type locker(_push_mutex);
        return _queue.template push<binder_method3<C, return_type, Arg1, Arg2, Arg3> >(c, f, a1, a2, a3);
	}

  	template <typename C, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
	bool push_method (C * c, return_type (C::* f) (Arg1, Arg2, Arg3, Arg4), Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4)
	{
        lock_guard_type locker(_push_mutex);
        return _queue.template push<binder_method4<C, return_type, Arg1, Arg2, Arg3, Arg4> >(c, f, a1, a2, a3, a4);
	}

  	template <typename C, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
	bool push_method (C * c, return_type (C::* f) (Arg1, Arg2, Arg3, Arg4, Arg5), Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5)
	{
        lock_guard_type locker(_push_mutex);
        return _queue.template push<binder_method5<C, return_type, Arg1, Arg2, Arg3, Arg4, Arg5> >(c, f, a1, a2, a3, a4, a5);
	}

  	template <typename C, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
	bool push_method (C * c, return_type (C::* f) (Arg1, Arg2, Arg3, Arg4, Arg5, Arg6), Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6)
	{
        lock_guard_type locker(_push_mutex);
        return _queue.template push<binder_method6<C, return_type, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6> >(c, f, a1, a2, a3, a4, a5, a6);
	}

  	template <typename C, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
	bool push_method (C * c, return_type (C::* f) (Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7), Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7)
	{
        lock_guard_type locker(_push_mutex);
        return _queue.template push<binder_method7<C, return_type, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7> >(c, f, a1, a2, a3, a4, a5, a6, a7);
	}

  	template <typename C, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8>
	bool push_method (C * c, return_type (C::* f) (Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8), Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8)
	{
        lock_guard_type locker(_push_mutex);
        return _queue.template push<binder_method8<C, return_type, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8> >(c, f, a1, a2, a3, a4, a5, a6, a7, a8);
	}    
    
	void pop ();
};

template <typename ReturnType, typename RingQueueTraits>
void active_queue_base<ReturnType, RingQueueTraits>::pop ()
{
    lock_guard_type locker(_call_mutex);
	binder_base<return_type> & bb = _queue.template front<binder_base<return_type> >();
    
    bb.~binder_base_type();
	_queue.pop(bb.size());
}

template <typename ReturnType, typename Traits>
class active_queue : public active_queue_base<ReturnType, Traits>
{
protected:
	typedef active_queue_base<ReturnType, Traits> base_class;
	typedef typename base_class::return_type      return_type;
	typedef typename base_class::binder_base_type binder_base_type;
    typedef typename base_class::queue_type       queue_type;
    typedef typename base_class::size_type        size_type;
    typedef typename base_class::lock_guard_type  lock_guard_type;
    
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
        : base_class(initial, max_capacity, base_class::default_increment_factor)
	{}
    
   	active_queue (size_type max_capacity)
        : base_class(0, max_capacity, base_class::default_increment_factor)
	{}

	return_type call ();
	return_type call_all ();
};

template <typename Traits>
class active_queue<void, Traits> : public active_queue_base<void, Traits>
{
protected:
	typedef active_queue_base<void, Traits>       base_class;
	typedef typename base_class::return_type      return_type;
	typedef typename base_class::binder_base_type binder_base_type;
    typedef typename base_class::queue_type       queue_type;
    typedef typename base_class::size_type        size_type;
    typedef typename base_class::lock_guard_type  lock_guard_type;

public:
	active_queue (size_type initial
            , size_type max_capacity
            , size_type increment_factor)
        : base_class(initial, max_capacity, increment_factor)
    {}

   	active_queue (size_type initial, size_type max_capacity)
        : base_class(initial, max_capacity, base_class::default_increment_factor)
	{}
    
   	active_queue (size_type max_capacity)
        : base_class(0, max_capacity, base_class::default_increment_factor)
	{}

	return_type call ();
	return_type call_all ();
};

template <typename ReturnType, typename Traits>
typename active_queue<ReturnType, Traits>::return_type
	active_queue<ReturnType, Traits>::call ()
{
    lock_guard_type locker(this->_call_mutex);
    
	binder_base_type & bb = this->_queue.template front<binder_base_type>();
    
	return_type r = bb();

	bb.~binder_base_type();
	this->_queue.pop(bb.size());

	return r;
}

template <typename Traits>
typename active_queue<void, Traits>::return_type 
    active_queue<void, Traits>::call ()
{
    lock_guard_type locker(this->_call_mutex);

	binder_base_type & bb = this->_queue.template front<binder_base_type>();
    
	bb();
	bb.~binder_base_type();
	this->_queue.pop(bb.size());
}

template <typename ReturnType, typename Traits>
inline typename active_queue<ReturnType, Traits>::return_type
	active_queue<ReturnType, Traits>::call_all ()
{
	return_type r;

	while (!this->empty())
		r = call();
	return r;
}

template <typename Traits>
inline typename active_queue<void, Traits>::return_type 
    active_queue<void, Traits>::call_all ()
{
	while (!this->empty())
		call();
}

}} // pfs::tl

#endif /* __PFS_ACTIVE_QUEUE_HPP__ */
