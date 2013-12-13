/**
 * @file entityref_p.hpp
 * @author wladt
 * @date Dec 13, 2013
 */

#ifndef __CWT_DOM_ENTITYREF_P_HPP__
#define __CWT_DOM_ENTITYREF_P_HPP__

#include "../include/cwt/dom/docfragment.hpp"

namespace cwt { namespace dom {

class entity_reference::impl : public node::impl
{
public:
    entity_reference::impl(document::impl*, node::impl* parent, const pfs::string& name);
    entity_reference::impl(node::impl* n, bool deep);

    // Reimplemented from node::impl
    node::impl* cloneNode(bool deep = true);
    node::type nodeType() const { return node::EntityReferenceNode; }
};

}} // cwt::dom

#endif /* __CWT_DOM_ENTITYREF_P_HPP__ */
