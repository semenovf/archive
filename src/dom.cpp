/*
  Helper to split a qualified name in the prefix and local name.
*/

#ifdef __COMMENT__

static void qt_split_namespace(pfs::string& prefix, pfs::string& name, const pfs::string& qName, bool hasURI)
{
    int i = qName.indexOf(QLatin1Char(':'));
    if (i == -1) {
        if (hasURI)
            prefix = QLatin1String("");
        else
            prefix.clear();
        name = qName;
    } else {
        prefix = qName.left(i);
        name = qName.mid(i + 1);
    }
}

/**************************************************************
 *
 * Functions for verifying legal data
 *
 **************************************************************/
dom_implementation::InvalidDataPolicy dom_implementation_impl::invalidDataPolicy
    = dom_implementation::AcceptInvalidChars;

// [5] Name ::= (Letter | '_' | ':') (NameChar)*

static pfs::string fixedXmlName(const pfs::string &_name, bool *ok, bool namespaces = false)
{
    pfs::string name, prefix;
    if (namespaces)
        qt_split_namespace(prefix, name, _name, true);
    else
        name = _name;

    if (name.isEmpty()) {
        *ok = false;
        return pfs::string();
    }

    if (dom_implementation_impl::invalidDataPolicy == dom_implementation::AcceptInvalidChars) {
        *ok = true;
        return _name;
    }

    pfs::string result;
    bool firstChar = true;
    for (int i = 0; i < name.size(); ++i) {
        QChar c = name.at(i);
        if (firstChar) {
            if (QXmlUtils::isLetter(c) || c.unicode() == '_' || c.unicode() == ':') {
                result.append(c);
                firstChar = false;
            } else if (dom_implementation_impl::invalidDataPolicy == dom_implementation::ReturnNullNode) {
                *ok = false;
                return pfs::string();
            }
        } else {
            if (QXmlUtils::isNameChar(c))
                result.append(c);
            else if (dom_implementation_impl::invalidDataPolicy == dom_implementation::ReturnNullNode) {
                *ok = false;
                return pfs::string();
            }
        }
    }

    if (result.isEmpty()) {
        *ok = false;
        return pfs::string();
    }

    *ok = true;
    if (namespaces && !prefix.isEmpty())
        return prefix + QLatin1Char(':') + result;
    return result;
}

// [14] CharData ::= [^<&]* - ([^<&]* ']]>' [^<&]*)
// '<', '&' and "]]>" will be escaped when writing

static pfs::string fixedCharData(const pfs::string &data, bool *ok)
{
    if (dom_implementation_impl::invalidDataPolicy == dom_implementation::AcceptInvalidChars) {
        *ok = true;
        return data;
    }

    pfs::string result;
    for (int i = 0; i < data.size(); ++i) {
        QChar c = data.at(i);
        if (QXmlUtils::isChar(c)) {
            result.append(c);
        } else if (dom_implementation_impl::invalidDataPolicy == dom_implementation::ReturnNullNode) {
            *ok = false;
            return pfs::string();
        }
    }

    *ok = true;
    return result;
}

// [15] Comment ::= '<!--' ((Char - '-') | ('-' (Char - '-')))* '-->'
// can't escape "--", since entities are not recognised within comments

static pfs::string fixedComment(const pfs::string &data, bool *ok)
{
    if (dom_implementation_impl::invalidDataPolicy == dom_implementation::AcceptInvalidChars) {
        *ok = true;
        return data;
    }

    pfs::string fixedData = fixedCharData(data, ok);
    if (!*ok)
        return pfs::string();

    for (;;) {
        int idx = fixedData.indexOf(QLatin1String("--"));
        if (idx == -1)
            break;
        if (dom_implementation_impl::invalidDataPolicy == dom_implementation::ReturnNullNode) {
            *ok = false;
            return pfs::string();
        }
        fixedData.remove(idx, 2);
    }

    *ok = true;
    return fixedData;
}

// [20] CData ::= (Char* - (Char* ']]>' Char*))
// can't escape "]]>", since entities are not recognised within comments

static pfs::string fixedCDataSection(const pfs::string &data, bool *ok)
{
    if (dom_implementation_impl::invalidDataPolicy == dom_implementation::AcceptInvalidChars) {
        *ok = true;
        return data;
    }

    pfs::string fixedData = fixedCharData(data, ok);
    if (!*ok)
        return pfs::string();

    for (;;) {
        int idx = fixedData.indexOf(QLatin1String("]]>"));
        if (idx == -1)
            break;
        if (dom_implementation_impl::invalidDataPolicy == dom_implementation::ReturnNullNode) {
            *ok = false;
            return pfs::string();
        }
        fixedData.remove(idx, 3);
    }

    *ok = true;
    return fixedData;
}

// [16] PI ::= '<?' PITarget (S (Char* - (Char* '?>' Char*)))? '?>'

static pfs::string fixedPIData(const pfs::string &data, bool *ok)
{
    if (dom_implementation_impl::invalidDataPolicy == dom_implementation::AcceptInvalidChars) {
        *ok = true;
        return data;
    }

    pfs::string fixedData = fixedCharData(data, ok);
    if (!*ok)
        return pfs::string();

    for (;;) {
        int idx = fixedData.indexOf(QLatin1String("?>"));
        if (idx == -1)
            break;
        if (dom_implementation_impl::invalidDataPolicy == dom_implementation::ReturnNullNode) {
            *ok = false;
            return pfs::string();
        }
        fixedData.remove(idx, 2);
    }

    *ok = true;
    return fixedData;
}

// [12] PubidLiteral ::= '"' PubidChar* '"' | "'" (PubidChar - "'")* "'"
// The correct quote will be chosen when writing

static pfs::string fixedPubidLiteral(const pfs::string &data, bool *ok)
{
    if (dom_implementation_impl::invalidDataPolicy == dom_implementation::AcceptInvalidChars) {
        *ok = true;
        return data;
    }

    pfs::string result;

    if(QXmlUtils::isPublicID(data))
        result = data;
    else if (dom_implementation_impl::invalidDataPolicy == dom_implementation::ReturnNullNode) {
        *ok = false;
        return pfs::string();
    }

    if (result.indexOf(QLatin1Char('\'')) != -1
        && result.indexOf(QLatin1Char('"')) != -1) {
        if (dom_implementation_impl::invalidDataPolicy == dom_implementation::ReturnNullNode) {
            *ok = false;
            return pfs::string();
        } else {
            result.remove(QLatin1Char('\''));
        }
    }

    *ok = true;
    return result;
}

// [11] SystemLiteral ::= ('"' [^"]* '"') | ("'" [^']* "'")
// The correct quote will be chosen when writing

static pfs::string fixedSystemLiteral(const pfs::string &data, bool *ok)
{
    if (dom_implementation_impl::invalidDataPolicy == dom_implementation::AcceptInvalidChars) {
        *ok = true;
        return data;
    }

    pfs::string result = data;

    if (result.indexOf(QLatin1Char('\'')) != -1
        && result.indexOf(QLatin1Char('"')) != -1) {
        if (dom_implementation_impl::invalidDataPolicy == dom_implementation::ReturnNullNode) {
            *ok = false;
            return pfs::string();
        } else {
            result.remove(QLatin1Char('\''));
        }
    }

    *ok = true;
    return result;
}


/**************************************************************
 *
 * nodelist_impl
 *
 **************************************************************/


nodelist_impl::nodelist_impl(node_impl *n_impl, const pfs::string &name) :
    ref(1)
{
    node_impl = n_impl;
    if (node_impl)
        node_impl->ref.ref();
    tagname = name;
    timestamp = 0;
}

nodelist_impl::nodelist_impl(node_impl *n_impl, const pfs::string &_nsURI, const pfs::string &localName) :
    ref(1)
{
    node_impl = n_impl;
    if (node_impl)
        node_impl->ref.ref();
    tagname = localName;
    nsURI = _nsURI;
    timestamp = 0;
}

nodelist_impl::~nodelist_impl()
{
    if (node_impl && !node_impl->ref.deref())
        delete node_impl;
}

bool nodelist_impl::operator==(const nodelist_impl &other) const
{
    return (node_impl == other.node_impl) && (tagname == other.tagname);
}

bool nodelist_impl::operator!=(const nodelist_impl &other) const
{
    return (node_impl != other.node_impl) || (tagname != other.tagname);
}



/**************************************************************
 *
 * nodelist
 *
 **************************************************************/

/*!
    Returns true if the node list \a n and this node list are equal;
    otherwise returns false.
*/
bool nodelist::operator==(const nodelist &n) const
{
    if (impl == n.impl)
        return true;
    if (!impl || !n.impl)
        return false;
    return (*impl == *n.impl);
}

/*!
    Returns true the node list \a n and this node list are not equal;
    otherwise returns false.
*/
bool nodelist::operator!=(const nodelist &n) const
{
    return !operator==(n);
}




/**************************************************************
 *
 * node_impl
 *
 **************************************************************/

node_impl::node_impl(document_impl *doc, node_impl *par) : ref(1)
{
    if (par)
        setParent(par);
    else
        setOwnerDocument(doc);
    prev = 0;
    next = 0;
    first = 0;
    last = 0;
    createdWithDom1Interface = true;
    lineNumber = -1;
    columnNumber = -1;
}


void node_impl::clear()
{
    node_impl* p = first;
    node_impl* n;

    while (p) {
        n = p->next;
        if (!p->ref.deref())
            delete p;
        p = n;
    }
    first = 0;
    last = 0;
}

node_impl* node_impl::namedItem(const pfs::string &n)
{
    node_impl* p = first;
    while (p) {
        if (p->nodeName() == n)
            return p;
        p = p->next;
    }
    return 0;
}



/**************************************************************
 *
 * node
 *
 **************************************************************/

#define IMPL ((node_impl*)impl)



/*!
    Returns true if \a n and this DOM node are equal; otherwise
    returns false.

    Any instance of node acts as a reference to an underlying data
    structure in document. The test for equality checks if the two
    references point to the same underlying node. For example:

    \snippet code/src_xml_dom_qdom.cpp 2

    The two nodes (element is a node subclass) both refer to
    the document's root element, and \c {element1 == element2} will
    return true. On the other hand:

    \snippet code/src_xml_dom_qdom.cpp 3

    Even though both nodes are empty elements carrying the same name,
    \c {element3 == element4} will return false because they refer to
    two different nodes in the underlying data structure.
*/
bool node::operator== (const node& n) const
{
    return (impl == n.impl);
}

/*!
    Returns true if \a n and this DOM node are not equal; otherwise
    returns false.
*/
bool node::operator!= (const node& n) const
{
    return (impl != n.impl);
}


