#include <cwt/test.h>
#include <cwt/logger.hpp>
#include <cwt/safeformat.hpp>
#include <cwt/thread.hpp>
#include "../include/cwt/io/inetsocket.hpp"
#include "../include/cwt/net/tcpserver.hpp"
#include <iostream>

using namespace cwt;
using namespace cwt::io;
using namespace cwt::net;

static const uint16_t port = 3999;


class ServerThread : public Thread
{
public:
	ServerThread(TcpServer * server) : m_server(server) {}
protected:
	virtual void run ();

private:
	TcpServer * m_server;
};

class ClientThread : public Thread
{
protected:
	virtual void run ();
};

void ServerThread::run ()
{
	TcpSocket * client = nullptr;

	if (m_server) {
		while ((client = m_server->accept()) == nullptr) {
			;
		}

		if (client) {
			Logger::debug(_Fr("Client accepted on: %s: %u") % client->hostname() % client->port());
		}

		while (client) {
			ByteArray ba;
			if(client->read(ba, 1024) > 0) {
				std::cout << "Received: " << ba.c_str() << std::endl;
				if (ba == ByteArray("quit")) {
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
	TcpSocket client;
	CWT_TEST_OK(client.open("localhost", port));
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
    CWT_CHECK_SIZEOF_TYPES;
    CWT_UNUSED2(argc, argv);
    CWT_BEGIN_TESTS(2);

    TcpServer server;
    CWT_TEST_FAIL(server.listen("localhost", port));

    ServerThread serverThread(& server);
    ClientThread clientThread;

    serverThread.start();
    clientThread.start();

    clientThread.wait();
    serverThread.wait();

    CWT_END_TESTS;
    return 0;
}
