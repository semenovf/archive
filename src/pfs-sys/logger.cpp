/**
 * @file   logger.cpp
 * @author wladt
 * @date   Feb 13, 2013 11:53:01 AM
 *
 * @brief
 */

#include <pfs/shared_ptr.hpp>
#include "pfs/error_code.hpp"
#include "logger_p.hpp"

namespace pfs {

const pfs::string logger::default_pattern("%d{ABSOLUTE} [%p]: %m");
const pfs::string logger::no_pattern;

struct logger_initializer
{
	logger_initializer (logger & l)
	{
		l.connect(logger::trace_priority, shared_ptr<appender>(new stdout_appender));
		l.connect(logger::debug_priority, shared_ptr<appender>(new stdout_appender));
		l.connect(logger::info_priority , shared_ptr<appender>(new stdout_appender));
		l.connect(logger::warn_priority , shared_ptr<appender>(new stderr_appender));
		l.connect(logger::error_priority, shared_ptr<appender>(new stderr_appender));
		l.connect(logger::fatal_priority, shared_ptr<appender>(new stderr_appender));
	}
};

logger & logger::default_logger ()
{
	static logger __default_logger;
	static logger_initializer li(__default_logger);
	return __default_logger;
}

void logger::connect (shared_ptr<appender> pappender)
{
	_emitters[trace_priority].connect(pappender.get(), & appender::print_helper);
	_emitters[debug_priority].connect(pappender.get(), & appender::print_helper);
	_emitters[info_priority].connect(pappender.get(), & appender::print_helper);
	_emitters[warn_priority].connect(pappender.get(), & appender::print_helper);
	_emitters[error_priority].connect(pappender.get(), & appender::print_helper);
	_emitters[fatal_priority].connect(pappender.get(), & appender::print_helper);

}

#if __COMMENT__

void log::print (const notification & nx)
{
    if (nx.count() > 0) {
    	notification::const_iterator it = nx.begin();
    	notification::const_iterator itEnd = nx.end();

        for (; it != itEnd; ++it) {
        	string msg;

            if (it->repetitions() > 1) {
            	msg.append(it->text());
            	msg.append(_u8(" (repeat "));
            	msg.append(to_string(it->repetitions()));
            	msg.append(_u8(" times)"));
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

#endif


string appender::patternify (int level, const string & pattern, const string & msg)
{
	pattern_context ctx;
	ctx.level = level;
	ctx.msg = & msg;
	fsm::fsm<string> fsm(pattern_fsm, &ctx);

	fsm::fsm<string>::result_type r = fsm.exec(0, pattern.cbegin(), pattern.cend());

	if (r.first && r.second == pattern.cend()) {
		return ctx.result;
	}

	string msg1;
	msg1.append(_u8("[<!INVALID PATTERN!>]: "));
	msg1.append(msg);
	return msg1;
}

} // pfs
