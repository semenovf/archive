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

struct CwtTestContext {
	int m_total_tests;
	int m_run_tests;
	int m_ok_tests;
	int m_failed_tests;
};
typedef struct CwtTestContext CwtTestContext;
extern CwtTestContext *__cwtTestContextPtr__;

EXTERN_C_BEGIN
DLL_API_EXPORT CwtTestContext* cwtResetTestContext(int ntests);
DLL_API_EXPORT CwtTestContext* cwtTestContext(void);
EXTERN_C_END


#define CWT_BEGIN_TESTS(n) \
	cwtResetTestContext(n)


#define CWT_END_TESTS {                                                       \
	CwtTestContext* __cwtTestContextPtr__ = cwtTestContext();                 \
    print_trace(_Tr("TEST: -----------------------------------")); 		      \
	printf_trace(_Tr("TEST: Results: total=%d, run=%d, ok=%d, failed=%d")     \
		, __cwtTestContextPtr__->m_total_tests                                \
        , __cwtTestContextPtr__->m_run_tests                                  \
        , __cwtTestContextPtr__->m_ok_tests                                   \
        , __cwtTestContextPtr__->m_failed_tests); 	                          \
	if( __cwtTestContextPtr__->m_total_tests != __cwtTestContextPtr__->m_run_tests )   \
		print_warn(_Tr("Incomplete tests"));                                  \
	printf_trace(_Tr("TEST: Total result: %s"),                               \
		(__cwtTestContextPtr__->m_run_tests                                   \
        == __cwtTestContextPtr__->m_ok_tests                                  \
           ? _Tr("success") : _Tr("failure")));                               \
	exit (__cwtTestContextPtr__->m_run_tests                                  \
        == __cwtTestContextPtr__->m_ok_tests ? EXIT_SUCCESS : EXIT_FAILURE);  \
}

#define CWT_TEST_OK(exp) if( (exp) ) {        	                              \
		CwtTestContext* __cwtTestContextPtr__ = cwtTestContext();             \
		__cwtTestContextPtr__->m_ok_tests++;						          \
		printf_trace(_Tr("TEST[%03u]: %s => ok")                              \
        	, ++__cwtTestContextPtr__->m_run_tests, __WIDEN(#exp));           \
	} else {                              	                                  \
		CwtTestContext* __cwtTestContextPtr__ = cwtTestContext();             \
		__cwtTestContextPtr__->m_failed_tests++;					          \
		printf_trace(_Tr("TEST[%03u]: %s => failed at %s:%d")                 \
			, ++__cwtTestContextPtr__->m_run_tests                            \
			, __WIDEN(#exp), __TFILE__, __LINE__);                            \
	}

#define CWT_TEST_OK2(exp,desc) if( (exp) ) {   	                              \
		CwtTestContext* __cwtTestContextPtr__ = cwtTestContext();             \
		__cwtTestContextPtr__->m_ok_tests++;						          \
		printf_trace(_Tr("TEST[%03u]: %s => ok")                              \
        	, ++__cwtTestContextPtr__->m_run_tests, desc);                    \
	} else {                              	                                  \
		CwtTestContext* __cwtTestContextPtr__ = cwtTestContext();             \
		__cwtTestContextPtr__->m_failed_tests++;					          \
		printf_trace(_Tr("TEST[%03u]: %s => failed at %s:%d")                 \
			, ++__cwtTestContextPtr__->m_run_tests, desc                      \
            , __TFILE__, __LINE__);                                           \
	}

/*
#define CWT_TEST_OK3(exp,fmt,args) if( (exp) ) {   	                          \
		CwtTestContext* __cwtTestContextPtr__ = cwtTestContext();             \
		__cwtTestContextPtr__->m_ok_tests++;						          \
		printf_trace(_Tr("TEST[%03u]: " fmt " => ok")                         \
        	, ++__cwtTestContextPtr__->m_run_tests, args);                    \
	} else {                              	                                  \
		CwtTestContext* __cwtTestContextPtr__ = cwtTestContext();             \
		__cwtTestContextPtr__->m_failed_tests++;					          \
		printf_trace(_Tr("TEST[%03u]: %s => failed at %s:%d")                 \
			, ++__cwtTestContextPtr__->m_run_tests, desc                      \
            , __TFILE__, __LINE__);                                           \
	}
*/



#define CWT_TEST_NOK(exp) if( !(exp) ) {        	                          \
		CwtTestContext* __cwtTestContextPtr__ = cwtTestContext();             \
		__cwtTestContextPtr__->m_ok_tests++;						          \
		printf_trace(_Tr("TEST[%03u]: ! %s => ok")                            \
			, ++__cwtTestContextPtr__->m_run_tests, __WIDEN(#exp));           \
	} else {                              	                                  \
		CwtTestContext* __cwtTestContextPtr__ = cwtTestContext();             \
		__cwtTestContextPtr__->m_failed_tests++;					          \
		printf_trace(_Tr("TEST[%03u]: ! %s => failed at %s:%d")               \
			, ++__cwtTestContextPtr__->m_run_tests                            \
            , __WIDEN(#exp), __TFILE__, __LINE__);                            \
	}


#define CWT_TEST_FAIL(exp) if( (exp) ) {        	                          \
		CwtTestContext* __cwtTestContextPtr__ = cwtTestContext();             \
		__cwtTestContextPtr__->m_ok_tests++;						          \
		printf_trace(_Tr("TEST[%03u]: %s => ok")                              \
			, ++__cwtTestContextPtr__->m_run_tests, __WIDEN(#exp));           \
	} else {                              	                                  \
		CwtTestContext* __cwtTestContextPtr__ = cwtTestContext();             \
		__cwtTestContextPtr__->m_failed_tests++;					          \
		printf_trace(_Tr("TEST[%03u]: %s => failed at %s:%d")                 \
			, ++__cwtTestContextPtr__->m_run_tests                            \
            , __WIDEN(#exp), __TFILE__, __LINE__);                            \
		CWT_END_TESTS;                                                        \
		exit(EXIT_FAILURE);                                                   \
	}

#define CWT_TEST_FAIL2(exp,desc) if( (exp) ) {        	                      \
		CwtTestContext* __cwtTestContextPtr__ = cwtTestContext();             \
		__cwtTestContextPtr__->m_ok_tests++;						          \
		printf_trace(_Tr("TEST[%03u]: %s => ok")                              \
			, ++__cwtTestContextPtr__->m_run_tests, desc);                    \
	} else {                              	                                  \
		CwtTestContext* __cwtTestContextPtr__ = cwtTestContext();             \
		__cwtTestContextPtr__->m_failed_tests++;					          \
		printf_trace(_Tr("TEST[%03u]: failed at %s:%d, %s")                   \
			, ++__cwtTestContextPtr__->m_run_tests                            \
            , __TFILE__, __LINE__, desc);                                     \
		CWT_END_TESTS;                                                        \
		exit(EXIT_FAILURE);                                                   \
	}

#endif /* __CWT_TEST_H__ */
