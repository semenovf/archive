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

namespace pfs {
namespace griotte {

class container;

class widget
{
    friend class container;
    
protected:    
    container * _parent;
    
public:
    widget () : _parent(0) {}
};

}}

#endif /* __PFS_GRIOTTE_WIDGET_HPP__ */

