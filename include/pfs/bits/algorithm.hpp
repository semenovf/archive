/*
 * algorithm.hpp
 *
 *  Created on: Oct 1, 2014
 *      Author: wladt
 */

#ifndef __PFS_BITS_ALGORITHM_HPP__
#define __PFS_BITS_ALGORITHM_HPP__

#include <algorithm>

// XXX Deprecated
namespace pfs {

template <typename Iter1, typename Iter2>
int compare (Iter1 it1, Iter1 itEnd1, Iter2 it2, Iter2 itEnd2)
{
	while (it1 < itEnd1 && it2 < itEnd2) {
		if (*it1 < *it2)
			return -1;

		if (!(*it1 == *it2))
			return 1;

		++it1;
		++it2;
	}

	return (it1 == itEnd1)
			? (it2 == itEnd2) ? 0 : -1
			: 1;
}

template <typename ForwardIt1, typename ForwardIt2>
ForwardIt1 search (ForwardIt1 first1, ForwardIt1 last1
		, ForwardIt2 first2, ForwardIt2 last2)
{
	return std::search(first1, last1, first2, last2);
}

template <typename InputIt, typename T>
InputIt find (InputIt first, InputIt last, const T & val)
{
	return std::find(first, last, val);
}

} // pfs


#endif /* __PFS_BITS_ALGORITHM_HPP__ */
