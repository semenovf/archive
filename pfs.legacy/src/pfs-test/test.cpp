/*
 * test.cpp
 *
 *  Created on: Nov 21, 2013
 *      Author: wladt
 */
#include "pfs/test/test.hpp"
#include "pfs/test/profiler.hpp"

#include <cstdlib>
#include <cstddef>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>

#if defined(NDEBUG) || defined(_NDEBUG)
#	define PFS_ASSERT(p) p
#else
#	include <cassert>
#	define PFS_ASSERT(p) assert(p)
#endif

namespace pfs { namespace test {

class context
{
public:
	context (size_t total);
	~context() { self = 0; }

	size_t total     () const { return _total; }
	size_t processed () const { return _processed; }
	size_t passed    () const { return _passed; }
	size_t failed    () const { return _failed; }

	void add_total (size_t n) { _total += n; }
	void test_ok ()      { ++_processed; ++_passed; }
	void test_failed ()  { ++_processed; ++_failed; }

	// 0 - success
	// 1 - failed
	// 2 - incomplete
	int  result () const
	{
		  return _failed > 0
				? 1
				: _processed == _total ? 0 : 2;
	}
	std::string output_prefix (bool passed) const;
	double elapsed_time() const;

	static context & instance ();
	static std::string stringify_time (double sec);

private:
	static context * self;

	size_t   _total;
	size_t   _processed;
	size_t   _passed;
	size_t   _failed;
	profiler _profiler;
};

context * context::self = 0;

inline context & context::instance ()
{
	PFS_ASSERT(self);
	return *self;
}

inline context::context (size_t total)
	: _total(total)
	, _processed(0)
	, _passed(0)
	, _failed(0)
	, _profiler() // start stopwatch
{
	PFS_ASSERT(self == 0);
	self = this;
}


double context::elapsed_time() const
{
	return _profiler.ellapsed();
}

/*
void context::timeDiff (const struct timeval & begin
		, const struct timeval & end
		, long & sec_diff, long & usec_diff)
{
	long diff = end.tv_usec - begin.tv_usec;
	usec_diff = diff < 0L ? 1000000 + diff : diff;
	sec_diff  = end.tv_sec - begin.tv_sec - (diff < long(0) ? 1 : 0);
}
*/

std::string context::stringify_time (double t)
{
	std::stringstream ss;
	long sec, usec;

	sec = static_cast<long>(t);
	usec = static_cast<long>((t - long(t)) * 1000000);

	ss << std::setw(6) << std::setfill('0') << sec << '.'
	   << std::setw(6) << std::setfill('0') << usec;

	return ss.str();
}

std::string context::output_prefix (bool passed) const
{
	std::stringstream ss;
	ss << stringify_time(_profiler.ellapsed())
		<< (passed ? " PASS[" : " FAIL[")
		<< std::setw(3) << std::setfill('0') << _processed
		<< "]: ";

	return ss.str();
}

void start (size_t n)
{
	context * ctx = new context(n);
	PFS_ASSERT(ctx);
}

int finish (bool abort)
{
	context & ctx = context::instance();
	double elapsed = ctx.elapsed_time();

	std::cout.precision(6);
    std::cout << "-----------------------------------" << std::endl
	          << "Results: total=" << ctx.total()
	          << ", run="          << ctx.processed()
	          << ", ok="           << ctx.passed()
	          << ", failed="       << ctx.failed() << std::endl;

    if (ctx.total() != ctx.processed())
		std::cerr << "WARN: Incomplete tests" << std::endl;

	std::cout << "Elapsed time: " << elapsed << " seconds" << std::endl;
	std::cout << "Total result: "
			  << (ctx.failed() > 0
			  	  ? "FAILURE"
			  	  : (ctx.processed() == ctx.total()
			  			  ? "SUCCESS"
			  			  : "INCOMPLETE"))
			  << std::endl;

	int r = ctx.result();
	delete & ctx;

	if (abort)
		::exit(r);

	return r;
}

void add_total (size_t n)
{
	context & ctx = context::instance();
	ctx.add_total(n);
}

void add_total_x (size_t n, std::string const & caption)
{
	context & ctx = context::instance();
	ctx.add_total(n);
    std::cout << ctx.output_prefix(true) << "*** " << caption << std::endl;
}

void todo (const std::string & expr, bool result, const std::string & filename, int line)
{
	context & ctx = context::instance();

	if (result) {
		ctx.test_ok();
		std::cout << ctx.output_prefix(true) << expr << std::endl;
	} else {
		ctx.test_failed();
		std::cout << ctx.output_prefix(false) << expr << " ["
				<< filename << ':' << line << ']' << std::endl;
	}
}

}} // pfs::test
