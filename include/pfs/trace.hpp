#include <cstdio>
#include <pfs/stopwatch.hpp>

#ifndef __PFS_TRACE_HPP__
#define __PFS_TRACE_HPP__

#if defined(CWT_TRACE_ENABLE)
#	define CWT_TRACE_FUNC() const trace_func __pfs_trace__ (__PRETTY_FUNCTION__)
#	define CWT_TRACE_METHOD() const trace_method __pfs_trace__ (__PRETTY_FUNCTION__, this)

#else
#	undef CWT_TRACE_METHOD
#	undef CWT_TRACE_FUNC
#	define CWT_TRACE_METHOD()
#	define CWT_TRACE_FUNC()
#endif

namespace pfs {

class trace_func
{
	static stopwatch __sw;
	const char * _fname;
public:
	trace_func (const char * pretty_func_name) : _fname (pretty_func_name)
	{
		printf("===BEGIN [%s]: %s\n", stopwatch::stringifyTime(__sw.ellapsed()).c_str(), _fname);
	}

	~trace_func ()
	{
		printf("=====END [%s]: %s\n", stopwatch::stringifyTime(__sw.ellapsed()).c_str(), _fname);
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
		printf("===BEGIN [%s]: [%p]:%s\n", stopwatch::stringifyTime(__sw.ellapsed()).c_str(), _instance, _fname);
	}

	~trace_method ()
	{
		printf("=====END [%s]: [%p]:%s\n", stopwatch::stringifyTime(__sw.ellapsed()).c_str(), _instance, _fname);
	}
};

} // pfs

#endif
