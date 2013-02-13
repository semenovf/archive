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

Logger::Level Logger::g_level = Trace;
Vector<LogAppender*> Logger::g_appenders;

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
	Logger::Level l = Logger::level();
	if (l < Logger::Debug) g_logEmitter.emitTrace.connect(this, &LogAppender::trace);
	if (l < Logger::Info)  g_logEmitter.emitDebug.connect(this, &LogAppender::debug);
	if (l < Logger::Warn)  g_logEmitter.emitInfo.connect(this, &LogAppender::info);
	if (l < Logger::Error) g_logEmitter.emitWarn.connect(this, &LogAppender::warn);
	if (l < Logger::Fatal) g_logEmitter.emitError.connect(this, &LogAppender::error);
	if (l < Logger::NoLog) g_logEmitter.emitFatal.connect(this, &LogAppender::fatal);
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


#define PLAIN_CHAR 1
#define FLAG_CHAR 2
String LogAppender::patternify(const String &pattern, const String &msg)
{
	String result(msg);
	CWT_UNUSED(pattern);
	return result;
}


void Logger::setLevel(Level level)
{
	if (level > Logger::Trace) g_logEmitter.emitTrace.disconnect_all();
	if (level > Logger::Debug) g_logEmitter.emitDebug.disconnect_all();
	if (level > Logger::Info)  g_logEmitter.emitInfo.disconnect_all();
	if (level > Logger::Warn)  g_logEmitter.emitWarn.disconnect_all();
	if (level > Logger::Error) g_logEmitter.emitError.disconnect_all();
	if (level > Logger::Fatal) g_logEmitter.emitFatal.disconnect_all();
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
