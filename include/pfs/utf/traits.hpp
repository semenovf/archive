/*
 * utf_traits.hpp
 *
 *  Created on: Dec 6, 2015
 *      Author: wladt
 */

#ifndef __PFS_UTF_TRAITS_HPP__
#define __PFS_UTF_TRAITS_HPP__

#include <pfs.hpp>
#include <pfs/unicode/unicode.hpp>

namespace pfs { namespace utf {

template <typename CodeUnitIterator, typename UtfTag>
struct traits
{
	typedef UtfTag               utf_tag;
	typedef unicode::char_type   value_type;
    typedef CodeUnitIterator     pointer;
	typedef unicode::char_type & reference;
    typedef ptrdiff_t            difference_type;

    static uint32_t decode (pointer & p);
    static pointer encode (value_type uc, pointer begin);

    static void advance_forward  (pointer & p, difference_type n);
    static void advance_backward (pointer & p, difference_type n);
};

template <typename CodeUnit>
struct tag_trait;

}} // pfs

#endif /* __PFS_UTF_TRAITS_HPP__ */
