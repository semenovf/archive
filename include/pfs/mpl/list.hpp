/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   list.hpp
 * Author: wladt
 *
 * Created on January 16, 2017, 12:08 PM
 */

#ifndef __PFS_MPL_LIST_HPP__
#define __PFS_MPL_LIST_HPP__

#include <pfs/cxxlang.hpp>
#include <pfs/iterator.hpp>

namespace pfs {
namespace mpl {

template <typename T, template <typename> class ListT>
struct list_traits
{
    typedef ListT<T>                                  native_type;
    typedef typename ListT<T>::size_type              size_type;
    typedef typename ListT<T>::difference_type        difference_type;
    typedef typename ListT<T>::value_type             value_type;
    typedef typename ListT<T>::reference              reference;
    typedef typename ListT<T>::const_reference        const_reference;
    typedef typename ListT<T>::iterator               iterator;
    typedef typename ListT<T>::const_iterator         const_iterator;
    typedef typename ListT<T>::reverse_iterator       reverse_iterator;
    typedef typename ListT<T>::const_reverse_iterator const_reverse_iterator;
    typedef ListT<T> data_type;
};

template <typename T, template <typename> class ListT>
class basic_list;

namespace details {
    
template <typename T, template <typename> class ListT>
class basic_list
{
public:
    typedef basic_list                                   self_type;
    typedef list_traits<T, ListT>                        traits_type;
    typedef typename traits_type::native_type            native_type;
    typedef typename traits_type::value_type             value_type;
    typedef typename traits_type::size_type              size_type;
    typedef typename traits_type::difference_type        difference_type;
    typedef typename traits_type::reference              reference;
    typedef typename traits_type::const_reference        const_reference;
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
	virtual void xpush_back (T const & value) = 0;
    virtual void xpop_back () = 0;
    virtual void xpush_front (T const & value) = 0;
    virtual void xpop_front () = 0;
    virtual void xresize (size_type count, value_type const & value) = 0;
    virtual void xsplice (iterator pos, self_type & rhs) = 0;
    virtual iterator xinsert (iterator pos, value_type const & value) = 0;
    virtual void xswap (self_type & rhs) = 0;

public:
    basic_list ()
    {}

    basic_list (native_type const & rhs)
        : _d(rhs)
    {}
    
    basic_list & operator = (native_type const & rhs)
    {
        if (this != & rhs)
            _d = rhs;
        return *this;
    }
    
    virtual native_type & native () = 0;
    virtual native_type const & native () const = 0;
};

} // details

template <typename T, template <typename> class ListT>
class list : public basic_list<T, ListT>
{
    typedef basic_list<T, ListT> base_type;
    typedef list self_type;

public:    
    typedef typename base_type::native_type            native_type;
    typedef typename base_type::value_type             value_type;
    typedef typename base_type::size_type              size_type;
    typedef typename base_type::difference_type        difference_type;
    typedef typename base_type::reference              reference;
    typedef typename base_type::const_reference        const_reference;
    typedef typename base_type::iterator               iterator;
    typedef typename base_type::const_iterator         const_iterator;
    typedef typename base_type::reverse_iterator       reverse_iterator;
    typedef typename base_type::const_reverse_iterator const_reverse_iterator;
   
public:
	explicit list ()
		: base_type()
	{}

    list (size_type count, T const & value)
        : base_type(count, value)
    {}
    
    explicit list (size_type count)
        : base_type(count)
    {}
    
    template <typename InputIt>
    list (InputIt first, InputIt last)
    {
        for (; first != last; ++first)
            push_back(*first);
    }
    
    list (native_type const & rhs)
        : base_type(rhs)
    {}

    self_type & operator = (native_type const & rhs)
    {
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
    
    reference front ()
    {
        return *this->begin();
    }
		
    const_reference front () const
    {
        return *this->begin();
    }
    
    reference back ()
    {
        iterator it = this->end();
        --it; 
        return *it;
    }
		
    const_reference back () const
    {
        const_iterator it = this->end();
        --it; 
        return *it;
    }
    
    size_type size () const pfs_noexcept
    {
        return this->xsize();
    }

	
    bool empty () const pfs_noexcept
    {
        return this->size() == 0;
    }
	
    void clear ()
    {
        this->xclear();
    }
    
    iterator erase (iterator position)
    {
        return this->xerase(position);
    }

    iterator erase (iterator first, iterator last)
    {
        return this->xerase(first, last);
    }

    void push_back (T const & value)
    {
        this->xpush_back(value);
    }
    
    void pop_back ()
    {
        this->xpop_back();
    }
    
    void push_front (T const & value)
    {
        this->xpush_front(value);
    }
		
    void pop_front ()
    {
        this->xpop_front();
    }
	
    void resize (size_type count)
    {
        this->resize(count, value_type());
    }
    
    void resize (size_type count, value_type const & value)
    {
        this->xresize(count, value);
    }

    void swap (self_type & rhs)
    {
        this->xswap(rhs);
    }
    
    void splice (iterator pos, self_type & rhs)
    {
        this->xsplice(pos, rhs);
    }
    
    iterator insert (iterator pos, value_type const & value)
    {
        this->xinsert(pos, value);
    }
    
    iterator insert (iterator pos, size_type count, value_type const & value)
    {
        if (count) {
            list tmp(count, value);
            this->splice(pos, tmp);
            ++pos;
        }
        return pos;
    }
    
    template <typename InputIt>
    iterator insert (iterator pos, InputIt first, InputIt last)
    {
        if (first != last) {
            list tmp(first, last);
            this->splice(pos, tmp);
            ++pos;
        }
        return pos;
    }
};

template <typename T, template <typename> class ListT>
inline bool operator == (list<T, ListT> const & lhs, list<T, ListT> const & rhs)
{
    return lhs.native() == rhs.native();
}

template <typename T, template <typename> class ListT>
inline bool operator != (list<T, ListT> const & lhs, list<T, ListT> const & rhs)
{
    return ! operator == (lhs, rhs);
}

template <typename T, template <typename> class ListT>
inline void swap (list<T, ListT> const & lhs, list<T, ListT> const & rhs)
{
    lhs.swap(rhs);
}

}}

#endif /* __PFS_MPL_LIST_HPP__ */
