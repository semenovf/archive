#ifndef QDOM_H
#define QDOM_H

#include <pfs/string.hpp>

class QIODevice;
class QTextStream;

class QXmlInputSource;
class QXmlReader;

class QDomDocumentPrivate;
class QDomDocumentTypePrivate;
class QDomDocumentFragmentPrivate;
class QDomNodePrivate;
class QDomNodeListPrivate;
class QDomImplementationPrivate;
class QDomElementPrivate;
class QDomNotationPrivate;
class QDomEntityPrivate;
class QDomEntityReferencePrivate;
class QDomProcessingInstructionPrivate;
class QDomAttrPrivate;
class QDomCharacterDataPrivate;
class QDomTextPrivate;
class QDomCommentPrivate;
class QDomCDATASectionPrivate;
class QDomNamedNodeMapPrivate;
class QDomImplementationPrivate;

class QDomNodeList;
class QDomElement;
class QDomText;
class QDomComment;
class QDomCDATASection;
class QDomProcessingInstruction;
class QDomAttr;
class QDomEntityReference;
class QDomDocument;
class QDomNamedNodeMap;
class QDomDocument;
class QDomDocumentFragment;
class QDomDocumentType;
class QDomImplementation;
class QDomNode;
class QDomEntity;
class QDomNotation;
class QDomCharacterData;

class  QDomImplementation
{
public:
    QDomImplementation();
    QDomImplementation(const QDomImplementation&);
    ~QDomImplementation();
    QDomImplementation& operator= (const QDomImplementation&);
    bool operator== (const QDomImplementation&) const;
    bool operator!= (const QDomImplementation&) const;

    // functions
    bool hasFeature(const pfs::string& feature, const pfs::string& version) const;
    QDomDocumentType createDocumentType(const pfs::string& qName, const pfs::string& publicId, const pfs::string& systemId);
    QDomDocument createDocument(const pfs::string& nsURI, const pfs::string& qName, const QDomDocumentType& doctype);

    enum InvalidDataPolicy { AcceptInvalidChars = 0, DropInvalidChars, ReturnNullNode };
    static InvalidDataPolicy invalidDataPolicy();
    static void setInvalidDataPolicy(InvalidDataPolicy policy);

    // Qt extension
    bool isNull();

private:
    QDomImplementationPrivate* impl;
    QDomImplementation(QDomImplementationPrivate*);

    friend class QDomDocument;
};

class  QDomNode
{
public:
    enum NodeType {
        ElementNode               = 1,
        AttributeNode             = 2,
        TextNode                  = 3,
        CDATASectionNode          = 4,
        EntityReferenceNode       = 5,
        EntityNode                = 6,
        ProcessingInstructionNode = 7,
        CommentNode               = 8,
        DocumentNode              = 9,
        DocumentTypeNode          = 10,
        DocumentFragmentNode      = 11,
        NotationNode              = 12,
        BaseNode                  = 21,// this is not in the standard
        CharacterDataNode         = 22 // this is not in the standard
    };

    enum EncodingPolicy
    {
        EncodingFromDocument      = 1,
        EncodingFromTextStream    = 2
    };

    QDomNode();
    QDomNode(const QDomNode&);
    QDomNode& operator= (const QDomNode&);
    bool operator== (const QDomNode&) const;
    bool operator!= (const QDomNode&) const;
    ~QDomNode();

    // DOM functions
    QDomNode insertBefore(const QDomNode& newChild, const QDomNode& refChild);
    QDomNode insertAfter(const QDomNode& newChild, const QDomNode& refChild);
    QDomNode replaceChild(const QDomNode& newChild, const QDomNode& oldChild);
    QDomNode removeChild(const QDomNode& oldChild);
    QDomNode appendChild(const QDomNode& newChild);
    bool hasChildNodes() const;
    QDomNode cloneNode(bool deep = true) const;
    void normalize();
    bool isSupported(const pfs::string& feature, const pfs::string& version) const;

    // DOM read-only attributes
    pfs::string nodeName() const;
    NodeType nodeType() const;
    QDomNode parentNode() const;
    QDomNodeList childNodes() const;
    QDomNode firstChild() const;
    QDomNode lastChild() const;
    QDomNode previousSibling() const;
    QDomNode nextSibling() const;
    QDomNamedNodeMap attributes() const;
    QDomDocument ownerDocument() const;
    pfs::string namespaceURI() const;
    pfs::string localName() const;
    bool hasAttributes() const;

