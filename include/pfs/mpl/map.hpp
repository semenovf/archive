/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   map.hpp
 * Author: wladt
 *
 * Created on January 16, 2017, 2:39 PM
 */

#ifndef __PFS_MPL_MAP_HPP__
#define __PFS_MPL_MAP_HPP__

#include <pfs/iterator.hpp>
#include <pfs/exception.hpp>

namespace pfs {
namespace mpl {

template <typename Key, typename T, template <typename, typename> class MapT>
struct map_traits
{
    typedef MapT<Key, T> const &                      const_impl_reference;
    typedef typename MapT<Key, T>::size_type          size_type;
    typedef typename MapT<Key, T>::value_type         value_type;
    typedef typename MapT<Key, T>::key_type           key_type;
    typedef typename MapT<Key, T>::mapped_type        mapped_type;
//    typedef typename MapT<Key, T>::pointer            pointer;
//    typedef typename MapT<Key, T>::const_pointer      const_pointer;
//    typedef typename MapT<Key, T>::reference          reference;
//    typedef typename MapT<Key, T>::const_reference    const_reference;
    typedef typename MapT<Key, T>::iterator           iterator;
    typedef typename MapT<Key, T>::const_iterator     const_iterator;
    typedef typename MapT<Key, T>::difference_type    difference_type;
    typedef typename MapT<Key, T>::reverse_iterator   reverse_iterator;
    typedef typename MapT<Key, T>::const_reverse_iterator const_reverse_iterator;
    
    typedef MapT<Key, T> data_type;
};

template <typename Key, typename T, template <typename, typename> class MapT>
class basic_map;

namespace details {
    
template <typename Key, typename T, template <typename, typename> class MapT>
class basic_map
{
public:
    typedef map_traits<Key, T, MapT>                     traits_type;
    typedef typename traits_type::const_impl_reference   const_impl_reference;
    typedef typename traits_type::size_type              size_type;
    typedef typename traits_type::key_type               key_type;
    typedef typename traits_type::mapped_type            mapped_type;
//    typedef typename traits_type::value_type             value_type;
//    typedef typename traits_type::pointer                pointer;
//    typedef typename traits_type::const_pointer          const_pointer;
//    typedef typename traits_type::reference              reference;
//    typedef typename traits_type::const_reference        const_reference;
    typedef typename traits_type::iterator               iterator;
    typedef typename traits_type::const_iterator         const_iterator;
    typedef typename traits_type::reverse_iterator       reverse_iterator;
    typedef typename traits_type::const_reverse_iterator const_reverse_iterator;
    typedef typename traits_type::difference_type        difference_type;
    typedef typename traits_type::data_type              data_type;

public:
    data_type _d;

protected:
    virtual iterator xbegin ()
    {
        return _d.begin();
    }
    
    virtual const_iterator xbegin () const
    {
        return _d.begin();
    }
    
    virtual iterator xend ()
    {
        return _d.end();
    }
    
    virtual const_iterator xend () const
    {
        return _d.end();
    }
    
    virtual reverse_iterator xrbegin ()
    {
        return _d.rbegin();
    }
    
    virtual const_reverse_iterator xrbegin () const
    {
        return _d.rbegin();
    }
    
    virtual reverse_iterator xrend ()
    {
        return _d.rend();
    }
    
    virtual const_reverse_iterator xrend () const
    {
        return _d.rend();
    }
    
    virtual size_type xsize () const
    {
        return _d.size();
    }
    
    virtual void xclear ()
    {
        _d.clear();
    }

    virtual iterator xerase (iterator position)
    {
        return _d.erase(position);
    }
    
    virtual iterator xerase (iterator first, iterator last)
    {
        if (first == xbegin() && last == xend())
            xclear();
        else
            while (first != last)
                xerase(first++);
    }
	
    void xswap (basic_map & rhs)
    {
        _d.swap(rhs._d);
    }

    virtual size_type xcount (Key const & key) const
    {
        return _d.count(key);
    }
    
    virtual iterator xfind (Key const & key)
    {
        return _d.find(key);
    }
		
