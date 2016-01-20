#ifndef __PFS_NET_INET4_ADDR_HPP__
#define __PFS_NET_INET4_ADDR_HPP__

#include <pfs/string.hpp>
#include <pfs/utility.hpp>

namespace pfs { namespace net {

/**
 * @brief Satisfies Concepts:
 *        @li CopyConstructible
 *        @li CopyAssignable
 */
class DLL_API inet4_addr
{
private:
	static const uint32_t invalid_addr_value = 0xFFFFFFFF;

private:
    uint32_t _addr;

public:
    /**
     * @brief Constructs invalid @c inet4_addr instance.
     */
    inet4_addr ()
		: _addr(invalid_addr_value)
	{}

    /**
     * @brief Constructs inet4_addr from four numeric parts.
     *
     * @details Each of the four numeric parts specifies a byte of the address;
     *          the bytes are assigned in left-to-right order to produce the binary address.
     *
     * @param a First numeric part.
     * @param b Second numeric part.
     * @param c Third numeric part.
     * @param d Fourth numeric part.
     */
    inet4_addr (uint8_t a, uint8_t b, uint8_t c, uint8_t d);


    /**
     * @brief Constructs inet4_addr from three numeric parts.
     *
     * @details Parts @a a and @a b specify the first two bytes of the binary address.
     *          Part @a c is interpreted as a 16-bit value that defines the rightmost
     *          two bytes of the binary address. This notation is suitable
     *          for specifying (outmoded) Class B network addresses.
     *
     * @param a First numeric part
     * @param b Second numeric part
     * @param c Third numeric parts.
     */
    inet4_addr (uint8_t a, uint8_t b, uint16_t c);

    /**
     * @brief Constructs inet4_addr from two numeric parts.
     *
     * @details Part @a a specifies the first byte of the binary address.
     *          Part @a b is interpreted as a 24-bit value that defines
     *          the rightmost three bytes of the binary address.
     *          This notation is suitable for specifying (outmoded)
     *          Class C network addresses.
     *
     * @param a First numeric part
     * @param b Second numeric part
     */
    inet4_addr (uint8_t a, uint32_t b);

    /**
     * @brief Constructs inet4_addr from one numeric part.
     *
     * @param a Numeric part.
     */
    inet4_addr (uint32_t a);

    /**
     * @brief Constructs inet4_addr from string representing the IPv4 address.
     *
     * @details @a s must be in one of the format:
     *             (1) a.b.c.d
     *             (2) a.b.C
     *             (3) a.B
     *             (4) A
     *
     * @param s String representing the IPv4 address.
     */
    inet4_addr (const string & s);


    /**
     * @brief Returns @c true if inet4_addr represents a valid IPv4 address.
     */
    operator bool () const
	{
    	return _addr != invalid_addr_value;
	}

    uint32_t native () const
    {
    	return _addr;
    }

    void swap (inet4_addr & other)
    {
    	pfs::swap(_addr, other._addr);
    }
};

#if __COMMENT__


#include <netdb.h> // getaddrinfo(), freeaddrinfo()

#if (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 1) || defined(_XOPEN_SOURCE) || defined(_POSIX_SOURCE)
#   define PFS_HAVE_GETADDRINFO 1
#endif

#ifdef PFS_HAVE_GETADDRINFO

struct addrinfo_iterator_data
{
    addrinfo_iterator_data () : result(nullptr), next(nullptr) {}
    ~addrinfo_iterator_data ()
    {
        if (result) {
            freeaddrinfo(result);
            result = nullptr;
            next = nullptr;
        }
    }
    struct addrinfo * result;
    struct addrinfo * next;
};

class addrinfo_iterator
{
    pfs::shared_ptr<addrinfo_iterator_data> _data;

public:
    addrinfo_iterator () : _data(new addrinfo_iterator_data) {}
    addrinfo_iterator (const addrinfo_iterator & it) : _data(it._data) {}

    addrinfo_iterator & operator = (const addrinfo_iterator & it)
    {
        _data = it._data;
        return *this;
    }

    bool operator == (const addrinfo_iterator & it)
    {
        return _data->next == it._data->next;
    }

    bool operator != (const addrinfo_iterator & it) { return !(*this == it); }
    addrinfo_iterator & operator ++ ()
    {
        _data->next = _data->next->ai_next;
        return *this;
    }

    addrinfo_iterator operator ++ (int)
    {
        addrinfo_iterator r(*this);
        this->operator ++();
        return r;
    }

    /* Official name of the host */
    pfs::string canonicalName () const
    {
        // Only field of the first of the addrinfo structures in the returned list
        // is set to point to the official name of the host.
        if (_data->result)
            return pfs::string(_data->result->ai_canonname);
        return pfs::string();
    }

    pfs::string hostname () const
    {
        return canonicalName();
    }

    uint32_t ip4addr () const
    {
        if (_data->next) {
            struct sockaddr_in * saddr = reinterpret_cast<struct sockaddr_in *>(_data->next->ai_addr);
            return ntohl(saddr->sin_addr.s_addr);
        }
        return 0;
    }

