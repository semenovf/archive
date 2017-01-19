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
#include <pfs/cxxlang.hpp>
#include <pfs/mpl/map.hpp>

namespace pfs {
namespace mpl {

namespace qt {

template <typename Key, typename T>
struct map_iterator : public QMap<Key, T>::iterator
{
    typedef typename QMap<Key, T>::iterator base_type;
//    typedef map_value<Key, T> value_type;
//    typedef value_type & reference;
//    typedef value_type * pointer;
//
//protected:
//    value_type value;
//    pointer    pvalue;
//    
//public:    
//    typedef bidirectional_iterator_tag iterator_category;
//    typedef ptrdiff_t                  difference_type;
//
//    map_iterator () pfs_noexcept
//        : base_type ()
//        , value(base_type::key(), base_type::value())
//        , pvalue(& value)
//    {}
//
    map_iterator (base_type lhs) pfs_noexcept
        : base_type(lhs)
    {}
//    
//    map_iterator & base () const
//    {
//        return static_cast<base_type &>(*this);
//    }
//    
//    reference operator * () const pfs_noexcept
//    {
//        return *pvalue; 
//    }
//
//    pointer operator -> () const pfs_noexcept
//    { 
//        return pvalue;
//    }
//
//    map_iterator & operator ++ () pfs_noexcept
//    {
//        base_type::operator ++ ();
//        value.first = this->key();
//        value.second = this->value();
//        return *this;
//    }
//
//    map_iterator operator ++ (int) pfs_noexcept
//    {
//        map_iterator r = *this;
//        base_type::operator ++ (1);
//        value.first = this->key();
//        value.second = this->value();
//        r.value.first = r.key();
//        r.value.second = r.value();
//        return r;
//    }
//
//    map_iterator & operator -- () pfs_noexcept
//    {
//        base_type::operator -- ();
//        value.first = this->key();
//        value.second = this->value();
//        return *this;
//    }
//
//    map_iterator operator -- (int) pfs_noexcept
//    {
//        map_iterator r = *this;
//        base_type::operator -- (1);
//        value.first = this->key();
//        value.second = this->value();
//        r.value.first = r.key();
//        r.value.second = r.value();
//        return r;
//    }
//    
//    bool operator == (map_iterator const & lhs) const pfs_noexcept
//    {
//        return base_type::operator == (lhs);
//    }
//
//    bool operator != (map_iterator const & lhs) const pfs_noexcept
//    {
//        return base_type::operator != (lhs);
//    }
};

template <typename Key, typename T>
struct map_const_iterator : public QMap<Key, T>::const_iterator
{
    typedef typename QMap<Key, T>::const_iterator base_type;
//    typedef pfs::pair<Key, T const &> value_type;
//    typedef value_type const & reference;
//    typedef value_type const * pointer;
//    typedef map_iterator<Key, T> iterator;
//
//protected:
//    value_type value;
//    pointer    pvalue;
//    
//public:    
//    typedef bidirectional_iterator_tag iterator_category;
//    typedef ptrdiff_t                  difference_type;
//
//    map_const_iterator () pfs_noexcept
//        : base_type()
//        , value(base_type::key(), base_type::value())
//        , pvalue(& value)
//    {}

    map_const_iterator (base_type lhs) pfs_noexcept
        : base_type(lhs)
    {}

//    map_const_iterator (iterator const & it) pfs_noexcept
//        : base_type(it)
//        , value(it.value)
//        , pvalue(& value)
//    {}
//
//    map_const_iterator & base () const
//    {
//        return static_cast<base_type &>(*this);
//    }
//
//    reference operator * () const pfs_noexcept
//    {
//        return *pvalue;
//    }
//
//    pointer operator -> () const pfs_noexcept
//    {
//        return pvalue;
//    }
//
//    map_const_iterator & operator ++ () pfs_noexcept
//    {
//        base_type::operator ++ ();
//        value.first = this->key();
//        value.second = this->value();
//        return *this;
//    }
//
//    map_const_iterator  operator ++ (int) pfs_noexcept
//    {
//        map_const_iterator r = *this;
//        base_type::operator ++ (1);
//        value.first = this->key();
//        value.second = this->value();
//        return r;
//    }
//
//    map_const_iterator & operator -- () pfs_noexcept
//    {
//        base_type::operator -- ();
//        value.first = this->key();
//        value.second = this->value();
//        return *this;
//    }
//
//    map_const_iterator operator -- (int) pfs_noexcept
//    {
//        map_const_iterator r = *this;
//        base_type::operator -- (1);
//        value.first = this->key();
//        value.second = this->value();
//        return r;
//    }
//
//    bool operator == (map_const_iterator const & lhs) const pfs_noexcept
//    {
//        return base_type::operator == (lhs);
//    }
//
//    bool operator != (map_const_iterator const & lhs) const pfs_noexcept
//    {
//        return base_type::operator != (lhs);
//    }
};

//template <typename Key, typename T>
//inline bool
//operator == (map_iterator<Key, T> const & lhs
//    , map_const_iterator<Key, T> const & rhs) pfs_noexcept
//{
//    return static_cast<typename QMap<Key, T>::iterator const & >(lhs)
//            == static_cast<typename QMap<Key, T>::const_iterator const & >(rhs); 
//}
//
//template <typename Key, typename T>
//inline bool
//operator != (map_iterator<Key, T> const & lhs
//    , map_const_iterator<Key, T> const & rhs) pfs_noexcept
//{
//    return ! (lhs == rhs); 
//}

} // qt

template <typename Key, typename T>
struct map_traits<Key, T, QMap>
{
    typedef QMap<Key, T>                                 native_type;
    typedef typename native_type::size_type              size_type;
    typedef typename native_type::key_type               key_type;
    typedef typename native_type::mapped_type            mapped_type;
    typedef typename qt::map_iterator<Key, T>            iterator;
    typedef typename qt::map_const_iterator<Key, T>      const_iterator;
    typedef typename std::reverse_iterator<iterator>     reverse_iterator;
    typedef typename std::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef typename native_type::difference_type        difference_type;
    typedef native_type                                  data_type;
};


template <typename Key, typename T>
class basic_map<Key, T, QMap>
    : public details::basic_map<Key, T, QMap>
{
protected:
    typedef details::basic_map<Key, T, QMap>           base_type;
    typedef typename base_type::size_type              size_type;
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
    
    virtual mapped_type & xat (Key const & key)
    {
        iterator it = this->xfind(key);
        if (it == this->xend())
            throw out_of_range("map::at");
        return it.value();
    }

    virtual mapped_type const & xat (Key const & key) const
    {
        const_iterator it = this->xfind(key);
        if (it == this->xend())
            throw out_of_range("map::at");
        return it.value();
    }
    
    virtual mapped_type & xsubscript (Key const & key)
    {
        return this->_d[key];
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

    virtual size_type xcount (Key const & key) const
    {
        return this->_d.count(key);
    }
    
    virtual iterator xfind (Key const & key)
    {
        return iterator(this->_d.find(key));
    }
		
    virtual const_iterator xfind (Key const & key) const
    {
        return const_iterator(this->_d.find(key));
    }
    
    virtual pfs::pair<iterator, bool> xinsert (Key const & key, T const & value)
    {
        iterator it = iterator(this->_d.insert(key, value));
        return pfs::pair<iterator, bool>(it, true);
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

#endif /* __PFS_MPL_IMPL_QT_MAP_HPP__ */
