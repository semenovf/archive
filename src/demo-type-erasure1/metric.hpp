/*
 * metric.hpp
 *
 *  Created on: Mar 22, 2016
 *      Author: wladt
 */

#ifndef __METRIC_HPP__
#define __METRIC_HPP__

#include <pfs/shared_ptr.hpp>

struct metric_base
{
	virtual ~metric_base () {}
};

template <typename T>
struct metric_impl : public metric_base
{
	T _d;

	metric_impl ()
	{}

	template <typename Arg1>
	metric_impl (Arg1 a1)
		: _d(a1)
	{}
};

class metric
{
	pfs::shared_ptr<metric_base> _d;

private:
	template <typename T>
	metric (metric_impl<T> * d)
		: _d(dynamic_cast<metric_base *>(d))
	{}

public:
	template <typename T>
	friend metric make_metric ()
	{
		return metric(new metric_impl<T>);
	}

	template <typename T, typename Arg1>
	friend metric make_metric (Arg1 a1)
	{
		return metric(new metric_impl<T>(a1));
	}
};

#endif /* __METRIC_HPP__ */
