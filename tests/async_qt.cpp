/*
 * async_qt.cpp
 *
 *  Created on: May 24, 2013
 *      Author: wladt
 */

#include <cwt/test.h>
#include <cstring>
#include <QtCore/QThread>
#include <QtConcurrent/QtConcurrent>

CWT_NS_BEGIN

template <typename result_type>
class future {
public:
	future(const QFuture<result_type>& future) : m_future(future) {}
	result_type result() { return m_future.result(); }
private:
	QFuture<result_type> m_future;
};

template<typename T>
future<T> async(T (*func_ptr)()) { return future<T>(QtConcurrent::run(func_ptr)); }

CWT_NS_END

int int_hello_void() { CWT_TRACE(">>int_hello_void()"); QThread::sleep(3); return 10; }
//using namespace cwt;

int main(int argc, char *argv[])
{
    CWT_CHECK_SIZEOF_TYPES;
    CWT_UNUSED2(argc, argv);
	CWT_BEGIN_TESTS(1);

	cwt::future<int> future = cwt::async(int_hello_void);
	CWT_TEST_OK(future.result() == 10);

    CWT_END_TESTS;
}
