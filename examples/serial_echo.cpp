#include <pfs/string.hpp>
#include <pfs/byte_string.hpp>
#include <cstring>
#include <cstdio>
#include <iostream>
#include "pfs/io/serial.hpp"

static void usage (const char *program)
{
	printf("\nUsage:\n\t%s -client|-server serial-port [SPEED] [FORMAT]\n", program);
	printf("\tdefault format is 8N1 and speed is 9600\n");
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

int main(int argc, char *argv[])
{
	bool is_server = false;
	bool port_ok = true;

    if (argc < 3) {
    	std::cerr <<  "Too few arguments" << std::endl;
    	usage(argv[0]);
    	return -1;
    }

    if (strcmp(argv[1], "-server") == 0)
    	is_server = true;

    pfs::io::serial port;

    port.open(_u8(argv[2]), pfs::io::device::ReadWrite | pfs::io::device::NonBlocking);

//    port.open(_u8(argv[2]), cwt::io::device::ReadWrite
//    		, 115200, 8, 1, cwt::io::serial::ParityNone);

    if (!port.opened()) {
    	return -1;
    }

    if (argc > 3) {
    	pfs::string br_str(argv[3]);
    	bool ok = true;
    	unsigned int br = br_str.toUnsignedInt(&ok);

    	if (!ok) {
    		std::cerr << "Bad baud rate value" << std::endl;
    		port.close();
    		return -1;
    	}
    	port_ok |= port.setBaudrate(br);
    } else {
    	port_ok |= port.setBaudrate(pfs::io::serial::BR9600);
    }

    if (argc > 4)
    	port_ok |= port.setFormat(_u8(argv[4]));
    else
    	port_ok |= port.setFormat(pfs::io::serial::Format8N1);

    if (!port_ok) {
    	return -1;
    }

    std::cerr << "Port parameters: " << port.toString() << std::endl;

    if (is_server) {
    	//printf("Print text and press 'Enter' to send it to serial port or 'Q' to exit loop ...\n");
    	std::cout << "Press 'Ctrl+C' to exit loop ..." << std::endl;
    	while(true) {
    		char bytes[128];
    		ssize_t nbytes = port.read(bytes, 127);
    		if (nbytes < 0) {
    			port.logErrors();
    			break;
    		}
    		if (nbytes > 0) {
    			pfs::byte_string hex(__to_hex(bytes, size_t(nbytes)));
    			//bytes[nbytes] = '\0';
    			std::cout << "bytes read: " << nbytes << "[" << bytes << "][" << hex << ']' << std::endl;
    		}
    	}
    } else {
    	pfs::byte_string bytes("Hello, World!");
    	port.write(bytes);
    }

    port.close();

    return 0;
}
