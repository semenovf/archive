/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <pfs.hpp>
#include "pfs/griotte/container.hpp"

namespace pfs {
namespace griotte {

void container::add (widget * w)
{
    PFS_ASSERT_X(w->_parent == 0, "Widget has parent already");
    _widgets.push_back(w);
}

void container::set (layout *)
{
    
}

}}