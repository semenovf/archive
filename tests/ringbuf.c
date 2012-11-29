/*
 * utils.c
 *
 *  Created on: 29.06.2012
 *      Author: user
 */


#include <stdio.h>
#include <cwt/types.h>
#include <cwt/test.h>
#include <cwt/ringbuf.h>

CwtRingBufferNS *__rbNS = NULL;

/* Test Maximum limit */
static void test_00(void)
{
	int i;
	CwtRingBuffer *rb = __rbNS->createSized(8, 128);
	for(i = 0; i < 256; i++ ) {
		if( i > 127 ) {
			CWT_TEST_OK(!__rbNS->put(rb, (BYTE)i));
			break;
		}
		CWT_TEST_OK(__rbNS->put(rb, (BYTE)i));
	}

	__rbNS->free(rb);
}


static void test_01(void)
{
	int i = 256;
	int n;
	BYTE obuf[7] = { 0xDE, 0xAD, 0xBE, 0xAF, 0xF0, 0x0D, 0xEB };
	BYTE ibuf[7];
	CwtRingBuffer *rb = __rbNS->create();

	while(i--) {
		__rbNS->pushBack(rb, obuf, 7);
	}

	CWT_TEST_OK(__rbNS->size(rb) == 1792);

	n = 0;
	while( __rbNS->peek(rb, ibuf, 7) ) {
		if( !(ibuf[0] == obuf[0]
		    && ibuf[1] == obuf[1]
		    && ibuf[2] == obuf[2]
		    && ibuf[3] == obuf[3]
		    && ibuf[4] == obuf[4]
		    && ibuf[5] == obuf[5]
		    && ibuf[6] == obuf[6])) {

			CWT_TEST_OK2(FALSE, _Tr("Input buffer content is not equal to output buffer content"));
			break;
		}
		__rbNS->popFront(rb, 7);
		n++;
	}

	CWT_TEST_OK(__rbNS->size(rb) == 0);
	CWT_TEST_OK(n == 256);
	__rbNS->free(rb);
}

int main(int argc, char *argv[])
{
	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	CWT_BEGIN_TESTS(132);

	__rbNS = cwt_ringbuffer_ns();

	test_00();
	test_01();

	CWT_END_TESTS;
}


