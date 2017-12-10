/*
 * ipx_addr.hpp
 *
 *  Created on: Jun 25, 2015
 *      Author: wladt
 */

#ifndef __PFS_IO_IPX_ADDR_HPP__
#define __PFS_IO_IPX_ADDR_HPP__

#include <pfs/string.hpp>

namespace pfs { namespace io {

enum ipx_frame_enum {
      IpxFrameNone
    , IpxFrameSnap
    , IpxFrame8022
    , IpxFrameEtherII
    , IpxFrame8023
    , IpxFrameTr8022
};

class DLL_API ipx_addr
{
public:
    static const int IpxNodeLen = 6;

private:
    uint8_t     _addr[IpxNodeLen];
    uint16_t    _port;
    ipx_frame_enum _frameType;

public:
    ipx_addr ();
    ipx_addr (const ipx_addr & addr);
    ipx_addr (const char * addr, uint16_t port, ipx_frame_enum frameType);
    ipx_addr (const unsigned char addr[IpxNodeLen], uint16_t port, ipx_frame_enum frameType);
    ipx_addr (uint8_t a0
            , uint8_t a1
            , uint8_t a2
            , uint8_t a3
            , uint8_t a4
            , uint8_t a5
            , uint16_t port, ipx_frame_enum frameType);

    ipx_addr & operator = (const ipx_addr & addr);
    const unsigned char * addrData () const { return _addr; }
    uint16_t port () const { return _port; }
    ipx_frame_enum frameType () const { return _frameType; }

    static string frameTypeToString (ipx_frame_enum frameType);
    static string frameTypeToCanonicalName (ipx_frame_enum frameType);
};

}} // pfs:io

#endif /* __PFS_IO_IPX_ADDR_HPP__ */
