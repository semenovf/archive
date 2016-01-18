/*
 * test_poll.cpp
 *
 *  Created on: Jan 13, 2016
 *      Author: wladt
 */

#include <pfs/test/test.hpp>
#include <pfs/vector.hpp>
#include <pfs/thread.hpp>
#include <pfs/byte_string.hpp>
#include <pfs/io/device.hpp>
#include <pfs/io/inet_server.hpp>
#include <pfs/io/pool.hpp>
#include <iostream>

#define BUFFER_SIZE 1
#define NCLIENTS    20
#define SERVER_ADDR _u8("127.0.0.1")
#define SERVER_PORT 10299
#define SERVER_BACKLOG 10

namespace {

using std::cout;
using std::endl;

using pfs::io::tcp_socket;
using pfs::io::tcp_server;
using pfs::net::inet4_addr;

static const char * loremipsum [] = {
	  "1.Lorem ipsum dolor sit amet, consectetuer adipiscing elit,"
	, "2.sed diam nonummy nibh euismod tincidunt ut laoreet dolore"
	, "3.magna aliquam erat volutpat. Ut wisi enim ad minim veniam,"
	, "4.quis nostrud exerci tation ullamcorper suscipit lobortis"
	, "5.nisl ut aliquip ex ea commodo consequat. Duis autem vel eum"
	, "6.iriure dolor in hendrerit in vulputate velit esse molestie"
	, "7.consequat, vel illum dolore eu feugiat nulla facilisis at"
	, "8.vero eros et accumsan et iusto odio dignissim qui blandit"
	, "9.praesent luptatum zzril delenit augue duis dolore te feugait"
	, "10.nulla facilisi. Nam liber tempor cum soluta nobis eleifend"
	, "11.option congue nihil imperdiet doming id quod mazim placerat"
	, "12.facer possim assum. Typi non habent claritatem insitam; est"
	, "13.usus legentis in iis qui facit eorum claritatem."
	, "14.Investigationes demonstraverunt lectores legere me lius quod"
	, "15.ii legunt saepius. Claritas est etiam processus dynamicus,"
	, "16.qui sequitur mutationem consuetudium lectorum. Mirum est"
	, "17.notare quam littera gothica, quam nunc putamus parum claram,"
	, "18.anteposuerit litterarum formas humanitatis per seacula quarta"
	, "19.decima et quinta decima. Eodem modo typi, qui nunc nobis"
	, "20.videntur parum clari, fiant sollemnes in futurum."
	, "21.Lorem ipsum dolor sit amet, consectetuer adipiscing elit,"
	, "22.sed diam nonummy nibh euismod tincidunt ut laoreet dolore"
	, "23.magna aliquam erat volutpat. \"Ut wisi enim ad minim veniam,"
	, "24.quis nostrud exerci tation ullamcorper suscipit lobortis"
	, "25.nisl ut aliquip ex ea commodo consequat. Duis autem vel eum"
	, "26.iriure dolor in hendrerit in vulputate velit esse molestie"
	, "27.consequat, vel illum dolore eu feugiat nulla facilisis at"
	, "28.vero eros et accumsan et iusto odio dignissim qui blandit"
	, "29.praesent luptatum zzril delenit augue duis dolore te feugait"
	, "30.nulla facilisi. Nam liber tempor cum soluta nobis eleifend"
	, "31.option congue nihil imperdiet doming id quod mazim placerat"
	, "32.facer possim assum. Typi non habent claritatem insitam; est"
	, "33.usus legentis in iis qui facit eorum claritatem."
	, "34.Investigationes demonstraverunt lectores legere me lius quod"
	, "35.ii legunt saepius. Claritas est etiam processus dynamicus,"
	, "36.qui sequitur mutationem consuetudium lectorum. Mirum est"
	, "37.notare quam littera gothica, quam nunc putamus parum claram,"
	, "38.anteposuerit litterarum formas humanitatis per seacula quarta"
	, "39.decima et quinta decima.\" Eodem modo typi, qui nunc nobis"
	, "40.videntur parum clari, fiant sollemnes in futurum."
};

struct dispatcher_context : public pfs::io::pool::dispatcher_context
{
	pfs::io::pool & pool;
	pfs::byte_string sample;

	dispatcher_context (pfs::io::pool & p)
			: pool(p)
	{
		int n = sizeof(loremipsum)/sizeof(loremipsum[0]);

		for (int i = 0; i < n; ++i) {
			sample.append(loremipsum[i]);
		}
	}

	virtual bool finish ()
	{
		return pool.device_count() == 0;
	}

	virtual void on_connected (pfs::io::device & d)
	{
		std::cout << "Socket connected" << std::endl;
	}

	virtual void on_ready_read (pfs::io::device & d)
	{
		pfs::byte_string bytes;
		pfs::error_code ex = d.read(bytes);

		TEST_OK(! ex);

		std::cout << "Bytes read: " << bytes.size() << std::endl;

		TEST_OK(bytes == sample);
	}

	virtual void on_disconnected (pfs::io::device & d)
	{
		std::cout << "Connection closed" << std::endl;
	}

	virtual void on_error (const pfs::error_code & ex)
	{
		std::cerr << "ERROR (server):" << pfs::to_string(ex) << std::endl;
	}
};

class ServerThread : public pfs::thread
{
	pfs::io::server _server;

public:
	ServerThread ()
		: pfs::thread()
	{
		ADD_TESTS(1);

		pfs::error_code ex = pfs::io::open_server(_server
				, pfs::io::open_params<tcp_server>(inet4_addr(SERVER_ADDR)
						, SERVER_PORT
						, SERVER_BACKLOG));

		if (ex) {
			std::cerr << "ERROR (server): open failed:" << pfs::to_string(ex) << std::endl;
		}

		TEST_FAIL2(!ex, "Open server socket");
	}

	virtual void run ()
	{
		ADD_TESTS(NCLIENTS);

		if (!_server.opened())
			return;

		pfs::io::pool pool;
		pool.push_back(_server);

		dispatcher_context ctx(pool);

		pool.dispatch(ctx, pfs::io::poll_all, 100);
		std::cout << "***SERVER FINISHED***" << std::endl;
	}
};

class ClientThread : public pfs::thread
{
public:
	ClientThread ()
		: pfs::thread()
	{}

	virtual void run ()
	{
		ADD_TESTS(2);

		pfs::io::device client;

		pfs::error_code ex = pfs::io::open_device(client
				, pfs::io::open_params<tcp_socket>(inet4_addr(SERVER_ADDR), SERVER_PORT));

		TEST_OK2(!ex, "Open client socket");

		if (ex) {
			std::cerr << "ERROR (client): " << pfs::to_string(ex) << std::endl;
			return;
		}

		int n = sizeof(loremipsum)/sizeof(loremipsum[0]);
		int n1 = 0;

		pfs::byte_string sample;

		for (int i = 0; i < n; ++i) {
			sample.append(loremipsum[i]);
		}

		for (int i = 0; i < n; ++i) {
			pfs::byte_string data(loremipsum[i]);

			if (client.write(data))
				++n1;
		}

		client.close();

		TEST_OK2(n == n1, "Data successfully sent by client");
	}
};

}

void test_pool_dispatcher ()
{
	ServerThread server;
	ClientThread clients[NCLIENTS];

	server.start();

	for (int i = 0; i < NCLIENTS; ++i) {
		clients[i].start();
	}

	for (int i = 0; i < NCLIENTS; ++i) {
		clients[i].wait();
	}

	server.wait();
}
