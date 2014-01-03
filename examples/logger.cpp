#include <cstdio>
#include <cwt/logger.hpp>
#include <pfs/string.hpp>
#include <cwt/safeformat.hpp>

int main(int argc, char *argv[])
{
    PFS_CHECK_SIZEOF_TYPES;
    PFS_UNUSED2(argc, argv);

    cwt::log::trace(_Fr("%s, %s!") % "Hello" % "World");
    pfs::string s(_Fr("%s, %s!") % "Hello" % "World");
    cwt::log::trace(_Fr("%s") % s);

    cwt::log::stdio_appender stdlogger;
    stdlogger.setPattern(pfs::string("%d{ABSOLUTE} [%p]: %m"));

    printf("--All messages will be print with date as ABSOLUTE specifier:\n");
    cwt::log::connectAppender(& stdlogger);
    stdlogger.setPriority(cwt::log::Trace);
    cwt::log::trace("logging trace");
    cwt::log::debug("logging debug");
    cwt::log::info("logging info");
    cwt::log::warn("logging warn");
    cwt::log::error("logging error");

    stdlogger.setPattern(pfs::string("%d{DATE} [%p]: %m"));
    printf("--All messages will be print excluding Trace and Debug with date as DATE specifier:\n");
    stdlogger.setPriority(cwt::log::Info);
    cwt::log::trace("logging trace");
    cwt::log::debug("logging debug");
    cwt::log::info("logging info");
    cwt::log::warn("logging warn");
    cwt::log::error("logging error");

    stdlogger.setPattern(pfs::string("%d{ISO8601} [%p]: %m"));
    printf("--Only Warn, Error an Fatal level messages will be print with date as ISO8601 specifier:\n");
    stdlogger.setPriority(cwt::log::Warn);
    cwt::log::trace("logging trace");
    cwt::log::debug("logging debug");
    cwt::log::info("logging info");
    cwt::log::warn("logging warn");
    cwt::log::error("logging error");

    stdlogger.setPriority(cwt::log::Trace);

    stdlogger.setPattern(pfs::string("%d{%d/%m/%Y %H:%M:%S} [%p]: %m"));
    cwt::log::trace("This is a message dated with format dd/mm/yyyy hh:mm:ss");
    stdlogger.setPattern(pfs::string("%d{ABSOLUTE} [%p]: {%20.30m}"));
    cwt::log::trace("This is a truncated message of logging with Trace priority");
    stdlogger.setPattern(pfs::string("%d{ABSOLUTE} [%p]: {%30m}"));
    cwt::log::trace("Left padding");
    stdlogger.setPattern(pfs::string("%d{ABSOLUTE} [%p]: {%-30m}"));
    cwt::log::trace("Right padding");

    cwt::log::disconnectAppender(&stdlogger);

    return EXIT_SUCCESS;
}