/*!
    Returns the name of the node.

    The meaning of the name depends on the subclass:

    \table
    \header \li Name \li Meaning
    \row \li attr \li The name of the attribute
    \row \li cdatasection \li The string "#cdata-section"
    \row \li comment \li The string "#comment"
    \row \li document \li The string "#document"
    \row \li document_fragment \li The string "#document-fragment"
    \row \li document_type \li The name of the document type
    \row \li element \li The tag name
    \row \li entity \li The name of the entity
    \row \li entity_reference \li The name of the referenced entity
    \row \li notation \li The name of the notation
    \row \li processing_instruction \li The target of the processing instruction
    \row \li text \li The string "#text"
    \endtable

    \b{Note:} This function does not take the presence of namespaces into account
    when processing the names of element and attribute nodes. As a result, the
    returned name can contain any namespace prefix that may be present.
    To obtain the node name of an element or attribute, use localName(); to
    obtain the namespace prefix, use namespaceURI().

    \sa nodeValue()
*/
pfs::string node::nodeName() const
{
    if (!impl)
        return pfs::string();

    if (!IMPL->prefix.isEmpty())
        return IMPL->prefix + QLatin1Char(':') + IMPL->name;
    return IMPL->name;
}

/*!
    Returns the value of the node.

    The meaning of the value depends on the subclass:
    \table
    \header \li Name \li Meaning
    \row \li attr \li The attribute value
    \row \li cdatasection \li The content of the CDATA section
    \row \li comment \li The comment
    \row \li processing_instruction \li The data of the processing instruction
    \row \li text \li The text
    \endtable

    All the other subclasses do not have a node value and will return
    an empty string.

    \sa setNodeValue(), nodeName()
*/
pfs::string node::nodeValue() const
{
    if (!impl)
        return pfs::string();
    return IMPL->value;
}

/*!
    \enum node::type

    This enum defines the type of the node:
    \value ElementNode
    \value AttributeNode
    \value TextNode
    \value CDATASectionNode
    \value EntityReferenceNode
    \value EntityNode
    \value ProcessingInstructionNode
    \value CommentNode
    \value DocumentNode
    \value DocumentTypeNode
    \value DocumentFragmentNode
    \value NotationNode
    \value BaseNode  A node object, i.e. not a node subclass.
    \value CharacterDataNode
*/

/*!
    Converts the node into a null node; if it was not a null node
    before, its type and contents are deleted.

    \sa isNull()
*/
void node::clear()
{
    if (impl && !impl->ref.deref())
        delete impl;
    impl = 0;
}

/*!
    Returns the first direct child node for which nodeName() equals \a
    name.

    If no such direct child exists, a \l{isNull()}{null node}
    is returned.

    \sa nodeName()
*/
node node::namedItem(const pfs::string& name) const
{
    if (!impl)
        return node();
    return node(impl->namedItem(name));
}

/*!
    Returns true if the node is an attribute; otherwise returns false.

    If this function returns true, it does not imply that this object
    is a QDomAttribute; you can get the QDomAttribute with
    toAttribute().

    \sa toAttr()
*/
bool node::isAttr() const
{
    if(impl)
        return impl->isAttr();
    return false;
}

/*!
    Returns true if the node is a CDATA section; otherwise returns
    false.

    If this function returns true, it does not imply that this object
    is a cdatasection; you can get the cdatasection with
    toCDATASection().

    \sa toCDATASection()
*/
bool node::isCDATASection() const
{
    if(impl)
        return impl->isCDATASection();
    return false;
}

/*!
    Returns true if the node is a document fragment; otherwise returns
    false.

    If this function returns true, it does not imply that this object
    is a document_fragment; you can get the document_fragment
    with toDocumentFragment().

    \sa toDocumentFragment()
*/
bool node::isDocumentFragment() const
{
    if(impl)
        return impl->isDocumentFragment();
    return false;
}

/*!
    Returns true if the node is a document; otherwise returns false.

    If this function returns true, it does not imply that this object
    is a document; you can get the document with toDocument().

    \sa toDocument()
*/
bool node::isDocument() const
{
    if(impl)
        return impl->isDocument();
    return false;
}

/*!
    Returns true if the node is a document type; otherwise returns
    false.

    If this function returns true, it does not imply that this object
    is a document_type; you can get the document_type with
    toDocumentType().

    \sa toDocumentType()
*/
bool node::isDocumentType() const
{
    if(impl)
        return impl->isDocumentType();
    return false;
}

/*!
    Returns true if the node is an element; otherwise returns false.

    If this function returns true, it does not imply that this object
    is a element; you can get the element with toElement().

    \sa toElement()
*/
bool node::isElement() const
{
    if(impl)
        return impl->isElement();
    return false;
}

/*!
    Returns true if the node is an entity reference; otherwise returns
    false.

    If this function returns true, it does not imply that this object
    is a entity_reference; you can get the entity_reference with
    toEntityReference().

    \sa toEntityReference()
*/
bool node::isEntityReference() const
{
    if(impl)
        return impl->isEntityReference();
    return false;
}

/*!
    Returns true if the node is a text node; otherwise returns false.

    If this function returns true, it does not imply that this object
    is a text; you can get the text with toText().

    \sa toText()
*/
bool node::isText() const
{
    if(impl)
        return impl->isText();
    return false;
}

/*!
    Returns true if the node is an entity; otherwise returns false.

    If this function returns true, it does not imply that this object
    is a entity; you can get the entity with toEntity().

    \sa toEntity()
*/
bool node::isEntity() const
{
    if(impl)
        return impl->isEntity();
    return false;
}

/*!
    Returns true if the node is a notation; otherwise returns false.

    If this function returns true, it does not imply that this object
    is a notation; you can get the notation with toNotation().

    \sa toNotation()
*/
bool node::isNotation() const
{
    if(impl)
        return impl->isNotation();
    return false;
}

/*!
    Returns true if the node is a processing instruction; otherwise
    returns false.

    If this function returns true, it does not imply that this object
    is a processing_instruction; you can get the
    QProcessingInstruction with toProcessingInstruction().

    \sa toProcessingInstruction()
*/
bool node::isProcessingInstruction() const
{
    if(impl)
        return impl->isProcessingInstruction();
    return false;
}

/*!
    Returns true if the node is a character data node; otherwise
    returns false.

    If this function returns true, it does not imply that this object
    is a chardata; you can get the chardata with
    toCharacterData().

    \sa toCharacterData()
*/
bool node::isCharacterData() const
{
    if (impl)
        return impl->isCharacterData();
    return false;
}

/*!
    Returns true if the node is a comment; otherwise returns false.

    If this function returns true, it does not imply that this object
    is a comment; you can get the comment with toComment().

    \sa toComment()
*/
bool node::isComment() const
{
    if (impl)
        return impl->isComment();
    return false;
}

#undef IMPL



/**************************************************************
 *
 * namednodemap_impl
 *
 **************************************************************/


namednodemap_impl* namednodemap_impl::clone(node_impl* p)
{
    QScopedPointer<namednodemap_impl> m(new namednodemap_impl(p));
    m->readonly = readonly;
    m->appendToParent = appendToParent;

    QHash<pfs::string, node_impl*>::const_iterator it = map.constBegin();
    for (; it != map.constEnd(); ++it) {
        node_impl *new_node = (*it)->cloneNode();
        new_node->setParent(p);
        m->setNamedItem(new_node);
    }

    // we are no longer interested in ownership
    m->ref.deref();
    return m.take();
}



/**************************************************************
 *
 * namednodemap
 *
 **************************************************************/

#define IMPL ((namednodemap_impl*)impl)


/*!
    Returns true if \a n and this named node map are equal; otherwise
    returns false.
*/
bool namednodemap::operator== (const namednodemap& n) const
{
    return (impl == n.impl);
}

/*!
    Returns true if \a n and this named node map are not equal;
    otherwise returns false.
*/
bool namednodemap::operator!= (const namednodemap& n) const
{
    return (impl != n.impl);
}

/*!
    Returns true if the map contains a node called \a name; otherwise
    returns false.

    \b{Note:} This function does not take the presence of namespaces into account.
    Use namedItemNS() to test whether the map contains a node with a specific namespace
    URI and name.
*/
bool namednodemap::contains(const pfs::string& name) const
{
    if (!impl)
        return false;
    return IMPL->contains(name);
}

#undef IMPL

/**************************************************************
 *
 * document_type_impl
 *
 **************************************************************/


static pfs::string quotedValue(const pfs::string &data)
{
    QChar quote = data.indexOf(QLatin1Char('\'')) == -1
                    ? QLatin1Char('\'')
                    : QLatin1Char('"');
    return quote + data + quote;
}

#undef IMPL

/**************************************************************
 *
 * document_fragment_impl
 *
 **************************************************************/

document_fragment_impl::document_fragment_impl(document_impl* doc, node_impl* parent)
    : node_impl(doc, parent)
{
    name = QLatin1String("#document-fragment");
}

document_fragment_impl::document_fragment_impl(node_impl* n, bool deep)
    : node_impl(n, deep)
{
}

node_impl* document_fragment_impl::cloneNode(bool deep)
{
    node_impl* p = new document_fragment_impl(this, deep);
    // We are not interested in this node
    p->ref.deref();
    return p;
}

/**************************************************************
 *
 * document_fragment
 *
 **************************************************************/

/*!
    Constructs an empty document fragment.
*/
document_fragment::document_fragment()
{
}

document_fragment::document_fragment(document_fragment_impl* n)
    : node(n)
{
}

/*!
    Constructs a copy of \a x.

    The data of the copy is shared (shallow copy): modifying one node
    will also change the other. If you want to make a deep copy, use
    cloneNode().
*/
document_fragment::document_fragment(const document_fragment& x)
    : node(x)
{
}

/*!
    Assigns \a x to this DOM document fragment.

    The data of the copy is shared (shallow copy): modifying one node
    will also change the other. If you want to make a deep copy, use
    cloneNode().
*/
document_fragment& document_fragment::operator= (const document_fragment& x)
{
    return (document_fragment&) node::operator=(x);
}

/*!
    \fn node::type document_fragment::nodeType() const

    Returns \c DocumentFragment.

    \sa isDocumentFragment(), node::toDocumentFragment()
*/

/**************************************************************
 *
 * chardata_impl
 *
 **************************************************************/

chardata_impl::chardata_impl(document_impl* d, node_impl* p,
                                                      const pfs::string& data)
    : node_impl(d, p)
{
    value = data;
    name = QLatin1String("#character-data");
}

chardata_impl::chardata_impl(chardata_impl* n, bool deep)
    : node_impl(n, deep)
{
}

node_impl* chardata_impl::cloneNode(bool deep)
{
    node_impl* p = new chardata_impl(this, deep);
    // We are not interested in this node
    p->ref.deref();
    return p;
}

int chardata_impl::dataLength() const
{
    return value.length();
}

pfs::string chardata_impl::substringData(unsigned long offset, unsigned long n) const
{
    return value.mid(offset, n);
}

void chardata_impl::insertData(unsigned long offset, const pfs::string& arg)
{
    value.insert(offset, arg);
}

void chardata_impl::deleteData(unsigned long offset, unsigned long n)
{
    value.remove(offset, n);
}

void chardata_impl::replaceData(unsigned long offset, unsigned long n, const pfs::string& arg)
{
    value.replace(offset, n, arg);
}

void chardata_impl::appendData(const pfs::string& arg)
{
    value += arg;
}

/**************************************************************
 *
 * chardata
 *
 **************************************************************/

#define IMPL ((chardata_impl*)impl)

