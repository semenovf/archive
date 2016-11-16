#include <iostream>
#include <pfs/byte_string.hpp>
#include <pfs/safeformat.hpp>
#include <pfs/sigslot.hpp>
#include <pfs/io/device_manager.hpp>
#include <pfs/io/inet_server.hpp>
#include <pfs/net/inet4_addr.hpp>

using std::cout;
using std::cerr;
using std::endl;

//
// Use this command (netcat) to test UDP server work properly
//
// $ echo -n "hello" | nc -w 1 -u -4 localhost 10000
//

#define __SIMPLE_SERVER__ 0
#define __POLL_SERVER__   0

extern "C" int simple_udp_server (int argc, char ** argv);
extern "C" int poll_udp_server (int argc, char ** argv);

struct handlers : pfs::has_slots<>
{
    void device_ready_read (pfs::io::device d)
    {
        //cout << _Sf("%s: device ready to read)")(d.url()).str() << endl;
        pfs::byte_string data;
        pfs::error_code ex = d.read(data);
        
        if (ex) {
            cerr << _Sf("read device error: %s")(pfs::to_string(ex)).str() << endl;
        } else {
            cout << "read: " << data << endl;
        }
    }
    
    void device_opened (pfs::io::device d)
    {
        cout << _Sf("%s: device successfully opened/connected")(d.url()).str() << endl; 
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
            cerr << _Sf("open device error: %s")(pfs::to_string(ex)).str() << endl;
        }
    }
    
	void device_disconnected (pfs::io::device d)
    {
        cerr << _Sf("%s: device closed/disconnected")(d.url()).str() << endl; 
    }

    void server_opened (pfs::io::server s)
    {
        cout << _Sf("%s: server successfully opened")(s.url()).str() << endl; 
    }

    void server_opening (pfs::io::server s)
    {
        cout << _Sf("%s: open server in progress")(s.url()).str() << endl; 
    }
    
    void server_open_failed (pfs::io::server s, pfs::error_code ex)
    {
        if (! s.is_null()) {
            cerr << _Sf("%s: open server error: %s")
                    (s.url())(pfs::to_string(ex)).str() << endl;
        } else {
            cerr << _Sf("open server error: %s")(pfs::to_string(ex)).str() << endl;
        }
    }
    
	void error (pfs::error_code ex)
    {
        cerr << _Sf("I/O error: %s")(pfs::to_string(ex)).str() << endl;
    }
};

int main (int argc, char * argv[])
{
#if __SIMPLE_SERVER__    
    
    return simple_udp_server(argc, argv);
    
#elif __POLL_SERVER__
    
    return poll_udp_server(argc, argv);
    
#else
    PFS_UNUSED2(argc, argv);
    
    int const millis = 100;
    pfs::io::device_manager devman(millis);
    pfs::net::inet4_addr ip(pfs::net::inet4_addr::any_addr_value);
    uint16_t port = 10000;
    handlers h;
    
    devman.ready_read.connect(& h, & handlers::device_ready_read);
    devman.opened.connect(& h, & handlers::device_opened);
    devman.opening.connect(& h, & handlers::device_opening);
    devman.open_failed.connect(& h, & handlers::device_open_failed);
    devman.disconnected.connect(& h, & handlers::device_disconnected);
    devman.server_opened.connect(& h, & handlers::server_opened);
    devman.server_opening.connect(& h, & handlers::server_opening);
    devman.server_open_failed.connect(& h, & handlers::server_open_failed);
    devman.error.connect(& h, & handlers::error);

    devman.new_server(pfs::io::open_params<pfs::io::udp_server>(ip, port
        , pfs::io::read_write | pfs::io::non_blocking));

    while (true) {
        devman.dispatch();
    }
    
    return 0;

#endif    
}
