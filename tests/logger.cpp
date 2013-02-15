#include <cwt/test.h>
#include <cwt/logger.hpp>
#include <cstdio>

using namespace cwt;

int main(int argc, char *argv[])
{
    CWT_CHECK_SIZEOF_TYPES;
    CWT_UNUSED2(argc, argv);
    CWT_BEGIN_TESTS(1);

    StdioLogAppender stdlogger;
    stdlogger.setPattern(_U("[%p: %m] Hello!"));

    printf("--All messages will be print:\n");
    Logger::connectAppender(&stdlogger);
    stdlogger.setPriority(Logger::Trace);
    Logger::trace("logging trace");
    Logger::debug("logging debug");
    Logger::info("logging info");
    Logger::warn("logging warn");
    Logger::error("logging error");
    Logger::fatal("logging fatal");

    printf("--All messages will be print excluding Trace and Debug:\n");
    stdlogger.setPriority(Logger::Info);
    Logger::trace("logging trace");
    Logger::debug("logging debug");
    Logger::info("logging info");
    Logger::warn("logging warn");
    Logger::error("logging error");
    Logger::fatal("logging fatal");

    printf("--Only Warn, Error an Fatal level messages will be print:\n");
    stdlogger.setPriority(Logger::Warn);
    Logger::trace("logging trace");
    Logger::debug("logging debug");
    Logger::info("logging info");
    Logger::warn("logging warn");
    Logger::error("logging error");
    Logger::fatal("logging fatal");

    Logger::disconnectAppender(&stdlogger);

    CWT_END_TESTS;
}
