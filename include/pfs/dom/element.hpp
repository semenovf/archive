/**
 * @file element.hpp
 * @author wladt
 * @date Dec 2, 2013
 */

#ifndef __PFS_DOM_ELEMENT_HPP__
#define __PFS_DOM_ELEMENT_HPP__

#include <pfs/string.hpp>

namespace pfs { namespace dom {

class element_impl;
class node;
class attr;
class document;
class nodelist;

class element : public node
{
	friend class node;
	friend class attr;
	friend class document;

protected:
	element (element_impl * n);

public:
	element ();
	element (const element & other);
	element & operator = (const element & other);

	pfs::string tagName () const;
	pfs::string attribute (const pfs::string & name, const pfs::string & defaultValue = pfs::string()) const
			{ return getAttribute(name, defaultValue); }
	pfs::string getAttribute (const pfs::string & name, const pfs::string & defaultValue = pfs::string()) const;
	void        setAttribute (const pfs::string & name, const pfs::string & value);
	void        removeAttribute (const pfs::string & name);
	attr        attributeNode (const pfs::string & name) { return getAttributeNode(name); }
	attr        getAttributeNode (const pfs::string & name);
	attr        setAttributeNode (const attr & newAttr);
	attr        removeAttributeNode (const attr & oldAttr);
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
					, const pfs::string & value);
	void        removeAttributeNS (const pfs::string & namespaceURI
					, const pfs::string & localName);


	attr        attributeNodeNS (const pfs::string & namespaceURI
					, const pfs::string & localName) const
					{ return getAttributeNodeNS(namespaceURI, localName); }
	attr        getAttributeNodeNS (const pfs::string & namespaceURI
					, const pfs::string & localName) const;
	attr        setAttributeNodeNS (const attr & newAttr);
	nodelist    elementsByTagNameNS (const pfs::string & namespaceURI
			    	, const pfs::string & localName) const
					{ return getElementsByTagNameNS (namespaceURI, localName); }
	nodelist    getElementsByTagNameNS (const pfs::string & namespaceURI
			    	, const pfs::string & localName) const;

	bool        hasAttribute (const pfs::string & name) const;
	bool        hasAttributeNS (const pfs::string & namespaceURI
					, const pfs::string & localName) const;
};

}} // pfs

#endif /* __PFS_DOM_ELEMENT_HPP__ */
