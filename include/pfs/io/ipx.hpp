#ifndef __PFS_IO_IPX_HPP__
#define __PFS_IO_IPX_HPP__

#include <pfs/io/device.hpp>
#include <pfs/io/ipx_addr.hpp>

namespace pfs { namespace io {

class ipx_socket;
class ipx_server;

class DLL_API ipx_socket : public device
{
	friend class ipx_server;

public:
	ipx_socket () : device() {}
	virtual ~ipx_socket () {}

	bool open (const ipx_addr & addr, int32_t oflags = device::NonBlocking);
	bool connect (const ipx_addr & addr);
};

class DLL_API ipx_server : public errorable_ext
{
private:
    pimpl _d;

public:
	ipx_server () : errorable_ext() {}

	bool opened () const;
	bool open (const ipx_addr & addr, int32_t oflags = device::NonBlocking);
	bool accept (ipx_socket * sock);
};

}} // pfs::io

#endif /* __PFS_IO_IPX_HPP__ */
