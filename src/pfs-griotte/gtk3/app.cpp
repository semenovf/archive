/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <iostream>
#include <gtk/gtk.h>
#include "pfs/griotte/griotte.hpp"

namespace pfs {
namespace griotte {

app * app::_s_self = 0;

namespace details {

struct app 
{
    GtkApplication * a;
};

}

app::app ()
    : _d(new details::app)
{
    PFS_ASSERT(_s_self == 0);
    _s_self = this;
}

app::~app ()
{
    if (_d) {
        g_object_unref(_d->app);
        delete _d;
    }
}

bool app::ready ()
{
    emit_error.connect(this, & app::on_error_default);

    _d->a = gtk_application_new("griotte-app", G_APPLICATION_FLAGS_NONE);
    
    if (! _d->a)
        return false;
    
    return true;
}

void app::on_error_default (string const & errstr)
{
    std::cerr << errstr << std::endl;
}

}}
