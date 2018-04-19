/*
 * typeinfo.hpp
 *
 *  Created on: Mar 24, 2015
 *      Author: wladt
 */

#ifndef __PFS_TYPEINFO_HPP__
#define __PFS_TYPEINFO_HPP__

#include <pfs.hpp>
#include <typeinfo>

namespace pfs {

template <typename T> const char * type_name () { return typeid(T).name(); }

template <> const char * type_name<char> () { return "char"; }
template <> const char * type_name<signed char> () { return "signed char"; }
template <> const char * type_name<unsigned char> () { return "unsigned char"; }
template <> const char * type_name<signed short int> () { return "signed short int"; }
template <> const char * type_name<unsigned short int> () { return "unsigned short int"; }
template <> const char * type_name<signed int> () { return "signed int"; }
template <> const char * type_name<unsigned int> () { return "unsigned int"; }
template <> const char * type_name<signed long int> () { return "signed long int"; }
template <> const char * type_name<unsigned long int> () { return "unsigned long int"; }
#ifdef PFS_HAVE_LONGLONG
template <> const char * type_name<signed long long int> () { return "signed long long int"; }
template <> const char * type_name<unsigned long long int> () { return "unsigned long long int"; }
#endif

template <> const char * type_name<float> () { return "float"; }
template <> const char * type_name<double> () { return "double"; }
#ifdef PFS_HAVE_LONG_DOUBLE
template <> const char * type_name<long double> () { return "long double"; }
#endif

} // pfs

#endif /* __PFS_TYPEINFO_HPP__ */
