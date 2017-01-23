/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   multimap.hpp
 * Author: wladt
 *
 * Created on January 20, 2017, 3:54 PM
 */

#ifndef __PFS_MPL_MULTIMAP_HPP__
#define __PFS_MPL_MULTIMAP_HPP__

#include <pfs/iterator.hpp>
#include <pfs/exception.hpp>
#include <pfs/utility.hpp>

namespace pfs {
namespace mpl {

template <typename Key, typename T, template <typename, typename> class MultiMapT>
struct multimap_traits
{
    typedef MultiMapT<Key, T>                        native_type;
    typedef typename native_type::size_type          size_type;
    typedef typename native_type::key_type           key_type;
    typedef typename native_type::mapped_type        mapped_type;
    typedef typename native_type::iterator           iterator;
    typedef typename native_type::const_iterator     const_iterator;
    typedef typename native_type::difference_type    difference_type;
    typedef typename native_type::reverse_iterator   reverse_iterator;
    typedef typename native_type::const_reverse_iterator const_reverse_iterator;
    typedef MultiMapT<Key, T> data_type;
};

template <typename Key, typename T, template <typename, typename> class MultiMapT>
class basic_multimap;

namespace details {
    
template <typename Key, typename T, template <typename, typename> class MultiMapT>
class basic_multimap
{
public:
    typedef multimap_traits<Key, T, MultiMapT>           traits_type;
    typedef typename traits_type::native_type            native_type;
    typedef typename traits_type::size_type              size_type;
    typedef typename traits_type::key_type               key_type;
    typedef typename traits_type::mapped_type            mapped_type;
    typedef typename traits_type::iterator               iterator;
    typedef typename traits_type::const_iterator         const_iterator;
    typedef typename traits_type::reverse_iterator       reverse_iterator;
    typedef typename traits_type::const_reverse_iterator const_reverse_iterator;
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
	
    virtual void xswap (basic_multimap & rhs) = 0;
    virtual size_type xcount (Key const & key) const = 0;
    virtual iterator xfind (Key const & key) = 0;
    virtual const_iterator xfind (Key const & key) const = 0;

    virtual pfs::pair<iterator,iterator> xequal_range (Key const & key) 
    {
        return pfs::pair<iterator,iterator>(this->xlower_bound(key)
                , this->xupper_bound(key));
    }
    
    virtual pfs::pair<const_iterator,const_iterator> xequal_range (Key const & key) const
    {
        return pfs::pair<iterator,iterator>(this->xlower_bound(key)
                , this->xupper_bound(key));
    }
    
    virtual iterator xlower_bound (Key const & key) = 0;
    virtual const_iterator xlower_bound (Key const & key) const = 0;
    virtual iterator xupper_bound (Key const & key) = 0;
    virtual const_iterator upper_bound (Key const & key) const = 0;
    virtual iterator xinsert (Key const & key, T const & value) = 0;

public:
    basic_multimap ()
    {}

    basic_multimap (native_type const & rhs)
        : _d(rhs)
    {}
    
    basic_multimap & operator = (native_type const & rhs)
    {
        if (this != & rhs)
            _d = rhs;
        return *this;
    }
    
    virtual native_type & native () = 0;
    virtual native_type const & native () const = 0;
};

} // details

template <typename Key, typename T, template <typename, typename> class MultiMapT>
class multimap : public basic_multimap<Key, T, MultiMapT>
{
    typedef basic_multimap<Key, T, MultiMapT> base_type;
    typedef multimap self_type;

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
	explicit multimap ()
		: base_type()
	{}

    multimap (native_type const & rhs)
        : base_type(rhs)
    {}
    
////    template <typename InputIt>
////    map (InputIt first, InputIt last);
//    
    multimap & operator = (native_type const & rhs)
    {
        if (this != & rhs)
            base_type::operator = (rhs);
        return *this;
    }
    
    iterator begin () pfs_noexcept
    {
        return this->xbegin();
    }
		
    const_iterator begin () const pfs_noexcept
    {
        return this->xbegin();
    }
		
    const_iterator cbegin () const pfs_noexcept
    {
        return this->begin();
    }
    
    iterator end () pfs_noexcept
    {
        return this->xend();
    }
		
    const_iterator end () const pfs_noexcept
    {
        return this->xend();
    }
		
    const_iterator cend () const pfs_noexcept
    {
        return this->end();
    }
    
    reverse_iterator rbegin () pfs_noexcept
    {
        return this->xrbegin();
    }
		
    const_reverse_iterator rbegin () const pfs_noexcept
    {
        return this->xrbegin();
    }
		
    const_reverse_iterator crbegin () const pfs_noexcept
    {
        return rbegin();
    }
    
    reverse_iterator rend () pfs_noexcept
    {
        return this->xrend();
    }
		
    const_reverse_iterator rend () const pfs_noexcept
    {
        return this->xrend();
    }
		
    const_reverse_iterator crend () const pfs_noexcept
    {
        return rend();
    }
    
    size_type size () const pfs_noexcept
    {
        return this->xsize();
    }
    
    bool empty () const pfs_noexcept
    {
        return size() == 0;
    }
	
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
	
    void swap (self_type & rhs)
    {
        this->xswap(rhs);
    }

    /**
     * @brief Returns the number of elements associated with key @a key.
     * @param key Key.
     * @return The number of elements associated with key @a key.
     */
    size_type count (key_type const & key) const
    {
        this->xcount(key);
    }
	
    iterator find (key_type const & key)
    {
        return this->xfind(key);
    }
		
    const_iterator find (key_type const & key) const
    {
        return this->xfind(key);
    }
    
    pfs::pair<iterator,iterator> equal_range (key_type const & key)
    {
        return this->xequal_range(key);
    }

    std::pair<const_iterator,const_iterator> equal_range (key_type const & key ) const
    {
        return this->xequal_range(key);
    }
    
    iterator lower_bound (key_type const & key)
    {
        return this->xlower_bound(key);
    }

    const_iterator lower_bound (key_type const & key) const
    {
        return this->xlower_bound(key);
    }
    
    iterator upper_bound (key_type const & key)
    {
        return this->xupper_bound(key);
    }

    const_iterator upper_bound (key_type const & key) const
    {
        return this->xupper_bound(key);
    }
    
    iterator insert (key_type const & key, mapped_type const & value)
    {
        return this->xinsert(key, value);
    }
    
//    template <typename InputIt>
//    void insert (InputIt first, InputIt last);
};

template <typename Key, typename T, template <typename, typename> class MultiMapT>
inline bool operator == (multimap<Key, T, MultiMapT> const & lhs, multimap<Key, T, MultiMapT> const & rhs)
{
    return lhs.native() == rhs.native();
}

template <typename Key, typename T, template <typename, typename> class MultiMapT>
inline bool operator != (multimap<Key, T, MultiMapT> const & lhs, multimap<Key, T, MultiMapT> const & rhs)
{
    return ! operator == (lhs, rhs);
}

template <typename Key, typename T, template <typename, typename> class MultiMapT>
inline void swap (multimap<Key, T, MultiMapT> const & lhs, multimap<Key, T, MultiMapT> const & rhs)
{
    lhs.swap(rhs);
}

}}


#endif /* __PFS_MPL_MULTIMAP_HPP__ */

