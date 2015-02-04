/**
 * @file map_impl_inc.hpp
 * @author wladt
 * @date Feb, 2014
 */

#ifndef __PFS_BITS_MAP_IMPL_INC_HPP__
#define __PFS_BITS_MAP_IMPL_INC_HPP__

namespace pfs {

template <typename _CodeUnitT, typename Holder>
template <typename Key, typename T, typename Compare, typename Allocator, typename Holder>
inline reference<Holder> map_ptr<Key, T, Compare, Allocator>::ref () const
{
	return reference<Holder>(*_holder, *this);
}

//template <typename _CodeUnitT, typename Holder>
//typename mbcs_string_ptr<_CodeUnitT, Holder>::value_type
//mbcs_string_ptr<_CodeUnitT, Holder>::operator * () const
//{
//	ucchar r;
//	size_type nremain = _holder->size() - _off;
//	PFS_ASSERT(r.decode<_CodeUnitT>(_holder->constData() + _off, nremain) > 0);
//	return r;
//}

} // pfs

#endif /* __PFS_BITS_MAP_IMPL_INC_HPP__ */
