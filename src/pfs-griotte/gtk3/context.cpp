/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

//#include <pfs/safeformat.hpp>
//#include <pfs/map.hpp>
#include <iostream>
#include "pfs/griotte/griotte.hpp"

namespace pfs {
namespace griotte {

context * context::_s_self = 0;

namespace details {

struct context 
{
};

}

context::context ()
    : _d(new details::context)
{
    PFS_ASSERT(_s_self == 0);
    _s_self = this;
}

context::~context ()
{
//    glfwTerminate();
    if (_d) {
        delete _d;
    }
}

bool context::init ()
{
    emit_error.connect(this, & context::on_error_default);
    
//    if (glfwInit() != GLFW_TRUE) {
//        return false;
//    }
    
    return true;
}

void context::on_error_default (string const & errstr)
{
    std::cerr << errstr << std::endl;
}

void context::poll_events ()
{
    // Poll for and process events
    //glfwPollEvents();
}

}}
