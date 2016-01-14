/*
 * test.cpp
 *
 *  Created on: Nov 21, 2013
 *      Author: wladt
 */
#include "pfs/test/test.hpp"
#include "pfs/test/stopwatch.hpp"
#include "test_p.hpp"

#include <cstdlib>
#include <cstddef>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>

namespace pfs { namespace test {

class context
{
public:
	context (size_t total);
	~context() { self = nullptr; }

	size_t total     () const { return _total; }
	size_t processed () const { return _processed; }
	size_t passed    () const { return _passed; }
	size_t failed    () const { return _failed; }

	void addTotal (size_t n) { _total += n; }
	void testOk ()      { ++_processed; ++_passed; }
	void testFailed ()  { ++_processed; ++_failed; }

	// 0 - success
	// 1 - failed
	// 2 - incomplete
	int  result () const
	{
		  return _failed > 0
				? 1
				: _processed == _total ? 0 : 2;
	}
	std::string outputPrefix (bool passed) const;
	double elapsedTime() const;

	static context & instance ();
	static std::string stringifyTime (double sec);

private:
	static context * self;

	size_t _total;
	size_t _processed;
	size_t _passed;
	size_t _failed;
	stopwatch _sw;
};

context * context::self = nullptr;

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
	, _sw() // start stopwatch
{
	PFS_ASSERT(self == nullptr);
	self = this;
}


double context::elapsedTime() const
{
	return _sw.ellapsed();
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

std::string context::stringifyTime (double t)
{
	std::stringstream ss;
	long sec, usec;

	sec = long(t);
	usec = long((t - long(t)) * 1000000);

	ss << std::setw(6) << std::setfill('0') << sec << '.'
	   << std::setw(6) << std::setfill('0') << usec;

	return ss.str();
}

std::string context::outputPrefix (bool passed) const
{
	std::stringstream ss;
	ss << stringifyTime(_sw.ellapsed())
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
	double elapsed = ctx.elapsedTime();

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

void addTotal (size_t n)
{
	context & ctx = context::instance();
	ctx.addTotal(n);
}

void todo (const std::string & expr, bool result, const std::string & filename, int line)
{
	context & ctx = context::instance();

	if (result) {
		ctx.testOk();
		std::cout << ctx.outputPrefix(true) << expr << std::endl;
	} else {
		ctx.testFailed();
		std::cout << ctx.outputPrefix(false) << expr << " ["
				<< filename << ':' << line << ']' << std::endl;
	}
}

}} // pfs::test
