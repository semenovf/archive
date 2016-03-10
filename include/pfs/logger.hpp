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

class appender;

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
		, priority_count = nolog_priority
	};

	typedef pfs::vector<shared_ptr<appender> > appender_vector_type;
	typedef pfs::signal2<int, const pfs::string &> emitter_type;

public:
	static const pfs::string default_pattern;
	static const pfs::string no_pattern;

private:
	appender_vector_type _appenders;
	int                  _level;
	emitter_type         _emitters[priority_count];

public:
	logger ()
		: _level(trace_priority)
	{}

	void print (int level, const pfs::string & msg)
	{
		if (level >= _level and level != nolog_priority)
			_emitters[level](level, msg);
	}

	void connect (int level, shared_ptr<appender> pappender);
	void connect (shared_ptr<appender> pappender);
	void disconnect (int level, shared_ptr<appender> pappender);
	void disconnect (int level);
	void disconnect_all ();

	void set_priority (priority level)
	{
		_level = level;
	}

	int priority ()
	{
		return _level;
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

class appender : public has_slots<>
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
	appender ()
		: _pattern(logger::default_pattern)
	{}

	appender (const pfs::string & pattern)
		: _pattern(pattern)
	{}

	virtual ~appender ()
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

class stdout_appender : public appender
{
public:
	stdout_appender ()
		: appender()
	{}

protected:
	virtual void print (const pfs::string & msg) override
	{
		std::cout << msg << std::endl;
	}
};

class stderr_appender : public appender
{
public:
	stderr_appender ()
		: appender()
	{}

protected:
	virtual void print (const pfs::string & msg) override
	{
		std::cerr << msg << std::endl;
	}
};

class stringlist_appender : public appender
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

inline void logger::connect (int level, shared_ptr<appender> pappender)
{
	PFS_ASSERT_BT(level >= 0 && level < priority_count);
	_emitters[level].connect(pappender.get(), & appender::print_helper);
}

inline void logger::disconnect (int level, shared_ptr<appender> pappender)
{
	PFS_ASSERT_BT(level >= 0 && level < priority_count);
	_emitters[level].disconnect(pappender.get());
}

inline void logger::disconnect (int level)
{
	PFS_ASSERT_BT(level >= 0 && level < priority_count);
	_emitters[level].disconnect_all();
}

inline void logger::disconnect_all ()
{
	for (int i = 0; i < priority_count; ++i)
		_emitters[i].disconnect_all();
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
