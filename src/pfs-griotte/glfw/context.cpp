/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <iostream>

#include "GLFW/glfw3.h"

#include "freetype2/ft2build.h"
#include FT_FREETYPE_H

#include <pfs/safeformat.hpp>
#include <pfs/map.hpp>
#include "pfs/griotte/griotte.hpp"

namespace pfs {
namespace griotte {

context * context::_s_self = 0;

namespace details {

struct context 
{
    FT_Library ft_library; // pointer
};

}

static pfs::map<FT_Error, string> __ft_errors_map;

inline string const & __ft_strerror (FT_Error ft_errno)
{
    static string const __ft_unknown_error("Unknown error");
    
    pfs::map<FT_Error, pfs::string>::const_iterator it = __ft_errors_map.find(ft_errno);
    
    return it == __ft_errors_map.cend()
            ? __ft_unknown_error
            : it->second;
}

context::context ()
    : _d(new details::context)
{
    PFS_ASSERT(_s_self == 0);
    _s_self = this;
}

context::~context ()
{
    glfwTerminate();
    
    if (_d) {
        if (_d->ft_library) {
            FT_Error ft_errno = FT_Done_FreeType(_d->ft_library);

            if (ft_errno != FT_Err_Ok) {
                emit_error(_Sf(GRIOTTE_ERROR_TITLE_FREETYPE ": %s")(__ft_strerror(ft_errno)).str());
                return;
            }

            _d->ft_library = 0;
        }
        
        delete _d;
    }
}

bool context::init ()
{
#   undef __FTERRORS_H__
#   define FT_ERRORDEF(e, v, s) __ft_errors_map.insert2(e, _u8(s));
#   define FT_ERROR_START_LIST __ft_errors_map.insert2(FT_Err_Ok, _u8("No error"));
#   define FT_ERROR_END_LIST
#   include FT_ERRORS_H

    emit_error.connect(this, & context::on_error_default);
    
    _d->ft_library = 0;
    
    if (glfwInit() != GLFW_TRUE) {
        return false;
    }
    
    FT_Error ft_errno = FT_Init_FreeType(& _d->ft_library);

    if (ft_errno != FT_Err_Ok) {
        emit_error(_Sf(GRIOTTE_ERROR_TITLE_FREETYPE ": %s")(__ft_strerror(ft_errno)).str());
        return false;
    }
    
    return true;
}

void context::font_engine_info (stringlist & info)
{
    if (_d && _d->ft_library) {
        info.push_back(_u8("name"));
        info.push_back(_u8("freetype"));

        FT_Int amajor = 0;
        FT_Int aminor = 0;
        FT_Int apatch = 0;
        
        FT_Library_Version(_d->ft_library, & amajor, & aminor, & apatch);
        
        info.push_back(_u8("major-version"));
        info.push_back(to_string(amajor));
        
        info.push_back(_u8("minor-version"));
        info.push_back(to_string(aminor));
        
        info.push_back(_u8("patch-version"));
        info.push_back(to_string(apatch));
    }
}

void context::on_error_default (string const & errstr)
{
    std::cerr << errstr << std::endl;
}

void context::poll_events ()
{
    // Poll for and process events
    glfwPollEvents();
}

}}