/*!
    \class chardata
    \reentrant
    \brief The chardata class represents a generic string in the DOM.

    \inmodule QtXml
    \ingroup xml-tools

    Character data as used in XML specifies a generic data string.
    More specialized versions of this class are text, comment
    and cdatasection.

    The data string is set with setData() and retrieved with data().
    You can retrieve a portion of the data string using
    substringData(). Extra data can be appended with appendData(), or
    inserted with insertData(). Portions of the data string can be
    deleted with deleteData() or replaced with replaceData(). The
    length of the data string is returned by length().

    The node type of the node containing this character data is
    returned by nodeType().

    \sa text, comment, cdatasection
*/

/*!
    Constructs an empty character data object.
*/
chardata::chardata()
{
}

/*!
    Constructs a copy of \a x.

    The data of the copy is shared (shallow copy): modifying one node
    will also change the other. If you want to make a deep copy, use
    cloneNode().
*/
chardata::chardata(const chardata& x)
    : node(x)
{
}

chardata::chardata(chardata_impl* n)
    : node(n)
{
}

/*!
    Assigns \a x to this character data.

    The data of the copy is shared (shallow copy): modifying one node
    will also change the other. If you want to make a deep copy, use
    cloneNode().
*/
chardata & chardata::operator= (const chardata& x)
{
    return (chardata&) node::operator=(x);
}

/*!
    Returns the string stored in this object.

    If the node is a \l{isNull()}{null node}, it will return
    an empty string.
*/
pfs::string chardata::data() const
{
    if (!impl)
        return pfs::string();
    return impl->nodeValue();
}

/*!
    Sets this object's string to \a v.
*/
void chardata::setData(const pfs::string& v)
{
    if (impl)
        impl->setNodeValue(v);
}

/*!
    Returns the length of the stored string.
*/
int chardata::length() const
{
    if (impl)
        return IMPL->dataLength();
    return 0;
}

/*!
    Returns the substring of length \a count from position \a offset.
*/
pfs::string chardata::substringData(unsigned long offset, unsigned long count)
{
    if (!impl)
        return pfs::string();
    return IMPL->substringData(offset, count);
}

/*!
    Appends the string \a arg to the stored string.
*/
void chardata::appendData(const pfs::string& arg)
{
    if (impl)
        IMPL->appendData(arg);
}

/*!
    Inserts the string \a arg into the stored string at position \a offset.
*/
void chardata::insertData(unsigned long offset, const pfs::string& arg)
{
    if (impl)
        IMPL->insertData(offset, arg);
}

/*!
    Deletes a substring of length \a count from position \a offset.
*/
void chardata::deleteData(unsigned long offset, unsigned long count)
{
    if (impl)
        IMPL->deleteData(offset, count);
}

/*!
    Replaces the substring of length \a count starting at position \a
    offset with the string \a arg.
*/
void chardata::replaceData(unsigned long offset, unsigned long count, const pfs::string& arg)
{
    if (impl)
        IMPL->replaceData(offset, count, arg);
}

/*!
    Returns the type of node this object refers to (i.e. \c TextNode,
    \c CDATASectionNode, \c CommentNode or \c CharacterDataNode). For
    a \l{isNull()}{null node}, returns \c CharacterDataNode.
*/
node::type chardata::nodeType() const
{
    if (!impl)
        return CharacterDataNode;
    return node::nodeType();
}

#undef IMPL

/**************************************************************
 *
 * attr_impl
 *
 **************************************************************/

attr_impl::attr_impl(document_impl* d, node_impl* parent, const pfs::string& name_)
    : node_impl(d, parent)
{
    name = name_;
    m_specified = false;
}

attr_impl::attr_impl(document_impl* d, node_impl* p, const pfs::string& nsURI, const pfs::string& qName)
    : node_impl(d, p)
{
    qt_split_namespace(prefix, name, qName, !nsURI.isNull());
    namespaceURI = nsURI;
    createdWithDom1Interface = false;
    m_specified = false;
}

attr_impl::attr_impl(attr_impl* n, bool deep)
    : node_impl(n, deep)
{
    m_specified = n->specified();
}

void attr_impl::setNodeValue(const pfs::string& v)
{
    value = v;
    text_impl *t = new text_impl(0, this, v);
    // keep the refcount balanced: appendChild() does a ref anyway.
    t->ref.deref();
    if (first) {
        delete removeChild(first);
    }
    appendChild(t);
}

node_impl* attr_impl::cloneNode(bool deep)
{
    node_impl* p = new attr_impl(this, deep);
    // We are not interested in this node
    p->ref.deref();
    return p;
}

bool attr_impl::specified() const
{
    return m_specified;
}

/* \internal
  Encode & escape \a str. Yes, it makes no sense to return a pfs::string,
  but is so for legacy reasons.

  Remember that content produced should be able to roundtrip with 2.11 End-of-Line Handling
  and 3.3.3 Attribute-Value Normalization.

  If \a performAVN is true, characters will be escaped to survive Attribute Value Normalization.
  If \a encodeEOLs is true, characters will be escaped to survive End-of-Line Handling.
*/
static pfs::string encodeText(const pfs::string &str,
                          QTextStream &s,
                          const bool encodeQuotes = true,
                          const bool performAVN = false,
                          const bool encodeEOLs = false)
{
#ifdef QT_NO_TEXTCODEC
    Q_UNUSED(s);
#else
    const QTextCodec *const codec = s.codec();
    Q_ASSERT(codec);
#endif
    pfs::string retval(str);
    int len = retval.length();
    int i = 0;

    while (i < len) {
        const QChar ati(retval.at(i));

        if (ati == QLatin1Char('<')) {
            retval.replace(i, 1, QLatin1String("&lt;"));
            len += 3;
            i += 4;
        } else if (encodeQuotes && (ati == QLatin1Char('"'))) {
            retval.replace(i, 1, QLatin1String("&quot;"));
            len += 5;
            i += 6;
        } else if (ati == QLatin1Char('&')) {
            retval.replace(i, 1, QLatin1String("&amp;"));
            len += 4;
            i += 5;
        } else if (ati == QLatin1Char('>') && i >= 2 && retval[i - 1] == QLatin1Char(']') && retval[i - 2] == QLatin1Char(']')) {
            retval.replace(i, 1, QLatin1String("&gt;"));
            len += 3;
            i += 4;
        } else if (performAVN &&
                   (ati == QChar(0xA) ||
                    ati == QChar(0xD) ||
                    ati == QChar(0x9))) {
            const pfs::string replacement(QLatin1String("&#x") + pfs::string::number(ati.unicode(), 16) + QLatin1Char(';'));
            retval.replace(i, 1, replacement);
            i += replacement.length();
            len += replacement.length() - 1;
        } else if (encodeEOLs && ati == QChar(0xD)) {
            retval.replace(i, 1, QLatin1String("&#xd;")); // Replace a single 0xD with a ref for 0xD
            len += 4;
            i += 5;
        } else {
#ifndef QT_NO_TEXTCODEC
            if(codec->canEncode(ati))
                ++i;
            else
#endif
            {
                // We have to use a character reference to get it through.
                const ushort codepoint(ati.unicode());
                const pfs::string replacement(QLatin1String("&#x") + pfs::string::number(codepoint, 16) + QLatin1Char(';'));
                retval.replace(i, 1, replacement);
                i += replacement.length();
                len += replacement.length() - 1;
            }
        }
    }

    return retval;
}



/**************************************************************
 *
 * attr
 *
 **************************************************************/

#define IMPL ((attr_impl*)impl)

/*!
    \class attr
    \reentrant
    \brief The attr class represents one attribute of a element.

    \inmodule QtXml
    \ingroup xml-tools

    For example, the following piece of XML produces an element with
    no children, but two attributes:

    \snippet code/src_xml_dom_qdom.cpp 7

    You can access the attributes of an element with code like this:

    \snippet code/src_xml_dom_qdom.cpp 8

    This example also shows that changing an attribute received from
    an element changes the attribute of the element. If you do not
    want to change the value of the element's attribute you must
    use cloneNode() to get an independent copy of the attribute.

    attr can return the name() and value() of an attribute. An
    attribute's value is set with setValue(). If specified() returns
    true the value was set with setValue(). The node this
    attribute is attached to (if any) is returned by ownerElement().

    For further information about the Document Object Model see
    \l{http://www.w3.org/TR/REC-DOM-Level-1/} and
    \l{http://www.w3.org/TR/DOM-Level-2-Core/}.
    For a more general introduction of the DOM implementation see the
    document documentation.
*/


/*!
    Constructs an empty attribute.
*/
attr::attr()
{
}

/*!
    Constructs a copy of \a x.

    The data of the copy is shared (shallow copy): modifying one node
    will also change the other. If you want to make a deep copy, use
    cloneNode().
*/
attr::attr(const attr& x)
    : node(x)
{
}

attr::attr(attr_impl* n)
    : node(n)
{
}

/*!
    Assigns \a x to this DOM attribute.

    The data of the copy is shared (shallow copy): modifying one node
    will also change the other. If you want to make a deep copy, use
    cloneNode().
*/
attr& attr::operator= (const attr& x)
{
    return (attr&) node::operator=(x);
}

/*!
    Returns the attribute's name.
*/
pfs::string attr::name() const
{
    if (!impl)
        return pfs::string();
    return impl->nodeName();
}

/*!
    Returns true if the attribute has been set by the user with setValue().
    Returns false if the value hasn't been specified or set.

    \sa setValue()
*/
bool attr::specified() const
{
    if (!impl)
        return false;
    return IMPL->specified();
}

/*!
    Returns the element node this attribute is attached to or a
    \l{node::isNull()}{null node} if this attribute is not
    attached to any element.
*/
element attr::ownerElement() const
{
    Q_ASSERT(impl->parent());
    if (!impl->parent()->isElement())
        return element();
    return element((element_impl*)(impl->parent()));
}

/*!
    Returns the value of the attribute or an empty string if the
    attribute has not been specified.

    \sa specified(), setValue()
*/
pfs::string attr::value() const
{
    if (!impl)
        return pfs::string();
    return impl->nodeValue();
}

/*!
    Sets the attribute's value to \a v.

    \sa value()
*/
void attr::setValue(const pfs::string& v)
{
    if (!impl)
        return;
    impl->setNodeValue(v);
    IMPL->m_specified = true;
}

/*!
    \fn node::type attr::nodeType() const

    Returns \l{node::type}{AttributeNode}.
*/

#undef IMPL

/**************************************************************
 *
 * element_impl
 *
 **************************************************************/

element_impl::element_impl(document_impl* d, node_impl* p,
                                          const pfs::string& tagname)
    : node_impl(d, p)
{
    name = tagname;
    m_attr = new namednodemap_impl(this);
}

element_impl::element_impl(document_impl* d, node_impl* p,
        const pfs::string& nsURI, const pfs::string& qName)
    : node_impl(d, p)
{
    qt_split_namespace(prefix, name, qName, !nsURI.isNull());
    namespaceURI = nsURI;
    createdWithDom1Interface = false;
    m_attr = new namednodemap_impl(this);
}

