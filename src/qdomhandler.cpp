/**
 * @file qdomhandler.cpp
 * @author wladt
 * @date Dec 11, 2013
 */



/**************************************************************
 *
 * QDomHandler
 *
 **************************************************************/

class QDomHandler : public QXmlDefaultHandler
{
public:
    QDomHandler(QDomDocumentPrivate* d, bool namespaceProcessing);
    ~QDomHandler();

    // content handler
    bool endDocument();
    bool startElement(const pfs::string& nsURI, const pfs::string& localName, const pfs::string& qName, const QXmlAttributes& atts);
    bool endElement(const pfs::string& nsURI, const pfs::string& localName, const pfs::string& qName);
    bool characters(const pfs::string& ch);
    bool processingInstruction(const pfs::string& target, const pfs::string& data);
    bool skippedEntity(const pfs::string& name);

    // error handler
    bool fatalError(const QXmlParseException& exception);

    // lexical handler
    bool startCDATA();
    bool endCDATA();
    bool startEntity(const pfs::string &);
    bool endEntity(const pfs::string &);
    bool startDTD(const pfs::string& name, const pfs::string& publicId, const pfs::string& systemId);
    bool comment(const pfs::string& ch);

    // decl handler
    bool externalEntityDecl(const pfs::string &name, const pfs::string &publicId, const pfs::string &systemId) ;

    // DTD handler
    bool notationDecl(const pfs::string & name, const pfs::string & publicId, const pfs::string & systemId);
    bool unparsedEntityDecl(const pfs::string &name, const pfs::string &publicId, const pfs::string &systemId, const pfs::string &notationName) ;

    void setDocumentLocator(QXmlLocator *locator);

    pfs::string errorMsg;
    int errorLine;
    int errorColumn;

private:
    QDomDocumentPrivate *doc;
    QDomNodePrivate *node;
    pfs::string entityName;
    bool cdata;
    bool nsProcessing;
    QXmlLocator *locator;
};


/**************************************************************
 *
 * QDomHandler
 *
 **************************************************************/

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

bool QDomHandler::startDTD(const pfs::string& name, const pfs::string& publicId, const pfs::string& systemId)
{
    doc->doctype()->name = name;
    doc->doctype()->publicId = publicId;
    doc->doctype()->systemId = systemId;
    return true;
}

bool QDomHandler::startElement(const pfs::string& nsURI, const pfs::string&, const pfs::string& qName, const QXmlAttributes& atts)
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

bool QDomHandler::endElement(const pfs::string&, const pfs::string&, const pfs::string&)
{
    if (!node || node == doc)
        return false;
    node = node->parent();

    return true;
}

bool QDomHandler::characters(const pfs::string&  ch)
{
    // No text as child of some document
    if (node == doc)
        return false;

    QScopedPointer<QDomNodePrivate> n;
    if (cdata) {
        n.reset(doc->createCDATASection(ch));
    } else if (!entityName.isEmpty()) {
        QScopedPointer<QDomEntityPrivate> e(new QDomEntityPrivate(doc, 0, entityName,
                pfs::string(), pfs::string(), pfs::string()));
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

bool QDomHandler::processingInstruction(const pfs::string& target, const pfs::string& data)
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
bool QDomHandler::skippedEntity(const pfs::string& name)
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

bool QDomHandler::startEntity(const pfs::string &name)
{
    entityName = name;
    return true;
}

bool QDomHandler::endEntity(const pfs::string &)
{
    entityName.clear();
    return true;
}

bool QDomHandler::comment(const pfs::string& ch)
{
    QDomNodePrivate *n;
    n = doc->createComment(ch);
    n->setLocation(locator->lineNumber(), locator->columnNumber());
    node->appendChild(n);
    return true;
}

bool QDomHandler::unparsedEntityDecl(const pfs::string &name, const pfs::string &publicId, const pfs::string &systemId, const pfs::string &notationName)
{
    QDomEntityPrivate* e = new QDomEntityPrivate(doc, 0, name,
            publicId, systemId, notationName);
    // keep the refcount balanced: appendChild() does a ref anyway.
    e->ref.deref();
    doc->doctype()->appendChild(e);
    return true;
}

bool QDomHandler::externalEntityDecl(const pfs::string &name, const pfs::string &publicId, const pfs::string &systemId)
{
    return unparsedEntityDecl(name, publicId, systemId, pfs::string());
}

bool QDomHandler::notationDecl(const pfs::string & name, const pfs::string & publicId, const pfs::string & systemId)
{
    QDomNotationPrivate* n = new QDomNotationPrivate(doc, 0, name, publicId, systemId);
    // keep the refcount balanced: appendChild() does a ref anyway.
    n->ref.deref();
    doc->doctype()->appendChild(n);
    return true;
}

void QDomHandler::setDocumentLocator(QXmlLocator *locator)
{
    this->locator = locator;
}