    // DOM attributes
    pfs::string nodeValue() const;
    void setNodeValue(const pfs::string&);
    pfs::string prefix() const;
    void setPrefix(const pfs::string& pre);

protected:
    QDomNodePrivate* impl;
    QDomNode(QDomNodePrivate*);

private:
    friend class QDomDocument;
    friend class QDomDocumentType;
    friend class QDomNodeList;
    friend class QDomNamedNodeMap;
};

class  QDomNodeList
{
public:
    QDomNodeList();
    QDomNodeList(const QDomNodeList&);
    QDomNodeList& operator= (const QDomNodeList&);
    bool operator== (const QDomNodeList&) const;
    bool operator!= (const QDomNodeList&) const;
    ~QDomNodeList();

    // DOM functions
    QDomNode item(int index) const;
    inline QDomNode at(int index) const { return item(index); } // Qt API consistency

    // DOM read only attributes
    int length() const;
    inline int count() const { return length(); } // Qt API consitancy
    inline int size() const { return length(); } // Qt API consistency
    inline bool isEmpty() const { return length() == 0; } // Qt API consistency

private:
    QDomNodeListPrivate* impl;
    QDomNodeList(QDomNodeListPrivate*);

    friend class QDomNode;
    friend class QDomElement;
    friend class QDomDocument;
};

class  QDomDocumentType : public QDomNode
{
public:
    QDomDocumentType();
    QDomDocumentType(const QDomDocumentType& x);
    QDomDocumentType& operator= (const QDomDocumentType&);

    // DOM read only attributes
    pfs::string name() const;
    QDomNamedNodeMap entities() const;
    QDomNamedNodeMap notations() const;
    pfs::string publicId() const;
    pfs::string systemId() const;
    pfs::string internalSubset() const;

    // Overridden from QDomNode
    inline QDomNode::NodeType nodeType() const { return DocumentTypeNode; }

private:
    QDomDocumentType(QDomDocumentTypePrivate*);

    friend class QDomImplementation;
    friend class QDomDocument;
    friend class QDomNode;
};

class  QDomDocument : public QDomNode
{
public:
    QDomDocument();
    explicit QDomDocument(const pfs::string& name);
    explicit QDomDocument(const QDomDocumentType& doctype);
    QDomDocument(const QDomDocument& x);
    QDomDocument& operator= (const QDomDocument&);

    // DOM functions
    QDomElement createElement(const pfs::string& tagName);
    QDomDocumentFragment createDocumentFragment();
    QDomText createTextNode(const pfs::string& data);
    QDomComment createComment(const pfs::string& data);
    QDomCDATASection createCDATASection(const pfs::string& data);
    QDomProcessingInstruction createProcessingInstruction(const pfs::string& target, const pfs::string& data);
    QDomAttr createAttribute(const pfs::string& name);
    QDomEntityReference createEntityReference(const pfs::string& name);
    QDomNodeList elementsByTagName(const pfs::string& tagname) const;
    QDomNode importNode(const QDomNode& importedNode, bool deep);
    QDomElement createElementNS(const pfs::string& nsURI, const pfs::string& qName);
    QDomAttr createAttributeNS(const pfs::string& nsURI, const pfs::string& qName);
    QDomNodeList elementsByTagNameNS(const pfs::string& nsURI, const pfs::string& localName);
    QDomElement elementById(const pfs::string& elementId);

    // DOM read only attributes
    QDomDocumentType doctype() const;
    QDomImplementation implementation() const;
    QDomElement documentElement() const;

    // Overridden from QDomNode
    inline QDomNode::NodeType nodeType() const { return DocumentNode; }

private:
    QDomDocument(QDomDocumentPrivate*);

    friend class QDomNode;
};

class  QDomNamedNodeMap
{
public:
    QDomNamedNodeMap();
    QDomNamedNodeMap(const QDomNamedNodeMap&);
    QDomNamedNodeMap& operator= (const QDomNamedNodeMap&);
    bool operator== (const QDomNamedNodeMap&) const;
    bool operator!= (const QDomNamedNodeMap&) const;
    ~QDomNamedNodeMap();

    // DOM functions
    QDomNode namedItem(const pfs::string& name) const;
    QDomNode setNamedItem(const QDomNode& newNode);
    QDomNode removeNamedItem(const pfs::string& name);
    QDomNode item(int index) const;
    QDomNode namedItemNS(const pfs::string& nsURI, const pfs::string& localName) const;
    QDomNode setNamedItemNS(const QDomNode& newNode);
    QDomNode removeNamedItemNS(const pfs::string& nsURI, const pfs::string& localName);

