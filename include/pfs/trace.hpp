#include <cstdio>

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

namespace pfs {

class trace_func
{
public:
	trace_func (const char * pretty_func_name) : _fname (pretty_func_name)
	{
		printf("===BEGIN: %s\n", _fname);
	}

	~trace_func ()
	{
		printf("=====END: %s\n", _fname);
	}
private:
	const char * _fname;
};

class trace_method
{
public:
	trace_method (const char * pretty_func_name, const void * instance)
		: _fname(pretty_func_name)
		, _instance(instance)
	{
		printf("===BEGIN: [%p]:%s\n", _instance, _fname);
	}

	~trace_method ()
	{
		printf("=====END: [%p]:%s\n", _instance, _fname);
	}
private:
	const char * _fname;
	const void * _instance;
};

} // pfs

#endif
