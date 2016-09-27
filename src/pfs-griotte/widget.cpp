/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "pfs/griotte/widget.hpp"
#include "pfs/griotte/layout.hpp"

namespace pfs {
namespace griotte {

widget::widget () 
    : _parent(0)
    , _layout(0) 
{}

widget::~widget ()
{
    widget_iterator it = _widgets.begin();
    widget_iterator it_end = _widgets.end();
    
    for (; it != it_end; ++it) {
        delete *it;
    }
}

void widget::add_widget (widget * w)
{
    w->_parent = this;
    _widgets.push_back(w);
}

void widget::set_layout (layout * l)
{
    if (_layout) {
        delete _layout;
        _layout = 0;
    }
    
    l->_parent = this;
    _layout = l;
}

}}