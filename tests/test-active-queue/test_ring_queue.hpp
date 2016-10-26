/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   test_ring_queue.hpp
 * Author: wladt
 *
 * Created on October 26, 2016, 9:10 AM
 */

#ifndef __TEST_RING_QUEUE_HPP__
#define __TEST_RING_QUEUE_HPP__

#include <pfs/test/test.hpp>
#include <pfs/exception.hpp>
#include <pfs/tl/ring_queue.hpp>

namespace test {
namespace ring_queue {

struct empty_exception : public pfs::length_error
{
    empty_exception () : length_error("") {}
};

struct traits
{
    typedef size_t size_type;
    typedef empty_exception empty_exception_type;
};

typedef pfs::tl::ring_queue<traits> ring_queue_type;

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
    ADD_TESTS(23);
    
    size_t const capacity = sizeof(A) + sizeof(B) + sizeof(C);
    ring_queue_type rq(capacity);
    
    A a = {'W'};
    B b = {1024};
    C c = {1234, 3.14159f};
    
    TEST_OK(rq.empty());
    TEST_OK(rq.capacity() == capacity);
    
    TEST_FAIL(rq.push<A>(a));
    TEST_FAIL(rq.push<B>(b));
    TEST_FAIL(rq.push<C>(c));
    
    TEST_OK(!rq.empty());
    TEST_OK(rq.count() == 3);
    
    TEST_OK(rq.front<A>().c == 'W');
    TEST_OK(rq.back<C>().x == 1234 && rq.back<C>().y == 3.14159f);
    
    rq.pop<A>();
    TEST_OK(rq.front<B>().x == 1024);
    TEST_OK(rq.back<C>().x == 1234 && rq.back<C>().y == 3.14159f);
    
    rq.pop<B>();
    TEST_OK(& rq.front<C>() == & rq.back<C>());
    TEST_OK(rq.back<C>().x == 1234 && rq.back<C>().y == 3.14159f);
    
    rq.pop<C>();
    TEST_OK(rq.empty());
    
    TEST_FAIL(rq.push<A>(a));
    TEST_FAIL(rq.push<B>(b));
    TEST_FAIL(rq.push<C>(c));

    TEST_OK(! rq.push<A>(a)); // Not enough space
    
    rq.pop<A>();
    TEST_OK(rq.count() == 2);
    TEST_FAIL(rq.push<A>(a));
    TEST_OK(rq.back<A>().c == 'W');
    TEST_OK(rq.front<B>().x == 1024);
    TEST_OK(rq.count() == 3);
}

}}

#endif /* __TEST_RING_QUEUE_HPP__ */

