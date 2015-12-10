/*
 * traits.hpp
 *
 *  Created on: Dec 6, 2015
 *      Author: wladt
 */

#ifndef __PFS_UTF8_TRAITS_HPP__
#define __PFS_UTF8_TRAITS_HPP__

#include <pfs/utf8/decode.hpp>
#include <pfs/utf8/encode.hpp>
#include <pfs/utf8/advance.hpp>
#include <pfs/utf/traits.hpp>

namespace pfs { namespace utf8 {

struct tag {};

}}

namespace pfs { namespace utf {

template <>
struct tag_trait<char>
{
	typedef pfs::utf8::tag type;
};

template <>
struct tag_trait<uint8_t>
{
	typedef pfs::utf8::tag type;
};

template <>
struct tag_trait<int8_t>
{
	typedef pfs::utf8::tag type;
};

template <typename CodeUnitIterator>
struct traits<CodeUnitIterator, pfs::utf8::tag>
{
	typedef pfs::utf8::tag   utf_tag;
	typedef uint32_t         value_type;
    typedef CodeUnitIterator pointer;
    typedef uint32_t &       reference;
    typedef ptrdiff_t        difference_type;

    static uint32_t decode (pointer & p)
    {
    	return pfs::utf8::decode(p);
    }

    static pointer encode (value_type uc, pointer begin)
    {
    	return pfs::utf8::encode(uc, begin);
    }

    static void advance_forward (pointer & p, difference_type n)
    {
    	pfs::utf8::advance_forward(p, n);
    }

    static void advance_backward (pointer & p, difference_type n)
    {
    	pfs::utf8::advance_backward(p, n);
    }
};

}} // pfs::utf

#endif /* __PFS_UTF8_TRAITS_HPP__ */
