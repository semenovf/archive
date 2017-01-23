/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   multimap.hpp
 * Author: wladt
 *
 * Created on January 20, 2017, 3:55 PM
 */

#ifndef __PFS_MPL_QT_MULTIMAP_HPP__
#define __PFS_MPL_QT_MULTIMAP_HPP__

#include <QMultiMap>
#include <pfs/cxxlang.hpp>
#include <pfs/mpl/multimap.hpp>

namespace pfs {
namespace mpl {

namespace qt {

template <typename Key, typename T>
struct multimap_iterator : public QMultiMap<Key, T>::iterator
{
    typedef typename QMultiMap<Key, T>::iterator base_type;

    multimap_iterator (base_type lhs) pfs_noexcept
        : base_type(lhs)
    {}
};

template <typename Key, typename T>
struct multimap_const_iterator : public QMultiMap<Key, T>::const_iterator
{
    typedef typename QMultiMap<Key, T>::const_iterator base_type;

    multimap_const_iterator (base_type lhs) pfs_noexcept
        : base_type(lhs)
    {}
};

} // qt

template <typename Key, typename T>
struct multimap_traits<Key, T, QMultiMap>
{
    typedef QMultiMap<Key, T>                            native_type;
    typedef typename native_type::size_type              size_type;
    typedef typename native_type::key_type               key_type;
    typedef typename native_type::mapped_type            mapped_type;
    typedef typename qt::map_iterator<Key, T>            iterator;
    typedef typename qt::map_const_iterator<Key, T>      const_iterator;
    typedef typename std::reverse_iterator<iterator>     reverse_iterator;
    typedef typename std::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef native_type                                  data_type;
};

template <typename Key, typename T>
class basic_multimap<Key, T, QMultiMap>
    : public details::basic_multimap<Key, T, QMultiMap>
{
    typedef details::basic_multimap<Key, T, QMultiMap> base_type;
    typedef basic_multimap<Key, T, QMultiMap>          self_type;
    
protected:
    typedef typename base_type::size_type              size_type;
    typedef typename base_type::key_type               key_type;
    typedef typename base_type::mapped_type            mapped_type;
    typedef typename base_type::native_type            native_type;
    typedef typename base_type::iterator               iterator;
    typedef typename base_type::const_iterator         const_iterator;
    typedef typename base_type::reverse_iterator       reverse_iterator;
    typedef typename base_type::const_reverse_iterator const_reverse_iterator;
    
protected:
    virtual iterator xbegin ()
    {
        return iterator(this->_d.begin());
    }
    
    virtual const_iterator xbegin () const
    {
        return const_iterator(this->_d.begin());
    }
    
    virtual iterator xend ()
    {
        return iterator(this->_d.end());
    }
    
    virtual const_iterator xend () const
    {
        return const_iterator(this->_d.end());
    }
    
    virtual reverse_iterator xrbegin ()
    {
        return reverse_iterator(this->xend());
    }
    
    virtual const_reverse_iterator xrbegin () const
    {
        return const_reverse_iterator(this->xend());
    }
    
    virtual reverse_iterator xrend ()
    {
        return reverse_iterator(this->xbegin());
    }
    
    virtual const_reverse_iterator xrend () const
    {
        return const_reverse_iterator(this->xbegin());
    }
    
    virtual size_type xsize () const
    {
        return this->_d.size();
    }
    
    virtual void xclear ()
    {
        this->_d.clear();
    }
    
    virtual iterator xerase (iterator position)
    {
        return iterator(this->_d.erase(position));
    }
    
    virtual void xswap (base_type & rhs)
    {
        this->_d.swap(rhs._d);
    }

    virtual size_type xcount (key_type const & key) const
    {
        return this->_d.count(key);
    }
   
    virtual iterator xfind (key_type const & key)
    {
        return iterator(this->_d.find(key));
    }
		
    virtual const_iterator xfind (key_type const & key) const
    {
        return const_iterator(this->_d.find(key));
    }

    virtual iterator xlower_bound (key_type const & key)
    {
        return this->_d.lowerBound(key);
    }
    
    virtual const_iterator xlower_bound (key_type const & key) const
    {
        return this->_d.lowerBound(key);
    }
    
    virtual iterator xupper_bound (key_type const & key)
    {
        return this->_d.upperBound(key);
    }
    
    virtual const_iterator xupper_bound (key_type const & key) const
    {
        return this->_d.upperBound(key);
    }
    
    virtual iterator xinsert (key_type const & key, mapped_type const & value)
    {
        return this->_d.insert();
    }
    
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

#endif /* __PFS_MPL_QT_MULTIMAP_HPP__ */

