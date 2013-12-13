/**
 * @file domimpl.cpp
 * @author wladt
 * @date Dec 11, 2013
 */

#include "../include/cwt/dom/domimpl.hpp"

namespace cwt { namespace dom {

bool dom_implementation::hasFeature (const pfs::string & feature, const pfs::string & version) const
{
    if (feature == pfs::string("XML")) {
        if (version.isEmpty() || version == pfs::string("1.0")) {
            return true;
        }
    }
    return false;
}


}} // cwt::dom



