#include <cstdio>
#include "../include/cwt/logger.hpp"
#include "../include/cwt/string.hpp"
#include "../include/cwt/safeformat.hpp"

using namespace cwt;

int main(int argc, char *argv[])
{
    CWT_CHECK_SIZEOF_TYPES;
    CWT_UNUSED2(argc, argv);

    Logger::trace(_Fr("%s, %s!") % "Hello" % "World");
    String s(_Fr("%s, %s!") % "Hello" % "World");
    Logger::trace(_Fr("%s") % s);

    StdioLogAppender stdlogger;
    stdlogger.setPattern(String("%d{ABSOLUTE} [%p]: %m"));

    printf("--All messages will be print with date as ABSOLUTE specifier:\n");
    Logger::connectAppender(&stdlogger);
    stdlogger.setPriority(Logger::Trace);
    Logger::trace("logging trace");
    Logger::debug("logging debug");
    Logger::info("logging info");
    Logger::warn("logging warn");
    Logger::error("logging error");

    stdlogger.setPattern(String("%d{DATE} [%p]: %m"));
    printf("--All messages will be print excluding Trace and Debug with date as DATE specifier:\n");
    stdlogger.setPriority(Logger::Info);
    Logger::trace("logging trace");
    Logger::debug("logging debug");
    Logger::info("logging info");
    Logger::warn("logging warn");
    Logger::error("logging error");

    stdlogger.setPattern(String("%d{ISO8601} [%p]: %m"));
    printf("--Only Warn, Error an Fatal level messages will be print with date as ISO8601 specifier:\n");
    stdlogger.setPriority(Logger::Warn);
    Logger::trace("logging trace");
    Logger::debug("logging debug");
    Logger::info("logging info");
    Logger::warn("logging warn");
    Logger::error("logging error");

    stdlogger.setPriority(Logger::Trace);

    stdlogger.setPattern(String("%d{%d/%m/%Y %H:%M:%S} [%p]: %m"));
    Logger::trace("This is a message dated with format dd/mm/yyyy hh:mm:ss");
    stdlogger.setPattern(String("%d{ABSOLUTE} [%p]: {%20.30m}"));
    Logger::trace("This is a truncated message of logging with Trace priority");
    stdlogger.setPattern(String("%d{ABSOLUTE} [%p]: {%30m}"));
    Logger::trace("Left padding");
    stdlogger.setPattern(String("%d{ABSOLUTE} [%p]: {%-30m}"));
    Logger::trace("Right padding");

    Logger::disconnectAppender(&stdlogger);

    return EXIT_SUCCESS;
}
