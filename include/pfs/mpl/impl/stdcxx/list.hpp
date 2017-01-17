/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   list.hpp
 * Author: wladt
 *
 * Created on January 16, 2017, 12:58 PM
 */

#ifndef __PFS_MPL_IMPL_STDCXX_LIST_HPP__
#define __PFS_MPL_IMPL_STDCXX_LIST_HPP__

#include <pfs/mpl/list.hpp>
#include <list>

namespace pfs {
namespace mpl {

namespace stdcxx {

template <typename T>
struct list {
    typedef std::list<T> type;
};

} // stdcxx

template <typename T>
struct list_traits<T, stdcxx::list>
{
    typedef typename stdcxx::list<T>::type native_type;
    typedef native_type data_type;
};

template <typename T>
class basic_list<T, stdcxx::list>
    : public details::basic_list<T, stdcxx::list>
{
};

}}

#endif /* __PFS_MPL_IMPL_STDCXX_LIST_HPP__ */

