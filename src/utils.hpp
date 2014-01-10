/**
 * @file utils.hpp
 * @author wladt
 * @date Jan 10, 2014
 */

#ifndef __CWT_UTILS_HPP__
#define __CWT_UTILS_HPP__

#include <pfs/string.hpp>

namespace cwt { namespace dom {

void split_namespace (pfs::string & prefix, pfs::string & name, const pfs::string & qName);

}} // cwt::dom

#endif /* __CWT_UTILS_HPP__ */
