/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   traits.hpp
 * Author: wladt
 *
 * Created on January 23, 2017, 3:27 PM
 */

#ifndef __PFS_MPL_CLI_TRAITS_HPP__
#define __PFS_MPL_CLI_TRAITS_HPP__

#include <pfs/mpl/list.hpp>
#include <pfs/mpl/multimap.hpp>
#include <pfs/mpl/string.hpp>

namespace pfs {
namespace mpl {
namespace cli {

template <typename StringT
    , template <typename> class ListT
    , template <typename, typename> class MultiMapT>
struct traits
{
    typedef pfs::mpl::string<StringT>          string_type;
    typedef typename string_type::value_type   char_type;
    typedef pfs::mpl::list<string_type, ListT> domain_list_type;
};

}}} // pfs::mpl::cli

#endif /* __PFS_MPL_CLI_TRAITS_HPP__ */

