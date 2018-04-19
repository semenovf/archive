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

// http://en.wikipedia.org/wiki/Thread-local_storage
enum TlsImplementation {
	  TlsPthreads = 1
	, TlsPosixThreads = TlsPthreads
	, TlsWin32
	, TlsCompilerSpecific // using '__thread' keyword for GCC (gcc.gnu.org/onlinedocs/gcc-3.3.1/gcc/Thread-Local.html)
	                      // or '__declspec(thread)' keywords for MSC (http://msdn.microsoft.com/en-us/library/6yh4a9k1.aspx)
};

extern TlsImplementation cwt_tls_implementation();

// FIXME Implement

template <typename T>
class DLL_API ThreadLS
{
	CWT_DENY_COPY(ThreadLS);

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

CWT_NS_END

#endif /* __CWT_THREADLS_HPP__ */
