#include <pfs/string.hpp>
#include <pfs/byte_string.hpp>
#include <iostream>

#include "../include/pfs/io/ipx.hpp.old"

using pfs::string;
using namespace pfs::io;
using std::cout;
using std::cerr;
using std::endl;

struct options
{
    bool         isServer;
    ipx_frame_enum  frameType;
    unsigned int port;
};

static const string HelpOpt("--help");
static const string ServerOpt("--server");
static const string FrameTypeOpt("--frame-type=");
static const string AddressOpt("--address=");
static const string PortOpt("--port=");

static const int MaxWS = 25;

static string fmt (const string & s)
{
    string r(s);
    r += string(MaxWS - r.length(), ' ');
    return r;
}

static string fmt (const string & s1, const string & s2)
{
    string r(s1);
    r += s2;
    r += string(MaxWS - r.length(), ' ');
    return r;
}

static void usage (const char * program)
{
    string indent("  ");
	cout << endl << "Usage:" << endl
	     << indent << program << ' ' << HelpOpt << " | OPTIONS" << endl
	     << endl
	     << "Notes:" << endl
	     << indent << "Parameters enclosed in brackets [...] are optional" << endl
	     << endl
	     << "Options: " << endl
	     << indent << fmt(ServerOpt)                       << "run application in server (listener) mode" << endl
	     << indent << fmt(FrameTypeOpt, _l1("FRAME-TYPE")) << "FRAME-TYPE is one of: none | snap | 802.2 | etherII | 802.3 | tr_802.2" << endl
	     << indent << fmt(AddressOpt, _l1("IPX-ADDR"))     << "IPX address in format [XXXXXXXX.]YYYYYYYYYYYY where X and Y are hex numbers" << endl
	     << indent << fmt(PortOpt, _l1("PORT"))            << "port number as decimal number (grater or equals to 0 and less or equals to 65535)" << endl
	     << endl;
}

//static pfs::byte_string __to_hex (const char * orig, size_t count)
//{
//	pfs::byte_string r;
//	pfs::byte_string num;
//
//	for (size_t i = 0; i < count; ++i) {
//		num = pfs::byte_string::toString(byte_t(orig[i]), 16);
//
//		if (num.size() < 2) {
//			r.append(1, '0');
//		}
//		r.append(num);
//
//		if (i < count - 1)
//			r.append(1, ' ');
//	}
//	return r;
//}

static void dump_opts (const options & opts)
{
    cout << "Mode      : " << (opts.isServer ? "server" : "client") << endl
         << "Frame type: " << ipx_addr::frameTypeToCanonicalName(opts.frameType)
                           << " [" << ipx_addr::frameTypeToString(opts.frameType) << ']' << endl
         << "Port      : " << opts.port << endl;
}

static bool parse_opts (int argc, char * argv[], options * opts)
{
    std::string errmsg;

    if (argc < 2) {
        usage(argv[0]);
        return false;
    }

    //
    // Initialize opts with default values
    //
    opts->isServer  = false;
    opts->frameType = IpxFrameNone;
    opts->port      = -1;

    for (int i = 1; i < argc; ++i) {
        string opt(argv[i]);

        if (opt == HelpOpt) {
            usage(argv[0]);
            return true;
        } else if (opt == ServerOpt) {
            opts->isServer = true;
        } else if (opt.startsWith(FrameTypeOpt)) {
            string value = opt.right(opt.length() - FrameTypeOpt.length());

            if (value == "none")
                opts->frameType = IpxFrameNone;
            else if (value == "snap")
                opts->frameType = IpxFrameSnap;
            else if (value == "802.2")
                opts->frameType = IpxFrame8022;
            else if (value == "etherII")
                opts->frameType = IpxFrameEtherII;
            else if (value == "802.3")
                opts->frameType = IpxFrame8023;
            else if (value == "tr_802.2")
                opts->frameType = IpxFrameTr8022;
            else {
                cerr << "Bad frame type: " << value << endl;
                return false;
            }
        } else if (opt.startsWith(PortOpt)) {
            string value = opt.right(opt.length() - PortOpt.length());
            opts->port = value.toUnsignedInt();
        }
    }

    if (opts->port <= 1024 || opts->port > 0xFFFF) {
        cerr << "Bad value for port" << endl;
        return false;
    }

    dump_opts(*opts);

    return true;
}

int main(int argc, char *argv[])
{
    options opts;

    if (! parse_opts(argc, argv, & opts))
        return -1;


//	bool is_server = false;
//
//    if (strcmp(argv[1], "-client") == 0) {
//        if (argc < 4) {
//        	std::cerr <<  "Too few arguments" << std::endl;
//        	usage(argv[0]);
//        	return -1;
//        }
//    	is_server = true;
//    }
//
//    if (strcmp(argv[1], "-server") == 0) {
//    	is_server = true;
//    }
//
//    int ipxPort = 51234;
//
//    //ipx_addr   ipxAddr(argv[2]);
//    ipx_addr   ipxAddr(0x6C, 0xF0, 0x49, 0x6F, 0x71, 0xFD);
//
//    //ipx_addr   serverAddr = !is_server ? ipx_addr(argv[3]) : ipx_addr();
//    ipx_addr serverAddr = !is_server ? ipx_addr(0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF) : ipx_addr();
//
//    if (is_server) {
//    	ipx_server ipxServer;
//    	ipxServer.open(ipxAddr, ipxPort, pfs::io::IpxFrame8022);
//
//        if (!ipxServer.opened()) {
//        	ipxServer.logErrors();
//        	return -1;
//        }
//
//    	do {
//    		ipx_socket ipxPeer;
//
//    		if (ipxServer.accept(& ipxPeer)) {
//				char bytes[256];
//				ssize_t nbytes = ipxPeer.read(bytes, 256);
//				if (nbytes < 0) {
//					ipxPeer.logErrors();
//					break;
//				}
//
//				if (nbytes > 0) {
//					pfs::byte_string hex(__to_hex(bytes, size_t(nbytes)));
//					std::cout << "bytes read: " << nbytes << "[" << bytes << "][" << hex << ']' << std::endl;
//
//					ipxPeer.write(bytes, nbytes);
//				}
//    		}
//    	} while (true);
//    } else {
//    	ipx_socket ipxSocket;
//    	ipxSocket.open(ipxAddr, ipxPort, pfs::io::IpxFrame8022);
//
//        if (!ipxSocket.opened()) {
//        	ipxSocket.logErrors();
//        	return -1;
//        }
//
//        if (!ipxSocket.connect(serverAddr, ipxPort)) {
//        	ipxSocket.logErrors();
//        } else {
//        	pfs::byte_string bytes("Hello, World!");
//        	ipxSocket.write(bytes);
//        }
//        ipxSocket.close();
//    }

    return 0;
}
