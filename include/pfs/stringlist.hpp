/**
 * @file stringlist.hpp
 * @author wladt
 * @date Apr 3, 2014
 */

#ifndef __PFS_STRINGLIST_HPP__
#define __PFS_STRINGLIST_HPP__

#include "bits/stringlist_basic.hpp"
#include "utf8string.hpp"

namespace pfs {

typedef stringlist_basic<utf8string> stringlist;

} // pfs

#endif // __PFS_STRINGLIST_HPP__