    virtual const_iterator xfind (Key const & key) const
    {
        return _d.find(key);
    }

public:
    basic_map ()
    {}

    basic_map (data_type const & d)
        : _d(d)
    {}
};

} // details

template <typename Key, typename T, template <typename, typename> class MapT>
class map : public basic_map<Key, T, MapT>
{
    typedef basic_map<Key, T, MapT> base_type;

public:    
//    typedef typename traits_type::const_impl_reference   const_impl_reference;
    typedef typename base_type::size_type              size_type;
    typedef typename base_type::key_type               key_type;
//    typedef typename base_type::value_type             value_type;
//    typedef typename traits_type::pointer                pointer;
//    typedef typename traits_type::const_pointer          const_pointer;
//    typedef typename traits_type::reference              reference;
//    typedef typename traits_type::const_reference        const_reference;
    typedef typename base_type::iterator               iterator;
    typedef typename base_type::const_iterator         const_iterator;
    typedef typename base_type::reverse_iterator       reverse_iterator;
    typedef typename base_type::const_reverse_iterator const_reverse_iterator;
    
//    typedef typename traits_type::difference_type        difference_type;
//    typedef typename traits_type::data_type              data_type;
   
public:
	explicit map ()
		: base_type()
	{}

	map (map const & rhs)
        : base_type(rhs)
	{}
    
//    template <typename InputIt>
//    map (InputIt first, InputIt last);
    
    map & operator = (map const & rhs)
    {
        if (this != & rhs)
            base_type::operator = (rhs);
        return *this;
    }
    
    iterator begin ()
    {
        return this->xbegin();
    }
		
    const_iterator begin () const
    {
        return this->xbegin();
    }
		
    const_iterator cbegin () const
    {
        return this->begin();
    }
    
    iterator end ()
    {
        return this->xend();
    }
		
    const_iterator end () const
    {
        return this->xend();
    }
		
    const_iterator cend () const
    {
        return this->end();
    }
    
    reverse_iterator rbegin ()
    {
        return this->xrbegin();
    }
		
    const_reverse_iterator rbegin () const
    {
        return this->xrbegin();
    }
		
    const_reverse_iterator crbegin () const
    {
        return this->rbegin();
    }
    
    reverse_iterator rend ()
    {
        return this->xrend();
    }
		
    const_reverse_iterator rend () const
    {
        return this->xrend();
    }
		
    const_reverse_iterator crend () const
    {
        return this->rend();
    }
    
    T & at (Key const & key)
    {
        iterator it = this->find(key);
        if (it == this->end())
            throw out_of_range("map::at");
        return *it;
    }

    T const & at (Key const & key) const
    {
        const_iterator it = this->find(key);
        if (it == this->end())
            throw out_of_range("map::at");
        return *it;
    }
    
    T & operator [] (Key const & key )
    {
        return base_type::operator [] (key);
    }
    
    size_type size () const
    {
        return this->xsize();
    }
    
    bool empty () const
    {
        return this->size() == 0;
    }
	
    //size_type max_size() const;
    
    void clear ()
    {
        this->xclear();
    }
	
    // 
    // Leave C++17-style signature only as it compatible to prior-C++11 and prior-C++17
    //
    iterator erase (iterator position)
    {
        this->xerase(position);
    }

    iterator erase (iterator first, iterator last)
    {
        this->xerase(first, last);
    }
	
    void swap (map & rhs)
    {
        this->xswap(rhs);
    }
    
    size_type count (Key const & key) const
    {
        this->xcount(key);
    }
	
    iterator find (Key const & key)
    {
        return this->xfind(key);
    }
		
    const_iterator find (Key const & key) const
    {
        return this->xfind(key);
    }
	
//    std::pair<iterator,iterator> equal_range( const Key& key );
//    std::pair<const_iterator,const_iterator> equal_range( const Key& key ) const;
//    iterator lower_bound( const Key& key );
//    const_iterator lower_bound( const Key& key ) const;
//    iterator upper_bound( const Key& key );
//    const_iterator upper_bound( const Key& key ) const;
   
};

}}

#endif /* __PFS_MPL_MAP_HPP__ */

