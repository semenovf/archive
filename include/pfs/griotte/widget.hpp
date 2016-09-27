/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   widget.hpp
 * Author: wladt
 *
 * Created on September 10, 2016, 2:14 PM
 */

#ifndef __PFS_GRIOTTE_WIDGET_HPP__
#define __PFS_GRIOTTE_WIDGET_HPP__

#include <pfs/list.hpp>
#include <pfs/griotte/types.hpp>

namespace pfs {
namespace griotte {

class scene;
class layout;

class widget
{
    friend class scene;
    
public:
    typedef pfs::list<widget *> list_type;
    typedef list_type::iterator widget_iterator;
    typedef list_type::const_iterator widget_const_iterator;
    
protected:
    widget * _parent;
    pfs::list<widget *> _widgets;
    layout *  _layout;
    rect_type _rect;   // holds the geometry of the widget relative to its parent
    
private:
    widget (widget const &);
    widget & operator = (widget const &);
    
protected:
    widget ();

    void add_widget (widget *);
    void set_layout (layout *);

public:
    virtual ~widget ();

    template <typename _WidgetType>
    _WidgetType & create_widget ();
    
    template <typename _LayoutType>
    _LayoutType & create_layout ();
    
    rect_type const & get_geometry () const
    {
        return _rect;
    }
    
    void set_geometry (unit_type x, unit_type y, unit_type w, unit_type h)
    {
        _rect = rect_type(x, y, w, h);
    }
    
    void set_geometry (rect_type const & rect)
    {
        _rect = rect;
    }
    
    /**
     * @brief Checks if point @a p is inside the widget's bounds (rectangle by default).
     * 
     * @param p Given point.
     * @return @c true if point @a p is inside the rectangle, @c false otherwise
     * 
     * @note Must be reimplemented if widget has bounds ... (TODO finish the sentence)
     */
    virtual bool is_inner_point (point_type const & p) const
    {
        _rect.is_inner_point(p);
    }
};

template <typename _WidgetType>
_WidgetType & widget::create_widget ()
{
    _WidgetType * w = new _WidgetType;
    this->add_widget(w);
    return *w;
}
    
template <typename _LayoutType>
_LayoutType & widget::create_layout ()
{
    _LayoutType * l = new _LayoutType;
    this->set_layout(l);
    return *l;
}


}}

#endif /* __PFS_GRIOTTE_WIDGET_HPP__ */

