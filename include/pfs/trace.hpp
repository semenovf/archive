#include <cstdio>

#ifndef __CWT_TRACE_HPP__
#define __CWT_TRACE_HPP__

#if defined(CWT_TRACE_ENABLE)
#	define CWT_TRACE_FUNC() const CwtTraceFunc __cwt_trace__ (__PRETTY_FUNCTION__)
#	define CWT_TRACE_METHOD() const CwtTraceMethod __cwt_trace__ (__PRETTY_FUNCTION__, this)

#else
#	undef CWT_TRACE_METHOD
#	undef CWT_TRACE_FUNC
#	define CWT_TRACE_METHOD()
#	define CWT_TRACE_FUNC()
#endif

class CwtTraceFunc
{
public:
	CwtTraceFunc (const char * pretty_func_name) : m_fname (pretty_func_name)
	{
		printf("===BEGIN: %s\n", m_fname);
	}

	~CwtTraceFunc ()
	{
		printf("=====END: %s\n", m_fname);
	}
private:
	const char * m_fname;
};

class CwtTraceMethod
{
public:
	CwtTraceMethod (const char * pretty_func_name, const void * instance)
		: m_fname(pretty_func_name)
		, m_instance(instance)
	{
		printf("===BEGIN: [%p]:%s\n", m_instance, m_fname);
	}

	~CwtTraceMethod ()
	{
		printf("=====END: [%p]:%s\n", m_instance, m_fname);
	}
private:
	const char * m_fname;
	const void * m_instance;
};

#endif
