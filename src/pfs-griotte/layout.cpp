/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "pfs/griotte/container.hpp"
#include "pfs/griotte/layout.hpp"

namespace pfs {
namespace griotte {

layout::layout (container & cont)
{
    cont.set(this);
}

}}