/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   window.cpp
 * Author: wladt
 * 
 * Created on August 2, 2016, 1:51 PM
 */

#include <gtk/gtk.h>
#include "pfs/griotte/griotte.hpp"
#include "pfs/griotte/window.hpp"
#include "internals.hpp"
#include "../cairo/internals.hpp"

namespace pfs {
namespace griotte {

static gboolean __repaint (GtkWidget * native_widget, cairo_t * cr, gpointer win_ptr)
{
    window * win = static_cast<window *>(win_ptr);
    scene & sc = win->get_scene();
    
    if (sc.is_invalid()) {
        sc.repaint();
    }
    
    return FALSE;
}

window::window ()
    : _d(new details::window)
{
    application & app = application::instance();
    _d->win = gtk_application_window_new(app._d->app);
    _d->drawing_area = gtk_drawing_area_new();
    
    gtk_container_add(GTK_CONTAINER(_d->win), _d->drawing_area);
  
    g_signal_connect(G_OBJECT(_d->drawing_area), "draw", G_CALLBACK(__repaint), this);
}

window::~window ()
{
    delete _d;
}

void window::show ()
{
    gtk_widget_show_all(_d->win);
}

void window::set_title (pfs::string const & title)
{
    gtk_window_set_title(GTK_WINDOW(_d->win), title.c_str());
}

void window::resize (int width, int height)
{
    gtk_window_resize_to_geometry(GTK_WINDOW(_d->win), width, height);
}

void window::move (int x, int y)
{
    gtk_window_move(GTK_WINDOW(_d->win), x, y);
}

void window::move (position_type const & pos)
{
    if (pos.value() & position_type::center_xy) {
        gtk_window_set_position(GTK_WINDOW(_d->win), GTK_WIN_POS_CENTER);
        return;
    } 
    
    
    int x = 0, y = 0;
    
// TODO
//    
//    if (pos.value & position_type::center_x) {
//        
//    }
//    
//    if (pos.value & position_type::center_y) {
//        
//    }

//    if (pos.value & position_type::center_y) {
//        
//    }
// ...
    
    move(x, y);
}

void window::maximize () 
{ 
    gtk_window_maximize(GTK_WINDOW(_d->win));
}

void window::minimize () 
{ 
    gtk_window_iconify(GTK_WINDOW(_d->win));
}

inline scene & window::get_scene (void * cr)
{
    _scene._d->context = static_cast<cairo_t *>(cr);
    return _scene;
}

}}