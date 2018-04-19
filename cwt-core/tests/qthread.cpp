/*
 * qthread.cpp
 *
 *  Created on: Sep 26, 2013
 *      Author: wladt
 */

#include <cwt/test.h>
#include <QThread>

//using namespace cwt;

void test_qthread ()
{
	struct X : public QThread {
		virtual void run ()
		{
			sleep(5);
		}
	};

	X x;
	x.start();
	if( !x.wait(1000) ) {
		x.terminate();
		x.wait();
	}
}

int main(int argc, char *argv[])
{
    CWT_CHECK_SIZEOF_TYPES;
    CWT_UNUSED2(argc, argv);
	CWT_BEGIN_TESTS(0);

	if (1) test_qthread();

    CWT_END_TESTS;
}




