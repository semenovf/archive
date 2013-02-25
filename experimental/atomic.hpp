/*
 * atomic.hpp
 *
 *  Created on: Feb 24, 2013
 *      Author: wladt
 */

#ifndef __CWT_ATOMIC_HPP__
#define __CWT_ATOMIC_HPP__

#include <cwt/cwt.h>
#include <cwt/mt.h>

CWT_NS_BEGIN

/* FIXME Need GOOD implementation */
/*
template <typename _T = int>
class Atomic {
public:
	Atomic(const _T &val) : m_val(val) { mt_init(m_mutex); }
	virtual ~Atomic() { mt_destroy(m_mutex); }

	operator _T () { return m_val; }
	Atomic& operator++() { mt_lock(m_mutex); ++m_val; mt_unlock(m_mutex); return *this; }
	Atomic& operator--() { mt_lock(m_mutex); --m_val; mt_unlock(m_mutex); return *this; }
private:
	mt_def(m_mutex);
	_T m_val;
};
*/

//__sync_bool_compare_and_swap

template <typename int_type>
class AtomicInt {
public:
	AtomicInt(int_type val) : i(val) {}
	virtual ~AtomicInt() { }
	AtomicInt& operator++() { ++i; return *this; }
	AtomicInt& operator--() { --i; return *this; }
	bool ref() { __sync_add_and_fetch(&i); return ((int)*this) > 0 ? true : false; }
	bool deref() { --(*this); return ((int)*this) > 0 ? true : false; }
private:
	int_type i;
};


CWT_NS_END

#endif /* __CWT_ATOMIC_HPP__ */