element_impl::element_impl(element_impl* n, bool deep) :
    node_impl(n, deep)
{
    m_attr = n->m_attr->clone(this);
    // Reference is down to 0, so we set it to 1 here.
    m_attr->ref.ref();
}

element_impl::~element_impl()
{
    if (!m_attr->ref.deref())
        delete m_attr;
}

node_impl* element_impl::cloneNode(bool deep)
{
    node_impl* p = new element_impl(this, deep);
    // We are not interested in this node
    p->ref.deref();
    return p;
}

pfs::string element_impl::attribute(const pfs::string& name_, const pfs::string& defValue) const
{
    node_impl* n = m_attr->namedItem(name_);
    if (!n)
        return defValue;

    return n->nodeValue();
}

pfs::string element_impl::attributeNS(const pfs::string& nsURI, const pfs::string& localName, const pfs::string& defValue) const
{
    node_impl* n = m_attr->namedItemNS(nsURI, localName);
    if (!n)
        return defValue;

    return n->nodeValue();
}

void element_impl::setAttribute(const pfs::string& aname, const pfs::string& newValue)
{
    node_impl* n = m_attr->namedItem(aname);
    if (!n) {
        n = new attr_impl(ownerDocument(), this, aname);
        n->setNodeValue(newValue);

        // Referencing is done by the map, so we set the reference counter back
        // to 0 here. This is ok since we created the attr_impl.
        n->ref.deref();
        m_attr->setNamedItem(n);
    } else {
        n->setNodeValue(newValue);
    }
}

void element_impl::setAttributeNS(const pfs::string& nsURI, const pfs::string& qName, const pfs::string& newValue)
{
    pfs::string prefix, localName;
    qt_split_namespace(prefix, localName, qName, true);
    node_impl* n = m_attr->namedItemNS(nsURI, localName);
    if (!n) {
        n = new attr_impl(ownerDocument(), this, nsURI, qName);
        n->setNodeValue(newValue);

        // Referencing is done by the map, so we set the reference counter back
        // to 0 here. This is ok since we created the attr_impl.
        n->ref.deref();
        m_attr->setNamedItem(n);
    } else {
        n->setNodeValue(newValue);
        n->prefix = prefix;
    }
}

void element_impl::removeAttribute(const pfs::string& aname)
{
    node_impl* p = m_attr->removeNamedItem(aname);
    if (p && p->ref.load() == 0)
        delete p;
}

attr_impl* element_impl::attributeNode(const pfs::string& aname)
{
    return (attr_impl*)m_attr->namedItem(aname);
}

attr_impl* element_impl::attributeNodeNS(const pfs::string& nsURI, const pfs::string& localName)
{
    return (attr_impl*)m_attr->namedItemNS(nsURI, localName);
}

attr_impl* element_impl::setAttributeNode(attr_impl* newAttr)
{
    node_impl* n = m_attr->namedItem(newAttr->nodeName());

    // Referencing is done by the maps
    m_attr->setNamedItem(newAttr);

    newAttr->setParent(this);

    return (attr_impl*)n;
}

attr_impl* element_impl::setAttributeNodeNS(attr_impl* newAttr)
{
    node_impl* n = 0;
    if (!newAttr->prefix.isNull())
        n = m_attr->namedItemNS(newAttr->namespaceURI, newAttr->name);

    // Referencing is done by the maps
    m_attr->setNamedItem(newAttr);

    return (attr_impl*)n;
}

attr_impl* element_impl::removeAttributeNode(attr_impl* oldAttr)
{
    return (attr_impl*)m_attr->removeNamedItem(oldAttr->nodeName());
}


bool element_impl::hasAttributeNS(const pfs::string& nsURI, const pfs::string& localName)
{
    return m_attr->containsNS(nsURI, localName);
}

pfs::string element_impl::text()
{
    pfs::string t(QLatin1String(""));

    node_impl* p = first;
    while (p) {
        if (p->isText() || p->isCDATASection())
            t += p->nodeValue();
        else if (p->isElement())
            t += ((element_impl*)p)->text();
        p = p->next;
    }

    return t;
}

/**************************************************************
 *
 * element
 *
 **************************************************************/

#define IMPL ((element_impl*)impl)

/*!
    \class element
    \reentrant
    \brief The element class represents one element in the DOM tree.

    \inmodule QtXml
    \ingroup xml-tools

    Elements have a tagName() and zero or more attributes associated
    with them. The tag name can be changed with setTagName().

    Element attributes are represented by attr objects that can
    be queried using the attribute() and attributeNode() functions.
    You can set attributes with the setAttribute() and
    setAttributeNode() functions. Attributes can be removed with
    removeAttribute(). There are namespace-aware equivalents to these
    functions, i.e. setAttributeNS(), setAttributeNodeNS() and
    removeAttributeNS().

    If you want to access the text of a node use text(), e.g.

    \snippet code/src_xml_dom_qdom.cpp 9

    The text() function operates recursively to find the text (since
    not all elements contain text). If you want to find all the text
    in all of a node's children, iterate over the children looking for
    text nodes, e.g.

    \snippet code/src_xml_dom_qdom.cpp 10

    Note that we attempt to convert each node to a text node and use
    text() rather than using firstChild().toText().data() or
    n.toText().data() directly on the node, because the node may not
    be a text element.

    You can get a list of all the decendents of an element which have
    a specified tag name with elementsByTagName() or
    elementsByTagNameNS().

    To browse the elements of a dom document use firstChildElement(), lastChildElement(),
    nextSiblingElement() and previousSiblingElement(). For example, to iterate over all
    child elements called "entry" in a root element called "database", you can use:

    \snippet code/src_xml_dom_qdom.cpp 11

    For further information about the Document Object Model see
    \l{W3C DOM Level 1}{Level 1} and
    \l{W3C DOM Level 2}{Level 2 Core}.
    For a more general introduction of the DOM implementation see the
    document documentation.
*/

/*!
    Constructs an empty element. Use the document::createElement()
    function to construct elements with content.
*/
element::element()
    : node()
{
}

/*!
    Constructs a copy of \a x.

    The data of the copy is shared (shallow copy): modifying one node
    will also change the other. If you want to make a deep copy, use
    cloneNode().
*/
element::element(const element& x)
    : node(x)
{
}

element::element(element_impl* n)
    : node(n)
{
}

/*!
    Assigns \a x to this DOM element.

    The data of the copy is shared (shallow copy): modifying one node
    will also change the other. If you want to make a deep copy, use
    cloneNode().
*/
element& element::operator= (const element& x)
{
    return (element&) node::operator=(x);
}

/*!
    \fn node::type element::nodeType() const

    Returns \c ElementNode.
*/

/*!
    Sets this element's tag name to \a name.

    \sa tagName()
*/
void element::setTagName(const pfs::string& name)
{
    if (impl)
        impl->name = name;
}

/*!
    Returns the tag name of this element. For an XML element like this:

    \snippet code/src_xml_dom_qdom.cpp 12

    the tagname would return "img".

    \sa setTagName()
*/
pfs::string element::tagName() const
{
    if (!impl)
        return pfs::string();
    return impl->nodeName();
}


/*!
    Returns a namednodemap containing all this element's attributes.

    \sa attribute(), setAttribute(), attributeNode(), setAttributeNode()
*/
namednodemap element::attributes() const
{
    if (!impl)
        return namednodemap();
    return namednodemap(IMPL->attributes());
}

/*!
    Returns the attribute called \a name. If the attribute does not
    exist \a defValue is returned.

    \sa setAttribute(), attributeNode(), setAttributeNode(), attributeNS()
*/
pfs::string element::attribute(const pfs::string& name, const pfs::string& defValue) const
{
    if (!impl)
        return defValue;
    return IMPL->attribute(name, defValue);
}

/*!
    Adds an attribute called \a name with value \a value. If an
    attribute with the same name exists, its value is replaced by \a
    value.

    \sa attribute(), setAttributeNode(), setAttributeNS()
*/
void element::setAttribute(const pfs::string& name, const pfs::string& value)
{
    if (!impl)
        return;
    IMPL->setAttribute(name, value);
}

/*!
  \fn void element::setAttribute(const pfs::string& name, int value)

    \overload
    The number is formatted according to the current locale.
*/

/*!
  \fn void element::setAttribute(const pfs::string& name, uint value)

    \overload
    The number is formatted according to the current locale.
*/

/*!
    \overload

    The number is formatted according to the current locale.
*/
void element::setAttribute(const pfs::string& name, qlonglong value)
{
    if (!impl)
        return;
    pfs::string x;
    x.setNum(value);
    IMPL->setAttribute(name, x);
}

/*!
    \overload

    The number is formatted according to the current locale.
*/
void element::setAttribute(const pfs::string& name, qulonglong value)
{
    if (!impl)
        return;
    pfs::string x;
    x.setNum(value);
    IMPL->setAttribute(name, x);
}

/*!
    \overload

    The number is formatted according to the current locale.
*/
void element::setAttribute(const pfs::string& name, float value)
{
    if (!impl)
        return;
    pfs::string x;
    x.setNum(value);
    IMPL->setAttribute(name, x);
}

/*!
    \overload

    The number is formatted according to the current locale.
*/
void element::setAttribute(const pfs::string& name, double value)
{
    if (!impl)
        return;
    pfs::string x;
    char buf[256];
    int count = qsnprintf(buf, sizeof(buf), "%.16g", value);
    if (count > 0)
        x = pfs::string::fromLatin1(buf, count);
    else
        x.setNum(value); // Fallback
    IMPL->setAttribute(name, x);
}

/*!
    Removes the attribute called name \a name from this element.

    \sa setAttribute(), attribute(), removeAttributeNS()
*/
void element::removeAttribute(const pfs::string& name)
{
    if (!impl)
        return;
    IMPL->removeAttribute(name);
}

/*!
    Returns the attr object that corresponds to the attribute
    called \a name. If no such attribute exists a
    \l{node::isNull()}{null attribute} is returned.

    \sa setAttributeNode(), attribute(), setAttribute(), attributeNodeNS()
*/
attr element::attributeNode(const pfs::string& name)
{
    if (!impl)
        return attr();
    return attr(IMPL->attributeNode(name));
}

/*!
    Adds the attribute \a newAttr to this element.

    If the element has another attribute that has the same name as \a
    newAttr, this function replaces that attribute and returns it;
    otherwise the function returns a
    \l{node::isNull()}{null attribute}.

    \sa attributeNode(), setAttribute(), setAttributeNodeNS()
*/
attr element::setAttributeNode(const attr& newAttr)
{
    if (!impl)
        return attr();
    return attr(IMPL->setAttributeNode(((attr_impl*)newAttr.impl)));
}

/*!
    Removes the attribute \a oldAttr from the element and returns it.

    \sa attributeNode(), setAttributeNode()
*/
attr element::removeAttributeNode(const attr& oldAttr)
{
    if (!impl)
        return attr(); // ### should this return oldAttr?
    return attr(IMPL->removeAttributeNode(((attr_impl*)oldAttr.impl)));
}

