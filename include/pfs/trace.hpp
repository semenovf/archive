#include <cstdio>
#include <pfs/stopwatch.hpp>

#ifndef __PFS_TRACE_HPP__
#define __PFS_TRACE_HPP__

#if defined(PFS_TRACE_ENABLE)
#	define PFS_TRACE_FUNC() const pfs::trace_func __pfs_trace__ (__PRETTY_FUNCTION__)
#	define PFS_TRACE_METHOD() const pfs::trace_method __pfs_trace__ (__PRETTY_FUNCTION__, this)
#else
#	undef PFS_TRACE_METHOD
#	undef PFS_TRACE_FUNC
#	define PFS_TRACE_METHOD()
#	define PFS_TRACE_FUNC()
#endif

#ifdef PFS_TRACE_PTHREAD
#include <pthread.h>
#endif

namespace pfs {

class trace_func
{
	static stopwatch __sw;
	const char * _fname;
public:
	trace_func (const char * pretty_func_name) : _fname (pretty_func_name)
	{
#ifdef PFS_TRACE_PTHREAD
		printf("===BEGIN [%s]: [thr=%lu]: %s\n", stopwatch::stringifyTime(__sw.ellapsed()).c_str()
				, pthread_self()
				, _fname);
#else
		printf("===BEGIN [%s]: %s\n", stopwatch::stringifyTime(__sw.ellapsed()).c_str(), _fname);
#endif
	}

	~trace_func ()
	{
#ifdef PFS_TRACE_PTHREAD
		printf("=====END [%s]: [thr=%lu]: %s\n", stopwatch::stringifyTime(__sw.ellapsed()).c_str()
				, pthread_self()
				, _fname);
#else
		printf("=====END [%s]: %s\n", stopwatch::stringifyTime(__sw.ellapsed()).c_str(), _fname);
#endif
	}
};

class trace_method
{
	static stopwatch __sw;
	const char * _fname;
	const void * _instance;

public:
	trace_method (const char * pretty_func_name, const void * instance)
		: _fname(pretty_func_name)
		, _instance(instance)
	{
#ifdef PFS_TRACE_PTHREAD
		printf("===BEGIN [%s]: [thr=%lu]: [%p]:%s\n", stopwatch::stringifyTime(__sw.ellapsed()).c_str()
				, pthread_self()
				, _instance
				, _fname);
#else
		printf("===BEGIN [%s]: [%p]:%s\n", stopwatch::stringifyTime(__sw.ellapsed()).c_str()
				, _instance
				, _fname);
#endif
	}

	~trace_method ()
	{
#ifdef PFS_TRACE_PTHREAD
		printf("=====END [%s]: [thr=%lu]: [%p]:%s\n", stopwatch::stringifyTime(__sw.ellapsed()).c_str()
				, pthread_self()
				, _instance
				, _fname);
#else
		printf("=====END [%s]: [%p]:%s\n", stopwatch::stringifyTime(__sw.ellapsed()).c_str()
				, _instance
				, _fname);
#endif
	}
};

} // pfs

#endif // __PFS_TRACE_HPP__
