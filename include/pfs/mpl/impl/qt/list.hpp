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

#include <QLinkedList>
#include <pfs/mpl/list.hpp>

namespace pfs {
namespace mpl {

template <typename T>
struct list_traits<T, QLinkedList>
{
    typedef QLinkedList<T>                                 native_type;
    typedef typename native_type::size_type                size_type;
    typedef typename native_type::value_type               value_type;
    typedef typename native_type::reference                reference;
    typedef typename native_type::const_reference          const_reference;
    typedef typename native_type::iterator                 iterator;
    typedef typename native_type::const_iterator           const_iterator;
    typedef typename std::reverse_iterator<iterator>       reverse_iterator;
    typedef typename std::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef native_type                                    data_type;
};

template <typename T>
class basic_list<T, QLinkedList>
    : public details::basic_list<T, QLinkedList>
{
    typedef details::basic_list<T, QLinkedList> base_type;
    typedef basic_list self_type;
    
protected:
    typedef typename base_type::native_type            native_type;
    typedef typename base_type::value_type             value_type;
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
        return this->_d.erase(position);
    }

    virtual iterator xerase (iterator first, iterator last)
    {
        return this->_d.erase(first, last);
    }

    virtual void xpush_back (T const & value)
    {
        this->_d.push_back(value);
    }

    virtual void xpop_back ()
    {
        this->_d.pop_back();
    }

    virtual void xpush_front (T const & value)
    {
        this->_d.push_front(value);
    }

    virtual void xpop_front ()
    {
        this->_d.pop_front();
    }
    
    virtual void xresize (size_type count, value_type const & value);

    virtual void xsplice (iterator pos, base_type & rhs)
    {
        // Emulate splice operation through move elements
        for (iterator it = this->xbegin(); it != this->xend(); ++it)
            pos = this->xinsert(pos, *it);
        static_cast<self_type &>(rhs).xclear();
    }

    virtual void xswap (base_type & rhs)
    {
        this->_d.swap(rhs._d);
    }

    virtual iterator xinsert (iterator pos, value_type const & value)
    {
        return this->_d.insert(pos, value);
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


template <typename T>
void basic_list<T, QLinkedList>::xresize (size_type count, value_type const & value)
{
    if (count < this->xsize()) {
        // Reduce container to first count elements
        //
        iterator first = this->xbegin();
        pfs::advance(first, count);
        this->xerase(first, this->xend());
    } else {
        for (size_type i = this->xsize(); i < count; ++i)
            this->_d.append(value);
    }
}


}}

#endif /* __PFS_MPL_IMPL_QT_LIST_HPP__ */

