/**
 * @file domimpl.cpp
 * @author wladt
 * @date Dec 11, 2013
 */

#include "domimpl_p.hpp"

namespace cwt { namespace dom {

bool dom_implementation::hasFeature(const pfs::string & feature, const pfs::string & version) const
{
    if (feature == pfs::string("XML")) {
        if (version.isEmpty() || version == pfs::string("1.0")) {
            return true;
        }
    }
    // ### add DOM level 2 features
    return false;
}


}} // cwt::dom



