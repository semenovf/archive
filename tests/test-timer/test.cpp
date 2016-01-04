/**
 * @file   test.cpp
 * @author
 * @date
 *
 * @brief testing ...
 */

#include <pfs/test/test.hpp>
#include <pfs/timer.hpp>
#include <iostream>
#include <cerrno>
#include <cstring>

using std::cout;
using std::cerr;
using std::endl;

int main(int argc, char *argv[])
{
	PFS_UNUSED(argc);
	PFS_UNUSED(argv);

	BEGIN_TESTS(3);

//    char    buffer[1024], *line;
    int t1, t2, warned1;

    if (timeout_init()) {
        cerr << "timeout_init(): " << strerror(errno) << endl;
        return 1;
    }

    cout << "You have five seconds to type something" << endl;
    t1 = timeout_set(2.5);
//    warned1 = 0;
    t2 = timeout_set(5.0);

    while (1) {
        if (timeout_passed(t1)) {
            /* Print only the first time we notice. */
//            if (!warned1++)
                cout << "Passed 2.5 seconds" << endl;
        }

        if (timeout_passed(t2)) {
        	cout << "Passed 5.0 seconds" << endl;
            break;
        }

        cout << "Wait ..." << endl;
//        line = fgets(buffer, sizeof(buffer), stdin);
//        if (line) {
//            cout << endl << "Ok, you typed: " << line << endl;
//            break;
//        }
    }

    /* The two timeouts are no longer needed */
    TEST_OK(timeout_unset(t1) >= 0);
    TEST_OK(timeout_unset(t2) >= 0);

    TEST_OK(timeout_done() == 0);

	END_TESTS;
}

