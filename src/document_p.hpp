/**
 * @file document_p.hpp
 * @author wladt
 * @date Dec 10, 2013
 */

#ifndef __CWT_DOM_DOCUMENT_P_HPP__
#define __CWT_DOM_DOCUMENT_P_HPP__

#include "../include/cwt/dom/document.hpp"
#include <pfs/shared_ptr.hpp>

namespace cwt { namespace dom {

class attr_impl;
class comment_impl;
class node_impl;
class text_impl;
class element_impl;
class entityref_impl;
class doctype_impl;
class document_fragment_impl;
class pinstruction_impl;
class domimpl_impl;
class cdatasection_impl;
class dom_implementation_impl;

class DLL_API document_impl : public node_impl
{
public:
    pfs::shared_ptr<dom_implementation_impl> _impl;
    pfs::shared_ptr<doctype_impl> _type;

    long _nodeListTime;

public:
    document_impl ();
    document_impl (const pfs::string & name);
    document_impl (doctype_impl * dt);
    document_impl (document_impl * n, bool deep);
    ~document_impl ();

    document createDocument () {
    	document doc(this);
    	this->ref.deref();
    	return doc;
    }

    doctype_impl * doctype () { return _type.get(); }
    dom_implementation_impl * implementation () { return _impl.get(); }
    element_impl * documentElement ();

    element_impl * createElement (const pfs::string & tagName);
    element_impl * createElementNS (const pfs::string & nsURI, const pfs::string & qName);
    document_fragment_impl * createDocumentFragment ();
    text_impl * createTextNode (const pfs::string & data);
    comment_impl * createComment (const pfs::string & data);
    cdatasection_impl * createCDATASection (const pfs::string & data);
    pinstruction_impl * createProcessingInstruction (const pfs::string & target, const pfs::string & data);
    attr_impl * createAttribute (const pfs::string & name);
    attr_impl * createAttributeNS (const pfs::string & nsURI, const pfs::string & qName);
    entityref_impl * createEntityReference (const pfs::string & name);
    node_impl * importNode (const node_impl * importedNode, bool deep);
//
//    void clear();

    virtual node_impl * cloneNode (bool deep = true);
    virtual node::type nodeType () const { return node::DocumentNode; }
};

}} // cwt

#endif /* __CWT_DOM_DOCUMENT_P_HPP__ */
