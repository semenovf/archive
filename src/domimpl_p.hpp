/**
 * @file domimpl_p.hpp
 * @author wladt
 * @date Dec 13, 2013
 */

#ifndef __CWT_DOM_DOMIMPL_P_HPP__
#define __CWT_DOM_DOMIMPL_P_HPP__

#include "../include/cwt/dom/domimpl.hpp"
#include <pfs/atomic.hpp>

namespace cwt { namespace dom {

class dom_implementation::impl
{
public:
    impl () { ; }

    impl * clone();
    pfs::atomic_integer ref;
    //static QDomImplementation::InvalidDataPolicy invalidDataPolicy;
};


}} // cwt::dom

#endif /* __CWT_DOM_DOMIMPL_P_HPP__ */
