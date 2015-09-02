/**
 * @file inet_socket_posix.cpp
 * @date Jul 3, 2015
 * @author wladt
 */

/* Sources:
 * Multicast over TCP/IP HOWTO
 *      http://tldp.org/HOWTO/Multicast-HOWTO.html
 * Socket programming:
 *      http://publib.boulder.ibm.com/infocenter/iseries/v5r3/topic/rzab6/rzab6soxoverview.htm
 * Socket application design recommendations:
 *      http://publib.boulder.ibm.com/infocenter/iseries/v5r3/topic/rzab6/rzab6designrec.htm
 * A connection-oriented server example:
 *      http://publib.boulder.ibm.com/infocenter/iseries/v5r3/topic/rzab6/rzab6xconoserver.htm
 * A connectionless server example:
 *      http://publib.boulder.ibm.com/infocenter/iseries/v5r3/topic/rzab6/rzab6xconlessserver.htm
 * A connection-oriented client example:
 *      http://publib.boulder.ibm.com/infocenter/iseries/v5r3/topic/rzab6/rzab6xconoclient.htm
 * A connectionless client example:
 *      http://publib.boulder.ibm.com/infocenter/iseries/v5r3/topic/rzab6/rzab6xconlessclient.htm
 * Introduction to non-blocking I/O:
 *      http://www.kegel.com/dkftpbench/nonblocking.html
 */

#include "inet_socket_posix.hpp"
#include <netdb.h> // getaddrinfo(), freeaddrinfo()

#if (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 1) || defined(_XOPEN_SOURCE) || defined(_POSIX_SOURCE)
#   define PFS_HAVE_GETADDRINFO 1
#endif

namespace pfs { namespace io {

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

bool inet_socket_impl::bind (errorable_ext & ex) // using getaddrinfo()
{
    PFS_UNUSED(ex);
    return false;
}

bool inet_socket_impl::bind (const inet4_addr & addr, errorable_ext & ex)
{
    PFS_UNUSED(ex);
    PFS_UNUSED(addr);
    return false;
}

bool inet_socket_impl::listen (errorable_ext & ex)
{
    PFS_UNUSED(ex);
    return false;
}

bool inet_socket_impl::connect (errorable_ext & ex)
{
    PFS_UNUSED(ex);
    return false;
}

bool inet_socket_impl::shutdown (errorable_ext & ex)
{
    PFS_UNUSED(ex);
    return false;
}

bool inet_socket_impl::close (errorable_ext & ex)
{
    PFS_UNUSED(ex);
    return false;
}

}} // pfs::io
