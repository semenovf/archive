/*
 * iterator.hpp
 *
 *  Created on: Dec 6, 2015
 *      Author: wladt
 */

#ifndef __PFS_UTF8_ITERATOR_HPP__
#define __PFS_UTF8_ITERATOR_HPP__

#include <pfs/utf/iterator.hpp>
#include <pfs/utf8/traits.hpp>
#include <pfs/utf8/decode.hpp>
#include <pfs/utf8/advance.hpp>

namespace pfs { namespace utf8 {

template <typename CodeUnitIterator>
struct iterator
{
	typedef pfs::utf::iterator<CodeUnitIterator, tag> type;
};

}} // pfs::utf8

#endif /* __PFS_UTF8_ITERATOR_HPP__ */
