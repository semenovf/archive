/*
 * inet_addr.cpp
 *
 *  Created on: Jun 25, 2015
 *      Author: wladt
 */

#include "pfs/io/inet_addr.hpp"
#include <pfs/stringlist.hpp>
#include <netinet/in.h>

namespace pfs { namespace io {

static const int32_t InvalidInetPort = -1;

inline uint32_t make_inet_addr (uint8_t a, uint8_t b, uint8_t c, uint8_t d)
{
    uint32_t r = 0;
    r |= (static_cast<uint32_t>(a) << 24);
    r |= (static_cast<uint32_t>(b) << 16);
    r |= (static_cast<uint32_t>(c) << 8);
    r |= static_cast<uint32_t>(d);
    return r;
}

inline uint32_t make_inet_addr (uint8_t a, uint8_t b, uint16_t c)
{
    uint32_t r = 0;
    r |= (static_cast<uint32_t>(a) << 24);
    r |= (static_cast<uint32_t>(b) << 16);
    r |= static_cast<uint32_t>(c);
    return r;
}

inline uint32_t make_inet_addr (uint8_t a, uint32_t b)
{
    uint32_t r = 0;
    r |= (static_cast<uint32_t>(a) << 24);
    r |= b;
    return r;
}

inline uint32_t make_inet_addr (uint32_t a)
{
    return a;
}

bool parse_inet_addr_part_helper (const string & part, uintegral_t & result)
{
    bool ok;
    uintegral_t r;

    if (part == _l1("0")) {
        r = 0;
    } else if (part.startsWith("0x") || part.startsWith("0X")) { // Hexadecimal format
        r = part.substr(2, part.length()).toUnsignedIntegral(& ok, 16);
        if (!ok)
            return false;
    } else if (part.startsWith('0')) {  // Octal format
        r = part.substr(1, part.length()).toUnsignedIntegral(& ok, 8);
        if (!ok)
            return false;
    } else {
        r = part.toUnsignedIntegral(& ok, 10);
        if (!ok)
            return false;
    }

    result = r;
    return true;
}

template <typename UInt>
bool parse_inet_addr_part (const string & part, UInt & result)
{
    uintegral_t r;

    if (!parse_inet_addr_part_helper(part, r))
        return false;

    if (r > max_type<UInt>())
        return false;

    result = static_cast<UInt>(r);
    return true;
}

static bool parse_inet_addr (const string & addrStr, uint32_t & addr, int32_t & port)
{
    stringlist parts = addrStr.split(_l1(":"));

    port = 0;

    if (parts.size() > 2)
        return false;

    //
    // Has port part
    //
    if (parts.size() == 2) {
        uintegral_t r;

        if (!parse_inet_addr_part_helper(parts[1], r))
            return false;

        if (r > max_type<uint16_t>())
            return false;

        port = static_cast<int32_t>(r);
    }

    stringlist octets = string(parts[0]).split(_l1("."));

    //
    // Invalid address
    //
    if (octets.size() < 1 && octets.size() > 4)
        return false;

    if (octets.size() == 4) {
        uint8_t a, b, c, d;

        if (!(parse_inet_addr_part<uint8_t> (octets[0], a)
                && parse_inet_addr_part<uint8_t> (octets[1], b)
                && parse_inet_addr_part<uint8_t> (octets[2], c)
                && parse_inet_addr_part<uint8_t> (octets[3], d)))
            return false;

        addr = make_inet_addr(a, b, c, d);
    } else if (octets.size() == 3) {
        uint8_t a, b;
        uint16_t c;

        if (!(parse_inet_addr_part<uint8_t> (octets[0], a)
                && parse_inet_addr_part<uint8_t> (octets[1], b)
                && parse_inet_addr_part<uint16_t> (octets[2], c)))
            return false;

        addr = make_inet_addr(a, b, c);
    } else if (octets.size() == 2) {
        uint8_t a;
        uint32_t b;

        if (!(parse_inet_addr_part<uint8_t> (octets[0], a)
                && parse_inet_addr_part<uint32_t> (octets[1], b)))
            return false;

        if (b > PFS_UINT24_MAX)
            return false;

        addr = make_inet_addr(a, b);
    } else {
        uint32_t a;

        if (!(parse_inet_addr_part<uint32_t> (octets[0], a)))
            return false;

        addr = make_inet_addr(a);
    }

    return true;
}

/**
 * @brief Constructs invalid @c inet_addr.
 */
inet_addr::inet_addr ()
    : _addr(0)
    , _port(InvalidInetPort)
{}


/**
 * @brief Copy constructor.
 *
 * @param addr Other @c inet_addr instance.
 */
inet_addr::inet_addr (const inet_addr & addr)
    : _addr(addr._addr)
    , _port(addr._port)
{}


/**
 * @brief Construct inet_addr from four numeric parts.
 *
 * @details Each of the four numeric parts specifies a byte of the address;
 *          the bytes are assigned in left-to-right order to produce the binary address.
 *
 * @param a0 First numeric part.
 * @param a1 Second numeric part.
 * @param a2 Third numeric part.
 * @param a3 Fourth numeric part.
 * @param port Port number.
 */
inet_addr::inet_addr (uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint16_t port)
    : _addr(0)
    , _port(static_cast<int32_t>(port))
{
    _addr = make_inet_addr(a, b, c, d);
}

/**
 * @brief Construct inet_addr from three numeric parts.
 *
 * @details Parts @c a and @c b specify the first two bytes of the binary address.
 *          Part @c c is interpreted as a 16-bit value that defines the rightmost
 *          two bytes of the binary address. This notation is suitable
 *          for specifying (outmoded) Class B network addresses.
 *
 * @param a First numeric part
 * @param b Second numeric part
 * @param c Third numeric parts.
 * @param port Port number.
 */
inet_addr::inet_addr (uint8_t a, uint8_t b, uint16_t c, uint16_t port)
    : _addr(0)
    , _port(static_cast<int32_t>(port))
{
    _addr = make_inet_addr(a, b, c);
}

/**
 * @brief Construct inet_addr from two numeric parts.
 *
 * @details Part @c a specifies the first byte of the binary address.
 *          Part @c b is interpreted as a 24-bit value that defines
 *          the rightmost three bytes of the binary address.
 *          This notation is suitable for specifying (outmoded)
 *          Class C network addresses.
 *
 * @param a First numeric part
 * @param b Second numeric part
 * @param port Port number.
 */
inet_addr::inet_addr (uint8_t a, uint32_t b, uint16_t port)
    : _addr(0)
    , _port(static_cast<int32_t>(port))
{
    _addr = make_inet_addr(a, b);
}

/**
 * @brief Construct inet_addr from one numeric part.
 *
 * @param a Numeric part.
 * @param port Port number.
 */
inet_addr::inet_addr (uint32_t a, uint16_t port)
    : _addr(0)
    , _port(static_cast<int32_t>(port))
{
    _addr = make_inet_addr(a);
}

/**
 * @brief Construct inet_addr from string.
 *
 * @details @c addrStr must be in one of the format:
 *             (1) a.b.c.d
 *             (2) a.b.c
 *             (3) a.b
 *             (4) a
 *
 * @param addrStr String represented the IPv4 address.
 * @param port Port number.
 */
inet_addr::inet_addr (const string & addrStr, uint16_t port)
    : _addr(0)
    , _port(static_cast<int32_t>(port))
{
    if (!parse_inet_addr(addrStr, _addr, _port)) {
        invalidate();
    }
}

/**
 * @brief Construct inet_addr from string.
 *
 * @details @c addrStr must be in one of the format:
 *             (1) a.b.c.d[:port]
 *             (2) a.b.c[:port]
 *             (3) a.b[:port]
 *             (4) a[:port]
 *          where @c port is optional.
 *
 * @param addrStr String represented the IPv4 address.
 */
inet_addr::inet_addr (const string & addrStr)
    : _addr(0)
    , _port(InvalidInetPort)
{
    if (!parse_inet_addr(addrStr, _addr, _port)) {
        invalidate();
    }
}

/**
 * @brief Copy operator.
 *
 * @param addr
 * @return
 */
inet_addr & inet_addr::operator = (const inet_addr & addr)
{
    _addr = addr._addr;
    _port = addr._port;
    return *this;
}

bool inet_addr::isValid () const
{
    return _port != InvalidInetPort;
}

inline void inet_addr::invalidate ()
{
    _port = InvalidInetPort;
}


static int format_type (const string & format)
{
    if (format == "a.b.c.d")   return 1;
    if (format == "a.b.c")     return 2;
    if (format == "a.b")       return 3;
    if (format == "a")         return 4;
    if (format == "a.b.c.d:p") return 5;
    if (format == "a.b.c:p")   return 6;
    if (format == "a.b:p")     return 7;
    if (format == "a:p")       return 8;
    return 5; // default
}

/**
 * @brief
 *
 * @details Valid format strings:
 *          (1) a.b.c.d
 *          (2) a.b.c
 *          (3) a.b
 *          (4) a
 *          (5) a.b.c.d:p
 *          (6) a.b.c:p
 *          (7) a.b:p
 *          (8) a:p
 *
 *          (5) is default format
 *
 * @param
 * @return
 */
string inet_addr::toString (const string & format, int base) const
{
    string r;
    string basePrefix;

    int ftype = format.isEmpty() ? 5 : format_type(format);

    if (!(base == 10 || base == 8 || base == 16))
        base = 10;

    if (base == 8)
        basePrefix = _l1("0");
    else if (base == 16)
        basePrefix = _l1("0x");

    if (ftype == 1 || ftype == 5) {
        uint32_t a = 0x000000FF & (_addr >> 24)
               , b = 0x000000FF & (_addr >> 16)
               , c = 0x000000FF & (_addr >> 8)
               , d = 0x000000FF & _addr;

        r   <<        (a ? basePrefix : _l1("")) << string::toString(a, base)
            << '.' << (b ? basePrefix : _l1("")) << string::toString(b, base)
            << '.' << (c ? basePrefix : _l1("")) << string::toString(c, base)
            << '.' << (d ? basePrefix : _l1("")) << string::toString(d, base);
    } else if (ftype == 2 || ftype == 6) {
        uint32_t a = 0x000000FF & (_addr >> 24)
               , b = 0x000000FF & (_addr >> 16)
               , c = 0x0000FFFF & _addr;

        r   <<        (a ? basePrefix : _l1("")) << string::toString(a, base)
            << '.' << (b ? basePrefix : _l1("")) << string::toString(b, base)
            << '.' << (c ? basePrefix : _l1("")) << string::toString(c, base);
    } else if (ftype == 3 || ftype == 7) {
        uint32_t a = 0x000000FF & (_addr >> 24)
               , b = 0x00FFFFFF & _addr;

        r   <<        (a ? basePrefix : _l1("")) << string::toString(a, base)
            << '.' << (b ? basePrefix : _l1("")) << string::toString(b, base);
    } else {
        r   << (_addr ? basePrefix : _l1("")) << string::toString(_addr, base);
    }

    if (ftype >= 5)
        r << ':' << (_port ? basePrefix : _l1("")) << string::toString(_port, base);

    return r;
}

}} // pfs::io


