/*
 * find.hpp
 *
 *  Created on: Oct 21, 2015
 *      Author: wladt
 */

#ifndef __PFS_ALGO_STRING_FIND_HPP__
#define __PFS_ALGO_STRING_FIND_HPP__

namespace pfs {

template <typename String>
typename String::iterator find (
		  typename String::const_iterator haystackBegin
		, typename String::const_iterator haystackEnd
		, typename String::const_iterator needleBegin
		, typename String::const_iterator needleEnd)
{
	typedef typename String::const_iterator const_iterator;
	typedef typename String::iterator iterator;

	if (haystackEnd < haystackBegin)
		return iterator(haystackEnd);

	if (needleEnd <= needleBegin)
		return iterator(haystackEnd);

	while (haystackBegin != haystackEnd) {
		if (*haystackBegin == *needleBegin) {
			const_iterator it(haystackBegin);
			const_iterator it1(needleBegin);

			++it;
			++it1;

			while (it != haystackEnd && it1 != needleEnd) {
				if (*it != *it1)
					break;
				++it;
				++it1;
			}

			if (it == haystackEnd && it1 == needleEnd)
				return iterator(haystackBegin);
		}

		++haystackBegin;
	}

	return iterator(haystackEnd);
}

} // pfs

#endif /* __PFS_ALGO_STRING_FIND_HPP__ */
