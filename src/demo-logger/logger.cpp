#include <iostream>
#include <pfs/string.hpp>
#include <pfs/safeformat.hpp>
#include "pfs/logger.hpp"

int main (int argc, char *argv[])
{
    PFS_UNUSED2(argc, argv);

    pfs::log_trace(_Sf("%s, %s!")("Hello")("World").str());
    pfs::string s(_Sf("%s, %s!")("Hello")("World").str());
    pfs::log_trace(_Sf("%s")(s).str());

//    pfs::stdout_appender stdout_appender;
//    pfs::stderr_appender stderr_appender;
//    stdout_appender.set_pattern(pfs::string("%d{ABSOLUTE} [%p]: %m"));
//    stderr_appender.set_pattern(pfs::string("%d{ABSOLUTE} [%p]: %m"));

//    std::cout << "--All messages will be print with date as ABSOLUTE specifier:" << std::endl;

#if __COMMENT__
    pfs::logger::disconnect_all_appenders();
    pfs::logger::set_priority(log::trace_priority);
    pfs::trace().connect(stdout_appender);
    pfs::debug().connect(stdout_appender);
    pfs::info().connect(stdout_appender);
    pfs::warn().connect(stderr_appender);
    pfs::error().connect(stderr_appender);

    trace("logging trace");
    debug("logging debug");
    info("logging info");
    warn("logging warn");
    error("logging error");

    stdout_appender.set_pattern(pfs::string("%d{DATE} [%p]: %m"));
    stderr_appender.set_pattern(pfs::string("%d{DATE} [%p]: %m"));

    printf("--All messages will be print excluding Trace and Debug with date as DATE specifier:\n");
    log::set_priority(log::info_priority);
    trace("logging trace");
    debug("logging debug");
    info("logging info");
    warn("logging warn");
    error("logging error");

    stdout_appender.set_pattern(pfs::string("%d{ISO8601} [%p]: %m"));
    stderr_appender.set_pattern(pfs::string("%d{ISO8601} [%p]: %m"));
    printf("--Only Warn and Error level messages will be print with date as ISO8601 specifier:\n");
    log::set_priority(log::warn_priority);
    trace("logging trace");
    debug("logging debug");
    info("logging info");
    warn("logging warn");
    error("logging error");

    log::set_priority(log::trace_priority);

    stdout_appender.set_pattern(_l1("%d{%d/%m/%Y %H:%M:%S} [%p]: %m"));
    trace("This is a message dated with format dd/mm/yyyy hh:mm:ss");
    stdout_appender.set_pattern(_l1("%d{ABSOLUTE} [%p]: {%20.30m}"));
    trace("This is a truncated message of logging with Trace priority");
    stdout_appender.set_pattern(_l1("%d{ABSOLUTE} [%p]: {%30m}"));
    trace("Left padding");
    stdout_appender.set_pattern(_l1("%d{ABSOLUTE} [%p]: {%-30m}"));
    trace("Right padding");
#endif

    pfs::logger mylogger;
    pfs::logger_appender & myappender = mylogger.add_appender<pfs::stdout_appender>();
    mylogger.connect(myappender);
    mylogger.trace(_u8("mylogger trace"));

    return EXIT_SUCCESS;
}
