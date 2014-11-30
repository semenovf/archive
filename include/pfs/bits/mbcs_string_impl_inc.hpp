/**
 * @file mbcs_string_impl_inc.hpp
 * @author wladt
 * @date Nov, 2014
 */

#ifndef __PFS_BITS_MBCS_STRING_IMPL_INC_HPP__
#define __PFS_BITS_MBCS_STRING_IMPL_INC_HPP__

namespace pfs {

template <typename _CodeUnitT, typename Holder>
reference<Holder> mbcs_string_ptr<_CodeUnitT, Holder>::ref () const
{
	return reference<Holder>(*_holder, *this);
}

template <typename _CodeUnitT, typename Holder>
typename mbcs_string_ptr<_CodeUnitT, Holder>::value_type
mbcs_string_ptr<_CodeUnitT, Holder>::operator * () const
{
	ucchar r;
	size_type nremain = _holder->size() - _off;
	PFS_ASSERT(r.decode<_CodeUnitT>(_holder->constData() + _off, nremain) > 0);
	return r;
}

template <typename _CodeUnitT, typename Holder>
typename mbcs_string_ptr<_CodeUnitT, Holder>::value_type
mbcs_string_ptr<_CodeUnitT, Holder>::operator [] (difference_type n) const
{
//	utf8string_ptr p(*this);
//	p += n;
//	return reference<Holder>(p->_holder, *this);
	mbcs_string_ptr<_CodeUnitT, Holder> p(*this);
	p += n;
	return *p;
}

template <typename _CodeUnitT, typename Holder>
mbcs_string_ptr<_CodeUnitT, Holder> & mbcs_string_ptr<_CodeUnitT, Holder>::operator
+= (typename mbcs_string_ptr<_CodeUnitT, Holder>::difference_type n)
{
	size_t invalidBytes = 0;
	const char * start = _holder->constData() + _off;
	const char * begin = _holder->constData();
	const char * end   = _holder->constData() + _holder->size();
	const char * p = mbcs_string_impl<_CodeUnitT>::increment(start, n, & invalidBytes);
	//PFS_ASSERT(p >= begin && p <= end && invalidBytes == 0);
	PFS_ASSERT(p >= begin);
	PFS_ASSERT(p <= end);
	PFS_ASSERT(invalidBytes == 0);
	_off = p - begin;
	return *this;
}

template <typename _CodeUnitT, typename Holder>
mbcs_string_ptr<_CodeUnitT, Holder> & mbcs_string_ptr<_CodeUnitT, Holder>::operator
-= (typename mbcs_string_ptr<_CodeUnitT, Holder>::difference_type n)
{
	size_t invalidBytes = 0;
	const char * start = _holder->constData() + _off;
	const char * begin = _holder->constData();
	const char * end   = _holder->constData() + _holder->size();
	const char * p = mbcs_string_impl<_CodeUnitT>::decrement(start, n, & invalidBytes);
	//PFS_ASSERT(p >= begin && p <= end && invalidBytes == 0);
	PFS_ASSERT(p >= begin);
	PFS_ASSERT(p <= end);
	PFS_ASSERT(invalidBytes == 0);
	_off = p - begin;
	return *this;
}

} // pfs

#endif /* __PFS_BITS_MBCS_STRING_IMPL_INC_HPP__ */
