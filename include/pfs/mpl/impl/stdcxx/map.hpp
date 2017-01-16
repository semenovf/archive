/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   map.hpp
 * Author: wladt
 *
 * Created on January 16, 2017, 3:03 PM
 */

#ifndef __PFS_MPL_IMPL_STDCXX_MAP_HPP__
#define __PFS_MPL_IMPL_STDCXX_MAP_HPP__

#include <pfs/mpl/map.hpp>
#include <map>

namespace pfs {
namespace mpl {

namespace stdcxx {

template <typename Key, typename T>
struct map
{
    typedef std::map<Key, T> type;
};

} // stdcxx

template <typename Key, typename T>
struct map_traits<Key, T, stdcxx::map>
{
    typedef typename stdcxx::map<Key, T>::type         impl_type;
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
    typedef typename impl_type::const_reverse_iterator const_reverse_iterator;
    typedef typename impl_type::reverse_iterator       reverse_iterator;
    typedef typename impl_type::difference_type        difference_type;
    typedef impl_type                                  data_type;
};

template <typename Key, typename T>
class basic_map<Key, T, stdcxx::map>
    : public details::basic_map<Key, T, stdcxx::map>
{
    typedef details::basic_map<Key, T, stdcxx::map> base_type;
    
protected:
    typedef typename base_type::iterator       iterator;
    typedef typename base_type::const_iterator const_iterator;
    
    virtual iterator xerase (const_iterator position)
    {
#if __cplusplus >= 201103        
        return _d.erase(position);
#else
        if (position != this->_d.end()) {
            iterator r(position);
            pfs::advance(r, 1);
            this->_d.erase(iterator(position));
            return r;
        }
        return this->_d.end();
#endif        
    }
    
    iterator xerase (const_iterator first, const_iterator last)
    {
#if __cplusplus >= 201103        
        return _d.erase(first, last); // from C++11
#else
#endif        
    }
        
};

}}

#endif /* __PFS_MPL_IMPL_STDCXX_MAP_HPP__ */
