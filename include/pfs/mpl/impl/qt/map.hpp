/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   map.hpp
 * Author: wladt
 *
 * Created on January 16, 2017, 3:12 PM
 */

#ifndef __PFS_MPL_IMPL_QT_MAP_HPP__
#define __PFS_MPL_IMPL_QT_MAP_HPP__

#include <QMap>
#include <pfs/mpl/map.hpp>

namespace pfs {
namespace mpl {

template <typename Key, typename T>
struct map_traits<Key, T, QMap>
{
    typedef QMap<Key, T>                               impl_type;
    typedef impl_type const &                          const_impl_reference;
    
    typedef typename impl_type::size_type              size_type;
    typedef typename impl_type::key_type               key_type;
    typedef typename impl_type::mapped_type            mapped_type;
//    typedef typename impl_type::value_type             value_type;
//    typedef typename impl_type::pointer                pointer;
//    typedef typename impl_type::const_pointer          const_pointer;
//    typedef typename impl_type::reference              reference;
//    typedef typename impl_type::const_reference        const_reference;
    typedef typename impl_type::iterator               iterator;
    typedef typename impl_type::const_iterator         const_iterator;
    typedef typename std::reverse_iterator<iterator>   reverse_iterator;
    typedef typename std::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef typename impl_type::difference_type        difference_type;
    typedef impl_type                                  data_type;
};


template <typename Key, typename T>
class basic_map<Key, T, QMap>
    : public details::basic_map<Key, T, QMap>
{
protected:
    typedef details::basic_map<Key, T, QMap> base_type;
    typedef typename base_type::reverse_iterator       reverse_iterator;
    typedef typename base_type::const_reverse_iterator const_reverse_iterator;
    
protected:
    using base_type::xrbegin;
    using base_type::xrend;
    
    virtual reverse_iterator xrbegin ()
    {
        return reverse_iterator(this->_d.end());
    }
    
    virtual const_reverse_iterator xrbegin () const
    {
        return const_reverse_iterator(this->_d.end());
    }
    
    virtual reverse_iterator xrend ()
    {
        return reverse_iterator(this->_d.begin());
    }
    
    virtual const_reverse_iterator xrend () const
    {
        return const_reverse_iterator(this->_d.begin());
    }
};

}}

#endif /* __PFS_MPL_IMPL_QT_MAP_HPP__ */
