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
#include <pfs/platform/strerror.hpp>

namespace pfs {

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

log & trace ()
{
	static pfs::shared_ptr<log> traceLog(new log(default_out_appender()));
	return *traceLog;
}

log & debug ()
{
	static pfs::shared_ptr<log> debugLog(new log(default_out_appender()));
	return *debugLog;
}

log & info  ()
{
	static pfs::shared_ptr<log> infoLog(new log(default_out_appender()));
	return *infoLog;
}

log & warn  ()
{
	static pfs::shared_ptr<log> warnLog(new log(default_err_appender()));
	return *warnLog;
}

log & error ()
{
	static pfs::shared_ptr<log> errorLog(new log(default_err_appender()));
	return *errorLog;
}

log & fatal ()
{
	static pfs::shared_ptr<log> fatalLog(new log(default_err_appender()));
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
			warn().print(log::Warn, pfs::string(platform::strerror(errn)));
		} else {
			pfs::string msg;
			msg << text << ": " << pfs::string(platform::strerror(errn));
			warn().print(log::Warn, msg);
		}
	}
}

void error (int errn, const pfs::string & text)
{
	if (log::level() < log::Fatal) {
		if (text.empty()) {
			error().print(log::Error, pfs::string(platform::strerror(errn)));
		} else {
			pfs::string msg;
			msg << text << pfs::string(platform::strerror(errn));
			error().print(log::Error, msg);
		}
	}
}

void log::print (const notification & nx)
{
    if (nx.count() > 0) {
    	notification::const_iterator it = nx.begin();
    	notification::const_iterator itEnd = nx.end();

        for (; it != itEnd; ++it) {
        	string msg;

            if (it->repetitions() > 1) {
            	string tmp;
            	msg.append(it->text());
            	msg.append(_u8(" (repeat "));
            	msg.append(lexical_cast(it->repetitions(), tmp));
            	msg.append(_u8(" times)"));
                error(msg);
            } else {
                msg = it->text();
            }

            switch (it->type()) {
            case notification_trace: trace(msg); break;
            case notification_debug: debug(msg); break;
            case notification_info:  info(msg);  break;
            case notification_warn:  warn(msg);  break;
            case notification_error: error(msg); break;
            case notification_fatal: fatal(msg); break;
            default: break;
            }
        }
    }
}

void fatal (int errn, const pfs::string & text)
{
	if (log::level() < log::NoLog) {
		if (text.isEmpty()) {
			fatal().print(log::Fatal, pfs::string(platform::strerror(errn)));
		} else {
			pfs::string msg;
			msg << text << pfs::string(platform::strerror(errn));
			fatal().print(log::Fatal, msg);
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

	pfs::string msg1;
	msg1 << "[<!INVALID PATTERN!>]: " << msg;
	return msg1;
}

void appender::print_helper (log::priority level, const pfs::string & msg)
{
	print(_pattern.isEmpty()
			? msg
			: patternify(level, _pattern, msg));
}

} // pfs
