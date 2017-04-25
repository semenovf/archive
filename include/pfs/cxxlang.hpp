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

#ifdef pfs_override
#   undef pfs_override
#endif

#if __cplusplus >= 201103L
#   define pfs_constexpr           constexpr
#   define pfs_noexcept            noexcept
#   define pfs_noexcept_expr(expr) noexcept(expr)
#   define pfs_override            override
#else
#   define pfs_constexpr
#   define pfs_noexcept
#   define pfs_noexcept_expr(expr)
#   define pfs_override
#endif

//
// https://infektor.net/posts/2017-01-19-using-cpp17-attributes-today.html
// http://clang.llvm.org/docs/LanguageExtensions.html#has-cpp-attribute
// 
// Bellow are C++17 three additional attributes were added to the standard which 
// enable additional compiler warnings when used in general code.
// 
// nodiscard 
//          specifies that a return value should not be discarded
// fallthrough 
//          indicates to the compiler that you intended a case in a switch 
//          statement to fall-through, requires an optional warning to be enabled
// maybe_unused 
//          silences warnings generated when a variable is only used in an assertion
//
#ifndef __has_cpp_attribute
#   define __has_cpp_attribute(name) 0
#endif

//
// nodiscard
//
#if __has_cpp_attribute(nodiscard)
#   define pfs_nodiscard [[nodiscard]]
#elif __has_cpp_attribute(gnu::warn_unused_result)
#   define pfs_nodiscard [[gnu::warn_unused_result]]
#else
#   define pfs_nodiscard
#endif

//
// fallthrough
//
#if __has_cpp_attribute(fallthrough)
#   define pfs_fallthrough [[fallthrough]]
#elif __has_cpp_attribute(clang::fallthrough)
#   define pfs_fallthrough [[clang::fallthrough]]
#else
#   define pfs_fallthrough
#endif

//
// maybe_unused
//
#if __has_cpp_attribute(maybe_unused)
#   define pfs_maybe_unused [[maybe_unused]]
#elif __has_cpp_attribute(gnu::unused)
#   define pfs_maybe_unused [[gnu::unused]]
#else
#   define pfs_maybe_unused
#endif

#endif /* __PFS_CXXLANG_HPP__ */

