/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <QPainter>
#include "pfs/griotte/painter.hpp"

namespace pfs {
namespace griotte {

namespace details {

struct painter
{
    //QMainWindow win;
};

}

painter::painter ()
    : _d(new details::painter) 
{}

painter::~painter ()
{
    delete _d;
}

//painter::painter ()
//    : _d(new details::painter)
//{}
//
//painter::~painter ()
//{
//    delete _d;
//}

}}
