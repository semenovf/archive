#include <iostream>
#include <iomanip>
#include <pfs/thread.hpp>
#include <pfs/griotte/griotte.hpp>
#include <pfs/griotte/push_button.hpp>
#include <pfs/griotte/vertical_layout.hpp>

using std::cout;
using std::cerr;
using std::endl;
using namespace pfs;

//using namespace pfs::griotte;

//void run (application & app, window & win)
//{
//    PFS_UNUSED(app);
//    scene & sc = win.get_scene();
//    widget & root = sc.root_widget();
//    
//    vertical_layout & vlayout1 = root.create_layout<vertical_layout>();
//    
//    push_button & button2 = root.create_widget<push_button>();
//    push_button & button3 = root.create_widget<push_button>();
//    
//    vlayout1.add_widget(button1);
//    vlayout1.add_widget(button2);
//    vlayout1.add_widget(button3);
//}
    

void init_scene (pfs::griotte::scene & scene)
{
    griotte::push_button & button1 = scene.create_widget<griotte::push_button>();
    griotte::push_button & button2 = scene.create_widget<griotte::push_button>();
    griotte::push_button & button3 = scene.create_widget<griotte::push_button>();
}

int main (int argc, char * argv[])
{
    PFS_UNUSED2(argc, argv);

    griotte::scene scene;
    init_scene(scene);
            
    griotte::application app;
    griotte::window win;
    
    win.set_title(_u8("Hello"));
    win.resize(400, 300);
    win.move(griotte::position_type::center_xy);
    win.show(scene);
    
    return app.run();
}
