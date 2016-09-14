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
public:
    typedef pfs::list<widget *> list_type;
    typedef list_type::iterator widget_iterator;
    typedef list_type::const_iterator widget_const_iterator;
    
protected:
    pfs::list<widget *> _widgets;
    layout * _layout;
    
public:
    virtual ~container ();
    
    template <typename _WidgetType>
    _WidgetType & create_widget ();
    
    template <typename _LayoutType>
    _LayoutType & create_layout ();
    
protected:
    void add_widget (widget *);
    void set_layout (layout *);
};

template <typename _WidgetType>
_WidgetType & container::create_widget ()
{
    _WidgetType * w = new _WidgetType;
    this->add_widget(w);
    return *w;
}
    
template <typename _LayoutType>
_LayoutType & container::create_layout ()
{
    _LayoutType * l = new _LayoutType;
    this->set_layout(l);
    return *l;
}

}}

#endif /* __PFS_GRIOTTE_CONTAINER_HPP__ */

