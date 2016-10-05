/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   window.cpp
 * Author: wladt
 * 
 * Created on October 5, 2016
 */

#include <QApplication>
#include <QDesktopWidget>
#include <QMainWindow>

#include "pfs/griotte/griotte.hpp"
#include "pfs/griotte/window.hpp"

namespace pfs {
namespace griotte {

namespace details {

struct window
{
    QMainWindow win;
};

}

window::window ()
    : _d(new details::window) 
{}

window::~window ()
{
    delete _d;
}

void window::show ()
{
    _d->win.show();
}

void window::set_title (pfs::string const & title)
{
    _d->win.setWindowTitle(QString::fromUtf8(title.c_str()));
}

void window::resize (int width, int height)
{
    _d->win.resize(width, height);
}

void window::move (int x, int y)
{
    _d->win.move(x, y);
}

void window::move (position_type const & pos)
{
    int   x = _d->win.x()
        , y = _d->win.y()
        , w = _d->win.width()
        , h = _d->win.height();
    
    QRect screenGeometry = QApplication::desktop()->screenGeometry();
    
    if (pos.value() & position_type::center_xy) {
        x = (screenGeometry.width() - w) / 2;
        y = (screenGeometry.height() - h) / 2;
        move(x, y);
        return;
    }
    
    if (pos.value() & position_type::center_x) {
        x = (screenGeometry.width() - w) / 2;
    }
    
    if (pos.value() & position_type::center_y) {
        y = (screenGeometry.height() - h) / 2;        
    }

    if (pos.value() & position_type::top) {
        y = 0;
    }

    if (pos.value() & position_type::bottom) {
        y = screenGeometry.height() - h;
    }
    
    if (pos.value() & position_type::left) {
        x = 0;
    }

    if (pos.value() & position_type::right) {
        x = screenGeometry.width() - w;
    }
    
    move(x, y);
}

void window::maximize () 
{ 
    _d->win.showMaximized();
}

void window::minimize () 
{ 
    _d->win.showMinimized();
}

}}