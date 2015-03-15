/*
 * thread_win.hpp
 *
 *  Created on: May 4, 2014
 *      Author: wladt
 */

#ifndef __PFS_THREAD_WIN_HPP__
#define __PFS_THREAD_WIN_HPP__

#include "../thread_p.hpp"
#include <cerrno>
#include <ctime>

// TODO Check for Windows
//#if defined(_POSIX_THREAD_PRIORITY_SCHEDULING) && (_POSIX_THREAD_PRIORITY_SCHEDULING-0 >= 0)
//#	define CWT_HAVE_THREAD_PRIORITY_SCHEDULING
//#endif

// TODO add new supporting platforms
// TODO Uncomment after pthread's implementation of thread local data has been tested.
//#if defined(PFS_CC_MSVC)
//#	define CWT_HAVE_TLS
//#endif

namespace pfs {

// @see http://msdn.microsoft.com/en-us/library/ms686991%28v=vs.85%29.aspx
// @see http://msdn.microsoft.com/en-us/library/ms686997%28v=vs.85%29.aspx

struct thread::data
{
	data ();
	~data ();

	static void set (void * data);
	static void * get ();
	static void clear ();
//    static void createKey ();
    static void destroy (void * /*pdata*/);

#ifdef CWT_HAVE_TLS
    static __declspec(thread) void * currentThreadData;
#else
    static DWORD tlsIndex;
#endif

//	pfs::shared_ptr<thread::impl> threadImpl;
//	pthread_t threadId;
};

inline void thread::data::set (void * data)
{
#ifdef CWT_HAVE_TLS
    currentThreadData = data;
#else
    tlsIndex = TlsAlloc();
    if (tlsIndex == TLS_OUT_OF_INDEXES) {
    	PFS_ERROR("TlsAlloc() error");
    	return;
    }

    if (TlsSetValue(tlsIndex, data) == 0) {
    	PFS_ERROR("TlsSetValue() error");
    	TlsFree(tlsIndex);
    	tlsIndex = TLS_OUT_OF_INDEXES;
    }
#endif
}

// Retrieve a data pointer for the current thread.
inline void * thread::data::get ()
{
#ifdef CWT_HAVE_TLS
    return currentThreadData;
#else
    PFS_ASSERT(tlsIndex != TLS_OUT_OF_INDEXES); // not set
    void * r = TlsGetValue(tlsIndex);
    PFS_VERIFY(!((r == nullptr) && (GetLastError() != ERROR_SUCCESS)));
    return r;
#endif
}

inline void thread::data::clear ()
{
#ifdef CWT_HAVE_TLS
    currentThreadData = nullptr;
#else
    if (tlsIndex != TLS_OUT_OF_INDEXES) {
		if (TlsSetValue(tlsIndex, nullptr) == 0) {
			PFS_ERROR("TlsSetValue() error");
			TlsFree(tlsIndex);
			tlsIndex = TLS_OUT_OF_INDEXES;
		}
    }
#endif
}

} // pfs

#endif /* __PFS_THREAD_WIN_HPP__ */