    // DOM read only attributes
    int length() const;
    int count() const { return length(); } // Qt API consitancy
    inline int size() const { return length(); } // Qt API consistency
    inline bool isEmpty() const { return length() == 0; } // Qt API consistency

    // Qt extension
    bool contains(const pfs::string& name) const;

private:
    QDomNamedNodeMapPrivate* impl;
    QDomNamedNodeMap(QDomNamedNodeMapPrivate*);

    friend class QDomNode;
    friend class QDomDocumentType;
    friend class QDomElement;
};

class  QDomDocumentFragment : public QDomNode
{
public:
    QDomDocumentFragment();
    QDomDocumentFragment(const QDomDocumentFragment& x);
    QDomDocumentFragment& operator= (const QDomDocumentFragment&);

    // Overridden from QDomNode
    inline QDomNode::NodeType nodeType() const { return DocumentFragmentNode; }

private:
    QDomDocumentFragment(QDomDocumentFragmentPrivate*);

    friend class QDomDocument;
    friend class QDomNode;
};

class  QDomCharacterData : public QDomNode
{
public:
    QDomCharacterData();
    QDomCharacterData(const QDomCharacterData& x);
    QDomCharacterData& operator= (const QDomCharacterData&);

    // DOM functions
    pfs::string substringData(unsigned long offset, unsigned long count);
    void appendData(const pfs::string& arg);
    void insertData(unsigned long offset, const pfs::string& arg);
    void deleteData(unsigned long offset, unsigned long count);
    void replaceData(unsigned long offset, unsigned long count, const pfs::string& arg);

    // DOM read only attributes
    int length() const;

    // DOM attributes
    pfs::string data() const;
    void setData(const pfs::string&);

    // Overridden from QDomNode
    QDomNode::NodeType nodeType() const;

private:
    QDomCharacterData(QDomCharacterDataPrivate*);

    friend class QDomDocument;
    friend class QDomText;
    friend class QDomComment;
    friend class QDomNode;
};

class  QDomAttr : public QDomNode
{
public:
    QDomAttr();
    QDomAttr(const QDomAttr& x);
    QDomAttr& operator= (const QDomAttr&);

    // DOM read only attributes
    pfs::string name() const;
    bool specified() const;
    QDomElement ownerElement() const;

    // DOM attributes
    pfs::string value() const;
    void setValue(const pfs::string&);

    // Overridden from QDomNode
    inline QDomNode::NodeType nodeType() const { return AttributeNode; }

private:
    QDomAttr(QDomAttrPrivate*);

    friend class QDomDocument;
    friend class QDomElement;
    friend class QDomNode;
};

class  QDomElement : public QDomNode
{
public:
    QDomElement();
    QDomElement(const QDomElement& x);
    QDomElement& operator= (const QDomElement&);

    // DOM functions
    pfs::string attribute(const pfs::string& name, const pfs::string& defValue = pfs::string() ) const;
    void setAttribute(const pfs::string& name, const pfs::string& value);
    void setAttribute(const pfs::string& name, qlonglong value);
    void setAttribute(const pfs::string& name, qulonglong value);
    inline void setAttribute(const pfs::string& name, int value)
        { setAttribute(name, qlonglong(value)); }
    inline void setAttribute(const pfs::string& name, uint value)
        { setAttribute(name, qulonglong(value)); }
    void setAttribute(const pfs::string& name, float value);
    void setAttribute(const pfs::string& name, double value);
    void removeAttribute(const pfs::string& name);
    QDomAttr attributeNode(const pfs::string& name);
    QDomAttr setAttributeNode(const QDomAttr& newAttr);
    QDomAttr removeAttributeNode(const QDomAttr& oldAttr);
    QDomNodeList elementsByTagName(const pfs::string& tagname) const;
    bool hasAttribute(const pfs::string& name) const;

    pfs::string attributeNS(const pfs::string nsURI, const pfs::string& localName, const pfs::string& defValue = pfs::string()) const;
    void setAttributeNS(const pfs::string nsURI, const pfs::string& qName, const pfs::string& value);
    inline void setAttributeNS(const pfs::string nsURI, const pfs::string& qName, int value)
        { setAttributeNS(nsURI, qName, qlonglong(value)); }
    inline void setAttributeNS(const pfs::string nsURI, const pfs::string& qName, uint value)
        { setAttributeNS(nsURI, qName, qulonglong(value)); }
    void setAttributeNS(const pfs::string nsURI, const pfs::string& qName, qlonglong value);
    void setAttributeNS(const pfs::string nsURI, const pfs::string& qName, qulonglong value);
    void setAttributeNS(const pfs::string nsURI, const pfs::string& qName, double value);
    void removeAttributeNS(const pfs::string& nsURI, const pfs::string& localName);
    QDomAttr attributeNodeNS(const pfs::string& nsURI, const pfs::string& localName);
    QDomAttr setAttributeNodeNS(const QDomAttr& newAttr);
    QDomNodeList elementsByTagNameNS(const pfs::string& nsURI, const pfs::string& localName) const;
    bool hasAttributeNS(const pfs::string& nsURI, const pfs::string& localName) const;

