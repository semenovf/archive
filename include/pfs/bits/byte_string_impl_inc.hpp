/**
 * @file byte_string_impl_inc.hpp
 * @author wladt
 * @date Dec, 2014
 */

#ifndef __PFS_BITS_BYTE_STRING_IMPL_INC_HPP__
#define __PFS_BITS_BYTE_STRING_IMPL_INC_HPP__

namespace pfs {

template <typename Holder>
inline reference<Holder> byte_string_ptr<Holder>::ref () const
{
	return reference<Holder>(*_holder, *this);
}

template <typename Holder>
inline typename byte_string_ptr<Holder>::value_type
byte_string_ptr<Holder>::operator * () const
{
	typename byte_string_ptr<Holder>::value_type r = *(_holder->constData() + _off);
	return r;
}

} // pfs

#endif /* __PFS_BITS_BYTE_STRING_IMPL_INC_HPP__ */
