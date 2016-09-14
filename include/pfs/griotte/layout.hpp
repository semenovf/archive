/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   layout.hpp
 * Author: wladt
 *
 * Created on September 10, 2016, 2:18 PM
 */

#ifndef __PFS_GRIOTTE_LAYOUT_HPP__
#define __PFS_GRIOTTE_LAYOUT_HPP__

namespace pfs {
namespace griotte {

class container;

class layout
{
    friend class container;
    
protected:    
    container * _parent;
    
private:
    layout (layout const &);
    layout & operator = (layout const &);

public:
    layout () : _parent(0) {}
    virtual ~layout () {}
};

}}

#endif /* __PFS_GRIOTTE_LAYOUT_HPP__ */

