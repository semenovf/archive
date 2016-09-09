#include <iostream>
#include <iomanip>
#include <pfs/thread.hpp>
#include <pfs/griotte/griotte.hpp>

using std::cout;
using std::cerr;
using std::endl;

struct main_activity : public pfs::has_slots<>
{
    void run (pfs::griotte::application & app, pfs::griotte::window & win)
    {
        win.set_title(_u8("Hello"));
        win.resize(400, 300);
        win.show();
    }
};

int main (int argc, char * argv[])
{
    PFS_UNUSED2(argc, argv);
    
    main_activity act;
    pfs::griotte::application app;
    app.activated.connect(& act, & main_activity::run);
    
    return app.run();
}