/*!
  Returns a nodelist containing all descendants of this element
  named \a tagname encountered during a preorder traversal of the
  element subtree with this element as its root. The order of the
  elements in the returned list is the order they are encountered
  during the preorder traversal.

  \sa elementsByTagNameNS(), document::elementsByTagName()
*/
nodelist element::elementsByTagName(const pfs::string& tagname) const
{
    return nodelist(new nodelist_impl(impl, tagname));
}



/*!
    Returns the attribute with the local name \a localName and the
    namespace URI \a nsURI. If the attribute does not exist \a
    defValue is returned.

    \sa setAttributeNS(), attributeNodeNS(), setAttributeNodeNS(), attribute()
*/
pfs::string element::attributeNS(const pfs::string nsURI, const pfs::string& localName, const pfs::string& defValue) const
{
    if (!impl)
        return defValue;
    return IMPL->attributeNS(nsURI, localName, defValue);
}

/*!
    Adds an attribute with the qualified name \a qName and the
    namespace URI \a nsURI with the value \a value. If an attribute
    with the same local name and namespace URI exists, its prefix is
    replaced by the prefix of \a qName and its value is repaced by \a
    value.

    Although \a qName is the qualified name, the local name is used to
    decide if an existing attribute's value should be replaced.

    \sa attributeNS(), setAttributeNodeNS(), setAttribute()
*/
void element::setAttributeNS(const pfs::string nsURI, const pfs::string& qName, const pfs::string& value)
{
    if (!impl)
        return;
    IMPL->setAttributeNS(nsURI, qName, value);
}

/*!
  \fn void element::setAttributeNS(const pfs::string nsURI, const pfs::string& qName, int value)

    \overload
*/

/*!
  \fn void element::setAttributeNS(const pfs::string nsURI, const pfs::string& qName, uint value)

    \overload
*/

/*!
    \overload
*/
void element::setAttributeNS(const pfs::string nsURI, const pfs::string& qName, qlonglong value)
{
    if (!impl)
        return;
    pfs::string x;
    x.setNum(value);
    IMPL->setAttributeNS(nsURI, qName, x);
}

/*!
    \overload
*/
void element::setAttributeNS(const pfs::string nsURI, const pfs::string& qName, qulonglong value)
{
    if (!impl)
        return;
    pfs::string x;
    x.setNum(value);
    IMPL->setAttributeNS(nsURI, qName, x);
}

/*!
    \overload
*/
void element::setAttributeNS(const pfs::string nsURI, const pfs::string& qName, double value)
{
    if (!impl)
        return;
    pfs::string x;
    x.setNum(value);
    IMPL->setAttributeNS(nsURI, qName, x);
}

/*!
    Removes the attribute with the local name \a localName and the
    namespace URI \a nsURI from this element.

    \sa setAttributeNS(), attributeNS(), removeAttribute()
*/
void element::removeAttributeNS(const pfs::string& nsURI, const pfs::string& localName)
{
    if (!impl)
        return;
    node_impl *n = IMPL->attributeNodeNS(nsURI, localName);
    if (!n)
        return;
    IMPL->removeAttribute(n->nodeName());
}

/*!
    Returns the attr object that corresponds to the attribute
    with the local name \a localName and the namespace URI \a nsURI.
    If no such attribute exists a \l{node::isNull()}{null
    attribute} is returned.

    \sa setAttributeNode(), attribute(), setAttribute()
*/
attr element::attributeNodeNS(const pfs::string& nsURI, const pfs::string& localName)
{
    if (!impl)
        return attr();
    return attr(IMPL->attributeNodeNS(nsURI, localName));
}

/*!
    Adds the attribute \a newAttr to this element.

    If the element has another attribute that has the same local name
    and namespace URI as \a newAttr, this function replaces that
    attribute and returns it; otherwise the function returns a
    \l{node::isNull()}{null attribute}.

    \sa attributeNodeNS(), setAttributeNS(), setAttributeNode()
*/
attr element::setAttributeNodeNS(const attr& newAttr)
{
    if (!impl)
        return attr();
    return attr(IMPL->setAttributeNodeNS(((attr_impl*)newAttr.impl)));
}

/*!
  Returns a nodelist containing all descendants of this element
  with local name \a localName and namespace URI \a nsURI encountered
  during a preorder traversal of the element subtree with this element
  as its root. The order of the elements in the returned list is the
  order they are encountered during the preorder traversal.

  \sa elementsByTagName(), document::elementsByTagNameNS()
*/
nodelist element::elementsByTagNameNS(const pfs::string& nsURI, const pfs::string& localName) const
{
    return nodelist(new nodelist_impl(impl, nsURI, localName));
}

/*!
    Returns true if this element has an attribute with the local name
    \a localName and the namespace URI \a nsURI; otherwise returns
    false.
*/
bool element::hasAttributeNS(const pfs::string& nsURI, const pfs::string& localName) const
{
    if (!impl)
        return false;
    return IMPL->hasAttributeNS(nsURI, localName);
}

/*!
    Returns the element's text or an empty string.

    Example:
    \snippet code/src_xml_dom_qdom.cpp 13

    The function text() of the element for the \c{<h1>} tag,
    will return the following text:

    \snippet code/src_xml_dom_qdom.cpp 14

    Comments are ignored by this function. It only evaluates text
    and cdatasection objects.
*/
pfs::string element::text() const
{
    if (!impl)
        return pfs::string();
    return IMPL->text();
}

#undef IMPL

/**************************************************************
 *
 * text_impl
 *
 **************************************************************/

text_impl::text_impl(document_impl* d, node_impl* parent, const pfs::string& val)
    : chardata_impl(d, parent, val)
{
    name = QLatin1String("#text");
}

text_impl::text_impl(text_impl* n, bool deep)
    : chardata_impl(n, deep)
{
}

node_impl* text_impl::cloneNode(bool deep)
{
    node_impl* p = new text_impl(this, deep);
    // We are not interested in this node
    p->ref.deref();
    return p;
}

text_impl* text_impl::splitText(int offset)
{
    if (!parent()) {
        qWarning("text::splitText  The node has no parent. So I can not split");
        return 0;
    }

    text_impl* t = new text_impl(ownerDocument(), 0, value.mid(offset));
    value.truncate(offset);

    parent()->insertAfter(t, this);

    return t;
}


/**************************************************************
 *
 * text
 *
 **************************************************************/

#define IMPL ((text_impl*)impl)

/*!
    \class text
    \reentrant
    \brief The text class represents text data in the parsed XML document.

    \inmodule QtXml
    \ingroup xml-tools

    You can split the text in a text object over two text
    objecs with splitText().

    For further information about the Document Object Model see
    \l{http://www.w3.org/TR/REC-DOM-Level-1/}{Level 1} and
    \l{http://www.w3.org/TR/DOM-Level-2-Core/}{Level 2 Core}.
    For a more general introduction of the DOM implementation see the
    document documentation.
*/

/*!
    Constructs an empty text object.

    To construct a text with content, use document::createTextNode().
*/
text::text()
    : chardata()
{
}

/*!
    Constructs a copy of \a x.

    The data of the copy is shared (shallow copy): modifying one node
    will also change the other. If you want to make a deep copy, use
    cloneNode().
*/
text::text(const text& x)
    : chardata(x)
{
}

text::text(text_impl* n)
    : chardata(n)
{
}

/*!
    Assigns \a x to this DOM text.

    The data of the copy is shared (shallow copy): modifying one node
    will also change the other. If you want to make a deep copy, use
    cloneNode().
*/
text& text::operator= (const text& x)
{
    return (text&) node::operator=(x);
}

/*!
    \fn node::type text::nodeType() const

    Returns \c TextNode.
*/

/*!
    Splits this DOM text object into two text objects. This object
    keeps its first \a offset characters and the second (newly
    created) object is inserted into the document tree after this
    object with the remaining characters.

    The function returns the newly created object.

    \sa node::normalize()
*/
text text::splitText(int offset)
{
    if (!impl)
        return text();
    return text(IMPL->splitText(offset));
}

#undef IMPL

/**************************************************************
 *
 * comment_impl
 *
 **************************************************************/

comment_impl::comment_impl(document_impl* d, node_impl* parent, const pfs::string& val)
    : chardata_impl(d, parent, val)
{
    name = QLatin1String("#comment");
}

comment_impl::comment_impl(comment_impl* n, bool deep)
    : chardata_impl(n, deep)
{
}


node_impl* comment_impl::cloneNode(bool deep)
{
    node_impl* p = new comment_impl(this, deep);
    // We are not interested in this node
    p->ref.deref();
    return p;
}

/**************************************************************
 *
 * comment
 *
 **************************************************************/

/*!
    \class comment
    \reentrant
    \brief The comment class represents an XML comment.

    \inmodule QtXml
    \ingroup xml-tools

    A comment in the parsed XML such as this:

    \snippet code/src_xml_dom_qdom.cpp 15

    is represented by comment objects in the parsed Dom tree.

    For further information about the Document Object Model see
    \l{W3C DOM Level 1}{Level 1} and
    \l{W3C DOM Level 2}{Level 2 Core}.
    For a more general introduction of the DOM implementation see the
    document documentation.
*/

/*!
    Constructs an empty comment. To construct a comment with content,
    use the document::createComment() function.
*/
comment::comment()
    : chardata()
{
}

/*!
    Constructs a copy of \a x.

    The data of the copy is shared (shallow copy): modifying one node
    will also change the other. If you want to make a deep copy, use
    cloneNode().
*/
comment::comment(const comment& x)
    : chardata(x)
{
}

comment::comment(comment_impl* n)
    : chardata(n)
{
}

/*!
    Assigns \a x to this DOM comment.

    The data of the copy is shared (shallow copy): modifying one node
    will also change the other. If you want to make a deep copy, use
    cloneNode().
*/
comment& comment::operator= (const comment& x)
{
    return (comment&) node::operator=(x);
}

/*!
    \fn node::type comment::nodeType() const

    Returns \c CommentNode.
*/

/**************************************************************
 *
 * cdatasection_impl
 *
 **************************************************************/

cdatasection_impl::cdatasection_impl(document_impl* d, node_impl* parent,
                                                    const pfs::string& val)
    : text_impl(d, parent, val)
{
    name = QLatin1String("#cdata-section");
}

cdatasection_impl::cdatasection_impl(cdatasection_impl* n, bool deep)
    : text_impl(n, deep)
{
}

node_impl* cdatasection_impl::cloneNode(bool deep)
{
    node_impl* p = new cdatasection_impl(this, deep);
    // We are not interested in this node
    p->ref.deref();
    return p;
}

/**************************************************************
 *
 * cdatasection
 *
 **************************************************************/

/*!
    \class cdatasection
    \reentrant
    \brief The cdatasection class represents an XML CDATA section.

    \inmodule QtXml
    \ingroup xml-tools

    CDATA sections are used to escape blocks of text containing
    characters that would otherwise be regarded as markup. The only
    delimiter that is recognized in a CDATA section is the "]]&gt;"
    string that terminates the CDATA section. CDATA sections cannot be
    nested. Their primary purpose is for including material such as
    XML fragments, without needing to escape all the delimiters.

    Adjacent cdatasection nodes are not merged by the
    node::normalize() function.

    For further information about the Document Object Model see
    \l{http://www.w3.org/TR/REC-DOM-Level-1/} and
    \l{http://www.w3.org/TR/DOM-Level-2-Core/}.
    For a more general introduction of the DOM implementation see the
    document documentation.
*/

