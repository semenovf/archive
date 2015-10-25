/*
 * find.hpp
 *
 *  Created on: Oct 21, 2015
 *      Author: wladt
 */

#ifndef __PFS_ALGO_STRING_FIND_HPP__
#define __PFS_ALGO_STRING_FIND_HPP__

namespace pfs {

template <typename InputIterator>
InputIterator find (
		  InputIterator haystackBegin
		, InputIterator haystackEnd
		, InputIterator needleBegin
		, InputIterator needleEnd)
{
	if (haystackEnd < haystackBegin)
		return InputIterator(haystackEnd);

	if (needleEnd <= needleBegin)
		return InputIterator(haystackEnd);

	while (haystackBegin != haystackEnd) {
		if (*haystackBegin == *needleBegin) {
			InputIterator it(haystackBegin);
			InputIterator it1(needleBegin);

			++it;
			++it1;

			while (it != haystackEnd && it1 != needleEnd) {
				if (*it != *it1)
					break;
				++it;
				++it1;
			}

			if (it1 == needleEnd)
				return InputIterator(haystackBegin);
		}

		++haystackBegin;
	}

	return InputIterator(haystackEnd);
}

} // pfs

#endif /* __PFS_ALGO_STRING_FIND_HPP__ */
