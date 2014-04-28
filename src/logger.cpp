/**
 * @file   logger.cpp
 * @author wladt
 * @date   Feb 13, 2013 11:53:01 AM
 *
 * @brief
 */

#include "logger_p.hpp"
#include <pfs/shared_ptr.hpp>
#include <cwt/safeformat.hpp>
#include <cstdarg>
#include <cstdlib>

extern pfs::string __strerror (int_t errn);

namespace cwt {

struct static_log_data
{
	cwt::log traceLog;
	cwt::log debugLog;
	cwt::log infoLog;
	cwt::log warnLog;
	cwt::log errorLog;
	cwt::log fatalLog;

	static_log_data () {

	}
} __static_log_data;


cwt::log & trace () { return __static_log_data.traceLog; }
cwt::log & debug () { return __static_log_data.debugLog; }
cwt::log & info  () { return __static_log_data.infoLog;  }
cwt::log & warn  () { return __static_log_data.warnLog;  }
cwt::log & error () { return __static_log_data.errorLog; }
cwt::log & fatal () { return __static_log_data.fatalLog; }


void log::connect (appender & a)
{
	_emitter.connect()
}

void log::disconnect (appender & a)
{
	_emitter.disconnect(& a);
}

void log::disconnectAll ()
{
	_emitter.disconnect_all();
}


class emitter
{
	typedef stdio_appender default_appender;
	stdio_appender logAppender;

private:
	emitter() : logAppender() {
		logAppender.setPattern(pfs::string("%d{ABSOLUTE} [%p]: %m"));
		// Do not set priority here !!!
		// logAppender.setPriority(Logger::Trace);

		// Make connections according to default log priority (set by default constructor)
		emitTrace.connect (& logAppender, & default_appender::trace);
		emitDebug.connect (& logAppender, & default_appender::debug);
		emitInfo.connect  (& logAppender, & default_appender::info);
		emitWarn.connect  (& logAppender, & default_appender::warn);
		emitError.connect (& logAppender, & default_appender::error);
		emitFatal.connect (& logAppender, & default_appender::fatal);
	}

public:
	~emitter() { disconnectAll(); }

	void disconnectAll ()
	{
		emitTrace.disconnect_all();
		emitDebug.disconnect_all();
		emitInfo.disconnect_all();
		emitWarn.disconnect_all();
		emitError.disconnect_all();
		emitFatal.disconnect_all();
	}

	void restoreDefaultAppenders ()
	{
		disconnectAll();
		emitTrace.connect (& logAppender, & default_appender::trace);
		emitDebug.connect (& logAppender, & default_appender::debug);
		emitInfo.connect  (& logAppender, & default_appender::info);
		emitWarn.connect  (& logAppender, & default_appender::warn);
		emitError.connect (& logAppender, & default_appender::error);
		emitFatal.connect (& logAppender, & default_appender::fatal);
	}

	static pfs::shared_ptr<emitter> instance() {
		static pfs::shared_ptr<emitter> logEmitter(new emitter);
		return logEmitter;
	}

	signal1<const pfs::string &> emitTrace;
	signal1<const pfs::string &> emitDebug;
	signal1<const pfs::string &> emitInfo;
	signal1<const pfs::string &> emitWarn;
	signal1<const pfs::string &> emitError;
	signal1<const pfs::string &> emitFatal;
};

void appender::connect()
{
	if (!_connected) {
		pfs::shared_ptr<emitter> logEmitter = emitter::instance();

		if (_level < log::Debug) logEmitter->emitTrace.connect(this, & appender::trace);
		if (_level < log::Info)  logEmitter->emitDebug.connect(this, & appender::debug);
		if (_level < log::Warn)  logEmitter->emitInfo.connect(this,  & appender::info);
		if (_level < log::Error) logEmitter->emitWarn.connect(this,  & appender::warn);
		if (_level < log::Fatal) logEmitter->emitError.connect(this, & appender::error);
		if (_level < log::NoLog) logEmitter->emitFatal.connect(this, & appender::fatal);

		_connected = true;
	}
}

void appender::disconnect()
{
	if (_connected) {
		pfs::shared_ptr<emitter> logEmitter = emitter::instance();

		logEmitter->emitTrace.disconnect(this);
		logEmitter->emitDebug.disconnect(this);
		logEmitter->emitInfo.disconnect(this);
		logEmitter->emitWarn.disconnect(this);
		logEmitter->emitError.disconnect(this);
		logEmitter->emitFatal.disconnect(this);

		_connected = false;
	}
}


void appender::setPriority(priority level)
{
	disconnect();
	_level = level;
	connect();
}


pfs::string appender::patternify(priority level, const pfs::string & pattern, const pfs::string & msg)
{
	pattern_context ctx;
	ctx.level = level;
	ctx.msg = & msg;
	pfs::fsm::fsm<pfs::string> fsm(pattern_fsm, &ctx);

	ssize_t len = fsm.exec(0, pattern.begin(), pattern.end());

	if (len >= 0 && size_t(len) == pattern.length()) {
		return ctx.result;
	}
	return safeformat(_u8("[<!INVALID PATTERN!>]: %s")) % msg;
}


/**
 * @brief Clear all buffers.
 *
 */
void strings_appender::clear()
{
	_traceStrings.clear();
	_debugStrings.clear();
	_infoStrings.clear();
	_warnStrings.clear();
	_errorStrings.clear();
	_fatalStrings.clear();
}


void trace(const pfs::string & text)
{
	emitter::instance()->emitTrace(text);
}

void debug(const pfs::string & text)
{
	emitter::instance()->emitDebug(text);
}

void info(const pfs::string & text)
{
	emitter::instance()->emitInfo(text);
}

void warn(const pfs::string & text)
{
	emitter::instance()->emitWarn(text);
}

void warn (int errn, const pfs::string & text)
{
	if (text.isEmpty()) {
		log::warn(__strerror(errn));
	} else {
		log::warn(safeformat(_u8("%s: %s")) % text % __strerror(errn));
	}
}


void error(const pfs::string & text)
{
	emitter::instance()->emitError(text);
}

void error (int errn, const pfs::string & text)
{
	if (text.isEmpty()) {
		log::error(__strerror(errn));
	} else {
		log::error(safeformat(_u8("%s: %s")) % text % __strerror(errn));
	}
}

void fatal(const pfs::string & text)
{
	emitter::instance()->emitFatal(text);
	abort();
}

void fatal (int errn, const pfs::string & text)
{
	if (text.isEmpty()) {
		log::fatal(__strerror(errn));
	} else {
		log::fatal(safeformat(_u8("%s: %s")) % text % __strerror(errn));
	}
}

void restoreDefaultAppenders ()
{
	emitter::instance()->restoreDefaultAppenders();
}

void disconnectAllAppenders ()
{
	emitter::instance()->disconnectAll();
}

} // cwt
