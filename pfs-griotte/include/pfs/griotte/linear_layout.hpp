/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   linear_layout.hpp
 * Author: wladt
 *
 * Created on September 14, 2016, 12:30 PM
 */

#ifndef __PFS_GRIOTTE_LINEAR_LAYOUT_HPP__
#define __PFS_GRIOTTE_LINEAR_LAYOUT_HPP__

#include <pfs/vector.hpp>
#include <pfs/griotte/layout.hpp>

namespace pfs {
namespace griotte {

class widget;

class linear_layout : public layout
{
protected:
    typedef vector<widget *> group_type;
    
    group_type _group;
    
public:
    linear_layout () : layout() {}
    virtual ~linear_layout () {}
    
    void add_widget (widget & w)
    {
        _group.push_back(& w);
    }
};

}}


#endif /* __PFS_GRIOTTE_LINEAR_LAYOUT_HPP__ */

