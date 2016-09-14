/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <pfs.hpp>
#include "pfs/griotte/container.hpp"
#include <pfs/griotte/layout.hpp>

namespace pfs {
namespace griotte {

container::~container ()
{
    widget_iterator it = _widgets.begin();
    widget_iterator it_end = _widgets.end();
    
    for (; it != it_end; ++it) {
        delete *it;
    }
}

void container::add_widget (widget * w)
{
    w->_parent = this;
    _widgets.push_back(w);
}

void container::set_layout (layout * l)
{
    if (_layout) {
        delete _layout;
        _layout = 0;
    }
    
    l->_parent = this;
    _layout = l;
}

}}