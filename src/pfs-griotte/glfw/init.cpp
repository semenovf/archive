/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "GLFW/glfw3.h"
#include "pfs/griotte/griotte.hpp"

namespace pfs {
namespace griotte {

bool init ()
{
    return glfwInit() == GLFW_TRUE;
}

void finish ()
{
    glfwTerminate();
}

void poll_events ()
{
    // Poll for and process events
    glfwPollEvents();
}

}}


