/*
 * test.c
 *
 *  Created on: 28.06.2012
 *      Author: user
 */


#include <cwt/test.h>

static CwtTestContext __cwt_test_context = { 0, 0, 0, 0 };


CwtTestContext* cwt_reset_test_context(int ntests)
{
	__cwt_test_context.m_total_tests = ntests;
	__cwt_test_context.m_run_tests
		= __cwt_test_context.m_ok_tests
		= __cwt_test_context.m_failed_tests
		= 0;
	return &__cwt_test_context;
}

CwtTestContext* cwt_test_context(void)
{
	return &__cwt_test_context;
}
