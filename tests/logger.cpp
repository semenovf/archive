#include <cwt/test.h>
#include <cwt/logger.hpp>

using namespace cwt;


int main(int argc, char *argv[])
{
    CWT_CHECK_SIZEOF_TYPES;
    CWT_UNUSED2(argc, argv);
    CWT_BEGIN_TESTS(1);

    StdioLogAppender stdlogger;

    Logger::connectAppender(&stdlogger);
    Logger::trace("logging trace");
    Logger::debug("logging debug");
    Logger::info("logging info");
    Logger::warn("logging warn");
    Logger::error("logging error");
    Logger::fatal("logging fatal");

    Logger::setLevel(Logger::Info);
    Logger::trace("logging trace");
    Logger::debug("logging debug");
    Logger::info("logging info");
    Logger::warn("logging warn");
    Logger::error("logging error");
    Logger::fatal("logging fatal");

    Logger::disconnectAppender(&stdlogger);

    CWT_END_TESTS;
}
