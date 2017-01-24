/*
 * fs_join.cpp
 *
 *  Created on: Oct 20, 2015
 *      Author: wladt
 */

#include <sstream>

namespace test {

struct canonical_data_t
{
	const string_type path_str;
	const string_type canonical;
};

static canonical_data_t canonical_data[] = {
	  { string_type("abc/../../abc/file.txt") , string_type("abc/file.txt") }
	, { string_type("/")                      , string_type("/") }
	, { string_type("/abc/../abc/file.txt")   , string_type("/abc/file.txt") }
	, { string_type("abc/../abc/file.txt")    , string_type("abc/file.txt") }
	, { string_type("abc/../../abc/file.txt") , string_type("abc/file.txt") }
	, { string_type(), string_type() }


//	  { { "/",  0 },  "/" }
//	, { { "/", "/", 0 },  "/" }
//	, { { "/", "/", "/", 0 },  "/" }
//	, { { "/", "a", "/", 0 },  "/a/" }
//	, { { "/", "a", 0 },  "/a" }
//	, { { "/", "a", "b", 0 }, "/a/b" }
//	, { { "/", "a", "b", "c", 0 }, "/a/b/c" }
//	, { { "a", "b", "c", 0 }, "a/b/c" }
//	, { { "a", "b", "/", 0 }, "a/b/" }
//	, { { "a", "b", "/", "/", 0 }, "a/b/" }
//	, { { "a", "b", "/", "/", "/", 0 }, "a/b/" }
//	, { { 0 }, 0 }
};

void canonical ()
{
	ADD_TESTS(1 + sizeof(canonical_data)/sizeof(canonical_data[0]) - 1);

	TEST_OK(path_type().canonical() == path_type());

	canonical_data_t * p = & canonical_data[0];

	while (!p->path_str.empty()) {
		std::ostringstream oss;

		path_type canonical = path_type(p->path_str).canonical();

		oss << "'" << canonical.native()
			<< "' is canonical for '"
			<< p->path_str << "'";

		bool result = canonical == path_type(p->canonical);

		if (!result) {
			oss << "' but expected '" << p->canonical << "'";
		}

		TEST_OK2(result, oss.str().c_str());

		++p;
	}
}

}