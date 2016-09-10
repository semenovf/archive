/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   node.hpp
 * Author: wladt
 *
 * Created on September 10, 2016, 2:04 PM
 */

#ifndef __PFS_GRIOTTE_NODE_HPP__
#define __PFS_GRIOTTE_NODE_HPP__

namespace pfs {
namespace griotte {

class node
{
    node * _prev;
    node * _next;
    node * _first;
    node * _last;
};

}}

#endif /* __PFS_GRIOTTE_NODE_HPP__ */

