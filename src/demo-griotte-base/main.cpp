#include <iostream>
#include <iomanip>
#include <pfs/thread.hpp>
#include <pfs/griotte/griotte.hpp>

using std::cout;
using std::cerr;
using std::endl;

int main (int argc, char * argv[])
{
    PFS_UNUSED2(argc, argv);
    
    pfs::griotte::app app;
    
    if (! app.ready()) {
        return -1;
    }
    
    app.activated.connect();
    app.exec();
    
//    pfs::griotte::window window(640, 480);
//    window.set_title(_u8("Hello, World!"));
//    
//    while (! window.should_close()) {
//        window.repaint();
//        gr.poll_events();
//    }
    
    return 0;
}
