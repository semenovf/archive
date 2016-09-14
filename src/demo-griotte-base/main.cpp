#include <iostream>
#include <iomanip>
#include <pfs/thread.hpp>
#include <pfs/griotte/griotte.hpp>
#include <pfs/griotte/push_button.hpp>
#include <pfs/griotte/vertical_layout.hpp>

using std::cout;
using std::cerr;
using std::endl;
using namespace pfs::griotte;

void run (application & app, window & win)
{
    PFS_UNUSED(app);
    scene & sc = win.get_scene();
    container & root = sc.get_root_container();
    
    vertical_layout & vlayout1 = root.create_layout<vertical_layout>();
    
    push_button & button1 = root.create_widget<push_button>();
    push_button & button2 = root.create_widget<push_button>();
    push_button & button3 = root.create_widget<push_button>();
    
    vlayout1.add_widget(button1);
    vlayout1.add_widget(button2);
    vlayout1.add_widget(button3);

    win.set_title(_u8("Hello"));
    win.resize(400, 300);
    win.move(position_type::center_xy);
    
    win.show();
}
    
int main (int argc, char * argv[])
{
    PFS_UNUSED2(argc, argv);
    
    pfs::griotte::application app(& run);
    return app.run();
}
