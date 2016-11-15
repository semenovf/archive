/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   inet_posix_utils.hpp
 * Author: wladt
 *
 * Created on November 15, 2016, 6:28 PM
 */

#ifndef __PFS_IO_INET_POSIX_UTILS_HPP__
#define __PFS_IO_INET_POSIX_UTILS_HPP__

#include <netinet/in.h>
#include <pfs/string.hpp>

namespace pfs {
namespace io {

bool is_nonblocking (int fd);
bool set_nonblocking (int fd, bool on);
pfs::string inet_socket_url (char const * proto, sockaddr_in const & sin);
int create_tcp_socket (bool non_blocking);
int create_udp_socket (bool non_blocking);
int close_socket (int fd);

}}

#endif /* __PFS_IO_INET_POSIX_UTILS_HPP__ */

