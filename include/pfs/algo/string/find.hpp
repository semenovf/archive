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
		  typename String::iterator haystackBegin
		, typename String::iterator haystackEnd
		, typename String::iterator needleBegin
		, typename String::iterator needleEnd)
{
	typedef typename String::iterator iterator;

	if (haystackEnd < haystackBegin)
		return iterator(haystackEnd);

	if (needleEnd <= needleBegin)
		return iterator(haystackEnd);

	while (haystackBegin != haystackEnd) {
		if (*haystackBegin == *needleBegin) {
			iterator it(haystackBegin);
			iterator it1(needleBegin);

			++it;
			++it1;

			while (it != haystackEnd && it1 != needleEnd) {
				if (*it != *it1)
					break;
				++it;
				++it1;
			}

			if (it1 == needleEnd)
				return iterator(haystackBegin);
		}

		++haystackBegin;
	}

	return iterator(haystackEnd);
}

} // pfs

#endif /* __PFS_ALGO_STRING_FIND_HPP__ */
