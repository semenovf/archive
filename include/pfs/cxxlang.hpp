/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   cxxlang.hpp
 * Author: wladt
 *
 * Created on January 18, 2017, 12:04 PM
 */

#ifndef __PFS_CXXLANG_HPP__
#define __PFS_CXXLANG_HPP__

#ifdef pfs_constexpr
#   undef pfs_constexpr
#endif

#ifdef pfs_noexcept
#   undef pfs_noexcept
#endif

#if __cplusplus >= 201103
#   define pfs_constexpr           constexpr
#   define pfs_noexcept            noexcept
#   define pfs_noexcept_expr(expr) noexcept(expr)
#else
#   define pfs_constexpr
#   define pfs_noexcept
#   define pfs_noexcept_expr(expr)
#endif

#endif /* __PFS_CXXLANG_HPP__ */

