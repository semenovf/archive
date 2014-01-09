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


/**************************************************************
 *
 * namednodemap
 *
 **************************************************************/

/*!
    Returns true if \a n and this named node map are equal; otherwise
    returns false.
*/
bool namednodemap::operator== (const namednodemap& n) const
{
    return (impl == n.impl);
}

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
