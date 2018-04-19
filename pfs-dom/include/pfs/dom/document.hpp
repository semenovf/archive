#ifndef __PFS_DOM_DOCUMENT_HPP__
#define __PFS_DOM_DOCUMENT_HPP__

#include <pfs/string.hpp>

namespace pfs { namespace dom {

class document_impl;
class node;
class dom_implementation;
class doctype;
class document_fragment;
class element;
class text;
class comment;
class cdatasection;
class processing_instruction;
class attr;
class entityref;
class nodelist;

class DLL_API document : public node
{
	friend class node;
	friend class document_impl;

protected:
	document (document_impl *);

public:
	document () : node() {}
	explicit document (const pfs::string & name);
	explicit document (const doctype & dt);
	document (const document & other);
	document & operator = (const document & other);
	~document () {}

	pfs::dom::doctype doctype () const;
	dom_implementation implementation () const;
	element document_element () const;

	element create_element (const pfs::string & tagname);
	element create_element_ns (const pfs::string & namespace_uri, const pfs::string & qualified_name);
	document_fragment create_document_fragment ();
	text create_text_node (const pfs::string & data);
	comment create_comment (const pfs::string & data);
	cdatasection create_cdata_section (const pfs::string & data);
	processing_instruction create_processing_instruction (const pfs::string & target, const pfs::string & data);
	attr create_attribute (const pfs::string & name);
	attr create_attribute_ns (const pfs::string & namespace_uri, const pfs::string & qualified_name);
	entityref create_entity_reference (const pfs::string & name);

	node import_node (const node & importedNode, bool deep);

	nodelist get_elements_by_tagname (const pfs::string & tagname) const;

	nodelist elements_by_tagname (const pfs::string & tagname) const
	{
		return get_elements_by_tagname(tagname);
	}

	nodelist get_elements_by_tagname_ns (const pfs::string & namespace_uri, const pfs::string & localname) const;
	nodelist elements_by_tagname_ns (const pfs::string & namespace_uri, const pfs::string & localname) const
	{
		return get_elements_by_tagname_ns(namespace_uri, localname);
	}

	element get_element_by_id (const pfs::string & element_id) const;

	element element_by_id (const pfs::string & element_id) const
	{
		return get_element_by_id(element_id);
	}
};

}} // pfs::dom

#endif /* __PFS_DOM_DOCUMENT_HPP__ */
