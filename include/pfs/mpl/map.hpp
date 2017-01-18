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
#include <pfs/utility.hpp>

namespace pfs {
namespace mpl {

template <typename Key, typename T, template <typename, typename> class MapT>
struct map_traits
{
    typedef MapT<Key, T>                              native_type;
    typedef typename MapT<Key, T>::size_type          size_type;
    typedef typename MapT<Key, T>::key_type           key_type;
    typedef typename MapT<Key, T>::mapped_type        mapped_type;
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
    typedef typename traits_type::native_type            native_type;
    typedef typename traits_type::size_type              size_type;
    typedef typename traits_type::key_type               key_type;
    typedef typename traits_type::mapped_type            mapped_type;
    typedef typename traits_type::iterator               iterator;
    typedef typename traits_type::const_iterator         const_iterator;
    typedef typename traits_type::reverse_iterator       reverse_iterator;
    typedef typename traits_type::const_reverse_iterator const_reverse_iterator;
    typedef typename traits_type::difference_type        difference_type;
    typedef typename traits_type::data_type              data_type;

public:
    data_type _d;

protected:
    virtual iterator xbegin () = 0;
    virtual const_iterator xbegin () const = 0;
    virtual iterator xend () = 0;
    virtual const_iterator xend () const = 0;
    virtual reverse_iterator xrbegin () = 0;
    virtual const_reverse_iterator xrbegin () const = 0;
    virtual reverse_iterator xrend () = 0;
    virtual const_reverse_iterator xrend () const = 0;
    virtual size_type xsize () const = 0;
    virtual mapped_type & xsubscript (Key const & key) = 0;
    virtual mapped_type & xat (Key const & key) = 0;
    virtual mapped_type const & xat (Key const & key) const = 0;
    virtual void xclear () = 0;
    virtual iterator xerase (iterator position) = 0;
    
    virtual iterator xerase (iterator first, iterator last)
    {
        if (first == this->xbegin() && last == this->xend())
            this->xclear();
        else
            while (first != last)
                this->xerase(first++);
    }
	
    virtual void xswap (basic_map & rhs) = 0;
    virtual size_type xcount (Key const & key) const = 0;
    virtual iterator xfind (Key const & key) = 0;
    virtual const_iterator xfind (Key const & key) const = 0;
    virtual pfs::pair<iterator, bool> xinsert (Key const & key, T const & value) = 0;

public:
    basic_map ()
    {}

    basic_map (native_type const & rhs)
        : _d(rhs)
    {}
    
    basic_map & operator = (native_type const & rhs)
    {
        if (this != & rhs)
            _d = rhs;
        return *this;
    }
    
    virtual native_type & native () = 0;
    virtual native_type const & native () const = 0;
};

} // details

template <typename Key, typename T, template <typename, typename> class MapT>
class map : public basic_map<Key, T, MapT>
{
    typedef basic_map<Key, T, MapT> base_type;

public:    
    typedef typename base_type::native_type            native_type;
    typedef typename base_type::size_type              size_type;
    typedef typename base_type::key_type               key_type;
    typedef typename base_type::mapped_type            mapped_type;
    typedef typename base_type::iterator               iterator;
    typedef typename base_type::const_iterator         const_iterator;
    typedef typename base_type::reverse_iterator       reverse_iterator;
    typedef typename base_type::const_reverse_iterator const_reverse_iterator;
   
public:
	explicit map ()
		: base_type()
	{}

	map (map const & rhs)
        : base_type(rhs)
	{}
    
    map (native_type const & rhs)
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

    map & operator = (native_type const & rhs)
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
        return rbegin();
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
        return rend();
    }
    
    /**
     * @brief  Access to %map data.
     * @param  kye  The key for which data should be retrieved.
     * @return  A reference to the data whose key is equivalent to @a key, if
     *          such a data is present in the %map.
     * @throw  pfs::out_of_range  If no such data is present.
     */
    mapped_type & at (Key const & key)
    {
        return this->xat(key);
    }

    mapped_type const & at (Key const & key) const
    {
        return this->xat(key);
    }
    
    /**
     * @brief  Subscript ( @c [] ) access to %map data.
     * @details Allows for easy lookup with the subscript ( @c [] )
     *          operator. Returns data associated with the key specified in
     *          subscript. If the key does not exist, a pair with that key
     *          is created using default values, which is then returned.
     *          Lookup requires logarithmic time.
     * @param key The key for which data should be retrieved.
     * @return A reference to the data of the (key,data) %pair.
     */
    mapped_type & operator [] (Key const & key)
    {
        return this->xsubscript(key);
    }
    
    size_type size () const
    {
        return this->xsize();
    }
    
    bool empty () const
    {
        return size() == 0;
    }
	
    //size_type max_size() const;
    
    /**
     *  @details Erases all elements in a %map. Note that this function only
     *           erases the elements, and that if the elements themselves are
     *           pointers, the pointed-to memory is not touched in any way.
     *           Managing the pointer is the user's responsibility.
     */    
    void clear ()
    {
        this->xclear();
    }
	
    // 
    // Leave C++17-style signature only as it compatible to prior-C++11 and later
    //
    iterator erase (iterator position)
    {
        return this->xerase(position);
    }

    iterator erase (iterator first, iterator last)
    {
        return this->xerase(first, last);
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
    
    pfs::pair<iterator, bool> insert (Key const & key, T const & value)
    {
        return this->xinsert(key, value);
    }
};

template <typename Key, typename T, template <typename, typename> class MapT>
inline bool operator == (map<Key, T, MapT> const & lhs, map<Key, T, MapT> const & rhs)
{
    return lhs.native() == rhs.native();
}

template <typename Key, typename T, template <typename, typename> class MapT>
inline bool operator != (map<Key, T, MapT> const & lhs, map<Key, T, MapT> const & rhs)
{
    return ! operator == (lhs, rhs);
}

template <typename Key, typename T, template <typename, typename> class MapT>
inline void swap (map<Key, T, MapT> const & lhs, map<Key, T, MapT> const & rhs)
{
    lhs.swap(rhs);
}

}}

#endif /* __PFS_MPL_MAP_HPP__ */

