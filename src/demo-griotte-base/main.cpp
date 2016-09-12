#include <iostream>
#include <iomanip>
#include <pfs/thread.hpp>
#include <pfs/griotte/griotte.hpp>

using std::cout;
using std::cerr;
using std::endl;
using namespace pfs::griotte;

void run (application & app, window & win)
{
    PFS_UNUSED(app);
    win.set_title(_u8("Hello"));
    win.resize(400, 300);
    win.move(position_type::center_xy);
    
    scene & sc = win.get_scene();
    
    win.show();
}
    
int main (int argc, char * argv[])
{
    PFS_UNUSED2(argc, argv);
    
    pfs::griotte::application app(& run);
    return app.run();
}
