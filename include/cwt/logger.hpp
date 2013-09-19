/**
 * @file   logger.hpp
 * @author wladt
 * @date   Feb 13, 2013 11:16:20 AM
 *
 * @brief
 */

#ifndef __CWT_LOGGER_HPP__
#define __CWT_LOGGER_HPP__

#include <cwt/cwt.h>
#include <cwt/string.hpp>
#include <cwt/sigslot.hpp>
#include <cwt/vector.hpp>
#include <cstdio>

#define CWT_SYS_WARN(s) \
		Logger::sys_warn(SafeFormat("%s[%d]: %s") % String(__FILE__) % int(__LINE__) % s)
#define CWT_SYS_WARN_RC(rc, s) \
		Logger::sys_warn(rc, SafeFormat("%s[%d]: %s") % String(__FILE__) % int(__LINE__) % s)

#define CWT_SYS_ERROR(s) \
		Logger::sys_error(SafeFormat("%s[%d]: %s") % String(__FILE__) % int(__LINE__) % s)
#define CWT_SYS_ERROR_RC(rc, s) \
		Logger::sys_error(rc, SafeFormat("%s[%d]: %s") % String(__FILE__) % int(__LINE__) % s)

#define CWT_SYS_FATAL(s) \
		Logger::sys_fatal(SafeFormat("%s[%d]: %s") % String(__FILE__) % int(__LINE__) % s)
#define CWT_SYS_FATAL_RC(rc, s) \
		Logger::sys_fatal(rc, SafeFormat("%s[%d]: %s") % String(__FILE__) % int(__LINE__) % s)

#define CWT_VERIFY(expr) if (! (expr)) { CWT_SYS_ERROR(#expr); }

CWT_NS_BEGIN

class LogAppender;

class Logger
{
public:
	enum Priority { Trace, Debug, Info, Warn, Error, Fatal, NoLog };

public:
	static void connectAppender(LogAppender *appender);
	static void disconnectAppender(LogAppender *appender);
	static void trace (const String & text);
	static void debug (const String & text);
	static void info  (const String & text);
	static void warn  (const String & text);
	static void warn  (int errn, const String & text);
	static void error (const String & text);
	static void error (int errn, const String & text);
	static void fatal (const String & text);
	static void fatal (int errn, const String & text);

	static void sys_warn  (const String & text);
	static void sys_warn  (int errn, const String & text);
	static void sys_error (const String & text);
	static void sys_error (int errn, const String & text);
	static void sys_fatal (const String & text);
	static void sys_fatal (int errn, const String & text);

private:
	/*static Vector<LogAppender*> g_appenders;*/
};


/* Pattern flags:
 * 	m 	message associated with the logging event.
 * */
class LogAppender : public has_slots<>
{
public:
	void connect();
	void disconnect();
public:
	LogAppender() : m_connected(false), m_pattern("%m"), m_priority(Logger::Trace) { }
	LogAppender(const String &pattern) : m_connected(false), m_pattern(pattern), m_priority(Logger::Trace) {}
	virtual ~LogAppender() { if(m_connected) disconnect(); }

	void setPattern(const String &pattern) { m_pattern = pattern; }
	void setPriority(Logger::Priority level);
	Logger::Priority level() { return m_priority; }

	virtual void trace (const String &) = 0;
	virtual void debug (const String &) = 0;
	virtual void info  (const String &) = 0;
	virtual void warn  (const String &) = 0;
	virtual void error (const String &) = 0;
	virtual void fatal (const String &) = 0;

protected:
	static String patternify(Logger::Priority priority, const String &pattern, const String & text);
	String patternify(Logger::Priority priority, const String & text) { return patternify(priority, m_pattern, text); }

protected:
	bool m_connected;
	String m_pattern;
	Logger::Priority m_priority;
};


class NullLogAppender : public LogAppender
{
public:
	NullLogAppender() {}
	~NullLogAppender() {}
	virtual void trace (const String &) {}
	virtual void debug (const String &) {}
	virtual void info  (const String &) {}
	virtual void warn  (const String &) {}
	virtual void error (const String &) {}
	virtual void fatal (const String &) {}
};


class StdioLogAppender : public LogAppender
{
public:
	StdioLogAppender() {}
	~StdioLogAppender() {}
	virtual void trace (const String & text) { ::fprintf(stdout, "%s\n", patternify(Logger::Trace, text).c_str()); }
	virtual void debug (const String & text) { ::fprintf(stdout, "%s\n", patternify(Logger::Debug, text).c_str()); }
	virtual void info  (const String & text) { ::fprintf(stdout, "%s\n", patternify(Logger::Info, text).c_str()); }
	virtual void warn  (const String & text) { ::fprintf(stderr, "%s\n", patternify(Logger::Warn, text).c_str()); }
	virtual void error (const String & text) { ::fprintf(stderr, "%s\n", patternify(Logger::Error, text).c_str()); }
	virtual void fatal (const String & text) { ::fprintf(stderr, "%s\n", patternify(Logger::Fatal, text).c_str()); }
};

class StringsLogAppender : public LogAppender
{
public:
	StringsLogAppender() {}
	~StringsLogAppender() {}
	virtual void trace (const String & text) { m_traceStrings.append(patternify(Logger::Trace, text)); }
	virtual void debug (const String & text) { m_debugStrings.append(patternify(Logger::Debug, text)); }
	virtual void info  (const String & text)  { m_infoStrings.append(patternify (Logger::Info, text)); }
	virtual void warn  (const String & text)  { m_warnStrings.append(patternify (Logger::Warn, text)); }
	virtual void error (const String & text) { m_errorStrings.append(patternify(Logger::Error, text)); }
	virtual void fatal (const String & text) { m_fatalStrings.append(patternify(Logger::Fatal, text)); }

	Vector<String>& traceStrings() { return m_traceStrings; }
	Vector<String>& debugStrings() { return m_debugStrings; }
	Vector<String>& infoStrings()  { return m_infoStrings; }
	Vector<String>& warnStrings()  { return m_warnStrings; }
	Vector<String>& errorStrings() { return m_errorStrings; }
	Vector<String>& fatalStrings() { return m_fatalStrings; }
	const Vector<String>& traceStrings() const { return m_traceStrings; }
	const Vector<String>& debugStrings() const { return m_debugStrings; }
	const Vector<String>& infoStrings()  const { return m_infoStrings; }
	const Vector<String>& warnStrings()  const { return m_warnStrings; }
	const Vector<String>& errorStrings() const { return m_errorStrings; }
	const Vector<String>& fatalStrings() const { return m_fatalStrings; }

	void clear();

private:
	Vector<String> m_traceStrings;
	Vector<String> m_debugStrings;
	Vector<String> m_infoStrings;
	Vector<String> m_warnStrings;
	Vector<String> m_errorStrings;
	Vector<String> m_fatalStrings;
};

inline void Logger::connectAppender(LogAppender *appender) { appender->connect(); }
inline void Logger::disconnectAppender(LogAppender *appender) { appender->disconnect(); }


CWT_NS_END

#endif /* __CWT_LOGGER_HPP__ */