/*!
    Constructs an empty CDATA section. To create a CDATA section with
    content, use the document::createCDATASection() function.
*/
cdatasection::cdatasection()
    : text()
{
}

/*!
    Constructs a copy of \a x.

    The data of the copy is shared (shallow copy): modifying one node
    will also change the other. If you want to make a deep copy, use
    cloneNode().
*/
cdatasection::cdatasection(const cdatasection& x)
    : text(x)
{
}

cdatasection::cdatasection(cdatasection_impl* n)
    : text(n)
{
}

/*!
    Assigns \a x to this CDATA section.

    The data of the copy is shared (shallow copy): modifying one node
    will also change the other. If you want to make a deep copy, use
    cloneNode().
*/
cdatasection& cdatasection::operator= (const cdatasection& x)
{
    return (cdatasection&) node::operator=(x);
}

/*!
    \fn node::type cdatasection::nodeType() const

    Returns \c CDATASection.
*/

/**************************************************************
 *
 * notation_impl
 *
 **************************************************************/

notation_impl::notation_impl(document_impl* d, node_impl* parent,
                                            const pfs::string& aname,
                                            const pfs::string& pub, const pfs::string& sys)
    : node_impl(d, parent)
{
    name = aname;
    m_pub = pub;
    m_sys = sys;
}

notation_impl::notation_impl(notation_impl* n, bool deep)
    : node_impl(n, deep)
{
    m_sys = n->m_sys;
    m_pub = n->m_pub;
}

node_impl* notation_impl::cloneNode(bool deep)
{
    node_impl* p = new notation_impl(this, deep);
    // We are not interested in this node
    p->ref.deref();
    return p;
}

/**************************************************************
 *
 * notation
 *
 **************************************************************/

#define IMPL ((notation_impl*)impl)

/*!
    \class notation
    \reentrant
    \brief The notation class represents an XML notation.

    \inmodule QtXml
    \ingroup xml-tools

    A notation either declares, by name, the format of an unparsed
    entity (see section 4.7 of the XML 1.0 specification), or is used
    for formal declaration of processing instruction targets (see
    section 2.6 of the XML 1.0 specification).

    DOM does not support editing notation nodes; they are therefore
    read-only.

    A notation node does not have any parent.

    You can retrieve the publicId() and systemId() from a notation
    node.

    For further information about the Document Object Model see
    \l{http://www.w3.org/TR/REC-DOM-Level-1/}{Level 1} and
    \l{http://www.w3.org/TR/DOM-Level-2-Core/}{Level 2 Core}.
    For a more general introduction of the DOM implementation see the
    document documentation.
*/


/*!
    Constructor.
*/
notation::notation()
    : node()
{
}

/*!
    Constructs a copy of \a x.

    The data of the copy is shared (shallow copy): modifying one node
    will also change the other. If you want to make a deep copy, use
    cloneNode().
*/
notation::notation(const notation& x)
    : node(x)
{
}

notation::notation(notation_impl* n)
    : node(n)
{
}

/*!
    Assigns \a x to this DOM notation.

    The data of the copy is shared (shallow copy): modifying one node
    will also change the other. If you want to make a deep copy, use
    cloneNode().
*/
notation& notation::operator= (const notation& x)
{
    return (notation&) node::operator=(x);
}

/*!
    \fn node::type notation::nodeType() const

    Returns \c NotationNode.
*/

/*!
    Returns the public identifier of this notation.
*/
pfs::string notation::publicId() const
{
    if (!impl)
        return pfs::string();
    return IMPL->m_pub;
}

/*!
    Returns the system identifier of this notation.
*/
pfs::string notation::systemId() const
{
    if (!impl)
        return pfs::string();
    return IMPL->m_sys;
}

#undef IMPL

/**************************************************************
 *
 * entity_impl
 *
 **************************************************************/

entity_impl::entity_impl(document_impl* d, node_impl* parent,
                                        const pfs::string& aname,
                                        const pfs::string& pub, const pfs::string& sys, const pfs::string& notation)
    : node_impl(d, parent)
{
    name = aname;
    m_pub = pub;
    m_sys = sys;
    m_notationName = notation;
}

entity_impl::entity_impl(entity_impl* n, bool deep)
    : node_impl(n, deep)
{
    m_sys = n->m_sys;
    m_pub = n->m_pub;
    m_notationName = n->m_notationName;
}

node_impl* entity_impl::cloneNode(bool deep)
{
    node_impl* p = new entity_impl(this, deep);
    // We are not interested in this node
    p->ref.deref();
    return p;
}

/*
  Encode an entity value upon saving.
*/
static QByteArray encodeEntity(const QByteArray& str)
{
    QByteArray tmp(str);
    int len = tmp.size();
    int i = 0;
    const char* d = tmp.constData();
    while (i < len) {
        if (d[i] == '%'){
            tmp.replace(i, 1, "&#60;");
            d = tmp.constData();
            len += 4;
            i += 5;
        }
        else if (d[i] == '"') {
            tmp.replace(i, 1, "&#34;");
            d = tmp.constData();
            len += 4;
            i += 5;
        } else if (d[i] == '&' && i + 1 < len && d[i+1] == '#') {
            // Don't encode &lt; or &quot; or &custom;.
            // Only encode character references
            tmp.replace(i, 1, "&#38;");
            d = tmp.constData();
            len += 4;
            i += 5;
        } else {
            ++i;
        }
    }

    return tmp;
}


/**************************************************************
 *
 * entity
 *
 **************************************************************/

#define IMPL ((entity_impl*)impl)

/*!
    \class entity
    \reentrant
    \brief The entity class represents an XML entity.

    \inmodule QtXml
    \ingroup xml-tools

    This class represents an entity in an XML document, either parsed
    or unparsed. Note that this models the entity itself not the
    entity declaration.

    DOM does not support editing entity nodes; if a user wants to make
    changes to the contents of an entity, every related
    entity_reference node must be replaced in the DOM tree by a
    clone of the entity's contents, and then the desired changes must
    be made to each of the clones instead. All the descendants of an
    entity node are read-only.

    An entity node does not have any parent.

    You can access the entity's publicId(), systemId() and
    notationName() when available.

    For further information about the Document Object Model see
    \l{http://www.w3.org/TR/REC-DOM-Level-1/}{Level 1} and
    \l{http://www.w3.org/TR/DOM-Level-2-Core/}{Level 2 Core}.
    For a more general introduction of the DOM implementation see the
    document documentation.
*/


/*!
    Constructs an empty entity.
*/
entity::entity()
    : node()
{
}


/*!
    Constructs a copy of \a x.

    The data of the copy is shared (shallow copy): modifying one node
    will also change the other. If you want to make a deep copy, use
    cloneNode().
*/
entity::entity(const entity& x)
    : node(x)
{
}

entity::entity(entity_impl* n)
    : node(n)
{
}

/*!
    Assigns \a x to this DOM entity.

    The data of the copy is shared (shallow copy): modifying one node
    will also change the other. If you want to make a deep copy, use
    cloneNode().
*/
entity& entity::operator= (const entity& x)
{
    return (entity&) node::operator=(x);
}

/*!
    \fn node::type entity::nodeType() const

    Returns \c EntityNode.
*/

/*!
    Returns the public identifier associated with this entity. If the
    public identifier was not specified an empty string is returned.
*/
pfs::string entity::publicId() const
{
    if (!impl)
        return pfs::string();
    return IMPL->m_pub;
}

/*!
    Returns the system identifier associated with this entity. If the
    system identifier was not specified an empty string is returned.
*/
pfs::string entity::systemId() const
{
    if (!impl)
        return pfs::string();
    return IMPL->m_sys;
}

/*!
    For unparsed entities this function returns the name of the
    notation for the entity. For parsed entities this function returns
    an empty string.
*/
pfs::string entity::notationName() const
{
    if (!impl)
        return pfs::string();
    return IMPL->m_notationName;
}

#undef IMPL

/**************************************************************
 *
 * entity_reference_impl
 *
 **************************************************************/

entity_reference_impl::entity_reference_impl(document_impl* d, node_impl* parent, const pfs::string& aname)
    : node_impl(d, parent)
{
    name = aname;
}

entity_reference_impl::entity_reference_impl(node_impl* n, bool deep)
    : node_impl(n, deep)
{
}

node_impl* entity_reference_impl::cloneNode(bool deep)
{
    node_impl* p = new entity_reference_impl(this, deep);
    // We are not interested in this node
    p->ref.deref();
    return p;
}


/**************************************************************
 *
 * entity_reference
 *
 **************************************************************/

/*!
    \class entity_reference
    \reentrant
    \brief The entity_reference class represents an XML entity reference.

    \inmodule QtXml
    \ingroup xml-tools

    A entity_reference object may be inserted into the DOM tree
    when an entity reference is in the source document, or when the
    user wishes to insert an entity reference.

    Note that character references and references to predefined
    entities are expanded by the XML processor so that characters are
    represented by their Unicode equivalent rather than by an entity
    reference.

    Moreover, the XML processor may completely expand references to
    entities while building the DOM tree, instead of providing
    entity_reference objects.

    If it does provide such objects, then for a given entity reference
    node, it may be that there is no entity node representing the
    referenced entity; but if such an entity exists, then the child
    list of the entity reference node is the same as that of the
    entity  node. As with the entity node, all descendants of the
    entity reference are read-only.

    For further information about the Document Object Model see
    \l{http://www.w3.org/TR/REC-DOM-Level-1/}{Level 1} and
    \l{http://www.w3.org/TR/DOM-Level-2-Core/}{Level 2 Core}.
    For a more general introduction of the DOM implementation see the
    document documentation.
*/

/*!
    Constructs an empty entity reference. Use
    document::createEntityReference() to create a entity reference
    with content.
*/
entity_reference::entity_reference()
    : node()
{
}

/*!
    Constructs a copy of \a x.

    The data of the copy is shared (shallow copy): modifying one node
    will also change the other. If you want to make a deep copy, use
    cloneNode().
*/
entity_reference::entity_reference(const entity_reference& x)
    : node(x)
{
}

entity_reference::entity_reference(entity_reference_impl* n)
    : node(n)
{
}

/*!
    Assigns \a x to this entity reference.

    The data of the copy is shared (shallow copy): modifying one node
    will also change the other. If you want to make a deep copy, use
    cloneNode().
*/
entity_reference& entity_reference::operator= (const entity_reference& x)
{
    return (entity_reference&) node::operator=(x);
}

/*!
    \fn node::type entity_reference::nodeType() const

    Returns \c EntityReference.
*/

/**************************************************************
 *
 * processing_instruction_impl
 *
 **************************************************************/

processing_instruction_impl::processing_instruction_impl(document_impl* d,
        node_impl* parent, const pfs::string& target, const pfs::string& data)
    : node_impl(d, parent)
{
    name = target;
    value = data;
}

processing_instruction_impl::processing_instruction_impl(processing_instruction_impl* n, bool deep)
    : node_impl(n, deep)
{
}


