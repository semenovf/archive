#ifndef __PFS_IO_INET_ADDR_HPP__
#define __PFS_IO_INET_ADDR_HPP__

#include <pfs/string.hpp>

namespace pfs { namespace io {

class DLL_API inet_addr
{
    uint32_t _addr;
    int32_t  _port;

public:
    inet_addr ();
    inet_addr (const inet_addr & addr);
    inet_addr (uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint16_t port);
    inet_addr (uint8_t a, uint8_t b, uint16_t c, uint16_t port);
    inet_addr (uint8_t a, uint32_t b, uint16_t port);
    inet_addr (uint32_t a , uint16_t port);
    inet_addr (const string & addrStr);
    inet_addr (const string & addrStr, uint16_t port);

    inet_addr & operator = (const inet_addr & addr);

    uint32_t addrData () const { return _addr; }

    void setPort (uint16_t port) { _port = port; }
    uint16_t port () const { return _port; }

    bool isValid () const;
    void invalidate ();

    string toString () const { return toDecString(); }
    string toString (const string & format, int base) const;
    string toDecString (const string & format = string()) const { return toString(format, 10); }
    string toOctalString (const string & format = string()) const { return toString(format, 8); }
    string toHexString (const string & format = string()) const { return toString(format, 16); }

    static inet_addr make (uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint16_t port) { return inet_addr(a, b, c, d, port); }
    static inet_addr make (uint8_t a, uint8_t b, uint16_t c, uint16_t port) { return inet_addr(a, b, c, port); }
    static inet_addr make (uint8_t a, uint32_t b, uint16_t port) { return inet_addr(a, b, port); }
    static inet_addr make (uint32_t a , uint16_t port) { return inet_addr(a, port); }
    static inet_addr make (const string & addrStr) { return inet_addr(addrStr); }
    static inet_addr make (const string & addrStr, uint16_t port) { return inet_addr(addrStr, port); }
};

}} // pfs:io

#endif /* __PFS_IO_INET_ADDR_HPP__ */
