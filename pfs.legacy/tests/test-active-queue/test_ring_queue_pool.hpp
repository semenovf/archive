/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   test_ring_queue_pool.hpp
 * Author: wladt
 *
 * Created on October 26, 2016, 9:48 AM
 */

#ifndef __TEST_RING_QUEUE_POOL_HPP__
#define __TEST_RING_QUEUE_POOL_HPP__

#include <pfs/test/test.hpp>
#include <pfs/atomic.hpp>
#include <pfs/mutex.hpp>
#include <pfs/exception.hpp>
#include <pfs/tl/ring_queue_pool.hpp>

namespace test {
namespace ring_queue_pool {

struct empty_exception : public pfs::length_error
{
    empty_exception () : length_error("") {}
};

struct traits
{
    typedef size_t             size_type;
    typedef pfs::atomic_size_t atomic_type;
    typedef pfs::mutex         mutex_type;
    typedef empty_exception    empty_exception_type;

    static size_t const default_increment_factor = 32;
};

typedef pfs::tl::ring_queue_pool<traits> ring_queue_pool_type;

struct A
{
    char c;
};

struct B
{
    int x;
};

struct C
{
    int x;
    double y;
};

void test_basic ()
{
    ADD_TESTS(18);
    
    size_t const initial_capacity = sizeof(A) + sizeof(B) + sizeof(C);
    size_t const max_capacity = initial_capacity * 2;
    size_t const increment_factor = 0;
    
    ring_queue_pool_type rqp(initial_capacity, max_capacity, increment_factor);

    A a = {'W'};
    B b = {1024};
    C c = {1234, 3.14159f};
    
    TEST_OK(rqp.empty());
    TEST_OK(rqp.count() == 0);
    
    TEST_FAIL(rqp.push<A>(a));
    TEST_FAIL(rqp.push<B>(b));
    TEST_FAIL(rqp.push<C>(c));
    
    TEST_OK(!rqp.empty());
    TEST_OK(rqp.count() == 3);
    
//    TEST_OK(rqp.front<A>().c == 'W');
//    TEST_OK(rqp.back<C>().x == 1234 && rqp.back<C>().y == 3.14159f);
    
    rqp.pop<A>();
//    TEST_OK(rqp.front<B>().x == 1024);
//    TEST_OK(rqp.back<C>().x == 1234 && rqp.back<C>().y == 3.14159f);
    
    rqp.pop<B>();
//    TEST_OK(& rqp.front<C>() == & rqp.back<C>());
//    TEST_OK(rqp.back<C>().x == 1234 && rqp.back<C>().y == 3.14159f);
    
    rqp.pop<C>();
    TEST_OK(rqp.empty());
    
    TEST_FAIL(rqp.push<A>(a));
    TEST_FAIL(rqp.push<B>(b));
    TEST_FAIL(rqp.push<C>(c));
    TEST_FAIL(rqp.push<A>(a));
    TEST_FAIL(rqp.push<B>(b));
    TEST_FAIL(rqp.push<C>(c));

    TEST_FAIL(rqp.count() == 6);
    TEST_FAIL(! rqp.push<A>(a)); // Not enough space
    
    rqp.pop<A>();
    TEST_OK(rqp.count() == 5);
    TEST_FAIL(rqp.push<A>(a));
//    TEST_OK(rqp.back<A>().c == 'W');
//    TEST_OK(rqp.front<B>().x == 1024);
}

}}

#endif /* __TEST_RING_QUEUE_POOL_HPP__ */

