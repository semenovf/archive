#include "pfs/io/serial.hpp"
#include <iostream>

using std::cout;
using std::cerr;
using std::endl;

static void usage(const char *program)
{
	cout << endl << "Usage:\n\t" << program << " serial-port [FORMAT]" << endl;
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
    	cerr << "Need to specify serial device path (name on Windows)" << endl;
    	usage(argv[0]);
    	return -1;
    }

    pfs::io::serial port;

    port.open(_u8(argv[1]), pfs::io::device::ReadWrite);

    if (!port.opened()) {
        port.print(cerr);
    	return -1;
    }

    cout << "Setting to 7E1 (baud rate = 9600)..." << endl;

    if (port.setBaudrate(pfs::io::serial::BR9600)
    		&& port.setDatabits(pfs::io::serial::DB7)
    		&& port.setStopbits(pfs::io::serial::SB1)
    		&& port.setParity(pfs::io::serial::ParityEven)
    		&& port.setFlowControl(pfs::io::serial::SoftwareFlowControl)) {
    	;
    }

    cout << port.toString() << endl;

    cout << "Setting to 8N1..." << endl;
    port.setFormat(pfs::io::serial::Format8N1);
    cout << port.toString() << endl;

    cout << "Setting to 7O1..." << endl;
    port.setFormat(_u8("7O1"));
    cout << port.toString() << endl;

    if (argc > 2) {
    	cout << "Setting to " << argv[2] << "..." << endl;
    	port.setFormat(_u8(argv[2]));
    	cout << port.toString() << endl;
    }

    port.close();

    return 0;
}
