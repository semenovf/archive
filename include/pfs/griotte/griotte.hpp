/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   griotte.hpp
 * Author: wladt
 *
 * Created on August 2, 2016, 11:57 AM
 */

#ifndef __PFS_GRIOTTE_HPP__
#define __PFS_GRIOTTE_HPP__

#include <pfs.hpp>
#include <pfs/griotte/window.hpp>

namespace pfs {
namespace griotte {

DLL_API bool init ();
DLL_API void finish ();
DLL_API void poll_events ();

}}

#endif /* __PFS_GRIOTTE_HPP__ */

