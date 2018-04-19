/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "pfs/griotte/painter.hpp"
#include "internals.hpp"

namespace pfs {
namespace griotte {

painter::painter ()
    : _d(new details::painter)
{}

painter::~painter ()
{
    delete _d;
}

}}