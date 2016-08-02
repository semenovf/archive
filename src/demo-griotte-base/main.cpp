#include <iostream>
#include <pfs/thread.hpp>
#include <pfs/griotte/griotte.hpp>

using std::cout;
using std::cerr;
using std::endl;

int main (int argc, char * argv[])
{
    PFS_UNUSED2(argc, argv);
    
    if (! pfs::griotte::init()) {
        cerr << "Initialization failed" << endl;
        return -1;
    }
    
    pfs::griotte::window window(640, 480);
    window.set_title(_u8("Hello, World!"));
    
    while (! window.should_close()) {
        window.repaint();
        pfs::griotte::poll_events();
    }
    
    pfs::griotte::finish();

    return 0;
}
