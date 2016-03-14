/**
 * @file   logger.hpp
 * @author wladt
 * @date   Feb 13, 2013 11:16:20 AM
 *
 * @brief
 */

#ifndef __PFS_LOGGER_HPP__
#define __PFS_LOGGER_HPP__

#include <iostream>
#include <pfs/string.hpp>
#include <pfs/stringlist.hpp>
#include <pfs/safeformat.hpp>
#include <pfs/sigslot.hpp>
#include <pfs/shared_ptr.hpp>
#include <pfs/vector.hpp>
//#include <pfs/notification.hpp>

#ifdef PFS_CC_MSVC
#	pragma warning(push)
#	pragma warning(disable:4251)
#endif

namespace pfs {

class logger_appender;

class DLL_API logger
{
public:
	enum priority
	{
		  trace_priority
		, debug_priority
		, info_priority
		, warn_priority
		, error_priority
		, fatal_priority
		, nolog_priority

		, priority_count
	};

	typedef logger_appender appender_type;

public:
	typedef pfs::vector<appender_type *> appender_vector_type;
	typedef pfs::signal2<int, const pfs::string &> emitter_type;

public:
	static const pfs::string default_pattern;
	static const pfs::string no_pattern;

private:
	struct data
	{
		int                  _level;
		appender_vector_type _appenders;
		emitter_type         _emitters[priority_count];
	};

	data * _d;

public:
	logger ()
		: _d(new data)
	{
		_d->_level = trace_priority;
	}

	~logger ()
	{
		if (_d) {
			clear();
			delete _d;
		}
	}

	void clear ();

	void move (logger & other)
	{
		if (_d) {
			clear();
			delete _d;
		}
		_d = other._d;
		other._d = 0;
	}

	template <typename Appender>
	appender_type & add_appender ()
	{
		_d->_appenders.push_back(new Appender);
		return *_d->_appenders.last();
	}

#if __cplusplus < 201103L
	template <typename Appender, typename Arg1>
	appender_type & add_appender (Arg1 a1)
	{
		_d->_appenders.push_back(new Appender(a1));
		return *_d->_appenders.last();
	}

	template <typename Appender, typename Arg1, typename Arg2>
	appender_type & add_appender (Arg1 a1, Arg2 a2)
	{
		_d->_appenders.push_back(new Appender(a1, a2));
		return *_d->_appenders.last();
	}

	template <typename Appender, typename Arg1, typename Arg2, typename Arg3>
	appender_type & add_appender (Arg1 a1, Arg2 a2, Arg3 a3)
	{
		_d->_appenders.push_back(new Appender(a1, a2, a3));
		return *_d->_appenders.last();
	}

	template <typename Appender, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
	appender_type & add_appender (Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4)
	{
		_d->_appenders.push_back(new Appender(a1, a2, a3, a4));
		return *_d->_appenders.last();
	}

	template <typename Appender, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
	appender_type & add_appender (Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5)
	{
		_d->_appenders.push_back(new Appender(a1, a2, a3, a4, a5));
		return *_d->_appenders.last();
	}

	template <typename Appender, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
	appender_type & add_appender (Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6)
	{
		_d->_appenders.push_back(new Appender(a1, a2, a3, a4, a5, a6));
		return *_d->_appenders.last();
	}

	template <typename Appender, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
	appender_type & add_appender (Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7)
	{
		_d->_appenders.push_back(new Appender(a1, a2, a3, a4, a5, a6, a7));
		return *_d->_appenders.last();
	}

	template <typename Appender, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8>
	appender_type & add_appender (Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8)
	{
		_d->_appenders.push_back(new Appender(a1, a2, a3, a4, a5, a6, a7, a8));
		return *_d->_appenders.last();
	}
#else
#	error "Need to implement `add_appender` using variadic templates"
#endif

	void print (int level, const pfs::string & msg)
	{
		if (level >= _d->_level and level != nolog_priority)
			_d->_emitters[level](level, msg);
	}

