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

#ifndef __PFS_MPL_FS_TRAITS_HPP__
#define __PFS_MPL_FS_TRAITS_HPP__

#include <pfs/mpl/list.hpp>
#include <pfs/mpl/string.hpp>
#include <pfs/mpl/string_builder.hpp>

namespace pfs {
namespace mpl {
namespace fs {

template <typename StringT
    , template <typename> class ListT>
struct traits
{
    typedef pfs::mpl::string<StringT>           string_type;
    typedef typename string_type::value_type    char_type;
    typedef pfs::mpl::list<string_type, ListT>  stringlist_type;
    typedef pfs::mpl::string_builder<char_type> string_builder_type;
    
    template <typename T>
    struct list
    {
        typedef pfs::mpl::list<T, ListT> type;
    };
    
    static string_type separator ()
    {
        return string_type("/");
    }
    
    class filesystem_error
    {
        string_type _msg;

    public:
        explicit filesystem_error (string_type const & arg);

        string_type const & what () const pfs_noexcept
        {
            return _msg;
        }
    };
};

}}} // pfs::mpl::cli

#endif /* __PFS_MPL_FS_TRAITS_HPP__ */

