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
#include <cwt/platform.hpp>

namespace cwt {

pfs::string log::DefaultPattern("%d{ABSOLUTE} [%p]: %m");
pfs::string log::NoPattern;
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
			warn().print(log::Warn, cwt::platform::strerror(errn));
		} else {
			warn().print(log::Warn, safeformat(_u8("%s: %s")) % text % cwt::platform::strerror(errn));
		}
	}
}

void error (int errn, const pfs::string & text)
{
	if (log::level() < log::Fatal) {
		if (text.isEmpty()) {
			error().print(log::Error, cwt::platform::strerror(errn));
		} else {
			error().print(log::Error, safeformat(_u8("%s: %s")) % text % cwt::platform::strerror(errn));
		}
	}
}

void fatal (int errn, const pfs::string & text)
{
	if (log::level() < log::NoLog) {
		if (text.isEmpty()) {
			fatal().print(log::Fatal, cwt::platform::strerror(errn));
		} else {
			fatal().print(log::Fatal, safeformat(_u8("%s: %s")) % text % cwt::platform::strerror(errn));
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

} // cwt
