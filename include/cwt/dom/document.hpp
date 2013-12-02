#ifndef __CWT_DOM_DOCUMENT_HPP__
#define __CWT_DOM_DOCUMENT_HPP__

#include <cwt/dom/node.hpp>

namespace cwt { namespace dom {

class dom;
class doctype;
class docfragment;
class element;
class text;
class comment;
class cdatasection;
class pinstruction;
class entityref;

class document : public node
{
	doctype * _docType;         // readonly
	dom *     _implementation;  // readonly
	element * _docElement;      // readonly

public:
	const doctype * docType { return _doctype; }
	const dom *     implementation () { return _dom; }
	const element * documentElement () { return _docElement; }

	element *       createElement (const pfs::string & tagName);  // raises(DOMException);
	docfragment *   createDocumentFragment ();
	text *          createTextNode (const pfs::string & data);
	comment *       createComment (const pfs::string & data);
	cdatasection *  createCDATASection (const pfs::string & data); // raises(DOMException);
	pinstruction *  createProcessingInstruction (const pfs::string & target,
			const pfs::string & data); // raises(DOMException);
	attr *          createAttribute (const pfs::string & name); // raises(DOMException);
	entityref *     createEntityReference (const pfs::string & name); // raises(DOMException);
	nodelist *      getElementsByTagName (const pfs::string & tagname);
	node *          importNode (node * importedNode, bool deep); // raises(DOMException);
	element *       createElementNS (const pfs::string & namespaceURI,
			const pfs::string & qualifiedName); // raises(DOMException);
	attr *          createAttributeNS (const pfs::string & namespaceURI,
			const pfs::string & qualifiedName); // raises(DOMException);
	nodelist *      getElementsByTagNameNS (const pfs::string & namespaceURI,
			const pfs::string & localName);
	element *       getElementById (const pfs::string & elementId);};
};

}} // cwt::dom

#endif /* __CWT_DOM_DOCUMENT_HPP__ */
