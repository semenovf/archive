/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   painter.hpp
 * Author: wladt
 *
 * Created on September 16, 2016, 10:26 AM
 */

#ifndef __PFS_GRIOTTE_PAINTER_HPP__
#define __PFS_GRIOTTE_PAINTER_HPP__

#include <pfs/string.hpp>
#include <pfs/griotte/types.hpp>

namespace pfs {
namespace griotte {

namespace details {
struct painter;
}

class painter
{
    friend class window;
    
    details::painter * _d;
    
private:
    painter (painter const &);
    painter & operator = (painter const &);
    
public:
    painter () {}
    virtual ~painter () {}
    
    void set_source_rgb (double red, double green, double blue);
    
    void move_to (double x, double y);
    
    void select_font_face (string const & family
            , font_slant_enum slant
            , font_weight_enum weight);

    void set_font_size (double size);

    void show_text (string const & text);    
};

}}

#endif /* __PFS_GRIOTTE_PAINTER_HPP__ */

