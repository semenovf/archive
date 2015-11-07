/*
 * find.hpp
 *
 *  Created on: Oct 21, 2015
 *      Author: wladt
 */

#ifndef __PFS_ALGO_STRING_FIND_HPP__
#define __PFS_ALGO_STRING_FIND_HPP__

namespace pfs {

template <typename InputIt1, typename InputIt2>
InputIt1 find (
		  InputIt1 haystackBegin
		, InputIt1 haystackEnd
		, InputIt2 needleBegin
		, InputIt2 needleEnd)
{
	if (haystackEnd < haystackBegin)
		return InputIt1(haystackEnd);

	if (needleEnd <= needleBegin)
		return InputIt1(haystackEnd);

	while (haystackBegin != haystackEnd) {
		if (*haystackBegin == *needleBegin) {
			InputIt1 it(haystackBegin);
			InputIt2 it1(needleBegin);

			++it;
			++it1;

			while (it != haystackEnd && it1 != needleEnd) {
				if (*it != *it1)
					break;
				++it;
				++it1;
			}

			if (it1 == needleEnd)
				return InputIt1(haystackBegin);
		}

		++haystackBegin;
	}

	return InputIt1(haystackEnd);
}

} // pfs

#endif /* __PFS_ALGO_STRING_FIND_HPP__ */
