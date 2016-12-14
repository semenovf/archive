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

namespace pfs {
namespace cli {

enum {
      combine_short_options = 0x01
    , single_dash_long_option = 0x02
};

template <typename T>
class option;

class route 
{
public:
    route ();
    
    template <typename T>
    void add (option<T> const & opt);
};

}} // pfs::cli

#endif /* __PFS_CLI_ROUTE_HPP__ */

