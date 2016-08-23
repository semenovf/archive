/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   noncopyable.hpp
 * Author: wladt
 *
 * Created on August 23, 2016, 8:55 PM
 */

#ifndef __PFS_NONCOPYABLE_HPP__
#define __PFS_NONCOPYABLE_HPP__

namespace pfs {

class noncopyable
{
#if __cplusplus >= 201103L

    noncopyable (const noncopyable &) = delete;
    noncopyable & operator = (const noncopyable &) = delete;

#else

private:
    noncopyable (const noncopyable &);
    noncopyable & operator = (const noncopyable &);

#endif
};

}

#endif /* __PFS_NONCOPYABLE_HPP__ */

