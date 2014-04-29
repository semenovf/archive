#include <cstdio>
#include <cwt/logger.hpp>
#include <pfs/string.hpp>
#include <cwt/safeformat.hpp>

int main(int argc, char *argv[])
{
    PFS_CHECK_SIZEOF_TYPES;
    PFS_UNUSED2(argc, argv);

    cwt::trace(_Fr("%s, %s!") % "Hello" % "World");
    pfs::string s(_Fr("%s, %s!") % "Hello" % "World");
    cwt::trace(_Fr("%s") % s);

    cwt::stdout_appender stdout_appender;
    cwt::stderr_appender stderr_appender;
    stdout_appender.setPattern(pfs::string("%d{ABSOLUTE} [%p]: %m"));
    stderr_appender.setPattern(pfs::string("%d{ABSOLUTE} [%p]: %m"));

    printf("--All messages will be print with date as ABSOLUTE specifier:\n");

    cwt::log::disconnectAllAppenders();
    cwt::log::setPriority(cwt::log::Trace);
    cwt::trace().connect(stdout_appender);
    cwt::debug().connect(stdout_appender);
    cwt::info().connect(stdout_appender);
    cwt::warn().connect(stderr_appender);
    cwt::error().connect(stderr_appender);

    cwt::trace("logging trace");
    cwt::debug("logging debug");
    cwt::info("logging info");
    cwt::warn("logging warn");
    cwt::error("logging error");

    stdout_appender.setPattern(pfs::string("%d{DATE} [%p]: %m"));
    stderr_appender.setPattern(pfs::string("%d{DATE} [%p]: %m"));

    printf("--All messages will be print excluding Trace and Debug with date as DATE specifier:\n");
    cwt::log::setPriority(cwt::log::Info);
    cwt::trace("logging trace");
    cwt::debug("logging debug");
    cwt::info("logging info");
    cwt::warn("logging warn");
    cwt::error("logging error");

    stdout_appender.setPattern(pfs::string("%d{ISO8601} [%p]: %m"));
    stderr_appender.setPattern(pfs::string("%d{ISO8601} [%p]: %m"));
    printf("--Only Warn and Error level messages will be print with date as ISO8601 specifier:\n");
    cwt::log::setPriority(cwt::log::Warn);
    cwt::trace("logging trace");
    cwt::debug("logging debug");
    cwt::info("logging info");
    cwt::warn("logging warn");
    cwt::error("logging error");

    cwt::log::setPriority(cwt::log::Trace);

    stdout_appender.setPattern(_l1("%d{%d/%m/%Y %H:%M:%S} [%p]: %m"));
    cwt::trace("This is a message dated with format dd/mm/yyyy hh:mm:ss");
    stdout_appender.setPattern(_l1("%d{ABSOLUTE} [%p]: {%20.30m}"));
    cwt::trace("This is a truncated message of logging with Trace priority");
    stdout_appender.setPattern(_l1("%d{ABSOLUTE} [%p]: {%30m}"));
    cwt::trace("Left padding");
    stdout_appender.setPattern(_l1("%d{ABSOLUTE} [%p]: {%-30m}"));
    cwt::trace("Right padding");

    return EXIT_SUCCESS;
}
