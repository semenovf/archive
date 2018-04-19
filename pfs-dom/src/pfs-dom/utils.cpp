/**
 * @file utils.cpp
 * @author wladt
 * @date Jan 10, 2014
 */

#include "utils.hpp"

namespace pfs { namespace dom {

void split_namespace (pfs::string & prefix, pfs::string & name, const pfs::string & qName)
{
	pfs::string::const_iterator it = qName.find(pfs::string(1, ':'));

    if (it == qName.cend()) {
        prefix.clear();
        name = qName;
    } else {
        prefix = qName.substr(qName.cbegin(), it);
        name = qName.substr(it + 1);
    }
}

}} // pfs::dom


