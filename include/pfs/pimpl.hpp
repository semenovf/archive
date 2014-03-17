/**
 * @file pimpl.hpp
 * @author wladt
 * @date Nov 29, 2013
 */

#ifndef __PFS_PIMPL_HPP__
#define __PFS_PIMPL_HPP__

#include <pfs/shared_ptr.hpp>
#include <pfs/atomic.hpp>

#ifdef __COMMENT__

template <typename ImplClass>
class pimpl
{
protected:
//    template <typename T1> friend  class shared_ptr;

    ImplClass * _value;
    atomic_int  _ref;

private:
    pimpl<ImplClass> & operator = (const pimpl<ImplClass> & other);
/*
        {
            shared_ptr copy(other);
            swap(copy);
            return *this;
        }
*/

public:
    pimpl () : _value(0), _ref(0)
    {}

    explicit pimpl (ImplClass * ptr) : _value(ptr), _ref(0)
    {
    	if (ptr) {
    		_ref.ref();
    	}
    }

    pimpl (const pimpl<ImplClass> & other) : _value(other._value), _ref(other._ref)
    {
    	if (other._value)
    		_ref.ref();
    }

    ~pimpl ()
    {
        if (!_value)
        	return;
        if (!_ref.deref()) {
            delete _value;
            _value = nullptr;
        }
    }

	bool isNull () const
	{
		return _value == nullptr;
	}

    ImplClass & operator * ()
    {
        if (!_value) {
        	_value = new ImplClass;
        	_ref.ref();
        }
        return *_value;
    }

    ImplClass * operator -> ()
    {
        if (!_value) {
        	_value = new ImplClass;
        	_ref.ref();
        }
        return _value;
    }

    ImplClass * get () const
    {
    	return _value;
    }

    void swap (pimpl & other)
    {
        ::pfs::swap<ref_count *>(_d, other._d);
        ::pfs::swap(this->_value, other._value);
    }

public:
/*	template <typename ImplClass1>
	void detach ()
	{
		if (_value != nullptr) {
			if (_ref.load() != 0) {
				pimpl<ImplClass> d(new Impl1(*_value));
				swap<ImplClass1>(d);
			}
		}
	}

	template <typename Impl1>
	Impl1 & cast ()
	{
		return *pfs::static_pointer_cast<Impl1,Impl>(_pimpl);
	}

	template <typename Impl1>
	const Impl1 & cast () const
	{
		return *pfs::static_pointer_cast<Impl1,Impl>(_pimpl);
	}

	template <typename Impl1>
	Impl1 & assign ()
	{
		if (_pimpl.get() == nullptr)
			_pimpl = pfs::dynamic_pointer_cast<Impl, Impl1>(pfs::make_shared<Impl1>());
		return static_cast<Impl1 &>(*_pimpl);
	}*/
};

} // pfs

#endif

#define PFS_PIMPL_INLINE(Class,ImplClassScope,Impl)            \
ImplClassScope:                                                \
	pfs::shared_ptr<Impl> _pimpl;                              \
protected:                                                     \
	void detach()                                              \
	{                                                          \
		if (!_pimpl.unique()) {                                \
			pfs::shared_ptr<Impl> d(new Impl(*_pimpl));        \
			_pimpl.swap(d);                                    \
		}                                                      \
	}                                                          \
                                                               \
	Class (const Impl & other) : _pimpl(new Impl(other)) {}    \
                                                               \
public:                                                        \
	Class (const Class & other) : _pimpl(other._pimpl) { }     \
	Class & operator = (const Class & other)                   \
	{                                                          \
		_pimpl = other._pimpl;                                 \
		return *this;                                          \
	}                                                          \
                                                               \
	bool isNull () const { return _pimpl.get() == nullptr; }   \
	                                                           \
    void swap (Class & other)                                  \
    {                                                          \
    	_pimpl.swap(other._pimpl);                             \
    }


#define PFS_PIMPL_DECL(Class,ImplClassScope,Impl,PimplScope)   \
ImplClassScope:                                                \
	class Impl;                                                \
PimplScope:	                                                   \
	pfs::shared_ptr<Impl> _pimpl;                              \
protected:                                                     \
	void detach();                                             \
	Class (const Impl & other);                                \
                                                               \
public:                                                        \
	Class (const Class & other) : _pimpl(other._pimpl) { }     \
	Class & operator = (const Class & other)                   \
	{                                                          \
		_pimpl = other._pimpl;                                 \
		return *this;                                          \
	}                                                          \
                                                               \
	bool isNull () const { return _pimpl.get() == nullptr; }   \
	                                                           \
    void swap (Class & other)                                  \
    {                                                          \
    	pfs::swap(_pimpl, other._pimpl);                       \
    }

// w/o copy constructor
//
#define PFS_PIMPL_DECL_NOCC(Class,Impl)                        \
	class Impl;                                                \
	pfs::shared_ptr<Impl> _pimpl;                              \
                                                               \
	void detach();                                             \
	Class (const Impl & other);                                \
                                                               \
public:                                                        \
	Class & operator = (const Class & other)                   \
	{                                                          \
		_pimpl = other._pimpl;                                 \
		return *this;                                          \
	}                                                          \
                                                               \
	bool isNull () const { return _pimpl.get() == nullptr; }   \
	                                                           \
    void swap (Class & other)                                  \
    {                                                          \
    	pfs::swap(_pimpl, other._pimpl);                       \
    }


#define PFS_PIMPL_DEF(Class,Impl)                              \
void Class::detach()                                           \
{                                                              \
	if (!_pimpl.unique()) {                                    \
		pfs::shared_ptr<Class::Impl> d(new Class::Impl(*_pimpl)); \
		_pimpl.swap(d);                                        \
	}                                                          \
}                                                              \
                                                               \
Class::Class (const Class::Impl & other)                       \
	: _pimpl(new Class::Impl(other)) {}

#endif /* __PFS_PIMPL_HPP__ */
