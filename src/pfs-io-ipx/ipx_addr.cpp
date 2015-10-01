/*
 * ipx_addr.cpp
 *
 *  Created on: Jun 17, 2015
 *      Author: wladt
 */

#include "pfs/io/ipx_addr.hpp"

namespace pfs { namespace io {

ipx_addr::ipx_addr ()
    : _port(0)
    , _frameType(IpxFrameNone)
{
	memset(_addr, '\xFF', IpxNodeLen * sizeof(unsigned char));
}

ipx_addr::ipx_addr (const ipx_addr & addr)
    : _port(addr._port)
    , _frameType(addr._frameType)

{
	memcpy(_addr, addr._addr, IpxNodeLen * sizeof(unsigned char));
}

ipx_addr::ipx_addr (const char * addr, uint16_t port, ipx_frame_enum frameType)
    : _port(port)
    , _frameType(frameType)
{
	if (strlen(addr) != IpxNodeLen)
		return;

	memcpy(_addr, reinterpret_cast<const unsigned char *>(addr), IpxNodeLen * sizeof(unsigned char));
}

ipx_addr::ipx_addr (const unsigned char addr[IpxNodeLen], uint16_t port, ipx_frame_enum frameType)
    : _port(port)
    , _frameType(frameType)
{
	memcpy(_addr, addr, IpxNodeLen * sizeof(unsigned char));
}

ipx_addr::ipx_addr (unsigned char a0
		, unsigned char a1
		, unsigned char a2
		, unsigned char a3
		, unsigned char a4
		, unsigned char a5
		, uint16_t port, ipx_frame_enum frameType)
    : _port(port)
    , _frameType(frameType)
{
	_addr[0] = a0;
	_addr[1] = a1;
	_addr[2] = a2;
	_addr[3] = a3;
	_addr[4] = a4;
	_addr[5] = a5;
}

ipx_addr & ipx_addr::operator = (const ipx_addr & addr)
{
	memcpy(_addr, addr._addr, IpxNodeLen * sizeof(unsigned char));
    _port = addr._port;
    _frameType = addr._frameType;

	return *this;
}

static const struct {
    string name;
    string canonical;
} FrameTypeStrings [] = {
      { _l1("IpxFrameNone"),    _l1("None") }
    , { _l1("IpxFrameSnap"),    _l1("Snap") }
    , { _l1("IpxFrame8022"),    _l1("802.2") }
    , { _l1("IpxFrameEtherII"), _l1("EtherII") }
    , { _l1("IpxFrame8023"),    _l1("802.3") }
    , { _l1("IpxFrameTr8022"),  _l1("Tr802.2") }
};

string ipx_addr::frameTypeToString (ipx_frame_enum frameType)
{
    return FrameTypeStrings[frameType].name;
}

string ipx_addr::frameTypeToCanonicalName (ipx_frame_enum frameType)
{
    return FrameTypeStrings[frameType].canonical;
}

}} // pfs::io


