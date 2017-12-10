#include <pfs/string.hpp>
#include <pfs/byte_string.hpp>
#include <pfs/random.hpp>
#include <iostream>

#include "../include/pfs/io/udp.hpp.old"


using pfs::string;
using pfs::byte_string;
using pfs::io::device;
using pfs::io::inet4_addr;
using pfs::io::udp_socket;
using pfs::io::udp_server;
using std::cout;
using std::cerr;
using std::endl;

struct options
{
    bool isServer;
    inet4_addr selfAddr;
    inet4_addr serverAddr;
};

static const string HelpOpt("--help");
static const string ServerOpt("--server");
static const string SelfAddressOpt("--addr=");
static const string ServerAddressOpt("--server-addr=");

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

static byte_string random_bytes (pfs::random & rnd, size_t maxLen)
{
    static byte_string alphabet("=+-?!@#$%&*0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
    byte_string r;
    size_t len = rnd.rand() % maxLen;

    while (!len)
        len = rnd.rand() % maxLen;

    for (size_t i = 0; i < len; ++i)
        r << alphabet[rnd.rand() % alphabet.length()];

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
	     << indent << "INET-ADDR must be in format: a[.b[.c[.d[:port]]]]" << endl
	     << endl
	     << "Options: " << endl
	     << indent << fmt(ServerOpt)                          << "run application in server (listener) mode" << endl
	     << indent << fmt(SelfAddressOpt, _l1("INET-ADDR"))   << "self address" << endl
	     << indent << fmt(ServerAddressOpt, _l1("INET-ADDR")) << "server address (for client only)" << endl
	     << endl;
}

static pfs::byte_string __to_hex (const char * orig, size_t count)
{
	pfs::byte_string r;
	pfs::byte_string num;

	for (size_t i = 0; i < count; ++i) {
		num = pfs::byte_string::toString(byte_t(orig[i]), 16);

		if (num.size() < 2) {
			r.append(1, '0');
		}
		r.append(num);

		if (i < count - 1)
			r.append(1, ' ');
	}
	return r;
}

static void dump_opts (const options & opts)
{
    cout << "Mode          : " << (opts.isServer ? "server" : "client") << endl
         << "Self address  : " << opts.selfAddr.toDecString() << endl;

    if (!opts.isServer) {
        cout << "Server address: " << opts.serverAddr.toDecString() << endl;
    }
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

    for (int i = 1; i < argc; ++i) {
        string opt(argv[i]);

        if (opt == HelpOpt) {
            usage(argv[0]);
            return true;
        } else if (opt == ServerOpt) {
            opts->isServer = true;
        } else if (opt.startsWith(SelfAddressOpt)) {
            string value = opt.right(opt.length() - SelfAddressOpt.length());
            opts->selfAddr = inet4_addr::make(value);
        } else if (opt.startsWith(ServerAddressOpt)) {
            string value = opt.right(opt.length() - ServerAddressOpt.length());
            opts->serverAddr = inet4_addr::make(value);
        }
    }

    if (!opts->selfAddr.isValid()) {
        cerr << "Bad address or it is not specified" << endl;
        return false;
    }

    if (!opts->isServer && !opts->serverAddr.isValid()) {
        cerr << "Bad server address or it is not specified" << endl;
        return false;
    }

    dump_opts(*opts);

    return true;
}

void server (const options & opts)
{
    udp_server sock;
    sock.open(opts.selfAddr, device::ReadWrite);

    if (!sock.opened()) {
        sock.logErrors();
        return;
    }

    cout << "Listening on address: " << sock.address().toDecString() << endl;

    do {
        udp_socket peer;

        if (sock.accept(& peer)) {
            byte_string bytes = peer.read(peer.available());

//            cout << "Data received from address: " << peer.address().toDecString() << endl;

            if (peer.isError()) {
                peer.logErrors();
                break;
            }

            if (bytes.length() > 0) {

                if (bytes == "exit") {
                    cout << "Bye!" << endl;
                    break;
                }

                byte_string hex(__to_hex(bytes.c_str(), bytes.length()));
                cout << "bytes rx: " << bytes.length() << " [" << bytes << "][" << hex << ']' << endl;

                ssize_t bw = peer.write(bytes);
                PFS_ASSERT(bw == static_cast<ssize_t>(bytes.length()));
                //cout << "bytes tx: " << bw << endl;
            }
        }
    } while (true);
}

void client (const options & opts, int niter)
{
    int txPackets = 0;
    int rxPackets = 0;
    int rxBadPackets = 0;

    udp_socket sock;
    sock.open(opts.selfAddr, device::ReadWrite);

    if (!sock.opened()) {
        sock.logErrors();
        return;
    }

    if (!sock.connect(opts.serverAddr)) {
        sock.logErrors();
        return;
    }

    cout << "Socket address: " << sock.address().toDecString() << endl;

    pfs::random rnd;

    for (int i = 0; i < niter; ++i) {
        byte_string bytes = random_bytes(rnd, 32);

        if (static_cast<ssize_t>(bytes.length()) == sock.write(bytes))
            ++txPackets;

        byte_string hex(__to_hex(bytes.c_str(), bytes.length()));
        cout << "bytes tx: " << bytes.length() << " [" << bytes << "][" << hex << ']' << endl;

        while (!sock.available())
            ;

        if (sock.available() > 0) {
            byte_string bytes1 = sock.read(sock.available());
            ++rxPackets;

            if (bytes1 != bytes) {
                ++rxBadPackets;
            }

            byte_string hex1(__to_hex(bytes1.c_str(), bytes1.length()));

            cout << "bytes rx: " << bytes1.length() << " [" << bytes1 << "][" << hex1 << ']' << endl;
        } else if (sock.isError()) {
            sock.logErrors();
            break;
        }
    }

    sock.write("exit", 4);

    cout << "Total packets sent    : " << txPackets << endl;
    cout << "Total packets received: " << rxPackets << endl;
    cout << "Total bad packets received: " << rxBadPackets << endl;

    sock.close();
}

int main(int argc, char *argv[])
{
    options opts;

    if (! parse_opts(argc, argv, & opts))
        return -1;

    if (opts.isServer)
        server(opts);
    else
        client(opts, 100000);

    return 0;
}
