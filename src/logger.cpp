/**
 * @file   logger.cpp
 * @author wladt
 * @date   Feb 13, 2013 11:53:01 AM
 *
 * @brief
 */

#include <cwt/logger.hpp>
#include <cstdarg>
#include "logger_p.hpp"

CWT_NS_BEGIN

void Logger::init()
{
	static StdioLogAppender logAppender;
	Logger::connectAppender(&logAppender);
}

class LogEmitter
{
public:
	LogEmitter() {}
	~LogEmitter() {
		emitTrace.disconnect_all();
		emitDebug.disconnect_all();
		emitInfo.disconnect_all();
		emitWarn.disconnect_all();
		emitError.disconnect_all();
		emitFatal.disconnect_all();
	}

	signal1<const String&> emitTrace;
	signal1<const String&> emitDebug;
	signal1<const String&> emitInfo;
	signal1<const String&> emitWarn;
	signal1<const String&> emitError;
	signal1<const String&> emitFatal;
};

static LogEmitter g_logEmitter;

void LogAppender::connect()
{
	if (m_priority < Logger::Debug) g_logEmitter.emitTrace.connect(this, &LogAppender::trace);
	if (m_priority < Logger::Info)  g_logEmitter.emitDebug.connect(this, &LogAppender::debug);
	if (m_priority < Logger::Warn)  g_logEmitter.emitInfo.connect(this, &LogAppender::info);
	if (m_priority < Logger::Error) g_logEmitter.emitWarn.connect(this, &LogAppender::warn);
	if (m_priority < Logger::Fatal) g_logEmitter.emitError.connect(this, &LogAppender::error);
	if (m_priority < Logger::NoLog) g_logEmitter.emitFatal.connect(this, &LogAppender::fatal);
}

void LogAppender::disconnect()
{
	g_logEmitter.emitTrace.disconnect(this);
	g_logEmitter.emitDebug.disconnect(this);
	g_logEmitter.emitInfo.disconnect(this);
	g_logEmitter.emitWarn.disconnect(this);
	g_logEmitter.emitError.disconnect(this);
	g_logEmitter.emitFatal.disconnect(this);
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
	Logger::error("invalid pattern for logger appender");
	return String::null();
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
	g_logEmitter.emitTrace(String().vsprintf(cformat, args));
	va_end(args);
}

void Logger::debug(const char * cformat, ...)
{
	va_list args;
	va_start(args, cformat);
	g_logEmitter.emitDebug(String().vsprintf(cformat, args));
	va_end(args);
}

void Logger::info(const char * cformat, ...)
{
	va_list args;
	va_start(args, cformat);
	g_logEmitter.emitInfo(String().vsprintf(cformat, args));
	va_end(args);
}

void Logger::warn(const char * cformat, ...)
{
	va_list args;
	va_start(args, cformat);
	g_logEmitter.emitWarn(String().vsprintf(cformat, args));
	va_end(args);
}

void Logger::error(const char * cformat, ...)
{
	va_list args;
	va_start(args, cformat);
	g_logEmitter.emitError(String().vsprintf(cformat, args));
	va_end(args);
}

void Logger::fatal(const char * cformat, ...)
{
	va_list args;
	va_start(args, cformat);
	g_logEmitter.emitFatal(String().vsprintf(cformat, args));
	va_end(args);
}


CWT_NS_END
