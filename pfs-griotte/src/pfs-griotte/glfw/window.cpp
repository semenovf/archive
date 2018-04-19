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

#include "GLFW/glfw3.h"
#include "pfs/griotte/activity.hpp"

namespace pfs {
namespace griotte {

namespace details {

struct window
{
    GLFWwindow * _native_window;
    
    window (int width, int height, pfs::string const & title, GLFWmonitor * monitor, GLFWwindow * share)
    {
        _native_window = glfwCreateWindow(width, height, title.c_str(), monitor, share);
        
        if (_native_window) {
            // Make the window's context current
            glfwMakeContextCurrent(_native_window);
            glClear(GL_COLOR_BUFFER_BIT);
        }
    }
    
    ~window ()
    {
        if (_native_window)
            glfwDestroyWindow(_native_window);
    }
};

}

activity::window (int width, int height)
    : _d(new details::window(width, height, _u8("Noname"), 0, 0))
{}

activity::window (int width, int height, pfs::string const & title)
    : _d(new details::window(width, height, title, 0, 0))
{}

activity::~window ()
{
    if (_d)
        delete _d;
}

bool activity::should_close ()
{
    if (_d->_native_window)
        return glfwWindowShouldClose(_d->_native_window) != 0;
    
    return true;
}

void activity::set_title (pfs::string const & title)
{
    if (_d->_native_window)
        glfwSetWindowTitle(_d->_native_window, title.c_str());
}

void activity::repaint ()
{
    if (_d->_native_window) {
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(_d->_native_window);
    }
}

void activity::maximize () 
{ 
    if (_d->_native_window) {
        glfwMaximizeWindow(_d->_native_window);
    }
}

void activity::minimize () 
{ 
    if (_d->_native_window) {
        glfwIconifyWindow(_d->_native_window);
    }
}

}}