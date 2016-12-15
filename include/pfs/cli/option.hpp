/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   option.hpp
 * Author: wladt
 *
 * Created on December 14, 2016, 5:05 PM
 */

#ifndef __PFS_CLI_OPTION_HPP__
#define __PFS_CLI_OPTION_HPP__

namespace pfs {
namespace cli {

namespace details {

struct mapped_type {};

template <typename ShortNameType
    , typename LongNameType
    , typename ShortMapType
    , typename LongMapType>
struct traits
{
    typedef ShortNameType  short_name_type;
    typedef LongNameType   long_name_type;
    typedef ShortMapType   short_map_type;
    typedef LongMapType    long_map_type;
    
    static short_name_type invalid_short_name ()
    {
        return short_name_type();
    }
    
    static long_name_type invalid_long_name ()
    {
        return long_name_type();
    }
};

template <typename T, typename Traits>
class option : public mapped_type
{
protected:
    typedef T                                value_type;
    typedef Traits                           traits_type;
    typedef typename traits_type::short_name_type short_name_type;
    typedef typename traits_type::long_name_type  long_name_type;
    
protected:
    short_name_type _short_name;
    long_name_type  _long_name;
    value_type      _value;
    
public:
    option (short_name_type short_name, long_name_type long_name)
        : _short_name(short_name)
        , _long_name(long_name)
        , _value()
    {}
        
    void set_default_value (value_type const & value)
    {
        _value = value;
    }
    
    short_name_type const & short_name () const
    {
        return _short_name;
    }

    long_name_type const & long_name () const
    {
        return _short_name;
    }
};

} // details

}} // pfs::cli

#endif /* __PFS_CLI_OPTION_HPP__ */

