/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   scene.hpp
 * Author: wladt
 *
 * Created on August 2, 2016, 4:36 PM
 */

#ifndef __PFS_GRIOTTE_SCENE_HPP__
#define __PFS_GRIOTTE_SCENE_HPP__

#include <pfs/list.hpp>
#include <pfs/griotte/types.hpp>

namespace pfs {
namespace griotte {

class widget;

struct node
{
    typedef list<node> list_type;
    typedef list_type::iterator node_iterator;
    typedef list_type::const_iterator const_node_iterator;

    node *    parent;
    list_type children;
    widget *  pwidget;
    
    node ()
        : parent(0)
        , pwidget(0)
    {}
    
    ~node ();
};

class scene
{
private:
    node _root;
    
private:
    scene (scene const &);
    scene & operator = (scene const &);
    
protected:
    void add_widget (widget * w, widget * parent);

public:
    scene () {}
    
    template <typename _WidgetType>
    _WidgetType & create_widget (widget * parent = 0);
    
    void delete_widget (widget * w);
};

template <typename _WidgetType>
_WidgetType & scene::create_widget (widget * parent)
{
    _WidgetType * w = new _WidgetType;
    this->add_widget(w, parent);
    return *w;
}


}}

#endif /* SCENE_HPP */

