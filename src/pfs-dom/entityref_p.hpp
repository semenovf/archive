/**
 * @file entityref_p.hpp
 * @author wladt
 * @date Dec 13, 2013
 */

#ifndef __PFS_DOM_ENTITYREF_P_HPP__
#define __PFS_DOM_ENTITYREF_P_HPP__

#include "pfs/dom/entityref.hpp"

namespace pfs { namespace dom {

class node_impl;
class document_impl;

class DLL_API entityref_impl : public node_impl
{
public:
    entityref_impl (document_impl *, node_impl * parent, const pfs::string & name);
    entityref_impl (node_impl * n, bool deep);

    virtual node_impl * cloneNode (bool deep = true);
    virtual node::type nodeType () const { return node::EntityReferenceNode; }
};

}} // pfs::dom

#endif /* __PFS_DOM_ENTITYREF_P_HPP__ */
