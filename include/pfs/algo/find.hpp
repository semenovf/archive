/*
 * find.hpp
 *
 *  Created on: Oct 21, 2015
 *      Author: wladt
 */

#ifndef __PFS_ALGO_FIND_HPP__
#define __PFS_ALGO_FIND_HPP__

#include <iterator>

namespace pfs {

template <typename InputIt1, typename InputIt2>
InputIt1 find (
		  InputIt1 haystack_begin
		, InputIt1 haystack_end
		, InputIt2 needle_begin
		, InputIt2 needle_end)
{
	if (haystack_end < haystack_begin)
		return InputIt1(haystack_end);

	if (needle_end <= needle_begin)
		return InputIt1(haystack_end);

	while (haystack_begin != haystack_end) {
		if (*haystack_begin == *needle_begin) {
			InputIt1 it(haystack_begin);
			InputIt2 it1(needle_begin);

			++it;
			++it1;

			while (it != haystack_end && it1 != needle_end) {
				if (*it != *it1)
					break;
				++it;
				++it1;
			}

			if (it1 == needle_end)
				return InputIt1(haystack_begin);
		}

		++haystack_begin;
	}

	return InputIt1(haystack_end);
}

template <typename InputIt1, typename InputIt2>
bool starts_with (
		  InputIt1 haystack_begin
		, InputIt1 haystack_end
		, InputIt2 needle_begin
		, InputIt2 needle_end)
{
	typename std::iterator_traits<InputIt1>::difference_type d1 = std::distance(haystack_begin, haystack_end);
	typename std::iterator_traits<InputIt2>::difference_type d2 = std::distance(needle_begin, needle_end);

	return d2 <= d1 ? find(haystack_begin
			, haystack_begin + d2
			, needle_begin
			, needle_end) == haystack_begin
		: false;
}

} // pfs

#endif /* __PFS_ALGO_STRING_FIND_HPP__ */