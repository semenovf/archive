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
struct list
{
    typedef std::list<T> type;
};

} // stdcxx

template <typename T>
struct list_traits<T, stdcxx::list>
{
    typedef typename stdcxx::list<T>::type             native_type;
    typedef typename native_type::size_type            size_type;
//    typedef typename native_type::value_type            value_type;
    typedef typename native_type::iterator               iterator;
    typedef typename native_type::const_iterator         const_iterator;
    typedef typename native_type::reverse_iterator       reverse_iterator;
    typedef typename native_type::const_reverse_iterator const_reverse_iterator;
//    typedef typename native_type::difference_type          difference_type;
    typedef native_type                                    data_type;
};

template <typename T>
class basic_list<T, stdcxx::list>
    : public details::basic_list<T, stdcxx::list>
{
    typedef details::basic_list<T, stdcxx::list> base_type;
    typedef basic_list self_type;
    
protected:
    typedef typename base_type::native_type            native_type;
    typedef typename base_type::size_type              size_type;
    typedef typename base_type::iterator               iterator;
    typedef typename base_type::const_iterator         const_iterator;
    typedef typename base_type::reverse_iterator       reverse_iterator;
    typedef typename base_type::const_reverse_iterator const_reverse_iterator;

    virtual iterator xbegin ()
    {
        return this->_d.begin();
    }
    
    virtual const_iterator xbegin () const
    {
        return this->_d.begin();
    }
    
    virtual iterator xend ()
    {
        return this->_d.end();
    }
    
    virtual const_iterator xend () const
    {
        return this->_d.end();
    }
    
    virtual reverse_iterator xrbegin ()
    {
        return this->_d.rbegin();
    }
    
    virtual const_reverse_iterator xrbegin () const
    {
        return this->_d.rbegin();
    }
    
    virtual reverse_iterator xrend ()
    {
        return this->_d.rend();
    }
    
    virtual const_reverse_iterator xrend () const
    {
        return this->_d.rend();
    }
    
//    virtual size_type xsize () const
//    {
//        return this->_d.size();
//    }
//    
//    virtual mapped_type & xat (Key const & key)
//    {
//        // Note: pfs::out_of_range is synonym for std::out_of_range
//        return this->_d.at(key);
//    }
//
//    virtual mapped_type const & xat (Key const & key) const
//    {
//        // Note: pfs::out_of_range is synonym for std::out_of_range
//        return this->_d.at(key);
//    }
//    
//    virtual mapped_type & xsubscript (Key const & key)
//    {
//        return this->_d[key];
//    }
//    
//    virtual void xclear ()
//    {
//        this->_d.clear();
//    }
//
//    virtual iterator xerase (iterator position)
//    {
//#if __cplusplus >= 201103
//        return _d.erase(position);
//#else
//        this->_d.erase(position++);
//        return position;
//#endif        
//    }
//
//    virtual iterator xerase (iterator first, iterator last)
//    {
//#if __cplusplus >= 201103
//        return _d.erase(first, last);
//#else
//        this->_d.erase(first, last);
//        return last;
//#endif        
//    }
    
    virtual void xswap (base_type & rhs)
    {
        this->_d.swap(rhs._d);
    }

//    virtual size_type xcount (Key const & key) const
//    {
//        return this->_d.count(key);
//    }
//    
//    virtual iterator xfind (Key const & key)
//    {
//        return this->_d.find(key);
//    }
//		
//    virtual const_iterator xfind (Key const & key) const
//    {
//        return this->_d.find(key);
//    }
//    
//    virtual pfs::pair<iterator, bool> xinsert (Key const & key, T const & value)
//    {
//        std::pair<iterator,bool> r = this->_d.insert(std::pair<Key, T>(key, value));
//        return pfs::pair<iterator,bool>(r.first, r.second);
//    }
//    
public:
    virtual native_type & native ()
    {
        return this->_d;
    }

    virtual native_type const & native () const
    {
        return this->_d;
    }
};

}}

#endif /* __PFS_MPL_IMPL_STDCXX_LIST_HPP__ */

