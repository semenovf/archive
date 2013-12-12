#ifndef __CWT_DOM_DOCUMENT_HPP__
#define __CWT_DOM_DOCUMENT_HPP__

#include <pfs/string.hpp>

namespace cwt { namespace dom {

class node;
//class document_type;
//class document_fragment;
//class dom_implementation;
//class element;
//class text;
//class comment;
//class cdatasection;
//class processing_instruction;
//class attr;
//class entity_reference;
//class nodelist;

class DLL_API document : public node
{
protected:
//	document (const pimpl_type & other) : _pimpl(other) {}

public:
    document () : node () {}
//    explicit document (const pfs::string & name);
//    explicit document (const document_type & doctype);
//    document (const document & other);
//    document & operator= (const document & other);
//    ~document ();

//	document_type docType () const;
//	dom_implementation implementation () const;
//	element documentElement () const;

//	element           createElement (const pfs::string & tagName); // raises(DOMException)
//	document_fragment createDocumentFragment ();
//	text              createTextNode (const pfs::string & data);
//	comment           createComment (const pfs::string & data);
//	cdatasection      createCDATASection (const pfs::string & data); // raises(DOMException)
//	processing_instruction createProcessingInstruction (const pfs::string & target, const pfs::string & data); // raises(DOMException);
//	attr              createAttribute (const pfs::string & name); // raises(DOMException)
//	entity_reference  createEntityReference (const pfs::string & name); // raises(DOMException)
//	element           createElementNS (const pfs::string & namespaceURI, const pfs::string & qualifiedName); // raises(DOMException)
//	attr              createAttributeNS (const pfs::string & namespaceURI, const pfs::string & qualifiedName); // raises(DOMException)
//	node              importNode (const node & importedNode, bool deep); // raises(DOMException);
//	nodelist          getElementsByTagName (const pfs::string & tagname);
//	nodelist          getElementsByTagNameNS (const pfs::string & namespaceURI, const pfs::string & localName);
//	element           getElementById (const pfs::string & elementId);
};

}} // cwt::dom

#endif /* __CWT_DOM_DOCUMENT_HPP__ */