    uint16_t port () const
    {
        return _data->next
                ? reinterpret_cast<struct sockaddr_in *>(_data->next->ai_addr)->sin_port
                : 0;
    }


    static addrinfo_iterator begin (const pfs::string & hostname);
    static addrinfo_iterator end ()
    {
        return addrinfo_iterator();
    }
};

addrinfo_iterator addrinfo_iterator::begin (const pfs::string & hostname)
{
    addrinfo_iterator it;

    const char * node = hostname.c_str();
    const char * service = nullptr; // service is no matter
    struct addrinfo hints;
    struct addrinfo * result;

    memset(& hints, 0, sizeof(struct addrinfo));
    hints.ai_flags     = (hostname.isEmpty() ? AI_PASSIVE : 0) | AI_CANONNAME;
    hints.ai_family    = AF_UNSPEC;    /* Allow IPv4 or IPv6, i.e. any address family */
    hints.ai_socktype  = 0;            /* Any type */
    hints.ai_protocol  = 0;            /* Any protocol */
    hints.ai_canonname = nullptr;
    hints.ai_addr      = nullptr;
    hints.ai_next      = nullptr;

    int rc = getaddrinfo(node, service, & hints, & result);
    if (!PFS_VERIFY_X(rc == 0
            , (pfs::string() << hostname
              << _u8(": get address info failure: ")
              << gai_strerror(rc)).c_str())) {
        return end();
    }

    pfs::shared_ptr<addrinfo_iterator_data> d(new addrinfo_iterator_data);
    d->result = result;
    d->next = result;
    it._data.swap(d);

    return it;
}

#else
#   error getaddrinfo() does not supported by this platform
#endif


bool inet_socket_impl::open (inet_proto_enum protocol, int32_t oflags, errorable_ext & ex)
{
    int proto = -1;
    int style = -1;

    switch (protocol) {
    case InetProtoUdp:    proto = IPPROTO_UDP; style = SOCK_DGRAM; break;
    case InetProtoTcp:    proto = IPPROTO_TCP; style = SOCK_STREAM; break;
    case InetProtoUnspec:
    default: break;
    }

    if (proto < 0 || style < 0) {
        ex.addError(_u8("bad inet protocol specified"));
        return false;
    }

    if (oflags & device::NonBlocking)
        style |= SOCK_NONBLOCK;

    _sockfd = ::socket(PF_INET, style, proto);

    if (_sockfd < 0) {
        ex.addSystemError(errno, _u8("failed to open inet socket"));
        return false;
    }

    return true;
}

#endif

}} // pfs:io

namespace pfs {

/**
 * @brief Converts IPv4 address to string.
 *
 * @details The format specification is a null-terminated string and may
 * 		contain special character sequences called conversion specifications,
 *      each of which is introduced by a '%' character and terminated by
 *      some other character known as a conversion specifier character.
 *      All other character sequences are ordinary character sequences.
 *
 *      The characters of ordinary character sequences (including the null byte)
 *      are copied verbatim from format to resulting string. However,
 *      the characters of conversion specifications are replaced as follows:
 *
 *      @li %a First byte of IPv4 address.
 *      @li %A The full 32-bit representation of IPv4 address.
 *      @li %b Second byte of IPv4 address.
 *      @li %B 24-bit representation of last part of IPv4 address
 *      @li %c Third byte of IPv4 address.
 *      @li %C 16-bit representation of last part of IPv4 address
 *      @li %d Fourth byte of IPv4 address.
 *      @li %% '%' character.
 *
 * @param addr Source address for conversion.
 * @param format Conversion format string. If @a format is empty the default format
 *      will be used: "%a.%b.%c.%d".
 * @param base Specifies the radix (8, 10 or 16) for output address parts.
 * @return String representation of IPv4 address. If @a addr is not valid
 *      result will be an empty string.
 *
 * @note 192.0.2.235           decimal
 *       0xC0.0x00.0x02.0xEB   hex
 *       0300.0000.0002.0353   octal
 *       0xC00002EB            decimal
 *       3221226219            hex
 *       030000001353          octal
 *
 *
 */
string to_string (const net::inet4_addr & addr, const string & format, int base);

/**
 * @brief Converts IPv4 address to string with base 10.
 * @param addr Source address for conversion.
 * @param format Conversion format string. If @a format is empty the default format
 * 		will be used: "%a.%b.%c.%d".
 * @return String representation of IPv4 address. If @a addr is not valid
 *      result will be an empty string.
 */
inline string to_string (const net::inet4_addr & addr, const string & format)
{
	return to_string(addr, format, 10);
}

/**
 * @brief Converts IPv4 address to string with format "%a.%b.%c.%d" and base 10.
 *
 * @param addr Source address for conversion.
 * @return String representation of IPv4 address. If @a addr is not valid
 *         result will be an empty string.
 */
inline string to_string (const net::inet4_addr & addr)
{
	return to_string(addr, string(), 10);
}

} //pfs

#endif /* __PFS_NET_INET4_ADDR_HPP__ */
