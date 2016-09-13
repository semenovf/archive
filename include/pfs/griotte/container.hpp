/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   container.hpp
 * Author: wladt
 *
 * Created on September 13, 2016, 4:05 PM
 */

#ifndef __PFS_GRIOTTE_CONTAINER_HPP__
#define __PFS_GRIOTTE_CONTAINER_HPP__

#include <pfs/list.hpp>
#include <pfs/griotte/widget.hpp>
//#include <pfs/griotte/layout.hpp>

namespace pfs {
namespace griotte {

class layout;
//class node
//{
//    node * _prev;
//    node * _next;
//    node * _first;
//    node * _last;
//};

class container : public widget
{
    friend class widget;
    
public:
    typedef pfs::list<widget *> list_type;
//    typedef list_type::iterator iterator;
//    typedef list_type::const_iterator const_iterator;
    
protected:
    pfs::list<widget *> _widgets;
    layout * _layout;
    
public:
    template <typename _WidgetType>
    _WidgetType & create ()
    {
        _WidgetType * w = new _WidgetType;
        w._parent = this;
        return *w;
    }
    
protected:
    void set (layout *);
};

}}

#endif /* __PFS_GRIOTTE_CONTAINER_HPP__ */

