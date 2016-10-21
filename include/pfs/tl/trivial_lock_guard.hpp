/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   lock_guard.hpp
 * Author: wladt
 *
 * Created on October 21, 2016, 6:50 PM
 */

#ifndef __PFS_TL_TRIVIAL_LOCK_GUARD_HPP__
#define __PFS_TL_TRIVIAL_LOCK_GUARD_HPP__

namespace pfs {
namespace tl {

template <typename Lockable>
class trivial_lock_guard
{
public:
    typedef Lockable mutex_type;

private:
    mutex_type & _mtx;

private:
    trivial_lock_guard (trivial_lock_guard const &);
    trivial_lock_guard & operator = (trivial_lock_guard const &);

public:
    explicit trivial_lock_guard (mutex_type & mtx)
    	: _mtx(mtx)
    {
    	_mtx.lock();
    }

    ~trivial_lock_guard ()
    {
    	_mtx.unlock();
    }
};

}}

#endif /* __PFS_TL_TRIVIAL_LOCK_GUARD_HPP__ */

