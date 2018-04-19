/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   push_button.hpp
 * Author: wladt
 *
 * Created on September 14, 2016, 12:09 PM
 */

#ifndef __PFS_GRIOTTE_PUSH_BUTTON_HPP__
#define __PFS_GRIOTTE_PUSH_BUTTON_HPP__

#include <pfs/griotte/button.hpp>

namespace pfs {
namespace griotte {

class push_button : public button
{
public:
    push_button () : button() {}
    virtual ~push_button () {}
};

}}

#endif /* __PFS_GRIOTTE_PUSH_BUTTON_HPP__ */
