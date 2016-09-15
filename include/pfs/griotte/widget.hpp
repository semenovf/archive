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

#include <pfs/griotte/types.hpp>

namespace pfs {
namespace griotte {

class container;

class widget
{
    friend class container;
    
protected:    
    container * _parent;
    rect_type   _rect;   // holds the geometry of the widget relative to its parent
    
private:
    widget (widget const &);
    widget & operator = (widget const &);
    
public:
    widget () : _parent(0) {}
    virtual ~widget () {}
    
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

}}

#endif /* __PFS_GRIOTTE_WIDGET_HPP__ */

