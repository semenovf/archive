/*
 * node.hpp
 *
 *  Created on: Nov 20, 2013
 *      Author: wladt
 */

#ifndef __CWT_DOM_NODE_HPP__
#define __CWT_DOM_NODE_HPP__

#include <pfs/string.hpp>
#include <pfs/map.hpp>
#include <pfs/shared_ptr.hpp>
#include <pfs/unitype.hpp>

namespace cwt { namespace dom {

class document;
class nodelist;
class namednodemap;

class DLL_API node
{
	  pfs::string    _nodeName;        // readonly
	  pfs::unitype   _nodeValue;
	  ushort_t       _nodeType;        // readonly
	  node *         _parentNode;      // readonly
	  pfs::shared_ptr<nodelist>
	                 _childNodes;      // readonly
	  node *         _firstChild;      // readonly
	  node *         _lastChild;       // readonly
	  node *         _previousSibling; // readonly
	  node *         _nextSibling;     // readonly
	  pfs::shared_ptr<namednodemap>
	                 _attributes;      // readonly
	  document *     _ownerDocument;   // readonly
	  pfs::string    _namespaceURI;    // readonly
	  pfs::string    _prefix;
	  pfs::string    _localName;       // readonly

public:
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

public:
	  node();

	  const pfs::string & nodeName () const    { return _nodeName; }
	  const pfs::unitype & nodeValue () const  { return _nodeValue; }
	  void setNodeValue (const pfs::unitype & value) { _nodeValue = value; }
	  ushort_t nodeType () const               { return _nodeType; }
	  const node * parentNode () const         { return _parentNode; }
	  const nodelist * childNodes () const     { return _childNodes.get(); }
	  const node * firstChild () const         { return _firstChild; }
	  const node * lastChild () const          { return _lastChild; }
	  const node * previousSibling () const    { return _previousSibling; }
	  const node * nextSibling () const        { return _nextSibling; }
	  const namednodemap * attributes () const { return _attributes.get(); }
	  const document * ownerDocument () const  { return _ownerDocument; }

	  node * insertBefore (node * newChild, node * refChild); // raises(DOMException);
	  node * replaceChild (node * newChild, node * oldChild); // raises(DOMException);
	  node * removeChild (node * oldChild);                   // raises(DOMException);
	  node * appendChild (node * newChild);                   // raises(DOMException);
	  bool   hasChildNodes () const;
	  node * cloneNode (bool deep);
	  void   normalize ();
	  bool   isSupported (const pfs::string & feature, const pfs::string & version) const;
	  const pfs::string & namespaceURI () const;

	  const pfs::string & prefix () const { return _prefix; }
	  void setPrefix (const pfs::string & p);  // raises(DOMException) on setting
	  const pfs::string & localName () const { return _localName; }
	  bool hasAttributes() const;
};

}} // cwt::dom

#endif /* __CWT_DOM_NODE_HPP__ */
