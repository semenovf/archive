#ifndef __CWT_DOM_DOCUMENT_HPP__
#define __CWT_DOM_DOCUMENT_HPP__

#include <pfs/string.hpp>

namespace cwt { namespace dom {

class document_impl;
class node;
class dom_implementation;
class document_type;
class document_fragment;
class element;
class text;
class comment;
class cdatasection;
class pinstruction;
class attr;
class entityref;
class nodelist;

class DLL_API document : public node
{
	friend class node;

protected:
	document (document_impl *);

public:
	document () : node() {}
	explicit document (const pfs::string & name);
	explicit document (const document_type & doctype);
	document (const document & other);
	document & operator = (const document & other);
	~document () {}

	document_type doctype () const;
	dom_implementation implementation () const;
	element documentElement () const;

	element createElement (const pfs::string & tagName);
	element createElementNS (const pfs::string & namespaceURI, const pfs::string & qualifiedName);
	document_fragment createDocumentFragment ();
	text createTextNode (const pfs::string & data);
	comment createComment (const pfs::string & data);
	cdatasection createCDATASection (const pfs::string & data);
	pinstruction createProcessingInstruction (const pfs::string & target, const pfs::string & data);
	attr createAttribute (const pfs::string & name);
	attr createAttributeNS (const pfs::string & namespaceURI, const pfs::string & qualifiedName);
	entityref createEntityReference (const pfs::string & name);

	node importNode (const node & importedNode, bool deep);

	nodelist getElementsByTagName (const pfs::string & tagname) const;
	nodelist elementsByTagName (const pfs::string & tagname) const
		{ return getElementsByTagName(tagname); }
	nodelist getElementsByTagNameNS (const pfs::string & namespaceURI, const pfs::string & localName) const;
	nodelist elementsByTagNameNS (const pfs::string & namespaceURI, const pfs::string & localName) const
		{ return getElementsByTagNameNS(namespaceURI, localName); }
	element  getElementById (const pfs::string & elementId) const;
	element  elementById (const pfs::string & elementId) const
		{ return getElementById(elementId);	}
};

}} // cwt::dom

#endif /* __CWT_DOM_DOCUMENT_HPP__ */
