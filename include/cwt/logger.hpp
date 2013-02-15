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

CWT_NS_BEGIN

class LogAppender;

class Logger
{
public:
	enum Priority { Trace, Debug, Info, Warn, Error, Fatal, NoLog };

public:
	static void init();
	static void connectAppender(LogAppender *appender);
	static void disconnectAppender(LogAppender *appender);
	static void trace(const char * cformat, ...);
	static void debug(const char * cformat, ...);
	static void info(const char * cformat, ...);
	static void warn(const char * cformat, ...);
	static void error(const char * cformat, ...);
	static void fatal(const char * cformat, ...);
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
	LogAppender() : m_pattern(_U("%m")), m_priority(Logger::Trace) { }
	LogAppender(const String &pattern) : m_pattern(pattern), m_priority(Logger::Trace) {}
	virtual ~LogAppender() { disconnect(); }

	void setPattern(const String &pattern) { m_pattern = pattern; }
	void setPriority(Logger::Priority level);
	Logger::Priority level() { return m_priority; }

	virtual void trace(const String&) = 0;
	virtual void debug(const String&) = 0;
	virtual void info(const String&) = 0;
	virtual void warn(const String&) = 0;
	virtual void error(const String&) = 0;
	virtual void fatal(const String&) = 0;

protected:
	static String patternify(Logger::Priority priority, const String &pattern, const String &msg);
	String patternify(Logger::Priority priority, const String &msg) { return patternify(priority, m_pattern, msg); }

private:
	String m_pattern;
	Logger::Priority m_priority;
};


class NullLogAppender : public LogAppender
{
public:
	NullLogAppender() {}
	virtual ~NullLogAppender() {}
	virtual void trace(const String&) {}
	virtual void debug(const String&) {}
	virtual void info(const String&) {}
	virtual void warn(const String&) {}
	virtual void error(const String&) {}
	virtual void fatal(const String&) {}
};


class StdioLogAppender : public LogAppender
{
public:
	StdioLogAppender() {}
	virtual ~StdioLogAppender() {}
	virtual void trace(const String &msg) { ::fprintf(stdout, "%s\n", patternify(Logger::Trace, msg).toUtf8().data()); }
	virtual void debug(const String &msg) { ::fprintf(stdout, "%s\n", patternify(Logger::Debug, msg).toUtf8().data()); }
	virtual void info(const String &msg)  { ::fprintf(stdout, "%s\n", patternify(Logger::Info, msg).toUtf8().data()); }
	virtual void warn(const String &msg)  { ::fprintf(stderr, "%s\n", patternify(Logger::Warn, msg).toUtf8().data()); }
	virtual void error(const String &msg) { ::fprintf(stderr, "%s\n", patternify(Logger::Error, msg).toUtf8().data()); }
	virtual void fatal(const String &msg) { ::fprintf(stderr, "%s\n", patternify(Logger::Fatal, msg).toUtf8().data()); }
};

class StringsLogAppender : public LogAppender
{
public:
	StringsLogAppender() {}
	virtual ~StringsLogAppender() {}
	virtual void trace(const String &msg) { m_traceStrings.append(patternify(Logger::Trace, msg)); }
	virtual void debug(const String &msg) { m_debugStrings.append(patternify(Logger::Debug, msg)); }
	virtual void info(const String &msg)  { m_infoStrings.append(patternify (Logger::Info, msg)); }
	virtual void warn(const String &msg)  { m_warnStrings.append(patternify (Logger::Warn, msg)); }
	virtual void error(const String &msg) { m_errorStrings.append(patternify(Logger::Error, msg)); }
	virtual void fatal(const String &msg) { m_fatalStrings.append(patternify(Logger::Fatal, msg)); }

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
