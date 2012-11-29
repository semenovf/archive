/*
 * test.c
 *
 *  Created on: 28.06.2012
 *      Author: user
 */


#include <cwt/test.h>

static CwtTestContext __cwtTestContext = { 0, 0, 0, 0 };


CwtTestContext* cwt_reset_test_context(int ntests)
{
	__cwtTestContext.m_total_tests = ntests;
	__cwtTestContext.m_run_tests
		= __cwtTestContext.m_ok_tests
		= __cwtTestContext.m_failed_tests
		= 0;
	return &__cwtTestContext;
}

CwtTestContext* cwt_test_context(void)
{
	return &__cwtTestContext;
}
