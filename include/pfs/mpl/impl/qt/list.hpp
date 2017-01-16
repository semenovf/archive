/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   list.hpp
 * Author: wladt
 *
 * Created on January 16, 2017, 2:34 PM
 */

#ifndef __PFS_MPL_IMPL_QT_LIST_HPP__
#define __PFS_MPL_IMPL_QT_LIST_HPP__

#include <QList>
#include <pfs/mpl/list.hpp>

namespace pfs {
namespace mpl {

template <typename T>
struct list_traits<T, QList>
{
    typedef QList<T>                                  impl_type;
    typedef impl_type const &                         const_impl_reference;
    typedef typename impl_type::difference_type       difference_type;
    typedef typename impl_type::size_type             size_type;
    typedef typename impl_type::value_type            value_type;
    typedef typename impl_type::pointer               pointer;
    typedef typename impl_type::const_pointer         const_pointer;
    typedef typename impl_type::reference             reference;
    typedef typename impl_type::const_reference       const_reference;
    typedef typename impl_type::iterator              iterator;
    typedef typename impl_type::const_iterator        const_iterator;
    typedef typename std::reverse_iterator<iterator>  reverse_iterator;
    typedef typename std::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef impl_type data_type;
};


template <typename T>
class basic_list<T, QList>
    : public details::basic_list<T, QList>
{
};

}}

#endif /* __PFS_MPL_IMPL_QT_LIST_HPP__ */

