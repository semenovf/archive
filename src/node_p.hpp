/**
 * @file node_p.hpp
 * @author wladt
 * @date Dec 10, 2013
 */

#ifndef __CWT_DOM_NODE_P_HPP__
#define __CWT_DOM_NODE_P_HPP__

#include "../include/cwt/dom/node.hpp"

namespace cwt { namespace dom {

class nodelist::impl;
class document::impl;

class node::impl
{
public:
	pfs::string  _name;
	pfs::string  _value;
	pfs::string  _namespaceURI;
	pfs::string  _prefix;

	/*
	 * Local part of the qualified name of this node.
	 * FIXME For nodes of any type other than ELEMENT_NODE and ATTRIBUTE_NODE
	 * and nodes created with a DOM Level 1 method, such as createElement
	 * from the Document interface, this is always null.
	 */
	pfs::string  _localName;

	node::pimpl_type _parent;
	node::pimpl_type _first;
	node::pimpl_type _last;
	node::pimpl_type _prev;
	node::pimpl_type _next;

public:
	impl (node::pimpl_type & parent)
		: _parent(parent) {}
//	impl (const pfs::shared_ptr<node::impl> & n, bool deep);
	virtual ~impl () {}

	node::pimpl_type parent () { return _parent; }
	void setParent (node::pimpl_type parent) { _parent = parent; }
//	bool isNull () const             { return nodeType() == NullNode; }
	bool isAttribute () const        { return nodeType() == AttributeNode; }
//	bool isCDATASection () const     { return nodeType() == CDATASectionNode; }
	bool isDocument () const         { return nodeType() == DocumentNode; }
	bool isDocumentFragment () const { return nodeType() == DocumentFragmentNode; }
//	bool isDocumentType () const     { return nodeType() == DocumentTypeNode; }
	bool isElement () const          { return nodeType() == ElementNode; }
//	bool isEntity () const           { return nodeType() == EntityNode; }
//	bool isEntityReference () const  { return nodeType() == EntityReferenceNode; }
//	bool isNotation () const         { return nodeType() == NotationNode; }
//	bool isProcessionInstriction () const { return nodeType() == ProcessingInstructionNode; }
//	bool isComment () const          { return nodeType() == CommentNode; }
//	bool isText () const
//	{
//		node::type t = nodeType();
//		return t == TextNode || t == CDATASectionNode;
//	}
//
	virtual void setNodeValue (const pfs::string & value) { _value = value; }
	virtual node::type nodeType () const { return node::InvalidNode; }
//	virtual node::impl * cloneNode (bool deep = true);

	document::impl * ownerDocument ();
};


}} // cwt

#endif /* __CWT_DOM_NODE_P_HPP__ */
