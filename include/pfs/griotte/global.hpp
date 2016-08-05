/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   global.hpp
 * Author: wladt
 *
 * Created on August 5, 2016, 10:50 AM
 */

#ifndef __PFS_GRIOTTE_GLOBAL_HPP__
#define __PFS_GRIOTTE_GLOBAL_HPP__

#ifndef PFS_DECLARE_NONCOPYABLE
#   define PFS_DECLARE_NONCOPYABLE(class_name)                                 \
        private:                                                               \
            class_name (class_name const & other);                             \
            class_name & operator = (class_name const & other);
#endif

#define GRIOTTE_ERROR_TITLE_FREETYPE "Freetype"

namespace pfs {
namespace griotte {


}}

#endif /* __PFS_GRIOTTE_GLOBAL_HPP__ */

