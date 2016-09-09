/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   griotte_p.hpp
 * Author: wladt
 *
 * Created on September 9, 2016, 5:00 PM
 */

#ifndef __PFS_GRIOTTE_GRIOTTE_P_HPP__
#define __PFS_GRIOTTE_GRIOTTE_P_HPP__

#include <gtk/gtk.h>

namespace pfs {
namespace griotte {

namespace details {

struct application 
{
    GtkApplication * app;
};

struct window
{
    GtkWidget * win;
};

} // details

}} // pfs::griotte

#endif /* __PFS_GRIOTTE_GRIOTTE_P_HPP__ */

