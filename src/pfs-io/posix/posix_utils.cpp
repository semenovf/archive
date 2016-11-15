/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "posix_utils.hpp"

namespace pfs {
namespace io {

bool is_nonblocking (int fd)
{
    int r = fcntl(fd, F_GETFL, 0);
    PFS_ASSERT(r >= 0);
    return (r & O_NONBLOCK);
}

bool set_nonblocking (int fd, bool on)
{
    int flags = ::fcntl(fd, F_GETFL, 0);

    if (on)
    	flags |= O_NONBLOCK;
    else
    	flags &= ~O_NONBLOCK;

    return ::fcntl(fd, F_SETFL, flags) >= 0;
}

pfs::string inet_socket_url (char const * proto, sockaddr_in const & sin)
{
    char str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, & sin.sin_addr, str, INET_ADDRSTRLEN);
    string r(proto);
    r.append("://");
    r.append(str);
    r.append(":");
    r.append(to_string(ntohs(sin.sin_port), 10));
    return r;
}

int create_tcp_socket (bool non_blocking)
{
  	int socktype = SOCK_STREAM;

	if (non_blocking)
		socktype |= SOCK_NONBLOCK;

	return ::socket(PF_INET, socktype, IPPROTO_TCP);
}

int create_udp_socket (bool non_blocking_flag)
{
	int socktype = SOCK_DGRAM;

	if (non_blocking_flag)
		socktype |= SOCK_NONBLOCK;

	return ::socket(PF_INET, socktype, IPPROTO_UDP);
}

int close_socket (int fd)
{
    if (fd > 0) {
    	shutdown(fd, SHUT_RDWR);

        return ::close(fd);
    }

    return 0;
}

}} // pfs::io
