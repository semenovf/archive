/**
 * @file fs.cpp
 * @author wladt
 * @date Apr 24, 2015
 */

#include <pfs/test/test.hpp>
#include <pfs/random.hpp>
#include <map>
#include <cstdio>

void test_random ()
{
    std::map<uint32_t, bool> map;
    pfs::random rnd;
    int i = 0;

    for (; i < 1000; ++i) {
        uint32_t r = rnd.rand();

        if (map.count(r) > 0)
            break;

        printf("%010u\r", r);

        map.insert(std::pair<uint32_t, bool>(r, true));
    }

    TEST_OK2(i == 1000, "All generated numbers are random");
}

int main (int argc, char *argv[])
{
	PFS_UNUSED2(argc, argv);
	BEGIN_TESTS(1);

	test_random();

	END_TESTS;
}



