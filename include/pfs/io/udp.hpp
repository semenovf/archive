#ifndef __PFS_IO_UDP_HPP__
#define __PFS_IO_UDP_HPP__

#include <pfs/io/device.hpp>
#include <pfs/io/inet_addr.hpp>

namespace pfs { namespace io {

class udp_server;

class DLL_API udp_socket : public device
{
    friend class udp_server;

public:
    udp_socket () : device() {}
    virtual ~udp_socket () {}

    bool open (const inet4_addr & addr, int32_t oflags = device::NonBlocking);
    bool connect (const inet4_addr & addr);
    inet4_addr address () const;
};

class DLL_API udp_server : public errorable_ext
{
private:
    pimpl _d;

public:
    udp_server () : errorable_ext() {}

    bool opened () const;
    bool open (const inet4_addr & addr, int32_t oflags = device::NonBlocking);
    bool accept (udp_socket * sock);
    inet4_addr address () const;
};

}} // pfs:io

#endif /* __PFS_IO_UDP_HPP__ */
