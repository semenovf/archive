/**
 * @file docfragment_p.hpp
 * @author wladt
 * @date Dec 13, 2013
 */

#ifndef __CWT_DOM_DOCFRAGMENT_P_HPP__
#define __CWT_DOM_DOCFRAGMENT_P_HPP__

#include "../include/cwt/dom/docfragment.hpp"

namespace cwt { namespace dom {

class node::impl;
class document::impl;

class document_fragment::impl : public node::impl
{
public:
    impl (document::impl *, node::impl* parent = 0);
    impl (node::impl* n, bool deep);

    // Reimplemented from node::impl
    virtual node::impl* cloneNode(bool deep = true);
    node::type nodeType() const { return node::DocumentFragmentNode; }
};

}} // cwt::dom

#endif /* __CWT_DOM_DOCFRAGMENT_P_HPP__ */
