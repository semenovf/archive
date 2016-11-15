#include <iostream>
#include <pfs/safeformat.hpp>
#include <pfs/sigslot.hpp>
#include <pfs/io/device_manager.hpp>
#include <pfs/io/inet_server.hpp>
#include <pfs/net/inet4_addr.hpp>

using std::cout;
using std::cerr;
using std::endl;

struct handlers : pfs::has_slots<>
{
    void device_ready_read (pfs::io::device d)
    {
        cout << _Sf("%s: device ready to read)")(d.url()).str() << endl; 
    }
    
    void device_opened (pfs::io::device d)
    {
        cout << _Sf("%s: device successfully opened/connected")(d.url()).str() << endl; 
    }
    
	void device_disconnected (pfs::io::device d)
    {
        cerr << _Sf("%s: device closed/disconnected")(d.url()).str() << endl; 
    }
    
    void device_opening (pfs::io::device d)
    {
        cout << _Sf("%s: open/connection device in progress")(d.url()).str() << endl; 
    }
    
    void device_open_failed (pfs::io::device d, pfs::error_code ex)
    {
        if (! d.is_null()) {
            cerr << _Sf("%s: open/connection device error: %s")
                    (d.url())(pfs::to_string(ex)).str() << endl;
        } else {
            cerr << _Sf("I/O error: %s")(pfs::to_string(ex)).str() << endl;
        }
    }
    
	void error (pfs::error_code ex)
    {
        cerr << _Sf("I/O error: %s")(pfs::to_string(ex)).str() << endl;
    }
};

int main (int argc, char * argv[])
{
    int const millis = 100;
    pfs::io::device_manager devman(millis);
    pfs::net::inet4_addr ip(_u8("127.0.0.1"));
    uint16_t port = 10000;
    handlers h;
    
    devman.ready_read.connect(& h, & handlers::device_ready_read);
    devman.opened.connect(& h, & handlers::device_opened);
    devman.disconnected.connect(& h, & handlers::device_disconnected);
    devman.opening.connect(& h, & handlers::device_opening);
    devman.open_failed.connect(& h, & handlers::device_open_failed);
    devman.error.connect(& h, & handlers::error);

    devman.new_server(pfs::io::open_params<pfs::io::udp_server>(ip, port
        , pfs::io::read_write | pfs::io::non_blocking));

    while (true) {
        devman.dispatch();
    }
    
    return 0;
}
