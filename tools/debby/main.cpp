#include <cwt/app.hpp>
#include <cwt/logger.hpp>
#include "proc.hpp"

using namespace cwt;

int main(int argc, char * argv[])
{
    App app(argc, argv);
    DebbyProc proc(argc > 1 ? String::fromUtf8(argv[1]) : String());
    return app.exec(proc);
}
