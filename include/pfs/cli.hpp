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

#include <pfs/string.hpp>
#include <pfs/map.hpp>
#include <pfs/list.hpp>
#include <pfs/unicode.hpp>
#include <pfs/cli/option.hpp>
#include <pfs/cli/route.hpp>

namespace pfs {
namespace cli {

template <typename Key, typename T>
struct map
{
    typedef pfs::map<Key, T> type;
};

template <typename T>
struct list
{
    typedef pfs::list<T> type;
};

typedef details::traits<pfs::string
    , map
    , list> traits;

#if __COMMENT__
template <typename T>
class option : public details::option<T, traits>
{
public:
    typedef details::option<T, traits>          base_type;
    typedef typename base_type::traits_type     traits_type;
    typedef typename base_type::value_type      value_type;
    typedef typename base_type::short_name_type short_name_type;
    typedef typename base_type::long_name_type  long_name_type;

public:
    /**
     * @brief Construct option with only short name ().
     * @param short_name Short name for option.
     */
    option (short_name_type const & short_name)
        : base_type(short_name, traits_type::invalid_long_name())
    {}

    option (char short_name)
        : base_type(short_name_type(short_name), traits_type::invalid_long_name())
    {}

    option (long_name_type const & long_name)
        : base_type(traits_type::invalid_short_name(), long_name)
    {}

    option (char const * long_name)
        : base_type(traits_type::invalid_short_name(), long_name_type(long_name))
    {}

    /**
     * @brief Construct option with only long name.
     * @param long_name Long name for option.
     */
    option (short_name_type const & short_name, long_name_type const & long_name)
        : base_type(short_name, long_name)
    {}

    option (char short_name, char const * long_name)
        : base_type(short_name_type(short_name), long_name_type(long_name))
    {}
};
#endif

typedef details::route<traits> route;

}} // pfs::cli

#endif /* __PFS_CLI_HPP__ */

