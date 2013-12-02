/**
 * @file element.hpp
 * @author wladt
 * @date Dec 2, 2013
 */

#ifndef __CWT_DOM_ELEMENT_HPP__
#define __CWT_DOM_ELEMENT_HPP__

#include <cwt/dom/node.hpp>

namespace cwt { namespace dom {

class attr;
class nodelist;

class element : public node
{
	pfs::string _tagName; // readonly

public:
	element ();

	const pfs::string & tageName () const { return _tagName; }

	pfs::string getAttribute (const pfs::string & name);
	void setAttribute (const pfs::string & name, const pfs::string & value); //	raises(DOMException);
	void removeAttribute (const pfs::string & name); // raises(DOMException);
	attr * getAttributeNode (const pfs::string & name);
	attr * setAttributeNode (attr * newAttr); // raises(DOMException);
	attr * removeAttributeNode (attr * oldAttr); // raises(DOMException);
	nodelist * getElementsByTagName (const pfs::string & name);
	pfs::string getAttributeNS (const pfs::string & namespaceURI,
			const pfs::string & localName);
	void setAttributeNS (const pfs::string & namespaceURI,
			const pfs::string & qualifiedName,
			const pfs::string & value); //  raises(DOMException);

	void removeAttributeNS (const pfs::string & namespaceURI,
			const pfs::string & localName);  // raises(DOMException);

	attr * getAttributeNodeNS (const pfs::string & namespaceURI,
			const pfs::string & localName);

	attr * setAttributeNodeNS (attr * newAttr); // raises(DOMException);
	nodelist * getElementsByTagNameNS (const pfs::string & namespaceURI,
			const pfs::string & localName);
	bool hasAttribute (const pfs::string & name);
	bool hasAttributeNS (const pfs::string & namespaceURI,
			const pfs::string & localName);
};

}} // cwt

#endif /* __CWT_DOM_ELEMENT_HPP__ */
