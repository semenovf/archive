/*
 * split.hpp
 *
 *  Created on: Oct 21, 2015
 *      Author: wladt
 */

#ifndef __PFS_MPL_ALGO_SPLIT_HPP__
#define __PFS_MPL_ALGO_SPLIT_HPP__

#include <pfs/mpl/algo/find.hpp>

namespace pfs {
namespace mpl {

enum {
      dont_keep_empty = false
    , keep_empty = true
};

// Sequence must provide methods:
// 		String::push_back(const value_type &) - appends symbol to sequence
// 		String::empty() - checks for empty
//
/**
 * @brief Splits into tokens and return token sequence.
 * 
 * @param flag Must be one of @c pfs::mpl::dont_keep_empty or @c pfs::mpl::keep_empty.
 * @return If @a result specified and it is not zero, so function will return @a result
 *         filled with tokens, otherwise result will be dynamically allocated before.
 */
template <typename InputIt1, typename InputIt2, typename Sequence>
Sequence * split (
		  InputIt1 begin
		, InputIt1 end
		, InputIt2 separator_begin
		, InputIt2 separator_end
		, bool flag
		, Sequence * result = 0)
{
	typedef typename Sequence::value_type value_type;
	typedef typename Sequence::difference_type difference_type;

	if (begin == end)
		return result;

	difference_type n = pfs::distance(separator_begin, separator_end);

    if (! result)
        result = new Sequence;
    
	// "/"

	while (begin != end) {
		InputIt1 it = pfs::mpl::find(begin
				, end
				, separator_begin
				, separator_end);

		if (it == end)
			break;

		value_type v(begin, it);

		if (!(v.empty() && !flag)) {
			result->push_back(v);
		}

		// Skip separator
		//
		pfs::advance(it, n);
		begin = it;
	}

	if (begin != end)
		result->push_back(value_type(begin, end));
	else {
		if (flag)
			result->push_back(value_type());
	}

	return result;
}

}} // pfs::mpl

#endif /* __PFS_MPL_ALGO_SPLIT_HPP__ */
