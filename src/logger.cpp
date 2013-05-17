/**
 * @file   logger.cpp
 * @author wladt
 * @date   Feb 13, 2013 11:53:01 AM
 *
 * @brief
 */

#include <cwt/logger.hpp>
#include <cstdarg>
#include <cstdlib>
#include "logger_p.hpp"

CWT_NS_BEGIN

class LogEmitter;

class DefaultLogAppender : public StdioLogAppender
{
public:
	typedef StdioLogAppender Base;
public:
	explicit DefaultLogAppender() : StdioLogAppender() { m_connected = true; m_priority = Logger::Trace; }
	~DefaultLogAppender() {} // override destructor (exclude disconnect() call)
};


class LogEmitter
{
	typedef StdioLogAppender DefaultLogAppender;

private:
	LogEmitter() : logAppender() {
		logAppender.setPattern(_U("%d{ABSOLUTE} [%p]: %m"));
		// Do not set priority here !!!
		// logAppender.setPriority(Logger::Trace);

		// Make connections according to default log priority (set by default constructor)
		emitTrace.connect (&logAppender, &DefaultLogAppender::trace);
		emitDebug.connect (&logAppender, &DefaultLogAppender::debug);
		emitInfo.connect  (&logAppender, &DefaultLogAppender::info);
		emitWarn.connect  (&logAppender, &DefaultLogAppender::warn);
		emitError.connect (&logAppender, &DefaultLogAppender::error);
		emitFatal.connect (&logAppender, &DefaultLogAppender::fatal);
	}

public:
	~LogEmitter() {
		emitTrace.disconnect_all();
		emitDebug.disconnect_all();
		emitInfo.disconnect_all();
		emitWarn.disconnect_all();
		emitError.disconnect_all();
		emitFatal.disconnect_all();
		CWT_TRACE("~LogEmitter()");
	}

	static shared_ptr<LogEmitter> instance() {
		static shared_ptr<LogEmitter> logEmitter(new LogEmitter);
		return logEmitter;
	}

	signal1<const String&> emitTrace;
	signal1<const String&> emitDebug;
	signal1<const String&> emitInfo;
	signal1<const String&> emitWarn;
	signal1<const String&> emitError;
	signal1<const String&> emitFatal;

private:
	DefaultLogAppender logAppender;
};

void LogAppender::connect()
{
	if (!m_connected) {
		shared_ptr<LogEmitter> logEmitter = LogEmitter::instance();

		if (m_priority < Logger::Debug) logEmitter->emitTrace.connect(this, &LogAppender::trace);
		if (m_priority < Logger::Info)  logEmitter->emitDebug.connect(this, &LogAppender::debug);
		if (m_priority < Logger::Warn)  logEmitter->emitInfo.connect(this, &LogAppender::info);
		if (m_priority < Logger::Error) logEmitter->emitWarn.connect(this, &LogAppender::warn);
		if (m_priority < Logger::Fatal) logEmitter->emitError.connect(this, &LogAppender::error);
		if (m_priority < Logger::NoLog) logEmitter->emitFatal.connect(this, &LogAppender::fatal);

		m_connected = true;
	}
}

void LogAppender::disconnect()
{
	if (m_connected) {
		shared_ptr<LogEmitter> logEmitter = LogEmitter::instance();

		logEmitter->emitTrace.disconnect(this);
		logEmitter->emitDebug.disconnect(this);
		logEmitter->emitInfo.disconnect(this);
		logEmitter->emitWarn.disconnect(this);
		logEmitter->emitError.disconnect(this);
		logEmitter->emitFatal.disconnect(this);

		m_connected = false;
	}
}


void LogAppender::setPriority(Logger::Priority level)
{
	disconnect();
	m_priority = level;
	connect();
}


String LogAppender::patternify(Logger::Priority priority, const String &pattern, const String &msg)
{
	LoggerPatternContext ctx;
	ctx.priority = priority;
	ctx.msg = &msg;
	Fsm<Char> fsm(pattern_fsm, &ctx);

	if (fsm.exec(0, pattern.data(), pattern.length()) == pattern.length()) {
		return ctx.result;
	}
	return String().sprintf(_Tr("[<!INVALID PATTERN!>]: %ls"), msg.utf16());
}


/**
 * @brief Clear all buffers.
 *
 */
void StringsLogAppender::clear()
{
	m_traceStrings.clear();
	m_debugStrings.clear();
	m_infoStrings.clear();
	m_warnStrings.clear();
	m_errorStrings.clear();
	m_fatalStrings.clear();
}


void Logger::trace(const char * cformat, ...)
{
	va_list args;
	va_start(args, cformat);
	LogEmitter::instance()->emitTrace(String().vsprintf(cformat, args));
	va_end(args);
}

void Logger::debug(const char * cformat, ...)
{
	va_list args;
	va_start(args, cformat);
	LogEmitter::instance()->emitDebug(String().vsprintf(cformat, args));
	va_end(args);
}

void Logger::info(const char * cformat, ...)
{
	va_list args;
	va_start(args, cformat);
	LogEmitter::instance()->emitInfo(String().vsprintf(cformat, args));
	va_end(args);
}

void Logger::warn(const char * cformat, ...)
{
	va_list args;
	va_start(args, cformat);
	LogEmitter::instance()->emitWarn(String().vsprintf(cformat, args));
	va_end(args);
}

void Logger::error(const char * cformat, ...)
{
	va_list args;
	va_start(args, cformat);
	LogEmitter::instance()->emitError(String().vsprintf(cformat, args));
	va_end(args);
}

void Logger::fatal(const char * cformat, ...)
{
	va_list args;
	va_start(args, cformat);
	LogEmitter::instance()->emitFatal(String().vsprintf(cformat, args));
	va_end(args);
	abort();
}


CWT_NS_END
