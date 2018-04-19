/*
 * join.hpp
 *
 *  Created on: Oct 25, 2015
 *      Author: wladt
 */

#ifndef __PFS_MPL_ALGO_JOIN_HPP__
#define __PFS_MPL_ALGO_JOIN_HPP__

namespace pfs {
namespace mpl {
// Sequence must provide push_back(const String &) methods
// String::empty()
//
template <typename InputIterator, typename Sequence>
Sequence * join (
		  InputIterator begin
		, InputIterator end
		, Sequence const & separator
		, Sequence * result = 0)
{
	if (begin == end)
		return result;

    if (!result)
        result = new Sequence;
    
	result->append(*begin++);

	while (begin != end) {
		result->append(separator);
		result->append(*begin++);
	}

	return result;
}

}} // pfs::mpl

#endif /* __PFS_MPL_ALGO_JOIN_HPP__ */
