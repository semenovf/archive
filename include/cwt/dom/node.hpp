/*
 * node.hpp
 *
 *  Created on: Nov 20, 2013
 *      Author: wladt
 */

#ifndef __CWT_DOM_NODE_HPP__
#define __CWT_DOM_NODE_HPP__

#include <cwt/string.hpp>
#include <cwt/map.hpp>
#include <cwt/unitype.hpp>

namespace cwt { namespace dom {

class DLL_API node
{
	  static const ushort_t ELEMENT_NODE                   = 1;
	  static const ushort_t ATTRIBUTE_NODE                 = 2;
	  static const ushort_t TEXT_NODE                      = 3;
	  static const ushort_t CDATA_SECTION_NODE             = 4;
	  static const ushort_t ENTITY_REFERENCE_NODE          = 5;
	  static const ushort_t ENTITY_NODE                    = 6;
	  static const ushort_t PROCESSING_INSTRUCTION_NODE    = 7;
	  static const ushort_t COMMENT_NODE                   = 8;
	  static const ushort_t DOCUMENT_NODE                  = 9;
	  static const ushort_t DOCUMENT_TYPE_NODE             = 10;
	  static const ushort_t DOCUMENT_FRAGMENT_NODE         = 11;
	  static const ushort_t NOTATION_NODE                  = 12;

#ifdef __COMMENT__
                                           // raises(DOMException) on setting
	                                        // raises(DOMException) on retrieval

	  // Modified in DOM Level 2:
	  readonly attribute Document         ownerDocument;
	  Node               insertBefore(in Node newChild,
	                                  in Node refChild)
	                                        raises(DOMException);
	  Node               replaceChild(in Node newChild,
	                                  in Node oldChild)
	                                        raises(DOMException);
	  Node               removeChild(in Node oldChild)
	                                        raises(DOMException);
	  Node               appendChild(in Node newChild)
	                                        raises(DOMException);
	  boolean            hasChildNodes();
	  Node               cloneNode(in boolean deep);
	  // Modified in DOM Level 2:
	  void               normalize();
	  // Introduced in DOM Level 2:
	  boolean            isSupported(in DOMString feature,
	                                 in DOMString version);
	  // Introduced in DOM Level 2:
	  readonly attribute DOMString        namespaceURI;
	  // Introduced in DOM Level 2:
	           attribute DOMString        prefix;
	                                        // raises(DOMException) on setting

	  // Introduced in DOM Level 2:
	  readonly attribute DOMString        localName;
	  // Introduced in DOM Level 2:

#endif

public:
	  node();

public:
	  bool hasAttributes() const;// { return }
	  void setValue (const cwt::unitype & value) { _nodeValue = value; }
	  const cwt::unitype & value () const { return _nodeValue; }

private:
	  cwt::string      _nodeName;        // readonly
	  cwt::unitype     _nodeValue;
	  ushort_t         _nodeType;        // readonly
	  node *           _parentNode;      // readonly
	  node_list *      _childNodes;      // readonly
	  node *           _firstChild;      // readonly
	  node *           _lastChild;       // readonly
	  node *           _previousSibling; // readonly
	  node *           _nextSibling;     // readonly
	  named_node_map * _attributes;      // readonly
};

}} // cwt::dom

#endif /* __CWT_DOM_NODE_HPP__ */