	void connect (int level, appender_type & ap);
	void connect (appender_type & ap);

	void disconnect (int level);
	void disconnect_all ();

	void set_priority (priority level)
	{
		_d->_level = level;
	}

	int priority ()
	{
		return _d->_level;
	}

	void trace (const pfs::string & text)
	{
		print(logger::trace_priority, text);
	}

	void debug (const pfs::string & text)
	{
		print(logger::debug_priority, text);
	}

	void info  (const pfs::string & text)
	{
		print(logger::info_priority, text);
	}

	void warn  (const pfs::string & text)
	{
		print(logger::warn_priority, text);
	}

	void error (const pfs::string & text)
	{
		print(logger::error_priority, text);
	}

	void fatal (const pfs::string & text)
	{
		print(logger::fatal_priority, text);
		abort();
	}

	static logger & default_logger ();
//	static void print (const notification & nx);
};

class logger_appender : public has_slots<>
{
	friend class logger;

protected:
	pfs::string _pattern;

protected:
	virtual void print (const pfs::string & msg) = 0;

	void print_helper (int level, const pfs::string & msg)
	{
		print(_pattern.empty()
				? msg
				: patternify(level, _pattern, msg));
	}

	static pfs::string patternify (int level, const pfs::string & pattern, const pfs::string & text);

public:
	logger_appender ()
		: _pattern(logger::default_pattern)
	{}

	logger_appender (const pfs::string & pattern)
		: _pattern(pattern)
	{}

	virtual ~logger_appender ()
	{}

	pfs::string pattern () const
	{
		return _pattern;
	}

	void set_pattern (const pfs::string & pattern)
	{
		_pattern = pattern;
	}
};

class stdout_appender : public logger_appender
{
public:
	stdout_appender ()
		: logger_appender()
	{}

protected:
	virtual void print (const pfs::string & msg) override
	{
		std::cout << msg << std::endl;
	}
};

class stderr_appender : public logger_appender
{
public:
	stderr_appender ()
		: logger_appender()
	{}

protected:
	virtual void print (const pfs::string & msg) override
	{
		std::cerr << msg << std::endl;
	}
};

class stringlist_appender : public logger_appender
{
	pfs::stringlist _strings;

public:
	stringlist_appender () {}
	const pfs::stringlist & data () const { return _strings; }
	pfs::stringlist & data () { return _strings; }

protected:
	virtual void print (const pfs::string & msg) override
	{
		_strings.push_back(msg);
	}
};

inline void logger::connect (int level, appender_type & ap)
{
	PFS_ASSERT(level >= 0 && level < priority_count);
	_d->_emitters[level].connect(& ap, & logger_appender::print_helper);
}

inline void logger::disconnect (int level)
{
	PFS_ASSERT(level >= 0 && level < priority_count);
	_d->_emitters[level].disconnect_all();
}

inline void logger::disconnect_all ()
{
	for (int i = 0; i < priority_count; ++i)
		_d->_emitters[i].disconnect_all();
}

inline void log_trace (const pfs::string & text)
{
	logger::default_logger().print(logger::trace_priority, text);
}

inline void log_debug (const pfs::string & text)
{
	logger::default_logger().print(logger::debug_priority, text);
}

inline void log_info  (const pfs::string & text)
{
	logger::default_logger().print(logger::info_priority, text);
}

inline void log_warn  (const pfs::string & text)
{
	logger::default_logger().print(logger::warn_priority, text);
}

inline void log_error (const pfs::string & text)
{
	logger::default_logger().print(logger::error_priority, text);
}

inline void log_fatal (const pfs::string & text)
{
	logger::default_logger().print(logger::fatal_priority, text);
	abort();
}

} // pfs

#ifdef PFS_CC_MSVC
#	pragma warning(pop)
#endif

#endif /* __PFS_LOGGER_HPP__ */
