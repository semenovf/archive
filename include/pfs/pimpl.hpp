/**
 * @file pimpl.hpp
 * @author wladt
 * @date Nov 29, 2013
 */

#ifndef __PFS_PIMPL_HPP__
#define __PFS_PIMPL_HPP__

#define PFS_PIMPL_INLINE(Class,Impl)                           \
private:                                                       \
	pfs::shared_ptr<Impl> _pimpl;                              \
                                                               \
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
    void swap (Class & other)                                  \
    {                                                          \
    	pfs::swap(_pimpl, other._pimpl);                       \
    }

#endif /* __PFS_PIMPL_HPP__ */
