/**
 * @file element.hpp
 * @author wladt
 * @date Dec 2, 2013
 */

#ifndef __CWT_DOM_ELEMENT_HPP__
#define __CWT_DOM_ELEMENT_HPP__

#include <pfs/string.hpp>
#include <cwt/dom/pimpl.hpp>

namespace cwt { namespace dom {

class node;
class attr;
class nodelist;

class element : public node
{
	CWT_DOM_PIMPL_DERIVED(element, node)

public:
	pfs::string tageName () const { return node::nodeName(); }
	pfs::string attribute (const pfs::string & name, const pfs::string & defaultValue = pfs::string()) const
					{ return getAttribute(name, defaultValue); }
	pfs::string getAttribute (const pfs::string & name, const pfs::string & defaultValue = pfs::string()) const;
	void        setAttribute (const pfs::string & name, const pfs::string & value); //	raises(DOMException);
	void        removeAttribute (const pfs::string & name); // raises(DOMException);
	attr        attributeNode (const pfs::string & name) { return getAttributeNode(name); }
	attr        getAttributeNode (const pfs::string & name);
	attr        setAttributeNode (const attr & newAttr); // raises(DOMException);
	attr        removeAttributeNode (const attr & oldAttr); // raises(DOMException);
	nodelist    elementsByTagName (const pfs::string & name) const { return getElementsByTagName(name); }
	nodelist    getElementsByTagName (const pfs::string & name) const ;

	pfs::string attributeNS (const pfs::string & namespaceURI
					, const pfs::string & localName
					, const pfs::string & defaultValue = pfs::string()) const
					{ return getAttributeNS(namespaceURI, localName, defaultValue); }
	pfs::string getAttributeNS (const pfs::string & namespaceURI
					, const pfs::string & localName
					, const pfs::string & defaultValue = pfs::string()) const;
	void        setAttributeNS (const pfs::string & namespaceURI
					, const pfs::string & qualifiedName
					, const pfs::string & value); //  raises(DOMException);
	void        removeAttributeNS (const pfs::string & namespaceURI
					, const pfs::string & localName);  // raises(DOMException);


	attr        attributeNodeNS (const pfs::string & namespaceURI
					, const pfs::string & localName) const
					{ return getAttributeNodeNS(namespaceURI, localName); }
	attr        getAttributeNodeNS (const pfs::string & namespaceURI
					, const pfs::string & localName) const;
	attr        setAttributeNodeNS (const attr & newAttr); // raises(DOMException);
	nodelist    elementsByTagNameNS (const pfs::string & namespaceURI
			    	, const pfs::string & localName) const
					{ return getElementsByTagNameNS (namespaceURI, localName); }
	nodelist    getElementsByTagNameNS (const pfs::string & namespaceURI
			    	, const pfs::string & localName) const;

	bool        hasAttribute (const pfs::string & name) const;
	bool        hasAttributeNS (const pfs::string & namespaceURI
					, const pfs::string & localName) const;
};

}} // cwt

#endif /* __CWT_DOM_ELEMENT_HPP__ */
