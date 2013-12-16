#ifndef __CWT_DOM_DOCUMENT_HPP__
#define __CWT_DOM_DOCUMENT_HPP__

#include <pfs/string.hpp>

namespace cwt { namespace dom {

class document_impl;
class node;
class dom_implementation;
class document_type;
//class document_fragment;
class element;
//class text;
//class comment;
//class cdatasection;
//class processing_instruction;
//class attr;
//class entity_reference;
//class nodelist;

class DLL_API document : public node
{
	friend class node;

protected:
	document (document_impl *);
//	document (const pfs::string & namespaceURI
//			, const pfs::string & qualifiedName
//			, const document_type & doctype);

public:
	document () : node() {}
	explicit document (const pfs::string & name);
	explicit document (const document_type & doctype);
	document (const document & other);
	document & operator = (const document & other);
	~document () {}


	element           createElement (const pfs::string & tagName); // raises(DOMException)
	element           createElementNS (const pfs::string & namespaceURI, const pfs::string & qualifiedName); // raises(DOMException)

//    // DOM functions
//    QDomDocumentFragment createDocumentFragment();
//    QDomText createTextNode(const QString& data);
//    QDomComment createComment(const QString& data);
//    QDomCDATASection createCDATASection(const QString& data);
//    QDomProcessingInstruction createProcessingInstruction(const QString& target, const QString& data);
//    QDomAttr createAttribute(const QString& name);
//    QDomEntityReference createEntityReference(const QString& name);
//    QDomNodeList elementsByTagName(const QString& tagname) const;
//    QDomNode importNode(const QDomNode& importedNode, bool deep);
//    QDomAttr createAttributeNS(const QString& nsURI, const QString& qName);
//    QDomNodeList elementsByTagNameNS(const QString& nsURI, const QString& localName);
//    QDomElement elementById(const QString& elementId);
//
//    // DOM read only attributes
//    QDomDocumentType doctype() const;
//    QDomImplementation implementation() const;
//    QDomElement documentElement() const;
//
//    // Overridden from QDomNode
//    inline QDomNode::NodeType nodeType() const { return DocumentNode; }
//
//    // Qt extensions
//    bool setContent(const QByteArray& text, bool namespaceProcessing, QString *errorMsg=0, int *errorLine=0, int *errorColumn=0 );
//    bool setContent(const QString& text, bool namespaceProcessing, QString *errorMsg=0, int *errorLine=0, int *errorColumn=0 );
//    bool setContent(QIODevice* dev, bool namespaceProcessing, QString *errorMsg=0, int *errorLine=0, int *errorColumn=0 );
//    bool setContent(QXmlInputSource *source, bool namespaceProcessing, QString *errorMsg=0, int *errorLine=0, int *errorColumn=0 );
//    bool setContent(const QByteArray& text, QString *errorMsg=0, int *errorLine=0, int *errorColumn=0 );
//    bool setContent(const QString& text, QString *errorMsg=0, int *errorLine=0, int *errorColumn=0 );
//    bool setContent(QIODevice* dev, QString *errorMsg=0, int *errorLine=0, int *errorColumn=0 );
//    bool setContent(QXmlInputSource *source, QXmlReader *reader, QString *errorMsg=0, int *errorLine=0, int *errorColumn=0 );
//
//    // Qt extensions
//    QString toString(int = 1) const;
//    QByteArray toByteArray(int = 1) const;
//
//private:
//    QDomDocument(QDomDocumentPrivate*);
//
//    friend class QDomNode;

//	document_type docType () const;
//	dom_implementation implementation () const;
//	element documentElement () const;

//	document_fragment createDocumentFragment ();
//	text              createTextNode (const pfs::string & data);
//	comment           createComment (const pfs::string & data);
//	cdatasection      createCDATASection (const pfs::string & data); // raises(DOMException)
//	processing_instruction createProcessingInstruction (const pfs::string & target, const pfs::string & data); // raises(DOMException)
//	attr              createAttribute (const pfs::string & name); // raises(DOMException)
//	entity_reference  createEntityReference (const pfs::string & name); // raises(DOMException)
//	attr              createAttributeNS (const pfs::string & namespaceURI, const pfs::string & qualifiedName); // raises(DOMException)
//	node              importNode (const node & importedNode, bool deep); // raises(DOMException)
//	nodelist          getElementsByTagName (const pfs::string & tagname);
//	nodelist          getElementsByTagNameNS (const pfs::string & namespaceURI, const pfs::string & localName);
//	element           getElementById (const pfs::string & elementId);
};

}} // cwt::dom

#endif /* __CWT_DOM_DOCUMENT_HPP__ */
