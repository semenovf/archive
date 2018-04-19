/*
 * pluggable.hpp
 *
 *  Created on: May 20, 2015
 *      Author: wladt
 */

#ifndef __PFS_PLUGGABLE_HPP__
#define __PFS_PLUGGABLE_HPP__

namespace pfs {

class pluggable
{
public:
    virtual ~pluggable () {}
};

#define PFS_PLUGIN_API extern "C" DLL_API
#define PFS_PLUGIN_CTOR_NAME "__plugin_ctor__"
#define PFS_PLUGIN_DTOR_NAME "__plugin_dtor__"
typedef pluggable * (* plugin_ctor_t)(void);
typedef void  (* plugin_dtor_t)(pluggable *);

} // pfs

#endif /* __PFS_PLUGGABLE_HPP__ */
