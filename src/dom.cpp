/**
 * @file dom.cpp
 * @author wladt
 * @date Jan 10, 2014
 */

#include "dom_p.hpp"
#include <cwt/logger.hpp>

namespace cwt { namespace xml {

static bool __accept_version(int major, int minor)
{
	return major == 1 && minor == 0;
}

cwt::dom::document createDocument (const pfs::string & xml_source
		, const pfs::string & namespaceURI
		, const pfs::string & qualifiedName
		, const cwt::dom::doctype & doctype)
{
	document_builder h;
	reader xml_reader(& h);

	if (!xml_reader.acceptVersion(__accept_version))
		return cwt::dom::document();

	if (!xml_reader.parse(xml_source))
		return cwt::dom::document();

	if (xml_reader.isError()) {
		std::cout << xml_reader.errorString() << std::endl;
		return cwt::dom::document();
	}

	return h._doc;
}

void document_builder::startElement (const pfs::string & tagname, const pfs::map<pfs::string, pfs::string> & atts)
{

}

void document_builder::endElement (const pfs::string & tagname)
{

}



/*
QDomHandler::QDomHandler(QDomDocumentPrivate* adoc, bool namespaceProcessing)
    : errorLine(0), errorColumn(0), doc(adoc), node(adoc), cdata(false),
        nsProcessing(namespaceProcessing), locator(0)
{
}

QDomHandler::~QDomHandler()
{
}

bool QDomHandler::endDocument()
{
    // ### is this really necessary? (rms)
    if (node != doc)
        return false;
    return true;
}

bool QDomHandler::startDTD(const QString& name, const QString& publicId, const QString& systemId)
{
    doc->doctype()->name = name;
    doc->doctype()->publicId = publicId;
    doc->doctype()->systemId = systemId;
    return true;
}

bool QDomHandler::startElement(const QString& nsURI, const QString&, const QString& qName, const QXmlAttributes& atts)
{
    // tag name
    QDomNodePrivate* n;
    if (nsProcessing) {
        n = doc->createElementNS(nsURI, qName);
    } else {
        n = doc->createElement(qName);
    }

    if (!n)
        return false;

    n->setLocation(locator->lineNumber(), locator->columnNumber());

    node->appendChild(n);
    node = n;

    // attributes
    for (int i=0; i<atts.length(); i++)
    {
        if (nsProcessing) {
            ((QDomElementPrivate*)node)->setAttributeNS(atts.uri(i), atts.qName(i), atts.value(i));
        } else {
            ((QDomElementPrivate*)node)->setAttribute(atts.qName(i), atts.value(i));
        }
    }

    return true;
}

bool QDomHandler::endElement(const QString&, const QString&, const QString&)
{
    if (!node || node == doc)
        return false;
    node = node->parent();

    return true;
}

bool QDomHandler::characters(const QString&  ch)
{
    // No text as child of some document
    if (node == doc)
        return false;

    QScopedPointer<QDomNodePrivate> n;
    if (cdata) {
        n.reset(doc->createCDATASection(ch));
    } else if (!entityName.isEmpty()) {
        QScopedPointer<QDomEntityPrivate> e(new QDomEntityPrivate(doc, 0, entityName,
                QString(), QString(), QString()));
        e->value = ch;
        e->ref.deref();
        doc->doctype()->appendChild(e.data());
        e.take();
        n.reset(doc->createEntityReference(entityName));
    } else {
        n.reset(doc->createTextNode(ch));
    }
    n->setLocation(locator->lineNumber(), locator->columnNumber());
    node->appendChild(n.data());
    n.take();

    return true;
}

bool QDomHandler::processingInstruction(const QString& target, const QString& data)
{
    QDomNodePrivate *n;
    n = doc->createProcessingInstruction(target, data);
    if (n) {
        n->setLocation(locator->lineNumber(), locator->columnNumber());
        node->appendChild(n);
        return true;
    }
    else
        return false;
}

extern bool qt_xml_skipped_entity_in_content;
bool QDomHandler::skippedEntity(const QString& name)
{
    // we can only handle inserting entity references into content
    if (!qt_xml_skipped_entity_in_content)
        return true;

    QDomNodePrivate *n = doc->createEntityReference(name);
    n->setLocation(locator->lineNumber(), locator->columnNumber());
    node->appendChild(n);
    return true;
}

bool QDomHandler::fatalError(const QXmlParseException& exception)
{
    errorMsg = exception.message();
    errorLine =  exception.lineNumber();
    errorColumn =  exception.columnNumber();
    return QXmlDefaultHandler::fatalError(exception);
}

bool QDomHandler::startCDATA()
{
    cdata = true;
    return true;
}

bool QDomHandler::endCDATA()
{
    cdata = false;
    return true;
}

bool QDomHandler::startEntity(const QString &name)
{
    entityName = name;
    return true;
}

bool QDomHandler::endEntity(const QString &)
{
    entityName.clear();
    return true;
}

bool QDomHandler::comment(const QString& ch)
{
    QDomNodePrivate *n;
    n = doc->createComment(ch);
    n->setLocation(locator->lineNumber(), locator->columnNumber());
    node->appendChild(n);
    return true;
}

bool QDomHandler::unparsedEntityDecl(const QString &name, const QString &publicId, const QString &systemId, const QString &notationName)
{
    QDomEntityPrivate* e = new QDomEntityPrivate(doc, 0, name,
            publicId, systemId, notationName);
    // keep the refcount balanced: appendChild() does a ref anyway.
    e->ref.deref();
    doc->doctype()->appendChild(e);
    return true;
}

bool QDomHandler::externalEntityDecl(const QString &name, const QString &publicId, const QString &systemId)
{
    return unparsedEntityDecl(name, publicId, systemId, QString());
}

bool QDomHandler::notationDecl(const QString & name, const QString & publicId, const QString & systemId)
{
    QDomNotationPrivate* n = new QDomNotationPrivate(doc, 0, name, publicId, systemId);
    // keep the refcount balanced: appendChild() does a ref anyway.
    n->ref.deref();
    doc->doctype()->appendChild(n);
    return true;
}

*/

}} // cwt::xml
