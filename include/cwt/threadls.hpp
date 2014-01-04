/**
 * threadls.hpp
 *
 *  Created on: Sep 19, 2013
 *      Author: wladt
 *
 * @brief Implements Thread Local Storage
 */

#ifndef __CWT_THREADLS_HPP__
#define __CWT_THREADLS_HPP__

#include <pfs/shared_ptr.hpp>

namespace cwt {

// FIXME Implement


template <typename T>
class DLL_API thread_storage
{
	PFS_DENY_COPY(thread_storage);

public:

	#ifdef __COMMENT__
private:
	struct cleanup_functor
	{
		virtual ~cleanup_functor ()
		{}

		virtual void operator () (void * data) = 0;
	};

    struct default_deleter : cleanup_functor
    {
        void operator () (void * data)
        {
            delete static_cast<T *>(data);
        }
    };

    struct custom_deleter : cleanup_functor
    {
        void (* cleanup_function)(T *);

        explicit custom_deleter (void (* cleanup_func)(T *)):
            cleanup_function(cleanup_func)
        {}

        void operator () (void * data)
        {
            cleanup_function(static_cast<T*>(data));
        }
    };


    shared_ptr<cleanup_functor> cleanup;

public:
    static void setData (const void * key, shared_ptr<cleanup_functor> func, void * tls_data, bool cleanup_existing);
    static void * getData (const void * key);

public:
    typedef T element_type;

    ThreadLS () : cleanup(new default_deleter) {}
    explicit ThreadLS (void (* func_ptr)(T *))
    {
        if (func_ptr) {
            cleanup.reset(new custom_deleter(func_ptr));
        }
    }

    ~ThreadLS()
    {
    	setData(this, shared_ptr<cleanup_functor>(), 0, true);
    }

    T * get () const { return static_cast<T *>(getData(this)); }
    T * operator-> () const  { return get(); }
    T & operator * () const    { return *get(); }
    T * release ()
    {
        const T * tmp = get();
        setData(this, shared_ptr<cleanup_functor>(), 0, false);
        return tmp;
    }

    void reset (T * new_value = 0)
    {
        const T * const current_value = get();
        if (current_value != new_value) {
            setData(this, cleanup, new_value, true);
        }
    }
#endif
};

} // cwt

#endif /* __CWT_THREADLS_HPP__ */
