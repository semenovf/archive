/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   canvas.hpp
 * Author: wladt
 *
 * Created on September 10, 2016, 2:08 PM
 */

#ifndef __PFS_GRIOTTE_CANVAS_HPP__
#define __PFS_GRIOTTE_CANVAS_HPP__

#include <pfs/griotte/widget.hpp>

namespace pfs {
namespace griotte {

class canvas : public widget
{
private:
    canvas (canvas const &);
    canvas & operator = (canvas const &);
};

}}

#endif /* __PFS_GRIOTTE_CANVAS_HPP__ */

