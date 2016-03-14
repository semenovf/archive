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

void logger::clear ()
{
	disconnect_all();
	appender_vector_type::iterator it    = _d->_appenders.begin();
	appender_vector_type::iterator itEnd = _d->_appenders.end();

	for (; it != itEnd; ++it) {
		delete *it;
	}

	_d->_appenders.clear();
}

struct logger_initializer
{
	logger_initializer (logger & l)
	{
		logger_appender & ap1 = l.add_appender<stdout_appender>();
		logger_appender & ap2 = l.add_appender<stderr_appender>();
		l.connect(logger::trace_priority, ap1);
		l.connect(logger::debug_priority, ap1);
		l.connect(logger::info_priority , ap1);
		l.connect(logger::warn_priority , ap2);
		l.connect(logger::error_priority, ap2);
		l.connect(logger::fatal_priority, ap2);
	}
};

logger & logger::default_logger ()
{
	static logger __default_logger;
	static logger_initializer li(__default_logger);
	return __default_logger;
}


void logger::connect (appender_type & ap)
{
	_d->_emitters[trace_priority].connect(& ap, & logger_appender::print_helper);
	_d->_emitters[debug_priority].connect(& ap, & logger_appender::print_helper);
	_d->_emitters[info_priority].connect(& ap, & logger_appender::print_helper);
	_d->_emitters[warn_priority].connect(& ap, & logger_appender::print_helper);
	_d->_emitters[error_priority].connect(& ap, & logger_appender::print_helper);
	_d->_emitters[fatal_priority].connect(& ap, & logger_appender::print_helper);
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


string logger_appender::patternify (int level, const string & pattern, const string & msg)
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
