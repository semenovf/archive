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

struct iterator
{
    typedef pfs::string string_type;
    
    int           _argc;
    char const ** _argv;
    
public:
    iterator (int argc, char const * argv[])
        : _argc(argc)
        , _argv(argv)
    {}
    
    iterator & operator ++ ()
    {
        --_argc;
        ++_argv;
        return *this;
    }
    
    pfs::string operator * ()
    {
        return pfs::string(*_argv);
    }
    
    bool operator == (iterator const & x)
    {
        return _argc == x._argc
                && _argv == x._argv;
    }

    bool operator != (iterator const & x)
    {
        return !this->operator == (x);
    }
};

inline iterator begin (int argc, char const *argv[])
{
    return iterator(argc, argv);
}

inline iterator end (int argc, char const *argv[])
{
    return iterator(0, argv + argc);
}

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

typedef details::traits<iterator
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

//typedef details::route<traits> route;

}} // pfs::cli

#endif /* __PFS_CLI_HPP__ */

