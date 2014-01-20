#include <cwt/test.hpp>
#include <pfs/string.hpp>
#include "../include/cwt/net/hostinfo.hpp"
#include <iostream>

void test_addr_info()
{
	cwt::net::addrinfo_iterator it = cwt::net::addrinfo_iterator::begin(_l1("localhost"));
	cwt::net::addrinfo_iterator itEnd = cwt::net::addrinfo_iterator::end();

	TEST_OK(it.canonicalName() == "localhost");

	while (it != itEnd) {
		std::cout << it.ipv4name() << ':' << it.port() << std::endl;
		++it;
	}

	it = cwt::net::addrinfo_iterator::begin(_l1("127.0.0.1"));
	TEST_OK(it.canonicalName() == "127.0.0.1");

	while (it != itEnd) {
		++it;
	}
}


int main(int argc, char *argv[])
{
    PFS_CHECK_SIZEOF_TYPES;
    PFS_UNUSED2(argc, argv);
    BEGIN_TESTS(2);

    test_addr_info();

    END_TESTS;
    return 0;
}
