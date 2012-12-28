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
#include <cwt/bytearr.h>

CwtRingBufferNS *__rb_ns = NULL;
CwtByteArrayNS  *__ba_ns = NULL;

/* Test Maximum limit */
static void test_00(void)
{
	int i;
	CwtRingBuffer *rb = __rb_ns->createSized(8, 128);
	for(i = 0; i < 256; i++ ) {
		if( i > 127 ) {
			CWT_TEST_OK(!__rb_ns->put(rb, (BYTE)i));
			break;
		}
		CWT_TEST_OK(__rb_ns->put(rb, (BYTE)i));
	}

	__rb_ns->free(rb);
}


static void test_01(void)
{
	int i = 256;
	int n;
	BYTE obuf[7] = { 0xDE, 0xAD, 0xBE, 0xAF, 0xF0, 0x0D, 0xEB };
	CwtRingBuffer *rb = __rb_ns->create();
	CwtByteArray  *ba = __ba_ns->create();

	while(i--)
		__rb_ns->pushBack(rb, obuf, 7);

	CWT_TEST_OK(__rb_ns->size(rb) == 1792);

	n = 0;
	while( __rb_ns->peek(rb, ba, 7) ) {
		const BYTE *ibuf;

		CWT_TEST_FAIL(__ba_ns->size(ba) == 7);
		ibuf = __ba_ns->constData(ba);

		if( !( ibuf[0] == obuf[0]
		    && ibuf[1] == obuf[1]
		    && ibuf[2] == obuf[2]
		    && ibuf[3] == obuf[3]
		    && ibuf[4] == obuf[4]
		    && ibuf[5] == obuf[5]
		    && ibuf[6] == obuf[6])) {

			CWT_TEST_OK2(FALSE, _Tr("Input buffer content is not equal to output buffer content"));
			break;
		}
		__rb_ns->popFront(rb, 7);
		__ba_ns->clear(ba);
		n++;
	}

	CWT_TEST_OK(__rb_ns->size(rb) == 0);
	CWT_TEST_OK(n == 256);
	__rb_ns->free(rb);
	__ba_ns->free(ba);
}

int main(int argc, char *argv[])
{
	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	CWT_BEGIN_TESTS(388);

	__rb_ns = cwt_ringbuffer_ns();
	__ba_ns = cwt_bytearray_ns();

	test_00();
	test_01();

	CWT_END_TESTS;
}


