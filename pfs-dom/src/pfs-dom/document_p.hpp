/**
 * @file document_p.hpp
 * @author wladt
 * @date Dec 10, 2013
 */

#ifndef __PFS_DOM_DOCUMENT_P_HPP__
#define __PFS_DOM_DOCUMENT_P_HPP__

#include <pfs/shared_ptr.hpp>
#include "pfs/dom/document.hpp"

namespace pfs { namespace dom {

class attr_impl;
class comment_impl;
class node_impl;
class text_impl;
class element_impl;
class entityref_impl;
class doctype_impl;
class document_fragment_impl;
class processing_instruction_impl;
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

    document create_document ()
    {
    	document doc(this);
    	this->ref.deref();
    	return doc;
    }

    doctype_impl * doctype ()
    {
    	return _type.get();
    }

    const doctype_impl * doctype () const
    {
    	return _type.get();
    }
    dom_implementation_impl * implementation () { return _impl.get(); }
    element_impl * document_element ();

    element_impl * create_element (const pfs::string & tagname);
    element_impl * create_element_ns (const pfs::string & ns_uri, const pfs::string & qname);
    document_fragment_impl * create_document_fragment ();
    text_impl * create_text_node (const pfs::string & data);
    comment_impl *           create_comment (const pfs::string & data);
    cdatasection_impl *      create_cdata_section (const pfs::string & data);
    processing_instruction_impl *      create_processing_instruction (const pfs::string & target, const pfs::string & data);
    attr_impl *              create_attribute (const pfs::string & name);
    attr_impl *              create_attribute_ns (const pfs::string & ns_uri, const pfs::string & qname);
    entityref_impl *         create_entity_reference (const pfs::string & name);
    node_impl *              import_node (const node_impl * importedNode, bool deep);
//
//    void clear();

    virtual node_impl * clone_node (bool deep = true);

    virtual node::type node_type () const
    {
    	return node::document_node;
    }
};

}} // pfs

#endif /* __PFS_DOM_DOCUMENT_P_HPP__ */
