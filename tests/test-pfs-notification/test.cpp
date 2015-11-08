/**
 * @file   test.cpp
 * @author
 * @date
 *
 * @brief testing ...
 */

#include <pfs/test/test.hpp>
#include <pfs/notification.hpp>
#include <iostream>

using std::cout;
using std::endl;
typedef pfs::basic_notification<std::string> notification_type;


int main(int argc, char *argv[])
{
	PFS_UNUSED(argc);
	PFS_UNUSED(argv);

	BEGIN_TESTS(3);

	notification_type nx;
	nx.append(notification_type::string_type("Error 1"));
	nx.append(notification_type::string_type("Error 2"));
	nx.append(notification_type::string_type("Error 3"));

	TEST_OK(nx.count() == 3);

	nx.append(notification_type::string_type("Error 3"));

	TEST_OK(nx.count() == 3); // Repetition appended
	TEST_OK(nx.last_repetitions() == 2);

	cout << nx;

	END_TESTS;
}

