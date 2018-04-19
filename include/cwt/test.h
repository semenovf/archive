/*
 * test.h
 *
 *  Created on: 06.12.2011
 *      Author: user
 */

#ifndef __CWT_TEST_H__
#define __CWT_TEST_H__

#include <stdio.h>
#include <stdlib.h>
#include <cwt/global.h>
#include <cwt/logger.h>

#ifndef CWT_USE_TESTS
#	error Need to compile sources with CWT_USE_TESTS macro defined
#endif

typedef struct _CwtTestContext {
	int m_total_tests;
	int m_run_tests;
	int m_ok_tests;
	int m_failed_tests;
} CwtTestContext;

EXTERN_C_BEGIN
DLL_API_EXPORT CwtTestContext* cwt_reset_test_context(int ntests);
DLL_API_EXPORT CwtTestContext* cwt_test_context(void);
EXTERN_C_END


#define CWT_BEGIN_TESTS(n)  cwt_reset_test_context(n)


#define CWT_END_TESTS {                                                       \
	CwtTestContext* test_ctx = cwt_test_context();                            \
    cwt_logger_ns()->trace(_Tr("TEST: -----------------------------------")); \
	cwt_logger_ns()->trace(_Tr("TEST: Results: total=%d, run=%d, ok=%d, failed=%d")     \
		, test_ctx->m_total_tests                                             \
        , test_ctx->m_run_tests                                               \
        , test_ctx->m_ok_tests                                                \
        , test_ctx->m_failed_tests); 	                                      \
	if( test_ctx->m_total_tests != test_ctx->m_run_tests )                    \
		cwt_logger_ns()->warn(_Tr("Incomplete tests"));                       \
	cwt_logger_ns()->trace(_Tr("TEST: Total result: %s"),                     \
		(test_ctx->m_run_tests                                                \
        == test_ctx->m_ok_tests                                               \
           ? _Tr("success") : _Tr("failure")));                               \
	exit (test_ctx->m_run_tests                                               \
        == test_ctx->m_ok_tests ? EXIT_SUCCESS : EXIT_FAILURE);               \
}

#define CWT_TEST_OK(exp) if( (exp) ) {        	                              \
		CwtTestContext* test_ctx = cwt_test_context();                        \
		test_ctx->m_ok_tests++;						                          \
		cwt_logger_ns()->trace(_Tr("TEST[%03u]: %s => ok")                    \
        	, ++test_ctx->m_run_tests, __WIDEN(#exp));                        \
	} else {                              	                                  \
		CwtTestContext* test_ctx = cwt_test_context();                        \
		test_ctx->m_failed_tests++;					                          \
		cwt_logger_ns()->trace(_Tr("TEST[%03u]: %s => failed at %s:%d")       \
			, ++test_ctx->m_run_tests                                         \
			, __WIDEN(#exp), __TFILE__, __LINE__);                            \
	}

#define CWT_TEST_OK2(exp,desc) if( (exp) ) {   	                              \
		CwtTestContext* test_ctx = cwt_test_context();                        \
		test_ctx->m_ok_tests++;						                          \
		cwt_logger_ns()->trace(_Tr("TEST[%03u]: %s => ok")                    \
        	, ++test_ctx->m_run_tests, desc);                                 \
	} else {                              	                                  \
		CwtTestContext* test_ctx = cwt_test_context();                        \
		test_ctx->m_failed_tests++;					                          \
		cwt_logger_ns()->trace(_Tr("TEST[%03u]: %s => failed at %s:%d")       \
			, ++test_ctx->m_run_tests, desc                                   \
            , __TFILE__, __LINE__);                                           \
	}

#define CWT_TEST_NOK(exp) if( !(exp) ) {        	                          \
		CwtTestContext* test_ctx = cwt_test_context();                        \
		test_ctx->m_ok_tests++;						                          \
		cwt_logger_ns()->trace(_Tr("TEST[%03u]: ! %s => ok")                  \
			, ++test_ctx->m_run_tests, __WIDEN(#exp));                        \
	} else {                              	                                  \
		CwtTestContext* test_ctx = cwt_test_context();                        \
		test_ctx->m_failed_tests++;					                          \
		cwt_logger_ns()->trace(_Tr("TEST[%03u]: ! %s => failed at %s:%d")     \
			, ++test_ctx->m_run_tests                                         \
            , __WIDEN(#exp), __TFILE__, __LINE__);                            \
	}


#define CWT_TEST_FAIL(exp) if( (exp) ) {        	                          \
		CwtTestContext* test_ctx = cwt_test_context();                        \
		test_ctx->m_ok_tests++;						                          \
		cwt_logger_ns()->trace(_Tr("TEST[%03u]: %s => ok")                    \
			, ++test_ctx->m_run_tests, __WIDEN(#exp));                        \
	} else {                              	                                  \
		CwtTestContext* test_ctx = cwt_test_context();                        \
		test_ctx->m_failed_tests++;					                          \
		cwt_logger_ns()->trace(_Tr("TEST[%03u]: %s => failed at %s:%d")       \
			, ++test_ctx->m_run_tests                                         \
            , __WIDEN(#exp), __TFILE__, __LINE__);                            \
		CWT_END_TESTS;                                                        \
		exit(EXIT_FAILURE);                                                   \
	}

#define CWT_TEST_FAIL2(exp,desc) if( (exp) ) {        	                      \
		CwtTestContext* test_ctx = cwt_test_context();                        \
		test_ctx->m_ok_tests++;						                          \
		cwt_logger_ns()->trace(_Tr("TEST[%03u]: %s => ok")                    \
			, ++test_ctx->m_run_tests, desc);                                 \
	} else {                              	                                  \
		CwtTestContext* test_ctx = cwt_test_context();                        \
		test_ctx->m_failed_tests++;					                          \
		cwt_logger_ns()->trace(_Tr("TEST[%03u]: failed at %s:%d, %s")         \
			, ++test_ctx->m_run_tests                                         \
            , __TFILE__, __LINE__, desc);                                     \
		CWT_END_TESTS;                                                        \
		exit(EXIT_FAILURE);                                                   \
	}

#endif /* __CWT_TEST_H__ */
