/**
 * @file mbcs_string_impl_inc.hpp
 * @author wladt
 * @date Nov, 2014
 */

#ifndef __PFS_BITS_MBCS_STRING_IMPL_INC_HPP__
#define __PFS_BITS_MBCS_STRING_IMPL_INC_HPP__

namespace pfs {

template <typename CodeUnitT, typename Holder>
inline reference<Holder> mbcs_string_ptr<CodeUnitT, Holder>::ref () const
{
	return reference<Holder>(*_holder, *this);
}

template <typename CodeUnitT, typename Holder>
typename mbcs_string_ptr<CodeUnitT, Holder>::value_type
mbcs_string_ptr<CodeUnitT, Holder>::operator * () const
{
	ucchar r;
	size_type sz = _holder->size() - _off;
	PFS_ASSERT(r.decode<CodeUnitT>(_holder->constData() + _off, sz) > 0);
	return r;
}

template <typename CodeUnitT, typename Holder>
typename mbcs_string_ptr<CodeUnitT, Holder>::value_type
mbcs_string_ptr<CodeUnitT, Holder>::operator [] (difference_type n) const
{
//	utf8string_ptr p(*this);
//	p += n;
//	return reference<Holder>(p->_holder, *this);
	mbcs_string_ptr<CodeUnitT, Holder> p(*this);
	p += n;
	return *p;
}

template <typename CodeUnitT, typename Holder>
mbcs_string_ptr<CodeUnitT, Holder> & mbcs_string_ptr<CodeUnitT, Holder>::operator
+= (typename mbcs_string_ptr<CodeUnitT, Holder>::difference_type n)
{
	const CodeUnitT * start = _holder->constData() + _off;
	const CodeUnitT * begin = _holder->constData();
	const CodeUnitT * end   = _holder->constData() + _holder->size();
	const CodeUnitT * p = mbcs_string_impl<CodeUnitT>::increment(start, n);
	PFS_ASSERT(p >= begin);
	PFS_ASSERT(p <= end);
	_off = p - begin;
	return *this;
}

template <typename CodeUnitT, typename Holder>
mbcs_string_ptr<CodeUnitT, Holder> & mbcs_string_ptr<CodeUnitT, Holder>::operator
-= (typename mbcs_string_ptr<CodeUnitT, Holder>::difference_type n)
{
	const CodeUnitT * start = _holder->constData() + _off;
	const CodeUnitT * begin = _holder->constData();
	const CodeUnitT * end   = _holder->constData() + _holder->size();
	const CodeUnitT * p = mbcs_string_impl<CodeUnitT>::decrement(start, n);
	PFS_ASSERT(p >= begin);
	PFS_ASSERT(p <= end);
	_off = p - begin;
	return *this;
}

} // pfs

#endif /* __PFS_BITS_MBCS_STRING_IMPL_INC_HPP__ */
