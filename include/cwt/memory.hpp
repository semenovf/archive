/**
 * @file   memory.hpp
 * @author wladt
 * @date   Feb 27, 2013 11:00:57 AM
 *
 * @brief
 */

#ifndef __CWT_MEMORY_HPP__
#define __CWT_MEMORY_HPP__

#include <cwt/cwt.h>
#include <memory>
#include <cwt/atomic.hpp>

CWT_NS_BEGIN

// FIXME add support for native C++11 implementation of unique_ptr and shared_ptr.

template <typename T>
class unique_ptr: public std::auto_ptr<T> {
public:
	explicit unique_ptr(T *p = 0) : std::auto_ptr<T>(p) { }
	unique_ptr(unique_ptr &a) throw() : std::auto_ptr<T>(a) { }

    template<typename T1>
    unique_ptr(unique_ptr<T1> &a) : std::auto_ptr<T>(a) { }
};

/*
struct default_deleter {
    void operator()(void *p) const {
        delete p;
    }
};
*/

template<typename _Alloc>
struct _Deleter
{
	void operator()(_Tp* __ptr)
	{
		_M_alloc.destroy(__ptr);
		_M_alloc.deallocate(__ptr, 1);
	}
	_Alloc _M_alloc;
};

/*

struct custom_deleter {

};
*/


template<class T> class shared_ptr
{
private:
//    typedef shared_ptr<T> shared_ptr_type;

public:
    typedef T element_type;

    shared_ptr(): m_value( 0 ), m_ref(0) {}
    shared_ptr(shared_ptr const &r) : px(r.px), pn(r.pn) {}
    ~shared_ptr() { deref(); }

	// assignment
	shared_ptr& operator = (shared_ptr const &r)
	{
		shared_ptr(r).swap(*this);
		return *this;
	}

	void reset() {
		shared_ptr().swap(*this);
	}



    bool unique() const
    {
        return pn.unique();
    }

    long use_count() const
    {
        return pn.use_count();
    }

    void swap(shared_ptr & other)
    {
        std::swap(px, other.px);
        pn.swap(other.pn);
    }

    void * _internal_get_deleter( boost::detail::sp_typeinfo const & ti ) const
    {
        return pn.get_deleter( ti );
    }

    void * _internal_get_untyped_deleter() const
    {
        return pn.get_untyped_deleter();
    }

    bool _internal_equiv( shared_ptr const & r ) const
    {
        return px == r.px && pn == r.pn;
    }

private:
    inline void deref() { deref(m_ref); }
    static void deref(ref_count *ref)
    {
        if (!ref) return;
        if (!ref->strongref.deref()) {
            ref->destroy();
        }
        if (!ref->weakref.deref())
            delete ref;
    }

private:
    element_type *m_value;   // contained pointer
    ref_count    *m_ref;     // reference counter
};

CWT_NS_END

#endif /* __CWT_MEMORY_HPP__ */
