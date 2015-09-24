/**
 * @file utils.hpp
 * @author wladt
 * @date Jan 10, 2014
 */

#ifndef __PFS_UTILS_HPP__
#define __PFS_UTILS_HPP__

#include <pfs/string.hpp>

namespace pfs { namespace dom {

void split_namespace (pfs::string & prefix, pfs::string & name, const pfs::string & qName);

}} // pfs::dom

#endif /* __PFS_UTILS_HPP__ */
