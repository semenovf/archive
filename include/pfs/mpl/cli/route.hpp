/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   route.hpp
 * Author: wladt
 *
 * Created on December 14, 2016, 5:05 PM
 */

#ifndef __PFS_CLI_ROUTE_HPP__
#define __PFS_CLI_ROUTE_HPP__

#include <pfs/mpl/list.hpp>
#include <pfs/mpl/multimap.hpp>
#include <pfs/mpl/string.hpp>

namespace pfs {
namespace mpl {
namespace cli {

// Parse flags
//
enum {
      shortopt_stacked      = 0x001 //!< Combine short options (stacked options) using form -abc (-a -b -c)
    , shortoptarg_separated = 0x002 //!< Allow short option with argument in form {- | /}o ARG
    , shortoptarg_delim     = 0x004 //!< Allow short option with argument in form {- | /}o{'=' | ':'}ARG
    , shortoptarg_merged    = 0x008 //!< Allow short option with argument in form {- | /}oARG
    , longoptarg_separated  = 0x010 //!< Allow long option with argument in form {-- | /}option ARG
    , longoptarg_delim      = 0x020 //!< Allow long option with argument in form {-- | /}option{'=' | ':'}ARG
};

static int const strict_flags  = shortoptarg_delim;
static int const relaxed_flags = shortopt_stacked 
        | shortoptarg_separated 
        | shortoptarg_delim
        | shortoptarg_merged
        | longoptarg_separated
        | longoptarg_delim;

template <typename StringT
    , typename <typename> class ListT
    , typename <typename, typename> class MultiMapT>
class route 
{
public:
//    typedef Traits                                   traits_type;
//
    typedef pfs::mpl::string<StringT> string_type;
//    typedef pfs::mpl::multimap<typename string_type::value_type, ListT> shortoptmap_type;
//    typedef pfs::mpl::multimap<typename string_type::value_type, ListT> shortoptmap_type;

//    typedef typename traits_type::longoptmap_traits  longoptmap_traits;
//    typedef typename traits_type::optlist_traits     optlist_traits;
//
//    typedef typename traits_type::option_type        value_type;
//    typedef typename traits_type::string_type        string_type;
//    typedef typename traits_type::char_type          char_type;
//    typedef typename traits_type::shortoptmap_type   shortoptmap_type;
//    typedef typename traits_type::longoptmap_type    longoptmap_type;
//    typedef typename traits_type::optlist_type       optlist_type;
    
    
protected:
    optlist_type     _optlist;
    shortoptmap_type _short_options;
    longoptmap_type  _long_options;
    
public:
    route () 
    {}
    
    ~route ();
    
    template <typename T>
    void add (T * pvalue
            , char_type short_name
            , string_type const & long_name
            , bool required
            , string_type const & description = string_type());

    template <typename T>
    void add (T * pvalue
            , char_type short_name
            , bool required
            , string_type const & description = string_type())
    {
        this->add(pvalue, short_name, string_type(), required, description);
    }

    template <typename T>
    void add (T * pvalue
            , string_type const & long_name
            , bool required
            , string_type const & description = string_type())
    {
        this->add(pvalue, char_type(0), long_name, required, description);
    }
    
    bool parse (int argc, char const ** argv, int flags = relaxed_flags);
};

template <typename Traits>
route<Traits>::~route ()
{
    typename optlist_traits::iterator it = optlist_traits::begin(_optlist);
    typename optlist_traits::iterator end = optlist_traits::end(_optlist);
    
    while (it != end) {
        delete *it;
        ++it;
    }
}

template <typename Traits>
template <typename T>
void route<Traits>::add (T * pvalue
            , char_type short_name
            , string_type const & long_name
            , bool required
            , string_type const & description)
{
    typedef pfs::cli::option<T, Traits> option_type;
    option_type * o = new option_type(pvalue, short_name, long_name, required, description);
    
    optlist_traits::push_back(_optlist, o);
            
    if (o->short_name() != char_type(0))
        shortoptmap_traits::insert(_short_options, o->short_name(), o);

    if (! string_traits::empty(o->long_name()))
        longoptmap_traits::insert(_long_options, o->long_name(), o);
}

template <typename Traits>
bool route<Traits>::parse (int argc, char const ** argv, int flags)
{
    typedef tuple<traits_type> tuple_type;
    typedef iterator<traits_type> iterator;
    
    iterator it  = begin<traits_type>(argc, argv);
    iterator it_end = end<traits_type>(argc, argv);
    
    while (it != it_end) {
        tuple_type t = it.split();
        
        
        ++it;
    }

    return false;
}

}}} // pfs::mpl::cli

#endif /* __PFS_CLI_ROUTE_HPP__ */

