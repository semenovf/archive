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
#include "griotte_p.hpp"

namespace pfs {
namespace griotte {

window::window ()
    : _d(new details::window)
{
    application & app = application::instance();
    _d->win = gtk_application_window_new(app._d->app);
}

window::~window ()
{
    if (_d)
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

void window::maximize () 
{ 
    gtk_window_maximize(GTK_WINDOW(_d->win));
}

void window::minimize () 
{ 
    gtk_window_iconify(GTK_WINDOW(_d->win));
}

}}