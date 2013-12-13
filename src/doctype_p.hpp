/**
 * @file doctype_p.hpp
 * @author wladt
 * @date Dec 10, 2013
 */

#ifndef __CWT_DOM_DOCTYPE_P_HPP__
#define __CWT_DOM_DOCTYPE_P_HPP__

#include "../include/cwt/dom/node.hpp"
#include "../include/cwt/dom/doctype.hpp"
#include "../include/cwt/dom/namednodemap.hpp"

namespace cwt { namespace dom {

class node::impl;
class document::impl;

class document_type::impl : public node::impl
{
public:
    impl (document::impl *, node::impl * parent = nullptr);
    impl (impl* n, bool deep);
    ~impl ();
    void init ();

    // Reimplemented from node::impl
    node::impl * cloneNode (bool deep = true);
    node::impl * insertBefore (node::impl * newChild, node::impl * refChild);
    node::impl * insertAfter (node::impl * newChild, node::impl * refChild);
    node::impl * replaceChild (node::impl * newChild, node::impl * oldChild);
    node::impl * removeChild (node::impl * oldChild);
    node::impl * appendChild (node::impl * newChild);

    node::type nodeType() const { return node::DocumentTypeNode; }

    // Variables
    namednodemap::impl* entities;
    namednodemap::impl* notations;
    pfs::string publicId;
    pfs::string systemId;
    pfs::string internalSubset;
};
}} // cwt::dom

#endif /* __CWT_DOM_DOCTYPE_P_HPP__ */