node_impl* processing_instruction_impl::cloneNode(bool deep)
{
    node_impl* p = new processing_instruction_impl(this, deep);
    // We are not interested in this node
    p->ref.deref();
    return p;
}


/**************************************************************
 *
 * processing_instruction
 *
 **************************************************************/

/*!
    \class processing_instruction
    \reentrant
    \brief The processing_instruction class represents an XML processing
    instruction.

    \inmodule QtXml
    \ingroup xml-tools

    Processing instructions are used in XML to keep processor-specific
    information in the text of the document.

    The XML declaration that appears at the top of an XML document,
    typically \tt{<?xml version='1.0' encoding='UTF-8'?>}, is treated by QDom as a
    processing instruction. This is unfortunate, since the XML declaration is
    not a processing instruction; among other differences, it cannot be
    inserted into a document anywhere but on the first line.

    Do not use this function to create an xml declaration, since although it
    has the same syntax as a processing instruction, it isn't, and might not
    be treated by QDom as such.

    The content of the processing instruction is retrieved with data()
    and set with setData(). The processing instruction's target is
    retrieved with target().

    For further information about the Document Object Model see
    \l{http://www.w3.org/TR/REC-DOM-Level-1/}{Level 1} and
    \l{http://www.w3.org/TR/DOM-Level-2-Core/}{Level 2 Core}.
    For a more general introduction of the DOM implementation see the
    document documentation.
*/

/*!
    Constructs an empty processing instruction. Use
    document::createProcessingInstruction() to create a processing
    instruction with content.
*/
processing_instruction::processing_instruction()
    : node()
{
}

/*!
    Constructs a copy of \a x.

    The data of the copy is shared (shallow copy): modifying one node
    will also change the other. If you want to make a deep copy, use
    cloneNode().
*/
processing_instruction::processing_instruction(const processing_instruction& x)
    : node(x)
{
}

processing_instruction::processing_instruction(processing_instruction_impl* n)
    : node(n)
{
}

/*!
    Assigns \a x to this processing instruction.

    The data of the copy is shared (shallow copy): modifying one node
    will also change the other. If you want to make a deep copy, use
    cloneNode().
*/
processing_instruction& processing_instruction::operator= (const processing_instruction& x)
{
    return (processing_instruction&) node::operator=(x);
}

/*!
    \fn node::type processing_instruction::nodeType() const

    Returns \c ProcessingInstructionNode.
*/

/*!
    Returns the target of this processing instruction.

    \sa data()
*/
pfs::string processing_instruction::target() const
{
    if (!impl)
        return pfs::string();
    return impl->nodeName();
}

/*!
    Returns the content of this processing instruction.

    \sa setData(), target()
*/
pfs::string processing_instruction::data() const
{
    if (!impl)
        return pfs::string();
    return impl->nodeValue();
}

/*!
    Sets the data contained in the processing instruction to \a d.

    \sa data()
*/
void processing_instruction::setData(const pfs::string& d)
{
    if (!impl)
        return;
    impl->setNodeValue(d);
}

/**************************************************************
 *
 * document_impl
 *
 **************************************************************/

document_impl::document_impl()
    : node_impl(0),
      impl(new dom_implementation_impl),
      nodeListTime(1)
{
    type = new document_type_impl(this, this);
    type->ref.deref();

    name = QLatin1String("#document");
}

document_impl::document_impl(const pfs::string& aname)
    : node_impl(0),
      impl(new dom_implementation_impl),
      nodeListTime(1)
{
    type = new document_type_impl(this, this);
    type->ref.deref();
    type->name = aname;

    name = QLatin1String("#document");
}

document_impl::document_impl(document_type_impl* dt)
    : node_impl(0),
      impl(new dom_implementation_impl),
      nodeListTime(1)
{
    if (dt != 0) {
        type = dt;
    } else {
        type = new document_type_impl(this, this);
        type->ref.deref();
    }

    name = QLatin1String("#document");
}

document_impl::document_impl(document_impl* n, bool deep)
    : node_impl(n, deep),
      impl(n->impl->clone()),
      nodeListTime(1)
{
    type = static_cast<document_type_impl*>(n->type->cloneNode());
    type->setParent(this);
}

document_impl::~document_impl()
{
}

void document_impl::clear()
{
    impl.reset();
    type.reset();
    node_impl::clear();
}

static void initializeReader(QXmlSimpleReader &reader, bool namespaceProcessing)
{
    reader.setFeature(QLatin1String("http://xml.org/sax/features/namespaces"), namespaceProcessing);
    reader.setFeature(QLatin1String("http://xml.org/sax/features/namespace-prefixes"), !namespaceProcessing);
    reader.setFeature(QLatin1String("http://trolltech.com/xml/features/report-whitespace-only-CharData"), false); // Shouldn't change in Qt 4
}

bool document_impl::setContent(QXmlInputSource *source, bool namespaceProcessing, pfs::string *errorMsg, int *errorLine, int *errorColumn)
{
    QXmlSimpleReader reader;
    initializeReader(reader, namespaceProcessing);
    return setContent(source, &reader, errorMsg, errorLine, errorColumn);
}

bool document_impl::setContent(QXmlInputSource *source, QXmlReader *reader, pfs::string *errorMsg, int *errorLine, int *errorColumn)
{
    clear();
    impl = new dom_implementation_impl;
    type = new document_type_impl(this, this);
    type->ref.deref();

    bool namespaceProcessing = reader->feature(QLatin1String("http://xml.org/sax/features/namespaces"))
        && !reader->feature(QLatin1String("http://xml.org/sax/features/namespace-prefixes"));

    QDomHandler hnd(this, namespaceProcessing);
    reader->setContentHandler(&hnd);
    reader->setErrorHandler(&hnd);
    reader->setLexicalHandler(&hnd);
    reader->setDeclHandler(&hnd);
    reader->setDTDHandler(&hnd);

    if (!reader->parse(source)) {
        if (errorMsg)
            *errorMsg = hnd.errorMsg;
        if (errorLine)
            *errorLine = hnd.errorLine;
        if (errorColumn)
            *errorColumn = hnd.errorColumn;
        return false;
    }

    return true;
}

node_impl* document_impl::cloneNode(bool deep)
{
    node_impl *p = new document_impl(this, deep);
    // We are not interested in this node
    p->ref.deref();
    return p;
}

element_impl* document_impl::documentElement()
{
    node_impl *p = first;
    while (p && !p->isElement())
        p = p->next;

    return static_cast<element_impl *>(p);
}

element_impl* document_impl::createElement(const pfs::string &tagName)
{
    bool ok;
    pfs::string fixedName = fixedXmlName(tagName, &ok);
    if (!ok)
        return 0;

    element_impl *e = new element_impl(this, 0, fixedName);
    e->ref.deref();
    return e;
}

element_impl* document_impl::createElementNS(const pfs::string &nsURI, const pfs::string &qName)
{
    bool ok;
    pfs::string fixedName = fixedXmlName(qName, &ok, true);
    if (!ok)
        return 0;

    element_impl *e = new element_impl(this, 0, nsURI, fixedName);
    e->ref.deref();
    return e;
}

document_fragment_impl* document_impl::createDocumentFragment()
{
    document_fragment_impl *f = new document_fragment_impl(this, (node_impl*)0);
    f->ref.deref();
    return f;
}

text_impl* document_impl::createTextNode(const pfs::string &data)
{
    bool ok;
    pfs::string fixedData = fixedCharData(data, &ok);
    if (!ok)
        return 0;

    text_impl *t = new text_impl(this, 0, fixedData);
    t->ref.deref();
    return t;
}

comment_impl* document_impl::createComment(const pfs::string &data)
{
    bool ok;
    pfs::string fixedData = fixedComment(data, &ok);
    if (!ok)
        return 0;

    comment_impl *c = new comment_impl(this, 0, fixedData);
    c->ref.deref();
    return c;
}

cdatasection_impl* document_impl::createCDATASection(const pfs::string &data)
{
    bool ok;
    pfs::string fixedData = fixedCDataSection(data, &ok);
    if (!ok)
        return 0;

    cdatasection_impl *c = new cdatasection_impl(this, 0, fixedData);
    c->ref.deref();
    return c;
}

processing_instruction_impl* document_impl::createProcessingInstruction(const pfs::string &target,
                                                                                   const pfs::string &data)
{
    bool ok;
    pfs::string fixedData = fixedPIData(data, &ok);
    if (!ok)
        return 0;
    // [17] PITarget ::= Name - (('X' | 'x') ('M' | 'm') ('L' | 'l'))
    pfs::string fixedTarget = fixedXmlName(target, &ok);
    if (!ok)
        return 0;

    processing_instruction_impl *p = new processing_instruction_impl(this, 0, fixedTarget, fixedData);
    p->ref.deref();
    return p;
}
attr_impl* document_impl::createAttribute(const pfs::string &aname)
{
    bool ok;
    pfs::string fixedName = fixedXmlName(aname, &ok);
    if (!ok)
        return 0;

    attr_impl *a = new attr_impl(this, 0, fixedName);
    a->ref.deref();
    return a;
}

attr_impl* document_impl::createAttributeNS(const pfs::string &nsURI, const pfs::string &qName)
{
    bool ok;
    pfs::string fixedName = fixedXmlName(qName, &ok, true);
    if (!ok)
        return 0;

    attr_impl *a = new attr_impl(this, 0, nsURI, fixedName);
    a->ref.deref();
    return a;
}

entity_reference_impl* document_impl::createEntityReference(const pfs::string &aname)
{
    bool ok;
    pfs::string fixedName = fixedXmlName(aname, &ok);
    if (!ok)
        return 0;

    entity_reference_impl *e = new entity_reference_impl(this, 0, fixedName);
    e->ref.deref();
    return e;
}

node_impl* document_impl::importNode(const node_impl *importedNode, bool deep)
{
    node_impl *node = 0;
    switch (importedNode->nodeType()) {
        case node::AttributeNode:
            node = new attr_impl((attr_impl*)importedNode, true);
            break;
        case node::DocumentFragmentNode:
            node = new document_fragment_impl((document_fragment_impl*)importedNode, deep);
            break;
        case node::ElementNode:
            node = new element_impl((element_impl*)importedNode, deep);
            break;
        case node::EntityNode:
            node = new entity_impl((entity_impl*)importedNode, deep);
            break;
        case node::EntityReferenceNode:
            node = new entity_reference_impl((entity_reference_impl*)importedNode, false);
            break;
        case node::NotationNode:
            node = new notation_impl((notation_impl*)importedNode, deep);
            break;
        case node::ProcessingInstructionNode:
            node = new processing_instruction_impl((processing_instruction_impl*)importedNode, deep);
            break;
        case node::TextNode:
            node = new text_impl((text_impl*)importedNode, deep);
            break;
        case node::CDATASectionNode:
            node = new cdatasection_impl((cdatasection_impl*)importedNode, deep);
            break;
        case node::CommentNode:
            node = new comment_impl((comment_impl*)importedNode, deep);
            break;
        default:
            break;
    }
    if (node) {
        node->setOwnerDocument(this);
        // The node constructor increases the refcount, so deref first to
        // keep refcount balanced.
        node->ref.deref();
    }
    return node;
}


