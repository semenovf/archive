uint32_t inet_socket::ipv4 () const
{
	if (opened()) {
		return ntohl(_pimpl->saddr.sin_addr.s_addr);
	}
	return 0;
}

#ifdef __COMMENT__

//extern bool init_inet_sockaddr (struct sockaddr_in & sockaddr, const HostAddress & hostAddr, uint16_t port);

bool Socket::Impl::open (int oflags)
{
	int type = 0;
	int domain = 0;
	int proto = 0;

	if ((oflags & SocketMask) == TcpSocket) {
	} else if ((oflags & SocketMask) == LocalSocket) {
		type |= SOCK_STREAM;
		domain = AF_LOCAL;
		proto = PF_LOCAL;
		this->socktype = LocalSocket;
	} else if ((oflags & SocketMask) == UdpSocket) {
		type |= SOCK_DGRAM;
		domain = AF_INET;
		proto = PF_INET;
		this->socktype = UdpSocket;
	} else if ((oflags & SocketMask) == LocalDgramSocket) {
		type |= SOCK_DGRAM;
		domain = AF_LOCAL;
		proto = PF_LOCAL;
		this->socktype = LocalDgramSocket;
	} else {
		Logger::error(_Tr("Unspecified socket type: must be stream, datagram or raw"));
		return false;
	}

	if (oflags & Device::NonBlocking)
		type |= SOCK_NONBLOCK;

	this->sockfd = socket(domain, type, proto);

	if (this->sockfd < 0) {
		Logger::error(errno, _Tr("Failed to open socket"));
		return false;
	}

	bool rc = true;

	if (oflags & Listener)
		this->isListener = true;

	// TODO add UDP and Local socket support
	if (this->isListener) {
		if (this->socktype == TcpSocket)
			rc = listenTcpSocket();
	} else {
		if (this->socktype == TcpSocket)
			rc = connectTcpSocket();
	}

	if (!rc) {
		::close(sockfd);
		sockfd = -1;
		return false;
	}

	return true;
}
}
#endif
