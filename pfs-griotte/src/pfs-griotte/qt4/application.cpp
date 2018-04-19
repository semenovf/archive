/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <QApplication>
#include <QTextCodec>

#include "pfs/griotte/window.hpp"
#include "pfs/griotte/application.hpp"

namespace pfs {
namespace griotte {

application * application::_s_self = 0;

namespace details {

struct application 
{
    int argc;
    QApplication app;
    
    application ()
        : argc(0)
        , app(argc, 0, true)
    {}
};

}

application::application ()
    : _d(new details::application)
{
    PFS_ASSERT(_s_self == 0);
    _s_self = this;
    
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
}

application::~application ()
{
    delete _d;
}

int application::run ()
{
    _d->app.setQuitOnLastWindowClosed(true);
    return _d->app.exec();
}

}}
