/**
 * @file entityref_p.hpp
 * @author wladt
 * @date Dec 13, 2013
 */

#ifndef __CWT_DOM_ENTITYREF_P_HPP__
#define __CWT_DOM_ENTITYREF_P_HPP__

#include "../include/cwt/dom/entityref.hpp"

namespace cwt { namespace dom {

class node_impl;
class document_impl;

class entityref_impl : public node_impl
{
public:
    entityref_impl (document_impl *, node_impl * parent, const pfs::string & name);
    entityref_impl (node_impl * n, bool deep);

    virtual node_impl * cloneNode (bool deep = true);
    virtual node::type nodeType () const { return node::EntityReferenceNode; }
};

}} // cwt::dom

#endif /* __CWT_DOM_ENTITYREF_P_HPP__ */
