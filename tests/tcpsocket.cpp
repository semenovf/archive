#include <cwt/test.hpp>
#include <pfs/bytearray.hpp>
#include <pfs/safeformat.hpp>
#include <cwt/logger.hpp>
#include <cwt/thread.hpp>
#include "cwt/io/inetsocket.hpp"
#include "cwt/io/tcpserver.hpp"
#include <iostream>

static const uint16_t port = 3999;

class ServerThread : public cwt::thread
{
public:
	ServerThread(cwt::io::tcp_server * server) : m_server(server) {}
protected:
	virtual void run ();

private:
	cwt::io::tcp_server * m_server;
};

class ClientThread : public cwt::thread
{
protected:
	virtual void run ();
};

void ServerThread::run ()
{
	cwt::io::tcp_socket * client = nullptr;

	if (m_server) {
		while ((client = m_server->accept()) == nullptr) {
			;
		}

		if (client) {
			std::cout << "Client accepted on: " << client->hostname() << ':' << client->port() << std::endl;
		}

		while (client) {
			pfs::bytearray ba;
			if (client->read(ba, 1024) > 0) {
				std::cout << "Received: " << ba.c_str() << std::endl;
				if (ba == pfs::bytearray("quit")) {
					std::cout << "Bye!" << std::endl;
					delete client;
					client = nullptr;
				}
			}
		}
	}
}

void ClientThread::run ()
{
	cwt::io::tcp_socket client;
	TEST_OK(client.open(_l1("localhost"), port));

	if (client.opened()) {
		client.write("Hello", 5);
		sleep(1);
		client.write("World!", 5);
		sleep(1);
		client.write("quit", 4);
	}
}

int main(int argc, char *argv[])
{
    PFS_CHECK_SIZEOF_TYPES;
    PFS_UNUSED2(argc, argv);
    BEGIN_TESTS(2);

    cwt::io::tcp_server server;
    TEST_FAIL(server.listen(_l1("localhost"), port));

    ServerThread serverThread(& server);
    ClientThread clientThread;

    serverThread.start();
    clientThread.start();

    serverThread.wait();
    clientThread.wait();

    END_TESTS;
    return 0;
}
