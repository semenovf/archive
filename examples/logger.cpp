#include <cstdio>
#include "pfs/logger.hpp"
#include <pfs/string.hpp>
#include <pfs/safeformat.hpp>

using namespace pfs;

int main(int argc, char *argv[])
{
    PFS_UNUSED2(argc, argv);

    trace(safeformat("%s, %s!")("Hello")("World")());
    pfs::string s(safeformat("%s, %s!")("Hello")("World")());
    trace(safeformat("%s")(s)());

    stdout_appender stdout_appender;
    stderr_appender stderr_appender;
    stdout_appender.set_pattern(pfs::string("%d{ABSOLUTE} [%p]: %m"));
    stderr_appender.set_pattern(pfs::string("%d{ABSOLUTE} [%p]: %m"));

    printf("--All messages will be print with date as ABSOLUTE specifier:\n");

    log::disconnect_all_appenders();
    log::set_priority(log::trace_priority);
    trace().connect(stdout_appender);
    debug().connect(stdout_appender);
    info().connect(stdout_appender);
    warn().connect(stderr_appender);
    error().connect(stderr_appender);

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

    return EXIT_SUCCESS;
}
