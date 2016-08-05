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
    
    pfs::griotte::context gr;
    
    if (! gr.init()) {
        return -1;
    }
    
    // View font engine information
    //
    pfs::stringlist info;
    gr.font_engine_info(info);

    std::cout << "Underlying font engine specification:" << std::endl;
    std::cout << "-------------------------------------" << std::endl;
    
    for (size_t i = 0; i < info.size(); i += 2) {
        std::cout << info[i] << ": " << info[i + 1] << std::endl;
    }
    
    pfs::griotte::window window(640, 480);
    window.set_title(_u8("Hello, World!"));
    
    while (! window.should_close()) {
        window.repaint();
        gr.poll_events();
    }
    
    return 0;
}
