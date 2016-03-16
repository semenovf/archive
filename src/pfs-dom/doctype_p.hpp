/**
 * @file doctype_p.hpp
 * @author wladt
 * @date Dec 10, 2013
 */

#ifndef __PFS_DOM_DOCTYPE_P_HPP__
#define __PFS_DOM_DOCTYPE_P_HPP__


#include "pfs/dom/doctype.hpp"

namespace pfs { namespace dom {

class node_impl;
class document_impl;
class namednodemap_impl;

class DLL_API doctype_impl : public node_impl
{
public:
    namednodemap_impl * _entities;
    namednodemap_impl * _notations;
    pfs::string _publicId;
    pfs::string _systemId;
    pfs::string _internalSubset;

public:
    doctype_impl (document_impl *, node_impl * parent = 0);
    doctype_impl (doctype_impl * other, bool deep);
    ~doctype_impl ();
    void init ();

    virtual node_impl * insertBefore (node_impl * newChild, node_impl * refChild);
    virtual node_impl * insertAfter  (node_impl * newChild, node_impl * refChild);
    virtual node_impl * replaceChild (node_impl * newChild, node_impl * oldChild);
    virtual node_impl * removeChild  (node_impl * oldChild);
    virtual node_impl * appendChild  (node_impl * newChild);

    virtual node_impl * cloneNode (bool deep = true);
    virtual node::type nodeType() const { return node::DocumentTypeNode; }
};

}} // pfs::dom

#endif /* __PFS_DOM_DOCTYPE_P_HPP__ */
