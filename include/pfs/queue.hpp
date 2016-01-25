/*
 * queue.hpp
 *
 *  Created on: Jan 24, 2016
 *      Author: wladt
 */

#ifndef __PFS_DEQUE_HPP__
#define __PFS_DEQUE_HPP__

#include <queue>

namespace pfs {

template<typename T, typename Sequence = std::deque<T> >
class queue : public std::queue<T, Sequence>
{
	typedef std::queue<T, Sequence> base_class;

public:
    typedef typename base_class::value_type       value_type;
    typedef typename base_class::reference        reference;
    typedef typename base_class::const_reference  const_reference;
    typedef typename base_class::size_type        size_type;
    typedef typename base_class::container_type   container_type;

public:
    explicit queue ()
    	: base_class ()
    {}

    explicit queue (const Sequence & s)
    	: base_class(s)
    {}

#if __cplusplus >= 201103L
      explicit queue (Sequence && s)
      	  : base_class(s)
      {}
#endif

};

template <typename T, typename Sequence = std::vector<T>,
	   typename Compare  = std::less<typename Sequence::value_type> >
class priority_queue : public std::priority_queue<T, Sequence, Compare>
{
	typedef std::priority_queue<T, Sequence, Compare> base_class;

public:
	typedef typename base_class::value_type       value_type;
	typedef typename base_class::reference        reference;
	typedef typename base_class::const_reference  const_reference;
	typedef typename base_class::size_type        size_type;
	typedef typename base_class::container_type   container_type;

	explicit priority_queue ()
	  : base_class()
	{}

	explicit priority_queue (const Compare & x)
	  : base_class(x)
	{}

	explicit priority_queue (const Compare & x, const Sequence & s)
	  : base_class(x, s)
	{}

#if __cplusplus >= 201103L

	explicit priority_queue(const Compare & x, Sequence && s)
	  : base_class(x, s)
	{}

#endif

	template<typename InputIterator>
	priority_queue (InputIterator first, InputIterator last,
			const Compare & x = Compare(),
			const Sequence & s = Sequence())
		: base_class(first, last, x, s)
	{}

#if __cplusplus >= 201103L

	template <typename InputIterator>
	priority_queue (InputIterator first, InputIterator last
			, const Compare & x
			, Sequence && s)
		: base_class(first, last, x, s)
	{}

#endif

};

} // pfs

#endif /* __PFS_DEQUE_HPP__ */