    // DOM read only attributes
    pfs::string tagName() const;
    void setTagName(const pfs::string& name); // Qt extension

    // Overridden from QDomNode
    QDomNamedNodeMap attributes() const;
    inline QDomNode::NodeType nodeType() const { return ElementNode; }

    pfs::string text() const;

private:
    QDomElement(QDomElementPrivate*);

    friend class QDomDocument;
    friend class QDomNode;
    friend class QDomAttr;
};

class  QDomText : public QDomCharacterData
{
public:
    QDomText();
    QDomText(const QDomText& x);
    QDomText& operator= (const QDomText&);

    // DOM functions
    QDomText splitText(int offset);

    // Overridden from QDomCharacterData
    inline QDomNode::NodeType nodeType() const { return TextNode; }

private:
    QDomText(QDomTextPrivate*);

    friend class QDomCDATASection;
    friend class QDomDocument;
    friend class QDomNode;
};

class  QDomComment : public QDomCharacterData
{
public:
    QDomComment();
    QDomComment(const QDomComment& x);
    QDomComment& operator= (const QDomComment&);

    // Overridden from QDomCharacterData
    inline QDomNode::NodeType nodeType() const { return CommentNode; }

private:
    QDomComment(QDomCommentPrivate*);

    friend class QDomDocument;
    friend class QDomNode;
};

class  QDomCDATASection : public QDomText
{
public:
    QDomCDATASection();
    QDomCDATASection(const QDomCDATASection& x);
    QDomCDATASection& operator= (const QDomCDATASection&);

    // Overridden from QDomText
    inline QDomNode::NodeType nodeType() const { return CDATASectionNode; }

private:
    QDomCDATASection(QDomCDATASectionPrivate*);

    friend class QDomDocument;
    friend class QDomNode;
};

class  QDomNotation : public QDomNode
{
public:
    QDomNotation();
    QDomNotation(const QDomNotation& x);
    QDomNotation& operator= (const QDomNotation&);

    // DOM read only attributes
    pfs::string publicId() const;
    pfs::string systemId() const;

    // Overridden from QDomNode
    inline QDomNode::NodeType nodeType() const { return NotationNode; }

private:
    QDomNotation(QDomNotationPrivate*);

    friend class QDomDocument;
    friend class QDomNode;
};

class  QDomEntity : public QDomNode
{
public:
    QDomEntity();
    QDomEntity(const QDomEntity& x);
    QDomEntity& operator= (const QDomEntity&);

    // DOM read only attributes
    pfs::string publicId() const;
    pfs::string systemId() const;
    pfs::string notationName() const;

    // Overridden from QDomNode
    inline QDomNode::NodeType nodeType() const { return EntityNode; }

private:
    QDomEntity(QDomEntityPrivate*);

    friend class QDomNode;
};

class  QDomEntityReference : public QDomNode
{
public:
    QDomEntityReference();
    QDomEntityReference(const QDomEntityReference& x);
    QDomEntityReference& operator= (const QDomEntityReference&);

    // Overridden from QDomNode
    inline QDomNode::NodeType nodeType() const { return EntityReferenceNode; }

private:
    QDomEntityReference(QDomEntityReferencePrivate*);

    friend class QDomDocument;
    friend class QDomNode;
};

class  QDomProcessingInstruction : public QDomNode
{
public:
    QDomProcessingInstruction();
    QDomProcessingInstruction(const QDomProcessingInstruction& x);
    QDomProcessingInstruction& operator= (const QDomProcessingInstruction&);

    // DOM read only attributes
    pfs::string target() const;

    // DOM attributes
    pfs::string data() const;
    void setData(const pfs::string& d);

    // Overridden from QDomNode
    inline QDomNode::NodeType nodeType() const { return ProcessingInstructionNode; }

private:
    QDomProcessingInstruction(QDomProcessingInstructionPrivate*);

    friend class QDomDocument;
    friend class QDomNode;
};


 QTextStream& operator<<(QTextStream&, const QDomNode&);


#endif // QDOM_H
