/**
 * @file node_p.hpp
 * @author wladt
 * @date Dec 10, 2013
 */

#ifndef __CWT_DOM_NODE_P_HPP__
#define __CWT_DOM_NODE_P_HPP__

#include "../include/cwt/dom/node.hpp"
#include <pfs/atomic.hpp>

namespace cwt { namespace dom {

class nodelist::impl;
class document::impl;

class node::impl
{
	pfs::atomic_integer _ref;

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

	/*
	 * The parent of this node.
	 * All nodes, except Attr, Document, DocumentFragment, Entity,
	 * and Notation may have a parent.
	 * However, if a node has just been created and not yet added to the tree,
	 * or if it has been removed from the tree, this is null.
	 */
	node::impl * _parent;
	node::impl * _first; // first child
	node::impl * _last;  // last child
	node::impl * _prev;  // previous sibling
	node::impl * _next;  // next sibling

public:
	impl (node::impl * parent = nullptr)
		: _ref(1)
		, _parent(parent)
		, _first(nullptr)
		, _last(nullptr)
		, _prev(nullptr)
		, _next(nullptr)
	 { ; }

	impl (const node::impl & other, bool deep = true);

	virtual ~impl () {}

//
//	node::pimpl_type parent () { return _parent; }
//	void setParent (const node::pimpl_type & parent) { _parent = parent; }
////	bool isNull () const             { return nodeType() == NullNode; }
//	bool isAttribute () const        { return nodeType() == AttributeNode; }
////	bool isCDATASection () const     { return nodeType() == CDATASectionNode; }
	bool isDocument () const         { return nodeType() == DocumentNode; }
//	bool isDocumentFragment () const { return nodeType() == DocumentFragmentNode; }
////	bool isDocumentType () const     { return nodeType() == DocumentTypeNode; }
//	bool isElement () const          { return nodeType() == ElementNode; }
////	bool isEntity () const           { return nodeType() == EntityNode; }
////	bool isEntityReference () const  { return nodeType() == EntityReferenceNode; }
////	bool isNotation () const         { return nodeType() == NotationNode; }
////	bool isProcessionInstriction () const { return nodeType() == ProcessingInstructionNode; }
////	bool isComment () const          { return nodeType() == CommentNode; }
////	bool isText () const
////	{
////		node::type t = nodeType();
////		return t == TextNode || t == CDATASectionNode;
////	}
////
//	virtual void setNodeValue (const pfs::string & value) { _value = value; }
	virtual node::type nodeType () const { return node::NullNode; }
//	virtual node::impl * cloneNode (bool deep = true);
//
	document::impl * ownerDocument ();
	void markDirty ();
};


}} // cwt

#endif /* __CWT_DOM_NODE_P_HPP__ */
