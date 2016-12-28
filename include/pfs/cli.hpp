/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   cli.hpp
 * Author: wladt
 *
 * Created on December 14, 2016, 5:04 PM
 */

#ifndef __PFS_CLI_HPP__
#define __PFS_CLI_HPP__

//#include <pfs/string.hpp>
//#include <pfs/map.hpp>
//#include <pfs/list.hpp>
//#include <pfs/unicode.hpp>
//#include <pfs/cli/option.hpp>
//#include <pfs/cli/route.hpp>

namespace pfs {
namespace cli {

#if __COMMENT__

#if __cplusplus >= 201103

template <typename Key, T>
using map = pfs::map<Key, T>;

template <typename T>
using list = pfs::list<T>;

typedef details::traits<
      pfs::string
    , map
    , list> traits;

#else // __cplusplus < 201103

template <typename Key, typename T>
struct map_wrapper
{
    typedef pfs::map<Key,T> type;
};

template <typename T>
struct list_wrapper
{
    typedef pfs::list<T> type;
};

typedef details::traits<
      pfs::string
    , map_wrapper
    , list_wrapper> traits;

#endif // __cplusplus >= 201103

//typedef details::tuple<traits>    tuple;
typedef details::iterator<traits> iterator;
//typedef details::route<traits>    route;

#endif

}} // pfs::cli

#endif /* __PFS_CLI_HPP__ */

