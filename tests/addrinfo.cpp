#include <cwt/test.h>
#include <cwt/safeformat.hpp>
#include "../include/cwt/net/hostinfo.hpp"
#include <iostream>

using namespace cwt;

void test_addr_info()
{
	net::addrinfo_iterator it = net::addrinfo_iterator::begin("localhost");
	net::addrinfo_iterator itEnd = net::addrinfo_iterator::end();

	CWT_TEST_OK(it.canonicalName() == String("localhost"));

	while (it != itEnd) {
		std::cout << it.ipv4name() << ':' << it.port() << std::endl;
		++it;
	}

	it = net::addrinfo_iterator::begin("127.0.0.1");
	CWT_TEST_OK(it.canonicalName() == String("127.0.0.1"));

	while (it != itEnd) {
		++it;
	}
}


int main(int argc, char *argv[])
{
    CWT_CHECK_SIZEOF_TYPES;
    CWT_UNUSED2(argc, argv);
    CWT_BEGIN_TESTS(2);

    test_addr_info();

    CWT_END_TESTS;
    return 0;
}
