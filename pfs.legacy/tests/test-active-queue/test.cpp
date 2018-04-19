/**
 * @file   test.cpp
 * @author
 * @date
 *
 * @brief testing ...
 */

#include <pfs/test/test.hpp>
#include <pfs.hpp>
#include "test_trivial_list.hpp"
#include "test_ring_queue.hpp"
#include "test_ring_queue_pool.hpp"
#include "test_active_queue.hpp"
#include "test_consumer_producer.hpp"

#include <iostream>

using std::cout;
using std::endl;

int main(int argc, char *argv[])
{
    PFS_UNUSED(argc);
    PFS_UNUSED(argv);

    BEGIN_TESTS(0);

    test::trivial_list::test_basic();
    test::ring_queue::test_basic();
    test::ring_queue_pool::test_basic();
    test::active_queue::test0::test();
    test::active_queue::test1::test();
    test::active_queue::test2::test();
    test::active_queue::test3::test();
    test::active_queue::test4::test();
    test::active_queue::consumer_producer::test();

    return END_TESTS;
}

