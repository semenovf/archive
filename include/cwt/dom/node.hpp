/*
 * node.hpp
 *
 *  Created on: Nov 20, 2013
 *      Author: wladt
 */

#ifndef __CWT_DOM_NODE_HPP__
#define __CWT_DOM_NODE_HPP__

#include <pfs/string.hpp>

namespace cwt { namespace dom {

class node_impl;
class document;
class nodelist;
class namednodemap;

class DLL_API node
{
	friend class namednodemap;
	friend class nodelist;

public:
	enum type {
		  NullNode                   = 0
		, InvalidNode                = NullNode
		, ElementNode                = 1
		, AttributeNode              = 2
		, TextNode                   = 3
		, CDATASectionNode           = 4
		, EntityReferenceNode        = 5
		, EntityNode                 = 6
		, ProcessingInstructionNode  = 7
		, CommentNode                = 8
		, DocumentNode               = 9
		, DocumentTypeNode           = 10
		, DocumentFragmentNode       = 11
		, NotationNode               = 12

		// TODO Refuse from this
        , BaseNode                   = 21// this is not in the standard
        , CharacterDataNode          = 22 // this is not in the standard

	};

protected:
	node_impl * _pimpl;

	node (node_impl *);

public:
	node () : _pimpl(nullptr) {}
	node (const node & other);
	node & operator = (const node & other);
	~node ();

	pfs::string  nodeName ()        const;
	pfs::string  nodeValue ()       const;
	node::type   nodeType ()        const;
	node         parentNode ()      const;
	node         firstChild ()      const;
	node         lastChild ()       const;
	node         previousSibling () const;
	node         nextSibling ()     const;
	nodelist     childNodes ()      const;
	namednodemap attributes ()      const;
	pfs::string  namespaceURI ()    const;
	pfs::string  prefix ()          const;
	pfs::string  localName ()       const;
	document     ownerDocument ()   const;

	bool isSupported (const pfs::string & feature, const pfs::string & version) const;

	bool hasChildNodes () const;
	bool hasAttributes () const;

	void normalize ();
	void setNodeValue (const pfs::string & value);
	void setPrefix (const pfs::string & p);  // raises(DOMException)

	node insertAfter  (const node & newChild, const node & refChild); // non-DOM
	node insertBefore (const node & newChild, const node & refChild); // raises(DOMException)
	node replaceChild (const node & newChild, const node & oldChild); // raises(DOMException)
	node removeChild  (const node & oldChild); // raises(DOMException)
	node appendChild  (const node & newChild); // raises(DOMException)

	node cloneNode (bool deep = true) const;

	bool isNull () const { return _pimpl == nullptr; }
};

}} // cwt::dom

#endif /* __CWT_DOM_NODE_HPP__ */
