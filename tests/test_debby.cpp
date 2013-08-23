/*
 * test_debby.cpp
 *
 *  Created on: Aug 23, 2013
 *      Author: wladt
 */

#include <cwt/test.h>
#include "../include/cwt/debby.hpp"

using namespace cwt;

void usage(const char * program)
{
	printf("Usage:\n\t%s db-uri\n", program);
	puts("\tdb-uri\tDatabase URI");
	puts("\tURI examples:");
	puts("\tsqlite3:///tmp/test.db?mode=rwc");
}


int main(int argc, char *argv[])
{
    CWT_CHECK_SIZEOF_TYPES;
    CWT_UNUSED2(argc, argv);
    CWT_BEGIN_TESTS(1);

    if (argc < 2) {
    	usage(argv[0]);
    	return -1;
    }

/*
    Debby debby;
    CWT_TESTdebby.parseFromFile(argv[1]);
*/

    CWT_END_TESTS;
    return 0;
}