/**************************************************************
 *
 * document
 *
 **************************************************************/

#define IMPL ((document_impl*)impl)

/*!
    Creates a document and sets the name of the document type to \a
    name.
*/
document::document(const pfs::string& name)
{
    // We take over ownership
    impl = new document_impl(name);
}

/*!
    Creates a document with the document type \a doctype.

    \sa dom_implementation::createDocumentType()
*/
document::document(const document_type& doctype)
{
    impl = new document_impl((document_type_impl*)(doctype.impl));
}

/*!
    Constructs a copy of \a x.

    The data of the copy is shared (shallow copy): modifying one node
    will also change the other. If you want to make a deep copy, use
    cloneNode().
*/
document::document(const document& x)
    : node(x)
{
}

document::document(document_impl* x)
    : node(x)
{
}

/*!
    Assigns \a x to this DOM document.

    The data of the copy is shared (shallow copy): modifying one node
    will also change the other. If you want to make a deep copy, use
    cloneNode().
*/
document& document::operator= (const document& x)
{
    return (document&) node::operator=(x);
}

/*!
    Returns the document type of this document.
*/
document_type document::doctype() const
{
    if (!impl)
        return document_type();
    return document_type(IMPL->doctype());
}

/*!
    Returns a dom_implementation object.
*/
dom_implementation document_implementation() const
{/*!
    \class document_fragment
    \reentrant
    \brief The document_fragment class is a tree of QDomNodes which is not usually a complete document.

    \inmodule QtXml
    \ingroup xml-tools

    If you want to do complex tree operations it is useful to have a
    lightweight class to store nodes and their relations.
    document_fragment stores a subtree of a document which does not
    necessarily represent a well-formed XML document.

    document_fragment is also useful if you want to group several
    nodes in a list and insert them all together as children of some
    node. In these cases document_fragment can be used as a
    temporary container for this list of children.

    The most important feature of document_fragment is that it is
    treated in a special way by node::insertAfter(),
    node::insertBefore(), node::replaceChild() and
    node::appendChild(): instead of inserting the fragment itself, all
    the fragment's children are inserted.
*/
    if (!impl)
        return dom_implementation();
    return dom_implementation(IMPL->implementation());
}

/*!
    Returns the root element of the document.
*/
element document::documentElement() const
{
    if (!impl)
        return element();
    return element(IMPL->documentElement());
}

/*!
    Creates a new element called \a tagName that can be inserted into
    the DOM tree, e.g. using node::appendChild().

    If \a tagName is not a valid XML name, the behavior of this function is governed
    by dom_implementation::InvalidDataPolicy.

    \sa createElementNS(), node::appendChild(), node::insertBefore(),
    node::insertAfter()
*/
element document::createElement(const pfs::string& tagName)
{
    if (!impl)
        impl = new document_impl();
    return element(IMPL->createElement(tagName));
}

/*!
    Creates a new document fragment, that can be used to hold parts of
    the document, e.g. when doing complex manipulations of the
    document tree.
*/
document_fragment document::createDocumentFragment()
{
    if (!impl)
        impl = new document_impl();
    return document_fragment(IMPL->createDocumentFragment());
}

/*!
    Creates a text node for the string \a value that can be inserted
    into the document tree, e.g. using node::appendChild().

    If \a value contains characters which cannot be stored as character
    data of an XML document (even in the form of character references), the
    behavior of this function is governed by dom_implementation::InvalidDataPolicy.

    \sa node::appendChild(), node::insertBefore(), node::insertAfter()
*/
text document::createTextNode(const pfs::string& value)
{
    if (!impl)
        impl = new document_impl();
    return text(IMPL->createTextNode(value));
}

/*!
    Creates a new comment for the string \a value that can be inserted
    into the document, e.g. using node::appendChild().

    If \a value contains characters which cannot be stored in an XML comment,
    the behavior of this function is governed by dom_implementation::InvalidDataPolicy.

    \sa node::appendChild(), node::insertBefore(), node::insertAfter()
*/
comment document::createComment(const pfs::string& value)
{
    if (!impl)
        impl = new document_impl();
    return comment(IMPL->createComment(value));
}

/*!
    Creates a new CDATA section for the string \a value that can be
    inserted into the document, e.g. using node::appendChild().

    If \a value contains characters which cannot be stored in a CDATA section,
    the behavior of this function is governed by
    dom_implementation::InvalidDataPolicy.

    \sa node::appendChild(), node::insertBefore(), node::insertAfter()
*/
cdatasection document::createCDATASection(const pfs::string& value)
{
    if (!impl)
        impl = new document_impl();
    return cdatasection(IMPL->createCDATASection(value));
}

/*!
    Creates a new processing instruction that can be inserted into the
    document, e.g. using node::appendChild(). This function sets
    the target for the processing instruction to \a target and the
    data to \a data.

    If \a target is not a valid XML name, or data if contains characters which cannot
    appear in a processing instruction, the behavior of this function is governed by
    dom_implementation::InvalidDataPolicy.

    \sa node::appendChild(), node::insertBefore(), node::insertAfter()
*/
processing_instruction document::createProcessingInstruction(const pfs::string& target,
                                                                    const pfs::string& data)
{
    if (!impl)
        impl = new document_impl();
    return processing_instruction(IMPL->createProcessingInstruction(target, data));
}


/*!
    Creates a new attribute called \a name that can be inserted into
    an element, e.g. using element::setAttributeNode().

    If \a name is not a valid XML name, the behavior of this function is governed by
    dom_implementation::InvalidDataPolicy.

    \sa createAttributeNS()
*/
attr document::createAttribute(const pfs::string& name)
{
    if (!impl)
        impl = new document_impl();
    return attr(IMPL->createAttribute(name));
}

/*!
    Creates a new entity reference called \a name that can be inserted
    into the document, e.g. using node::appendChild().

    If \a name is not a valid XML name, the behavior of this function is governed by
    dom_implementation::InvalidDataPolicy.

    \sa node::appendChild(), node::insertBefore(), node::insertAfter()
*/
entity_reference document::createEntityReference(const pfs::string& name)
{
    if (!impl)
        impl = new document_impl();
    return entity_reference(IMPL->createEntityReference(name));
}

/*!
    Returns a nodelist, that contains all the elements in the
    document with the name \a tagname. The order of the node list is
    the order they are encountered in a preorder traversal of the
    element tree.

    \sa elementsByTagNameNS(), element::elementsByTagName()
*/
nodelist document::elementsByTagName(const pfs::string& tagname) const
{
    return nodelist(new nodelist_impl(impl, tagname));
}

/*!
    Imports the node \a importedNode from another document to this
    document. \a importedNode remains in the original document; this
    function creates a copy that can be used within this document.

    This function returns the imported node that belongs to this
    document. The returned node has no parent. It is not possible to
    import document and document_type nodes. In those cases
    this function returns a \l{node::isNull()}{null node}.

    If \a deep is true, this function imports not only the node \a
    importedNode but its whole subtree; if it is false, only the \a
    importedNode is imported. The argument \a deep has no effect on
    attr and entity_reference nodes, since the descendants of
    attr nodes are always imported and those of
    entity_reference nodes are never imported.

    The behavior of this function is slightly different depending on
    the node types:
    \table
    \header \li Node Type \li Behavior
    \row \li attr
         \li The owner element is set to 0 and the specified flag is
            set to true in the generated attribute. The whole subtree
            of \a importedNode is always imported for attribute nodes:
            \a deep has no effect.
    \row \li document
         \li Document nodes cannot be imported.
    \row \li document_fragment
         \li If \a deep is true, this function imports the whole
            document fragment; otherwise it only generates an empty
            document fragment.
    \row \li document_type
         \li Document type nodes cannot be imported.
    \row \li element
         \li Attributes for which attr::specified() is true are
            also imported, other attributes are not imported. If \a
            deep is true, this function also imports the subtree of \a
            importedNode; otherwise it imports only the element node
            (and some attributes, see above).
    \row \li entity
         \li Entity nodes can be imported, but at the moment there is
            no way to use them since the document type is read-only in
            DOM level 2.
    \row \li entity_reference
         \li Descendants of entity reference nodes are never imported:
            \a deep has no effect.
    \row \li notation
         \li Notation nodes can be imported, but at the moment there is
            no way to use them since the document type is read-only in
            DOM level 2.
    \row \li processing_instruction
         \li The target and value of the processing instruction is
            copied to the new node.
    \row \li text
         \li The text is copied to the new node.
    \row \li cdatasection
         \li The text is copied to the new node.
    \row \li comment
         \li The text is copied to the new node.
    \endtable

    \sa element::setAttribute(), node::insertBefore(),
        node::insertAfter(), node::replaceChild(), node::removeChild(),
        node::appendChild()
*/
node document::importNode(const node& importedNode, bool deep)
{
    if (!impl)
        impl = new document_impl();
    return node(IMPL->importNode(importedNode.impl, deep));
}

/*!
    Creates a new element with namespace support that can be inserted
    into the DOM tree. The name of the element is \a qName and the
    namespace URI is \a nsURI. This function also sets
    node::prefix() and node::localName() to appropriate values
    (depending on \a qName).

    If \a qName is an empty string, returns a null element regardless of
    whether the invalid data policy is set.

    \sa createElement()
*/
element document::createElementNS(const pfs::string& nsURI, const pfs::string& qName)
{
    if (!impl)
        impl = new document_impl();
    return element(IMPL->createElementNS(nsURI, qName));
}

/*!
    Creates a new attribute with namespace support that can be
    inserted into an element. The name of the attribute is \a qName
    and the namespace URI is \a nsURI. This function also sets
    node::prefix() and node::localName() to appropriate values
    (depending on \a qName).

    If \a qName is not a valid XML name, the behavior of this function is governed by
    dom_implementation::InvalidDataPolicy.

    \sa createAttribute()
*/
attr document::createAttributeNS(const pfs::string& nsURI, const pfs::string& qName)
{
    if (!impl)
        impl = new document_impl();
    return attr(IMPL->createAttributeNS(nsURI, qName));
}

/*!
    Returns a nodelist that contains all the elements in the
    document with the local name \a localName and a namespace URI of
    \a nsURI. The order of the node list is the order they are
    encountered in a preorder traversal of the element tree.

    \sa elementsByTagName(), element::elementsByTagNameNS()
*/
nodelist document::elementsByTagNameNS(const pfs::string& nsURI, const pfs::string& localName)
{
    return nodelist(new nodelist_impl(impl, nsURI, localName));
}

/*!
    Returns the element whose ID is equal to \a elementId. If no
    element with the ID was found, this function returns a
    \l{node::isNull()}{null element}.

    Since the QDomClasses do not know which attributes are element
    IDs, this function returns always a
    \l{node::isNull()}{null element}.
    This may change in a future version.
*/
element document::elementById(const pfs::string& /*elementId*/)
{
    qWarning("elementById() is not implemented and will always return a null node.");
    return element();
}

/*!
    \fn node::type document::nodeType() const

    Returns \c DocumentNode.
*/

#undef IMPL


#endif
