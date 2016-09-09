/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <iostream>
#include "pfs/griotte/griotte.hpp"
#include "pfs/griotte/window.hpp"
#include "griotte_p.hpp"

namespace pfs {
namespace griotte {

application * application::_s_self = 0;

static void __activate (GtkApplication * native_app, gpointer app_ptr)
{
    PFS_UNUSED(native_app);
    
    application * app = static_cast<application *>(app_ptr);
    window main_win;
    app->activated(*app, main_win);
}

application::application (string const & application_id)
    : _d(new details::application)
{
    PFS_ASSERT(_s_self == 0);
    _s_self = this;
    _d->app = 0;
    
    string app_id;
    
    if (application_id.empty()) {
        app_id = _u8("pfs.griotte.app");
    } else {
        app_id = application_id;
    }
    
    emit_error.connect(this, & application::on_error_default);
    
    if (! g_application_id_is_valid(app_id.c_str())) {
        emit_error(_u8("invalid application id"));
        return;
    }

    _d->app = gtk_application_new(app_id.c_str(), G_APPLICATION_FLAGS_NONE);
    
    if (_d->app) {
        g_signal_connect(_d->app, "activate", G_CALLBACK(__activate), this);
    }
}

application::~application ()
{
    if (_d) {
        if (_d->app) {
            g_object_unref(_d->app);
        }
        
        delete _d;
    }
}

int application::run ()
{
    if (_d->app) {
        int argc = 0;
        char ** argv = 0;
        return g_application_run(G_APPLICATION(_d->app), argc, argv);
    }
    
    return -1;
}

void application::on_error_default (string const & errstr)
{
    std::cerr << "Error: " << errstr << std::endl;
}

}}
