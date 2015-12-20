#ifndef __PFS_IO_INET_ADDR_HPP__
#define __PFS_IO_INET_ADDR_HPP__

#include <pfs/string.hpp>

namespace pfs { namespace io {

class DLL_API inet4_addr
{
    uint32_t _addr;
    int32_t  _port;

public:
    inet4_addr ();
    inet4_addr (const inet4_addr & addr);
    inet4_addr (uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint16_t port);
    inet4_addr (uint8_t a, uint8_t b, uint16_t c, uint16_t port);
    inet4_addr (uint8_t a, uint32_t b, uint16_t port);
    inet4_addr (uint32_t a , uint16_t port);
    inet4_addr (const string & s);
    inet4_addr (const string & s, uint16_t port);

    inet4_addr & operator = (const inet4_addr & addr);

    uint32_t raw_addr () const
    {
    	return _addr;
    }

    void set_port (uint16_t port)
    {
    	_port = port;
    }

    uint16_t port () const
    {
    	return _port;
    }

//    bool isValid () const;
//    void invalidate ();

    //string toString () const { return toDecString(); }
    //string toString (const string & format, int base) const;
    //string toDecString (const string & format = string()) const { return toString(format, 10); }
    //string toOctalString (const string & format = string()) const { return toString(format, 8); }
    //string toHexString (const string & format = string()) const { return toString(format, 16); }

//    static inet4_addr make (uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint16_t port) { return inet4_addr(a, b, c, d, port); }
//    static inet4_addr make (uint8_t a, uint8_t b, uint16_t c, uint16_t port) { return inet4_addr(a, b, c, port); }
//    static inet4_addr make (uint8_t a, uint32_t b, uint16_t port) { return inet4_addr(a, b, port); }
//    static inet4_addr make (uint32_t a , uint16_t port) { return inet4_addr(a, port); }
//    static inet4_addr make (const string & addrStr) { return inet4_addr(addrStr); }
//    static inet4_addr make (const string & addrStr, uint16_t port) { return inet4_addr(addrStr, port); }
};

}} // pfs:io

namespace pfs {

string & lexical_cast (string & result, const io::inet4_addr & addr, const string & format, int base);

inline string & lexical_cast (string & result, const io::inet4_addr & addr, const string & format)
{
	return lexical_cast(result, addr, format, 10);
}

inline string & lexical_cast (string & result, const io::inet4_addr & addr) // format, 10
{
	return lexical_cast(result, addr, string(), 10);
}

} //pfs

#endif /* __PFS_IO_INET_ADDR_HPP__ */
