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

#include <cwt/shared_ptr.hpp>

CWT_NS_BEGIN

struct cleanup_functor
{
	virtual ~cleanup_functor ()
	{}

	virtual void operator () (void * data) = 0;
};

DLL_API void cwt_set_tls_data (void const * key, shared_ptr<cleanup_functor> func, void * tss_data, bool cleanup_existing);
DLL_API void * cwt_get_tls_data (void const * key);

template <typename T>
class ThreadLS
{
	CWT_DENY_COPY(ThreadLS);

private:
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
    	cwt_set_tls_data(this, shared_ptr<cleanup_functor>(), 0, true);
    }

    T * get () const { return static_cast<T *>(cwt_get_tls_data(this)); }
    T * operator-> () const  { return get(); }
    T & operator * () const    { return *get(); }
    T * release ()
    {
        const T * tmp = get();
        cwt_set_tls_data(this, shared_ptr<cleanup_functor>(), 0, false);
        return tmp;
    }

    void reset (T * new_value = 0)
    {
        const T * const current_value = get();
        if (current_value != new_value) {
            cwt_set_tls_data(this, cleanup, new_value, true);
        }
    }
};


CWT_NS_END

#endif /* __CWT_THREADLS_HPP__ */
