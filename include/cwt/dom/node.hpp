/*
 * node.hpp
 *
 *  Created on: Nov 20, 2013
 *      Author: wladt
 */

#ifndef __CWT_DOM_NODE_HPP__
#define __CWT_DOM_NODE_HPP__

#include <pfs/string.hpp>
#include <cwt/dom/pimpl.hpp>

namespace cwt { namespace dom {

class document;
//class nodelist;
//class namednodemap;

class DLL_API node
{
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
	};

	CWT_DOM_PIMPL_INLINE(node)

public:
	pfs::string  nodeName ()        const;
	pfs::string  nodeValue ()       const;
	node::type   nodeType ()        const;
	node         parentNode ()      const;
	node         firstChild ()      const;
	node         lastChild ()       const;
	node         previousSibling () const;
	node         nextSibling ()     const;
	nodelist     childNodes ()      const;
//	namednodemap attributes ()      const;
	pfs::string  namespaceURI ()    const;
	pfs::string  prefix ()          const;
	pfs::string  localName ()       const;
	document     ownerDocument ()   const;

	bool isSupported (const pfs::string & feature, const pfs::string & version) const;

	bool hasChildNodes () const;
//	bool hasAttributes() const;

//	void normalize ();
	void setNodeValue (const pfs::string & value);
	void setPrefix (const pfs::string & p);  // raises(DOMException)

//	node insertBefore (const node & newChild, const node & refChild); // raises(DOMException)
//	node replaceChild (const node & newChild, const node & oldChild); // raises(DOMException)
//	node removeChild (const node & oldChild); // raises(DOMException)
//	node appendChild (const node & newChild); // raises(DOMException)
//	node cloneNode (bool deep);
};

}} // cwt::dom

#endif /* __CWT_DOM_NODE_HPP__ */
