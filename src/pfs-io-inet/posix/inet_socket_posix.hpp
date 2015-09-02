/**
 * @file inet_socket_posix.hpp
 *
 * @date Jun 25, 2015
 * @author wladt
 * @copyright
 */

#ifndef __PFS_POSIX_INET_SOCKET_POSIX_HPP__
#define __PFS_POSIX_INET_SOCKET_POSIX_HPP__

#include "pfs/io/inet_addr.hpp"
#include "pfs/io/device.hpp"
#include <netinet/in.h>

namespace pfs { namespace io {

enum inet_proto_enum {
      InetProtoUnspec
    , InetProtoUdp
    , InetProtoTcp
    , InetProtoLocal
};

class inet_socket_impl : public device_impl
{
    int _sockfd;

protected:

    bool open     (inet_proto_enum proto, int32_t oflags, errorable_ext & ex);
    bool bind     (errorable_ext & ex); // using getaddrinfo()
    bool bind     (const inet4_addr & addr, errorable_ext & ex);
    bool listen   (errorable_ext & ex);
    bool connect  (errorable_ext & ex);
    bool shutdown (errorable_ext & ex);
    bool close    (errorable_ext & ex);

public:
    inet_socket_impl ();
//
//    virtual ssize_t readBytes      (byte_t bytes[], size_t n, errorable_ext & ex);
//    virtual ssize_t writeBytes     (const byte_t bytes[], size_t n, errorable_ext & ex);
//    virtual size_t  bytesAvailable () const;
//    virtual bool    closeDevice    (errorable_ext & ex);
//    virtual bool    deviceIsOpened () const { return _sockfd >= 0; }
//    virtual void    flushDevice    () {}
//
//    bool open (const inet_addr & addr, int32_t oflags, errorable_ext & ex);
};

}} // pfs::io

#endif /* __PFS_POSIX_INET_SOCKET_POSIX_HPP__ */
