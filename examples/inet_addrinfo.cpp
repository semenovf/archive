/*
 * inet_addrinfo.cpp
 *
 *  Created on: Jul 6, 2015
 *      Author: wladt
 */

#include "../src/posix/inet_socket_posix.cpp"
#include <iostream>

using std::cout;
using std::cerr;
using std::endl;

int main (int argc, char * argv[])
{
#ifdef PFS_HAVE_GETADDRINFO
    PFS_UNSUED2(argc, argv);
#else
    PFS_UNSUED2(argc, argv);
    cerr << "WARN: getaddrinfo() does not supported by this platform" << endl;
#endif
    return 0;
}

