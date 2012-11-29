/*
 * mt.h
 *
 *  Created on: Aug 24, 2011
 *  Modified on: Sep 25, 2012
 *      Author: wladt
 * 
 *  Great thanks to Sarah Thompson for her http://sigslot.sourceforge.net/
 */

#ifndef __CWT_MT_H__
#define __CWT_MT_H__

#ifndef __CWT_GLOBAL_H__
#	error "Do not direct include of <cwt/mt.h> header, only implicitly through inclusion <cwt/global.h>"
#else
#	if !defined(CWT_MT)
#		define CWT_SINGLE_THREADED
#	elif defined(CWT_OS_WIN)
#		define CWT_WIN32_THREADS
#		include <windows.h>
#	elif defined(__GNUG__)
#		define CWT_POSIX_THREADS
#		include <pthread.h>
#	else
#		define CWT_SINGLE_THREADED
#	endif

/*
#	ifndef CWT_DEFAULT_MT_POLICY
#		ifdef CWT_SINGLE_THREADED
#			define CWT_DEFAULT_MT_POLICY single_threaded
#		else
#			define CWT_DEFAULT_MT_POLICY multi_threaded_local
#		endif
#	endif
*/

#ifdef CWT_SINGLE_THREADED
#	define CWT_LOCK
#	define CWT_UNLOCK
#else
#	define CWT_LOCK   cwt_mt_ns()->global()->lock()
#	define CWT_UNLOCK cwt_mt_ns()->global()->unlock()
#endif

typedef struct _CwtThreaded {
	BOOL is_global;
} CwtThreaded;

typedef struct _CwtMtNS {
	CwtThreaded* (*global) (void);
	CwtThreaded* (*local)  (void);
	void         (*free)   (CwtThreaded *);
	void         (*lock)(struct _CwtThreaded *);
	void         (*unlock)(struct _CwtThreaded *);
} CwtMtNS;

EXTERN_C_BEGIN
DLL_API CwtMtNS* cwt_mt_ns(void);
EXTERN_C_END

/*
	template<class mt_policy=JQ_DEFAULT_MT_POLICY>
	class AutoLock
	{
	public:
		mt_policy *m_mutex;
		AutoLock(mt_policy *mtx) : m_mutex(mtx) { m_mutex->lock(); }
		~AutoLock() { m_mutex->unlock(); }
	};
*/


/*
	// Macro that provides a locking implementation. Use the Lock() and Unlock()
	// methods to protect a section of code from simultaneous access by multiple
	// threads. The AutoLock class is a helper that will hold the lock while in
	// scope.
*/
/*
#define JQ_IMPLEMENT_LOCKING(ClassName)                             \
	protected:                                                      \
		class AutoLock                                              \
		{                                                           \
		public:                                                     \
			AutoLock(ClassName *base)                               \
				: m_base(base) { m_base->m_mutex.lock(); }          \
			~AutoLock() { m_base->m_mutex.unlock(); }               \
		private:                                                    \
			ClassName* m_base;                                      \
		};                                                          \
	private:                                                        \
		JQ_DEFAULT_MT_POLICY m_mutex;
*/

#endif  /*__CWT_GLOBAL_H__*/

#endif /* __CWT_MT_H__ */
