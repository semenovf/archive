/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "pfs/griotte/scene.hpp"
#include "pfs/griotte/widget.hpp"
#include "pfs/griotte/layout.hpp"

namespace pfs {
namespace griotte {

widget::widget () 
    : _pnode(0)
//    , _layout(0) 
{}

widget::~widget ()
{}

//void widget::set_layout (layout * l)
//{
//    if (_layout) {
//        delete _layout;
//        _layout = 0;
//    }
//    
//    l->_parent = this;
//    _layout = l;
//}

}}