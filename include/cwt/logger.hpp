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

class Logger;

/* Pattern flags:
 * 	m 	message associated with the logging event.
 * */
class LogAppender : public has_slots<>
{
public:
	void connect();
	void disconnect();
public:
	LogAppender() : m_pattern(_U("%m")) {}
	LogAppender(const String &pattern) : m_pattern(pattern) {}
	virtual ~LogAppender() { disconnect(); }

	void setPattern(const String &pattern) { m_pattern = pattern; }

	virtual void trace(const String&) = 0;
	virtual void debug(const String&) = 0;
	virtual void info(const String&) = 0;
	virtual void warn(const String&) = 0;
	virtual void error(const String&) = 0;
	virtual void fatal(const String&) = 0;

protected:
	static String patternify(const String &pattern, const String &msg);

private:
	String m_pattern;
};

class Logger
{
public:
	enum Level { Trace, Debug, Info, Warn, Error, Fatal, NoLog };

public:
	static void connectAppender(LogAppender *appender) { appender->connect(); }
	static void disconnectAppender(LogAppender *appender) { appender->disconnect(); }
	static void setLevel(Level l);
	static Level level() { return g_level; }
	static void trace(const char * cformat, ...);
	static void debug(const char * cformat, ...);
	static void info(const char * cformat, ...);
	static void warn(const char * cformat, ...);
	static void error(const char * cformat, ...);
	static void fatal(const char * cformat, ...);
private:
	static Level g_level;
	static Vector<LogAppender*> g_appenders;
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
	virtual void trace(const String &msg) { ::fprintf(stdout, "%s\n", msg.toUtf8().data()); }
	virtual void debug(const String &msg) { ::fprintf(stdout, "%s\n", msg.toUtf8().data()); }
	virtual void info(const String &msg)  { ::fprintf(stdout, "%s\n", msg.toUtf8().data()); }
	virtual void warn(const String &msg)  { ::fprintf(stderr, "%s\n", msg.toUtf8().data()); }
	virtual void error(const String &msg) { ::fprintf(stderr, "%s\n", msg.toUtf8().data()); }
	virtual void fatal(const String &msg) { ::fprintf(stderr, "%s\n", msg.toUtf8().data()); }
};


CWT_NS_END

#endif /* __CWT_LOGGER_HPP__ */
