/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   vertical_layout.hpp
 * Author: wladt
 *
 * Created on September 14, 2016, 12:15 PM
 */

#ifndef __PFS_GRIOTTE_VERTICAL_LAYOUT_HPP__
#define __PFS_GRIOTTE_VERTICAL_LAYOUT_HPP__

#include <pfs/griotte/linear_layout.hpp>

namespace pfs {
namespace griotte {

class widget;

class vertical_layout : public linear_layout
{
public:
    vertical_layout () : linear_layout() {}
    virtual ~vertical_layout () {}
};

}}

#endif /* __PFS_GRIOTTE_VERTICAL_LAYOUT_HPP__ */

