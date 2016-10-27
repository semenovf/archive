/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   trivial_list.hpp
 * Author: wladt
 *
 * Created on October 25, 2016, 2:09 PM
 */

#ifndef __PFS_TL_TRIVIAL_LIST_HPP__
#define __PFS_TL_TRIVIAL_LIST_HPP__

#include <pfs.hpp> // TODO REMOVE THIS INCLDUE

namespace pfs {
namespace tl {

template <typename T>
class trivial_list
{
    struct node {
        node * prev;
        node * next;
        T    * value;
    };
    
    node * _first;
    node * _last;
    
public:    
    typedef T            value_type;
    typedef T &          reference;
    typedef T const &    const_reference;
    typedef node *       iterator;
    typedef node const * const_iterator;
    
private:
    void push_back_helper (node * p);
        
public:
    trivial_list ()
        : _first(0)
        , _last(0)
    {}

    ~trivial_list ();
    
    bool empty () const
    {
        return _first == 0;
    }
    
    iterator begin ()
    {
        return _first;
    }

    iterator end ()
    {
        return 0;
    }

    const_iterator begin () const
    {
        return _first;
    }

    const_iterator end () const
    {
        return 0;
    }

    const_iterator cbegin () const
    {
        return begin();
    }

    const_iterator cend () const
    {
        return 0;
    }
    
    reference front ()
    {
        if (!_first) {
            PFS_ASSERT(_first);
        }
        PFS_ASSERT(_first->value);
        return *_first->value; 
    }

    reference back ()
    {
        if (! _last) {
            PFS_ASSERT(_last);
        }
        PFS_ASSERT(_last->value);
        return *_last->value; 
    }

    const_reference front () const
    {
        return *_first->value; 
    }

    const_reference back () const
    {
        return *_last->value; 
    }
    
    void emplace_back ()
    {
        node * p = (node *)::operator new (sizeof(node) + sizeof(T));
        push_back_helper(p);
        p->value = new (p + 1) T;
    }

    template <typename Arg1>
    void emplace_back (Arg1 a1)
    {
        node * p = (node *)::operator new (sizeof(node) + sizeof(T));
        push_back_helper(p);
        p->value = new (p + 1) T(a1);
    }

    template <typename Arg1, typename Arg2>
    void emplace_back (Arg1 a1, Arg2 a2)
    {
        node * p = (node *)::operator new (sizeof(node) + sizeof(T));
        push_back_helper(p);
        p->value = new (p + 1) T(a1, a2);
    }

    template <typename Arg1, typename Arg2, typename Arg3>
    void emplace_back (Arg1 a1, Arg2 a2, Arg3 a3)
    {
        node * p = (node *)::operator new (sizeof(node) + sizeof(T));
        push_back_helper(p);
        p->value = new (p + 1) T(a1, a2, a3);
    }

    template <typename Arg1, typename Arg2, typename Arg3, typename Arg4>
    void emplace_back (Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4)
    {
        node * p = (node *)::operator new (sizeof(node) + sizeof(T));
        push_back_helper(p);
        p->value = new (p + 1) T(a1, a2, a3, a4);
    }

    template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
    void emplace_back (Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5)
    {
        node * p = (node *)::operator new (sizeof(node) + sizeof(T));
        push_back_helper(p);
        p->value = new (p + 1) T(a1, a2, a3, a4, a5);
    }

    template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
    void emplace_back (Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6)
    {
        node * p = (node *)::operator new (sizeof(node) + sizeof(T));
        push_back_helper(p);
        p->value = new (p + 1) T(a1, a2, a3, a4, a5, a6);
    }

    template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
    void emplace_back (Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7)
    {
        node * p = (node *)::operator new (sizeof(node) + sizeof(T));
        push_back_helper(p);
        p->value = new (p + 1) T(a1, a2, a3, a4, a5, a6, a7);
    }

    template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8>
    void emplace_back (Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8)
    {
        node * p = (node *)::operator new (sizeof(node) + sizeof(T));
        push_back_helper(p);
        p->value = new (p + 1) T(a1, a2, a3, a4, a5, a6, a7, a8);
    }
    
    /**
     * @brief Remove element at given position.
     * @param it  Iterator pointing to element to be erased.
     * @return An iterator pointing to the next element (or end()).
     */
    iterator erase (iterator it);
};

template <typename T>
trivial_list<T>::~trivial_list ()
{
    node * p  = _first;
    node * p1 = p;

    while (p) {
        p1 = p->next;
        p->value->~T();
        p->next = 0;
        p->prev = 0;
        delete p;
        p = p1;
    }
    
    _first = 0;
    _last = 0;
}

template <typename T>
void trivial_list<T>::push_back_helper (node * p)
{
    if (!_last && !_first) {
        p->next = p->prev = 0;
        _first = p;
        _last = p;
    } else {
        p->prev = _last;
        p->next = 0;
        _last->next = p;
        _last = p;
    }
}

template <typename T>
typename trivial_list<T>::iterator trivial_list<T>::erase (iterator it)
{
    iterator result = end();
    
    if (it) {
        if (it->prev == 0 && it->next == 0) { // The only element, ...
            _first = 0;                      // ... result is end()
            _last = 0;
        } else if (it->prev == 0) {          // First element, ...
            _first = it->next;               // result is new first element
            _first->prev = 0;
            result = _first;
        } else if (it->next == 0) {          // Last element, ...
            _last = it->prev;                // result is end()
            _last->next = 0;
        } else {                             // Other case, ... 
            it->prev->next = it->next;       // .. result is next element
            it->next->prev = it->prev;
            result = it->next;
        }
        
        it->value->~T();
        it->next = 0;
        it->prev = 0;
        delete it;
    }
    
    return result;
}

}} // pfs::tl

#endif /* __PFS_TL_TRIVIAL_LIST_HPP__ */

