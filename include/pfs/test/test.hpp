/*
 * test.hpp
 *
 *  Created on: 21.11.2013
 *      Author: wladt
 */

#ifndef __PFS_TEST_HPP__
#define __PFS_TEST_HPP__

#include <string>
#include "dllapi.h"

#define BEGIN_TESTS(n) pfs::test::start(n);
#define END_TESTS      pfs::test::finish(false);
#define ABORT_TESTS    pfs::test::finish(true);
#define ADD_TESTS(n)   pfs::test::addTotal(n);

#define TEST_OK(expr) if ((expr)) {                          \
		pfs::test::todo(#expr, true, __FILE__, __LINE__);    \
	} else {                                                 \
		pfs::test::todo(#expr, false, __FILE__, __LINE__);   \
	}

#define TEST_OK_X(expr, stmt) if ((expr)) {                  \
		pfs::test::todo(#expr, true, __FILE__, __LINE__);    \
	} else {                                                 \
		pfs::test::todo(#expr, false, __FILE__, __LINE__);   \
		stmt;                                                \
	}

#define TEST_OK2(expr,desc) if ((expr)) {                    \
		pfs::test::todo(desc, true, __FILE__, __LINE__);     \
	} else {                                                 \
		pfs::test::todo(desc, false, __FILE__, __LINE__);    \
	}

#define TEST_FAIL(expr) if ((expr)) {                        \
		pfs::test::todo(#expr, true, __FILE__, __LINE__);    \
	} else {                                                 \
		pfs::test::todo(#expr, false, __FILE__, __LINE__);   \
		ABORT_TESTS                                          \
	}

#define TEST_FAIL_X(expr,stmt) if ((expr)) {                 \
		pfs::test::todo(#expr, true, __FILE__, __LINE__);    \
	} else {                                                 \
		pfs::test::todo(#expr, false, __FILE__, __LINE__);   \
		stmt;                                                \
		ABORT_TESTS                                          \
	}

#define TEST_FAIL2(expr,desc) if ((expr)) {                  \
		pfs::test::todo(desc, true, __FILE__, __LINE__);     \
	} else {                                                 \
		pfs::test::todo(desc, false, __FILE__, __LINE__);    \
		ABORT_TESTS                                          \
}

namespace pfs { namespace test {

void DLL_API start  (size_t n);
int  DLL_API finish (bool abort);
void DLL_API addTotal (size_t n);
void DLL_API todo (const std::string & expr, bool result, const std::string & filename, int line);

}} // pfs::test

#endif /* __PFS_TEST_H__ */