/**
 * @file   logger.cpp
 * @author wladt
 * @date   Feb 13, 2013 11:53:01 AM
 *
 * @brief
 */

#include "logger_p.hpp"
#include <pfs/shared_ptr.hpp>
#include <pfs/atomic.hpp>
#include <cwt/safeformat.hpp>

extern pfs::string __strerror (int_t errn);

namespace cwt {

pfs::string log::DefaultPattern("%d{ABSOLUTE} [%p]: %m");
pfs::atomic_integer<int> __priorityLevel;

void log::setPriority (log::priority level) { __priorityLevel.store(level); }
log::priority log::level () { return log::priority(__priorityLevel.load()); }

appender & default_out_appender ()
{
	static pfs::shared_ptr<stdout_appender> appender(new stdout_appender);
	return *appender;
}

appender & default_err_appender ()
{
	static pfs::shared_ptr<stderr_appender> appender(new stderr_appender);
	return *appender;
}

cwt::log & trace ()
{
	static pfs::shared_ptr<cwt::log> traceLog(new cwt::log(default_out_appender()));
	return *traceLog;
}

cwt::log & debug ()
{
	static pfs::shared_ptr<cwt::log> debugLog(new cwt::log(default_out_appender()));
	return *debugLog;
}

cwt::log & info  ()
{
	static pfs::shared_ptr<cwt::log> infoLog(new cwt::log(default_out_appender()));
	return *infoLog;
}

cwt::log & warn  ()
{
	static pfs::shared_ptr<cwt::log> warnLog(new cwt::log(default_err_appender()));
	return *warnLog;
}

cwt::log & error ()
{
	static pfs::shared_ptr<cwt::log> errorLog(new cwt::log(default_err_appender()));
	return *errorLog;
}

cwt::log & fatal ()
{
	static pfs::shared_ptr<cwt::log> fatalLog(new cwt::log(default_err_appender()));
	return *fatalLog;
}

void trace (const pfs::string & text) {	if (log::level() < log::Debug) trace().print(log::Trace, text); }
void debug (const pfs::string & text) { if (log::level() < log::Info)  debug().print(log::Debug, text); }
void info  (const pfs::string & text) { if (log::level() < log::Warn)  info().print(log::Info, text); }
void warn  (const pfs::string & text) { if (log::level() < log::Error) warn().print(log::Warn, text); }
void error (const pfs::string & text) { if (log::level() < log::Fatal) error().print(log::Error, text); }
void fatal (const pfs::string & text) { if (log::level() < log::NoLog) fatal().print(log::NoLog, text); abort(); }

void warn  (int errn, const pfs::string & text)
{
	if (log::level() < log::Error) {
		if (text.isEmpty()) {
			warn().print(log::Warn, __strerror(errn));
		} else {
			warn().print(log::Warn, safeformat(_u8("%s: %s")) % text % __strerror(errn));
		}
	}
}

void error (int errn, const pfs::string & text)
{
	if (log::level() < log::Fatal) {
		if (text.isEmpty()) {
			error().print(log::Error, __strerror(errn));
		} else {
			error().print(log::Error, safeformat(_u8("%s: %s")) % text % __strerror(errn));
		}
	}
}

void fatal (int errn, const pfs::string & text)
{
	if (log::level() < log::NoLog) {
		if (text.isEmpty()) {
			fatal().print(log::Fatal, __strerror(errn));
		} else {
			fatal().print(log::Fatal, safeformat(_u8("%s: %s")) % text % __strerror(errn));
		}
	}
	abort();
}

void log::disconnectAllAppenders ()
{
	trace().disconnectAll();
	debug().disconnectAll();
	info().disconnectAll();
	warn().disconnectAll();
	error().disconnectAll();
	fatal().disconnectAll();
}

void log::restoreDefaultAppenders ()
{
	disconnectAllAppenders();
	trace().connect(default_out_appender());
	debug().connect(default_out_appender());
	info().connect(default_out_appender());
	warn().connect(default_err_appender());
	error().connect(default_err_appender());
	fatal().connect(default_err_appender());
}

pfs::string appender::patternify (log::priority level, const pfs::string & pattern, const pfs::string & msg)
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

void appender::print_helper (log::priority level, const pfs::string & msg)
{
	print(_pattern.isEmpty()
			? msg
			: patternify(level, _pattern, msg));
}

#ifdef __COMMENT__
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

#endif

} // cwt
