/*
 * threadls_unix.cpp
 *
 *  Created on: Sep 19, 2013
 *      Author: wladt
 */


#include <pfs/vector.hpp>
#include <pfs/threadls.hpp>
#include "thread_unix.hpp" // for CWT_HAVE_TLS

namespace pfs {
// FIXME Implement these !

thread::tls_implementation_type thread::tls_implementation ()
{
#ifdef CWT_HAVE_TLS
	return thread::TlsCompilerSpecific;
#else
	return thread::TlsPosixThreads;
#endif
}

#ifdef __COMMENT__
static Mutex destructorsMutex;
typedef Vector<void (*)(void *)> DestructorMap;

static DestructorMap * destructors()
{
	static DestructorMap staticVariable;
	static atomic_pointer<DestructorMap> pointer;
	return pointer;
}


ThreadStorageData::ThreadStorageData(void (* cleanup)(void *))
{
    AutoLock<> locker(& destructorsMutex);
    DestructorMap * destr = destructors();

    if (! destr) {
        /*
         the destructors vector has already been destroyed, yet a new
         ThreadStorage is being allocated. this can only happen during global
         destruction, at which point we assume that there is only one thread.
         in order to keep ThreadStorage working, we need somewhere to store
         the data, best place we have in this situation is at the tail of the
         current thread's tls vector. the destructor is ignored, since we have
         no where to store it, and no way to actually call it.
         */
        ThreadData * data = ThreadData::current();
        id = data->tls.count();
        DEBUG_MSG("ThreadStorageData: Allocated id %d, destructor %p cannot be stored", id, func);
        return;
    }
    for (id = 0; id < destr->count(); id++) {
        if (destr->at(id) == 0)
            break;
    }
    if (id == destr->count()) {
        destr->append(func);
    } else {
        (*destr)[id] = func;
    }
    //DEBUG_MSG("ThreadStorageData: Allocated id %d, destructor %p", id, func);
}



//struct future_object_base;
//struct tss_cleanup_function;
//struct thread_exit_callback_node;

struct tss_data_node
{
    shared_ptr<ThreadLS::cleanup_functor> m_func;
    void * m_value;

    tss_data_node (shared_ptr<ThreadLS::cleanup_functor> func, void * value)
        : m_func(func)
        , m_value(value)
    {}
};

#endif

} // pfs
