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
dom_implementation::InvalidDataPolicy dom_implementation::impl::invalidDataPolicy
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

    if (dom_implementation::impl::invalidDataPolicy == dom_implementation::AcceptInvalidChars) {
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
            } else if (dom_implementation::impl::invalidDataPolicy == dom_implementation::ReturnNullNode) {
                *ok = false;
                return pfs::string();
            }
        } else {
            if (QXmlUtils::isNameChar(c))
                result.append(c);
            else if (dom_implementation::impl::invalidDataPolicy == dom_implementation::ReturnNullNode) {
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
    if (dom_implementation::impl::invalidDataPolicy == dom_implementation::AcceptInvalidChars) {
        *ok = true;
        return data;
    }

    pfs::string result;
    for (int i = 0; i < data.size(); ++i) {
        QChar c = data.at(i);
        if (QXmlUtils::isChar(c)) {
            result.append(c);
        } else if (dom_implementation::impl::invalidDataPolicy == dom_implementation::ReturnNullNode) {
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
    if (dom_implementation::impl::invalidDataPolicy == dom_implementation::AcceptInvalidChars) {
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
        if (dom_implementation::impl::invalidDataPolicy == dom_implementation::ReturnNullNode) {
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
    if (dom_implementation::impl::invalidDataPolicy == dom_implementation::AcceptInvalidChars) {
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
        if (dom_implementation::impl::invalidDataPolicy == dom_implementation::ReturnNullNode) {
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
    if (dom_implementation::impl::invalidDataPolicy == dom_implementation::AcceptInvalidChars) {
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
        if (dom_implementation::impl::invalidDataPolicy == dom_implementation::ReturnNullNode) {
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
    if (dom_implementation::impl::invalidDataPolicy == dom_implementation::AcceptInvalidChars) {
        *ok = true;
        return data;
    }

    pfs::string result;

    if(QXmlUtils::isPublicID(data))
        result = data;
    else if (dom_implementation::impl::invalidDataPolicy == dom_implementation::ReturnNullNode) {
        *ok = false;
        return pfs::string();
    }

    if (result.indexOf(QLatin1Char('\'')) != -1
        && result.indexOf(QLatin1Char('"')) != -1) {
        if (dom_implementation::impl::invalidDataPolicy == dom_implementation::ReturnNullNode) {
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
    if (dom_implementation::impl::invalidDataPolicy == dom_implementation::AcceptInvalidChars) {
        *ok = true;
        return data;
    }

    pfs::string result = data;

    if (result.indexOf(QLatin1Char('\'')) != -1
        && result.indexOf(QLatin1Char('"')) != -1) {
        if (dom_implementation::impl::invalidDataPolicy == dom_implementation::ReturnNullNode) {
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
 * dom_implementation::impl
 *
 **************************************************************/

dom_implementation::impl* dom_implementation::impl::clone()
{
    return new dom_implementation::impl;
}

/**************************************************************
 *
 * dom_implementation
 *
 **************************************************************/



/**************************************************************
 *
 * QDomNodeListPrivate
 *
 **************************************************************/

QDomNodeListPrivate::QDomNodeListPrivate(node::impl *n_impl) : ref(1)
{
    node_impl = n_impl;
    if (node_impl)
        node_impl->ref.ref();
    timestamp = 0;
}

QDomNodeListPrivate::QDomNodeListPrivate(node::impl *n_impl, const pfs::string &name) :
    ref(1)
{
    node_impl = n_impl;
    if (node_impl)
        node_impl->ref.ref();
    tagname = name;
    timestamp = 0;
}

QDomNodeListPrivate::QDomNodeListPrivate(node::impl *n_impl, const pfs::string &_nsURI, const pfs::string &localName) :
    ref(1)
{
    node_impl = n_impl;
    if (node_impl)
        node_impl->ref.ref();
    tagname = localName;
    nsURI = _nsURI;
    timestamp = 0;
}

QDomNodeListPrivate::~QDomNodeListPrivate()
{
    if (node_impl && !node_impl->ref.deref())
        delete node_impl;
}

bool QDomNodeListPrivate::operator==(const QDomNodeListPrivate &other) const
{
    return (node_impl == other.node_impl) && (tagname == other.tagname);
}

bool QDomNodeListPrivate::operator!=(const QDomNodeListPrivate &other) const
{
    return (node_impl != other.node_impl) || (tagname != other.tagname);
}

void QDomNodeListPrivate::createList()
{
    if (!node_impl)
        return;

    const document::impl *const doc = node_impl->ownerDocument();
    if (doc && timestamp != doc->nodeListTime)
        timestamp = doc->nodeListTime;

    node::impl* p = node_impl->first;

    list.clear();
    if (tagname.isNull()) {
        while (p) {
            list.append(p);
            p = p->next;
        }
    } else if (nsURI.isNull()) {
        while (p && p != node_impl) {
            if (p->isElement() && p->nodeName() == tagname) {
                list.append(p);
            }
            if (p->first)
                p = p->first;
            else if (p->next)
                p = p->next;
            else {
                p = p->parent();
                while (p && p != node_impl && !p->next)
                    p = p->parent();
                if (p && p != node_impl)
                    p = p->next;
            }
        }
    } else {
        while (p && p != node_impl) {
            if (p->isElement() && p->name==tagname && p->namespaceURI==nsURI) {
                list.append(p);
            }
            if (p->first)
                p = p->first;
            else if (p->next)
                p = p->next;
            else {
                p = p->parent();
                while (p && p != node_impl && !p->next)
                    p = p->parent();
                if (p && p != node_impl)
                    p = p->next;
            }
        }
    }
}

node::impl* QDomNodeListPrivate::item(int index)
{
    if (!node_impl)
        return 0;

    const document::impl *const doc = node_impl->ownerDocument();
    if (!doc || timestamp != doc->nodeListTime)
        createList();

    if (index >= list.size())
        return 0;

    return list.at(index);
}

int QDomNodeListPrivate::length() const
{
    if (!node_impl)
        return 0;

    const document::impl *const doc = node_impl->ownerDocument();
    if (!doc || timestamp != doc->nodeListTime) {
        QDomNodeListPrivate *that = const_cast<QDomNodeListPrivate *>(this);
        that->createList();
    }

    return list.count();
}

/**************************************************************
 *
 * QDomNodeList
 *
 **************************************************************/

/*!
    \class QDomNodeList
    \reentrant
    \brief The QDomNodeList class is a list of node objects.

    \inmodule QtXml
    \ingroup xml-tools

    Lists can be obtained by document::elementsByTagName() and
    node::childNodes(). The Document Object Model (DOM) requires
    these lists to be "live": whenever you change the underlying
    document, the contents of the list will get updated.

    You can get a particular node from the list with item(). The
    number of items in the list is returned by length().

    For further information about the Document Object Model see
    \l{http://www.w3.org/TR/REC-DOM-Level-1/}{Level 1} and
    \l{http://www.w3.org/TR/DOM-Level-2-Core/}{Level 2 Core}.
    For a more general introduction of the DOM implementation see the
    document documentation.

    \sa node::childNodes(), document::elementsByTagName()
*/

/*!
    Creates an empty node list.
*/
QDomNodeList::QDomNodeList()
{
    impl = 0;
}

QDomNodeList::QDomNodeList(QDomNodeListPrivate* p)
{
    impl = p;
}

/*!
    Constructs a copy of \a n.
*/
QDomNodeList::QDomNodeList(const QDomNodeList& n)
{
    impl = n.impl;
    if (impl)
        impl->ref.ref();
}

/*!
    Assigns \a n to this node list.
*/
QDomNodeList& QDomNodeList::operator=(const QDomNodeList &n)
{
    if (n.impl)
        n.impl->ref.ref();
    if (impl && !impl->ref.deref())
        delete impl;
    impl = n.impl;
    return *this;
}

/*!
    Returns true if the node list \a n and this node list are equal;
    otherwise returns false.
*/
bool QDomNodeList::operator==(const QDomNodeList &n) const
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
bool QDomNodeList::operator!=(const QDomNodeList &n) const
{
    return !operator==(n);
}

/*!
    Destroys the object and frees its resources.
*/
QDomNodeList::~QDomNodeList()
{
    if (impl && !impl->ref.deref())
        delete impl;
}

/*!
    Returns the node at position \a index.

    If \a index is negative or if \a index >= length() then a null
    node is returned (i.e. a node for which node::isNull() returns
    true).

    \sa length()
*/
node QDomNodeList::item(int index) const
{
    if (!impl)
        return node();

    return node(impl->item(index));
}

/*!
    Returns the number of nodes in the list.
*/
int QDomNodeList::length() const
{
    if (!impl)
        return 0;
    return impl->length();
}

/*!
    \fn bool QDomNodeList::isEmpty() const

    Returns true if the list contains no items; otherwise returns false.
    This function is provided for Qt API consistency.
*/

/*!
    \fn int QDomNodeList::count() const

    This function is provided for Qt API consistency. It is equivalent to length().
*/

/*!
    \fn int QDomNodeList::size() const

    This function is provided for Qt API consistency. It is equivalent to length().
*/

/*!
    \fn node QDomNodeList::at(int index) const

    This function is provided for Qt API consistency. It is equivalent
    to item().

    If \a index is negative or if \a index >= length() then a null
    node is returned (i.e. a node for which node::isNull() returns
    true).
*/

/**************************************************************
 *
 * node::impl
 *
 **************************************************************/

inline void node::impl::setOwnerDocument(document::impl *doc)
{
    ownerNode = doc;
    hasParent = false;
}

node::impl::node::impl(document::impl *doc, node::impl *par) : ref(1)
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

node::impl::node::impl(node::impl *n, bool deep) : ref(1)
{
    setOwnerDocument(n->ownerDocument());
    prev = 0;
    next = 0;
    first = 0;
    last = 0;

    name = n->name;
    value = n->value;
    prefix = n->prefix;
    namespaceURI = n->namespaceURI;
    createdWithDom1Interface = n->createdWithDom1Interface;
    lineNumber = -1;
    columnNumber = -1;

    if (!deep)
        return;

    for (node::impl* x = n->first; x; x = x->next)
        appendChild(x->cloneNode(true));
}

node::impl::~node::impl()
{
    node::impl* p = first;
    node::impl* n;

    while (p) {
        n = p->next;
        if (!p->ref.deref())
            delete p;
        else
            p->setNoParent();
        p = n;
    }
    first = 0;
    last = 0;
}

void node::impl::clear()
{
    node::impl* p = first;
    node::impl* n;

    while (p) {
        n = p->next;
        if (!p->ref.deref())
            delete p;
        p = n;
    }
    first = 0;
    last = 0;
}

node::impl* node::impl::namedItem(const pfs::string &n)
{
    node::impl* p = first;
    while (p) {
        if (p->nodeName() == n)
            return p;
        p = p->next;
    }
    return 0;
}


node::impl* node::impl::insertBefore(node::impl* newChild, node::impl* refChild)
{
    // Error check
    if (!newChild)
        return 0;

    // Error check
    if (newChild == refChild)
        return 0;

    // Error check
    if (refChild && refChild->parent() != this)
        return 0;

    // "mark lists as dirty"
    document::impl *const doc = ownerDocument();
    if(doc)
        doc->nodeListTime++;

    // Special handling for inserting a fragment. We just insert
    // all elements of the fragment instead of the fragment itself.
    if (newChild->isDocumentFragment()) {
        // Fragment is empty ?
        if (newChild->first == 0)
            return newChild;

        // New parent
        node::impl* n = newChild->first;
        while (n)  {
            n->setParent(this);
            n = n->next;
        }

        // Insert at the beginning ?
        if (!refChild || refChild->prev == 0) {
            if (first)
                first->prev = newChild->last;
            newChild->last->next = first;
            if (!last)
                last = newChild->last;
            first = newChild->first;
        } else {
            // Insert in the middle
            newChild->last->next = refChild;
            newChild->first->prev = refChild->prev;
            refChild->prev->next = newChild->first;
            refChild->prev = newChild->last;
        }

        // No need to increase the reference since document_fragment
        // does not decrease the reference.

        // Remove the nodes from the fragment
        newChild->first = 0;
        newChild->last = 0;
        return newChild;
    }

    // No more errors can occur now, so we take
    // ownership of the node.
    newChild->ref.ref();

    if (newChild->parent())
        newChild->parent()->removeChild(newChild);

    newChild->setParent(this);

    if (!refChild) {
        if (first)
            first->prev = newChild;
        newChild->next = first;
        if (!last)
            last = newChild;
        first = newChild;
        return newChild;
    }

    if (refChild->prev == 0) {
        if (first)
            first->prev = newChild;
        newChild->next = first;
        if (!last)
            last = newChild;
        first = newChild;
        return newChild;
    }

    newChild->next = refChild;
    newChild->prev = refChild->prev;
    refChild->prev->next = newChild;
    refChild->prev = newChild;

    return newChild;
}

node::impl* node::impl::insertAfter(node::impl* newChild, node::impl* refChild)
{
    // Error check
    if (!newChild)
        return 0;

    // Error check
    if (newChild == refChild)
        return 0;

    // Error check
    if (refChild && refChild->parent() != this)
        return 0;

    // "mark lists as dirty"
    document::impl *const doc = ownerDocument();
    if(doc)
        doc->nodeListTime++;

    // Special handling for inserting a fragment. We just insert
    // all elements of the fragment instead of the fragment itself.
    if (newChild->isDocumentFragment()) {
        // Fragment is empty ?
        if (newChild->first == 0)
            return newChild;

        // New parent
        node::impl* n = newChild->first;
        while (n) {
            n->setParent(this);
            n = n->next;
        }

        // Insert at the end
        if (!refChild || refChild->next == 0) {
            if (last)
                last->next = newChild->first;
            newChild->first->prev = last;
            if (!first)
                first = newChild->first;
            last = newChild->last;
        } else { // Insert in the middle
            newChild->first->prev = refChild;
            newChild->last->next = refChild->next;
            refChild->next->prev = newChild->last;
            refChild->next = newChild->first;
        }

        // No need to increase the reference since document_fragment
        // does not decrease the reference.

        // Remove the nodes from the fragment
        newChild->first = 0;
        newChild->last = 0;
        return newChild;
    }

    // Release new node from its current parent
    if (newChild->parent())
        newChild->parent()->removeChild(newChild);

    // No more errors can occur now, so we take
    // ownership of the node
    newChild->ref.ref();

    newChild->setParent(this);

    // Insert at the end
    if (!refChild) {
        if (last)
            last->next = newChild;
        newChild->prev = last;
        if (!first)
            first = newChild;
        last = newChild;
        return newChild;
    }

    if (refChild->next == 0) {
        if (last)
            last->next = newChild;
        newChild->prev = last;
        if (!first)
            first = newChild;
        last = newChild;
        return newChild;
    }

    newChild->prev = refChild;
    newChild->next = refChild->next;
    refChild->next->prev = newChild;
    refChild->next = newChild;

    return newChild;
}

node::impl* node::impl::replaceChild(node::impl* newChild, node::impl* oldChild)
{
    if (!newChild || !oldChild)
        return 0;
    if (oldChild->parent() != this)
        return 0;
    if (newChild == oldChild)
        return 0;

    // mark lists as dirty
    document::impl *const doc = ownerDocument();
    if(doc)
        doc->nodeListTime++;

    // Special handling for inserting a fragment. We just insert
    // all elements of the fragment instead of the fragment itself.
    if (newChild->isDocumentFragment()) {
        // Fragment is empty ?
        if (newChild->first == 0)
            return newChild;

        // New parent
        node::impl* n = newChild->first;
        while (n) {
            n->setParent(this);
            n = n->next;
        }


        if (oldChild->next)
            oldChild->next->prev = newChild->last;
        if (oldChild->prev)
            oldChild->prev->next = newChild->first;

        newChild->last->next = oldChild->next;
        newChild->first->prev = oldChild->prev;

        if (first == oldChild)
            first = newChild->first;
        if (last == oldChild)
            last = newChild->last;

        oldChild->setNoParent();
        oldChild->next = 0;
        oldChild->prev = 0;

        // No need to increase the reference since document_fragment
        // does not decrease the reference.

        // Remove the nodes from the fragment
        newChild->first = 0;
        newChild->last = 0;

        // We are no longer interested in the old node
        if (oldChild)
            oldChild->ref.deref();

        return oldChild;
    }

    // No more errors can occur now, so we take
    // ownership of the node
    newChild->ref.ref();

    // Release new node from its current parent
    if (newChild->parent())
        newChild->parent()->removeChild(newChild);

    newChild->setParent(this);

    if (oldChild->next)
        oldChild->next->prev = newChild;
    if (oldChild->prev)
        oldChild->prev->next = newChild;

    newChild->next = oldChild->next;
    newChild->prev = oldChild->prev;

    if (first == oldChild)
        first = newChild;
    if (last == oldChild)
        last = newChild;

    oldChild->setNoParent();
    oldChild->next = 0;
    oldChild->prev = 0;

    // We are no longer interested in the old node
    if (oldChild)
        oldChild->ref.deref();

    return oldChild;
}

node::impl* node::impl::removeChild(node::impl* oldChild)
{
    // Error check
    if (oldChild->parent() != this)
        return 0;

    // "mark lists as dirty"
    document::impl *const doc = ownerDocument();
    if(doc)
        doc->nodeListTime++;

    // Perhaps oldChild was just created with "createElement" or that. In this case
    // its parent is document but it is not part of the documents child list.
    if (oldChild->next == 0 && oldChild->prev == 0 && first != oldChild)
        return 0;

    if (oldChild->next)
        oldChild->next->prev = oldChild->prev;
    if (oldChild->prev)
        oldChild->prev->next = oldChild->next;

    if (last == oldChild)
        last = oldChild->prev;
    if (first == oldChild)
        first = oldChild->next;

    oldChild->setNoParent();
    oldChild->next = 0;
    oldChild->prev = 0;

    // We are no longer interested in the old node
    oldChild->ref.deref();

    return oldChild;
}

node::impl* node::impl::appendChild(node::impl* newChild)
{
    // No reference manipulation needed. Done in insertAfter.
    return insertAfter(newChild, 0);
}

document::impl* node::impl::ownerDocument()
{
    node::impl* p = this;
    while (p && !p->isDocument()) {
        if (!p->hasParent)
            return (document::impl*)p->ownerNode;
        p = p->parent();
    }

    return static_cast<document::impl *>(p);
}

node::impl* node::impl::cloneNode(bool deep)
{
    node::impl* p = new node::impl(this, deep);
    // We are not interested in this node
    p->ref.deref();
    return p;
}

static void qNormalizeNode(node::impl* n)
{
    node::impl* p = n->first;
    text::impl* t = 0;

    while (p) {
        if (p->isText()) {
            if (t) {
                node::impl* tmp = p->next;
                t->appendData(p->nodeValue());
                n->removeChild(p);
                p = tmp;
            } else {
                t = (text::impl*)p;
                p = p->next;
            }
        } else {
            p = p->next;
            t = 0;
        }
    }
}
void node::impl::normalize()
{
    // ### This one has moved from element::impl to this position. It is
    // not tested.
    qNormalizeNode(this);
}

void node::impl::setLocation(int lineNumber, int columnNumber)
{
    this->lineNumber = lineNumber;
    this->columnNumber = columnNumber;
}

/**************************************************************
 *
 * node
 *
 **************************************************************/

#define IMPL ((node::impl*)impl)

/*!
    \class node
    \reentrant
    \brief The node class is the base class for all the nodes in a DOM tree.

    \inmodule QtXml
    \ingroup xml-tools


    Many functions in the DOM return a node.

    You can find out the type of a node using isAttr(),
    isCDATASection(), isDocumentFragment(), isDocument(),
    isDocumentType(), isElement(), isEntityReference(), isText(),
    isEntity(), isNotation(), isProcessingInstruction(),
    isCharacterData() and isComment().

    A node can be converted into one of its subclasses using
    toAttr(), toCDATASection(), toDocumentFragment(), toDocument(),
    toDocumentType(), toElement(), toEntityReference(), toText(),
    toEntity(), toNotation(), toProcessingInstruction(),
    toCharacterData() or toComment(). You can convert a node to a null
    node with clear().

    Copies of the node class share their data using explicit
    sharing. This means that modifying one node will change all
    copies. This is especially useful in combination with functions
    which return a node, e.g. firstChild(). You can make an
    independent (deep) copy of the node with cloneNode().

    A node can be null, much like a null pointer. Creating a copy
    of a null node results in another null node. It is not
    possible to modify a null node, but it is possible to assign another,
    possibly non-null node to it. In this case, the copy of the null node
    will remain null. You can check if a node is null by calling isNull().
    The empty constructor of a node (or any of the derived classes) creates
    a null node.

    Nodes are inserted with insertBefore(), insertAfter() or
    appendChild(). You can replace one node with another using
    replaceChild() and remove a node with removeChild().

    To traverse nodes use firstChild() to get a node's first child (if
    any), and nextSibling() to traverse. node also provides
    lastChild(), previousSibling() and parentNode(). To find the first
    child node with a particular node name use namedItem().

    To find out if a node has children use hasChildNodes() and to get
    a list of all of a node's children use childNodes().

    The node's name and value (the meaning of which varies depending
    on its type) is returned by nodeName() and nodeValue()
    respectively. The node's type is returned by nodeType(). The
    node's value can be set with setNodeValue().

    The document to which the node belongs is returned by
    ownerDocument().

    Adjacent text nodes can be merged into a single node with
    normalize().

    \l element nodes have attributes which can be retrieved with
    attributes().

    element and attr nodes can have namespaces which can be
    retrieved with namespaceURI(). Their local name is retrieved with
    localName(), and their prefix with prefix(). The prefix can be set
    with setPrefix().

    You can write the XML representation of the node to a text stream
    with save().

    The following example looks for the first element in an XML document and
    prints the names of all the elements that are its direct children.

    \snippet code/src_xml_dom_qdom.cpp 1

    For further information about the Document Object Model see
    \l{W3C DOM Level 1}{Level 1} and
    \l{W3C DOM Level 2}{Level 2 Core}.
    For a more general introduction of the DOM implementation see the
    document documentation.
*/

/*!
    Constructs a \l{isNull()}{null} node.
*/
node::node()
{
    impl = 0;
}

/*!
    Constructs a copy of \a n.

    The data of the copy is shared (shallow copy): modifying one node
    will also change the other. If you want to make a deep copy, use
    cloneNode().
*/
node::node(const node &n)
{
    impl = n.impl;
    if (impl)
        impl->ref.ref();
}

/*!  \internal
  Constructs a new node for the data \a n.
*/
node::node(node::impl *n)
{
    impl = n;
    if (impl)
        impl->ref.ref();
}

/*!
    Assigns a copy of \a n to this DOM node.

    The data of the copy is shared (shallow copy): modifying one node
    will also change the other. If you want to make a deep copy, use
    cloneNode().
*/
node& node::operator=(const node &n)
{
    if (n.impl)
        n.impl->ref.ref();
    if (impl && !impl->ref.deref())
        delete impl;
    impl = n.impl;
    return *this;
}

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
    Destroys the object and frees its resources.
*/
node::~node()
{
    if (impl && !impl->ref.deref())
        delete impl;
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
    Sets the node's value to \a v.

    \sa nodeValue()
*/
void node::setNodeValue(const pfs::string& v)
{
    if (!impl)
        return;
    IMPL->setNodeValue(v);
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
    Returns the type of the node.

    \sa toAttr(), toCDATASection(), toDocumentFragment(),
    toDocument(), toDocumentType(), toElement(), toEntityReference(),
    toText(), toEntity(), toNotation(), toProcessingInstruction(),
    toCharacterData(), toComment()
*/
node::type node::nodeType() const
{
    if (!impl)
        return node::BaseNode;
    return IMPL->nodeType();
}

/*!
    Returns the parent node. If this node has no parent, a null node
    is returned (i.e. a node for which isNull() returns true).
*/
node node::parentNode() const
{
    if (!impl)
        return node();
    return node(IMPL->parent());
}

/*!
    Returns a list of all direct child nodes.

    Most often you will call this function on a element object.

    For example, if the XML document looks like this:

    \snippet code/src_xml_dom_qdom.cpp 4

    Then the list of child nodes for the "body"-element will contain
    the node created by the &lt;h1&gt; tag and the node created by the
    &lt;p&gt; tag.

    The nodes in the list are not copied; so changing the nodes in the
    list will also change the children of this node.

    \sa firstChild(), lastChild()
*/
QDomNodeList node::childNodes() const
{
    if (!impl)
        return QDomNodeList();
    return QDomNodeList(new QDomNodeListPrivate(impl));
}

/*!
    Returns the first child of the node. If there is no child node, a
    \l{isNull()}{null node} is returned. Changing the
    returned node will also change the node in the document tree.

    \sa lastChild(), childNodes()
*/
node node::firstChild() const
{
    if (!impl)
        return node();
    return node(IMPL->first);
}

/*!
    Returns the last child of the node. If there is no child node, a
    \l{isNull()}{null node} is returned. Changing the
    returned node will also change the node in the document tree.

    \sa firstChild(), childNodes()
*/
node node::lastChild() const
{
    if (!impl)
        return node();
    return node(IMPL->last);
}

/*!
    Returns the previous sibling in the document tree. Changing the
    returned node will also change the node in the document tree.

    For example, if you have XML like this:

    \snippet code/src_xml_dom_qdom.cpp 5

    and this node represents the &lt;p&gt; tag, previousSibling()
    will return the node representing the &lt;h1&gt; tag.

    \sa nextSibling()
*/
node node::previousSibling() const
{
    if (!impl)
        return node();
    return node(IMPL->prev);
}

/*!
    Returns the next sibling in the document tree. Changing the
    returned node will also change the node in the document tree.

    If you have XML like this:

    \snippet code/src_xml_dom_qdom.cpp 6

    and this node represents the <p> tag, nextSibling() will
    return the node representing the <h2> tag.

    \sa previousSibling()
*/
node node::nextSibling() const
{
    if (!impl)
        return node();
    return node(IMPL->next);
}


// ###### don't think this is part of the DOM and
/*!
    Returns a named node map of all attributes. Attributes are only
    provided for \l{element}s.

    Changing the attributes in the map will also change the attributes
    of this node.
*/
QDomNamedNodeMap node::attributes() const
{
    if (!impl || !impl->isElement())
        return QDomNamedNodeMap();

    return QDomNamedNodeMap(static_cast<element::impl *>(impl)->attributes());
}

/*!
    Returns the document to which this node belongs.
*/
document node::ownerDocument() const
{
    if (!impl)
        return document();
    return document(IMPL->ownerDocument());
}

/*!
    Creates a deep (not shallow) copy of the node.

    If \a deep is true, then the cloning is done recursively which
    means that all the node's children are deep copied too. If \a deep
    is false only the node itself is copied and the copy will have no
    child nodes.
*/
node node::cloneNode(bool deep) const
{
    if (!impl)
        return node();
    return node(IMPL->cloneNode(deep));
}

/*!
    Calling normalize() on an element converts all its children into a
    standard form. This means that adjacent text objects will be
    merged into a single text object (cdatasection nodes are not
    merged).
*/
void node::normalize()
{
    if (!impl)
        return;
    IMPL->normalize();
}

/*!
    Returns true if the DOM implementation implements the feature \a
    feature and this feature is supported by this node in the version
    \a version; otherwise returns false.

    \sa dom_implementation::hasFeature()
*/
bool node::isSupported(const pfs::string& feature, const pfs::string& version) const
{
    dom_implementation i;
    return i.hasFeature(feature, version);
}

/*!
    Returns the namespace URI of this node or an empty string if the
    node has no namespace URI.

    Only nodes of type \l{node::type}{ElementNode} or
    \l{node::type}{AttributeNode} can have
    namespaces. A namespace URI must be specified at creation time and
    cannot be changed later.

    \sa prefix(), localName(), document::createElementNS(),
        document::createAttributeNS()
*/
pfs::string node::namespaceURI() const
{
    if (!impl)
        return pfs::string();
    return IMPL->namespaceURI;
}

/*!
    Returns the namespace prefix of the node or an empty string if the
    node has no namespace prefix.

    Only nodes of type \l{node::type}{ElementNode} or
    \l{node::type}{AttributeNode} can have
    namespaces. A namespace prefix must be specified at creation time.
    If a node was created with a namespace prefix, you can change it
    later with setPrefix().

    If you create an element or attribute with
    document::createElement() or document::createAttribute(),
    the prefix will be an empty string. If you use
    document::createElementNS() or
    document::createAttributeNS() instead, the prefix will not be
    an empty string; but it might be an empty string if the name does
    not have a prefix.

    \sa setPrefix(), localName(), namespaceURI(),
        document::createElementNS(),
        document::createAttributeNS()
*/
pfs::string node::prefix() const
{
    if (!impl)
        return pfs::string();
    return IMPL->prefix;
}

/*!
    If the node has a namespace prefix, this function changes the
    namespace prefix of the node to \a pre. Otherwise this function
    does nothing.

    Only nodes of type \l{node::type}{ElementNode} or
    \l{node::type}{AttributeNode} can have
    namespaces. A namespace prefix must have be specified at creation
    time; it is not possible to add a namespace prefix afterwards.

    \sa prefix(), localName(), namespaceURI(),
        document::createElementNS(),
        document::createAttributeNS()
*/
void node::setPrefix(const pfs::string& pre)
{
    if (!impl || IMPL->prefix.isNull())
        return;
    if (isAttr() || isElement())
        IMPL->prefix = pre;
}

/*!
    If the node uses namespaces, this function returns the local name
    of the node; otherwise it returns an empty string.

    Only nodes of type \l{node::type}{ElementNode} or
    \l{node::type}{AttributeNode} can have
    namespaces. A namespace must have been specified at creation time;
    it is not possible to add a namespace afterwards.

    \sa prefix(), namespaceURI(), document::createElementNS(),
        document::createAttributeNS()
*/
pfs::string node::localName() const
{
    if (!impl || IMPL->createdWithDom1Interface)
        return pfs::string();
    return IMPL->name;
}

/*!
    Returns true if the node has attributes; otherwise returns false.

    \sa attributes()
*/
bool node::hasAttributes() const
{
    if (!impl || !impl->isElement())
        return false;
    return static_cast<element::impl *>(impl)->hasAttributes();
}

/*!
    Inserts the node \a newChild before the child node \a refChild.
    \a refChild must be a direct child of this node. If \a refChild is
    \l{isNull()}{null} then \a newChild is inserted as the
    node's first child.

    If \a newChild is the child of another node, it is reparented to
    this node. If \a newChild is a child of this node, then its
    position in the list of children is changed.

    If \a newChild is a document_fragment, then the children of the
    fragment are removed from the fragment and inserted before \a
    refChild.

    Returns a new reference to \a newChild on success or a \l{isNull()}{null node} on failure.

    The DOM specification disallow inserting attribute nodes, but due
    to historical reasons QDom accept them nevertheless.

    \sa insertAfter(), replaceChild(), removeChild(), appendChild()
*/
node node::insertBefore(const node& newChild, const node& refChild)
{
    if (!impl)
        return node();
    return node(IMPL->insertBefore(newChild.impl, refChild.impl));
}

/*!
    Inserts the node \a newChild after the child node \a refChild. \a
    refChild must be a direct child of this node. If \a refChild is
    \l{isNull()}{null} then \a newChild is appended as this
    node's last child.

    If \a newChild is the child of another node, it is reparented to
    this node. If \a newChild is a child of this node, then its
    position in the list of children is changed.

    If \a newChild is a document_fragment, then the children of the
    fragment are removed from the fragment and inserted after \a
    refChild.

    Returns a new reference to \a newChild on success or a \l{isNull()}{null node} on failure.

    The DOM specification disallow inserting attribute nodes, but due
    to historical reasons QDom accept them nevertheless.

    \sa insertBefore(), replaceChild(), removeChild(), appendChild()
*/
node node::insertAfter(const node& newChild, const node& refChild)
{
    if (!impl)
        return node();
    return node(IMPL->insertAfter(newChild.impl, refChild.impl));
}

/*!
    Replaces \a oldChild with \a newChild. \a oldChild must be a
    direct child of this node.

    If \a newChild is the child of another node, it is reparented to
    this node. If \a newChild is a child of this node, then its
    position in the list of children is changed.

    If \a newChild is a document_fragment, then \a oldChild is
    replaced by all of the children of the fragment.

    Returns a new reference to \a oldChild on success or a \l{isNull()}{null node} an failure.

    \sa insertBefore(), insertAfter(), removeChild(), appendChild()
*/
node node::replaceChild(const node& newChild, const node& oldChild)
{
    if (!impl || !newChild.impl || !oldChild.impl)
        return node();
    return node(IMPL->replaceChild(newChild.impl, oldChild.impl));
}

/*!
    Removes \a oldChild from the list of children. \a oldChild must be
    a direct child of this node.

    Returns a new reference to \a oldChild on success or a \l{isNull()}{null node} on failure.

    \sa insertBefore(), insertAfter(), replaceChild(), appendChild()
*/
node node::removeChild(const node& oldChild)
{
    if (!impl)
        return node();

    if (oldChild.isNull())
        return node();

    return node(IMPL->removeChild(oldChild.impl));
}

/*!
    Appends \a newChild as the node's last child.

    If \a newChild is the child of another node, it is reparented to
    this node. If \a newChild is a child of this node, then its
    position in the list of children is changed.

    If \a newChild is a document_fragment, then the children of the
    fragment are removed from the fragment and appended.

    If \a newChild is a element and this node is a document that
    already has an element node as a child, \a newChild is not added as
    a child and a null node is returned.

    Returns a new reference to \a newChild on success or a \l{isNull()}{null node} on failure.

    Calling this function on a null node(created, for example, with
    the default constructor) does nothing and returns a \l{isNull()}{null node}.

    The DOM specification disallow inserting attribute nodes, but for
    historical reasons, QDom accepts them anyway.

    \sa insertBefore(), insertAfter(), replaceChild(), removeChild()
*/
node node::appendChild(const node& newChild)
{
    if (!impl) {
        qWarning("Calling appendChild() on a null node does nothing.");
        return node();
    }
    return node(IMPL->appendChild(newChild.impl));
}

/*!
    Returns true if the node has one or more children; otherwise
    returns false.
*/
bool node::hasChildNodes() const
{
    if (!impl)
        return false;
    return IMPL->first != 0;
}

/*!
    Returns true if this node is null (i.e. if it has no type or
    contents); otherwise returns false.
*/
bool node::isNull() const
{
    return (impl == 0);
}

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

/*!
    Returns the first child element with tag name \a tagName if tagName is non-empty;
    otherwise returns the first child element.  Returns a null element if no
    such child exists.

    \sa lastChildElement(), previousSiblingElement(), nextSiblingElement()
*/

element node::firstChildElement(const pfs::string &tagName) const
{
    for (node child = firstChild(); !child.isNull(); child = child.nextSibling()) {
        if (child.isElement()) {
            element elt = child.toElement();
            if (tagName.isEmpty() || elt.tagName() == tagName)
                return elt;
        }
    }
    return element();
}

/*!
    Returns the last child element with tag name \a tagName if tagName is non-empty;
    otherwise returns the last child element. Returns a null element if no
    such child exists.

    \sa firstChildElement(), previousSiblingElement(), nextSiblingElement()
*/

element node::lastChildElement(const pfs::string &tagName) const
{
    for (node child = lastChild(); !child.isNull(); child = child.previousSibling()) {
        if (child.isElement()) {
            element elt = child.toElement();
            if (tagName.isEmpty() || elt.tagName() == tagName)
                return elt;
        }
    }
    return element();
}

/*!
    Returns the next sibling element with tag name \a tagName if \a tagName
    is non-empty; otherwise returns any next sibling element.
    Returns a null element if no such sibling exists.

    \sa firstChildElement(), previousSiblingElement(), lastChildElement()
*/

element node::nextSiblingElement(const pfs::string &tagName) const
{
    for (node sib = nextSibling(); !sib.isNull(); sib = sib.nextSibling()) {
        if (sib.isElement()) {
            element elt = sib.toElement();
            if (tagName.isEmpty() || elt.tagName() == tagName)
                return elt;
        }
    }
    return element();
}

/*!
    Returns the previous sibilng element with tag name \a tagName if \a tagName
    is non-empty; otherwise returns any previous sibling element.
    Returns a null element if no such sibling exists.

    \sa firstChildElement(), nextSiblingElement(), lastChildElement()
*/

element node::previousSiblingElement(const pfs::string &tagName) const
{
    for (node sib = previousSibling(); !sib.isNull(); sib = sib.previousSibling()) {
        if (sib.isElement()) {
            element elt = sib.toElement();
            if (tagName.isEmpty() || elt.tagName() == tagName)
                return elt;
        }
    }
    return element();
}

/*!
    \since 4.1

    For nodes created by document::setContent(), this function
    returns the line number in the XML document where the node was parsed.
    Otherwise, -1 is returned.

    \sa columnNumber(), document::setContent()
*/
int node::lineNumber() const
{
    return impl ? impl->lineNumber : -1;
}

/*!
    \since 4.1

    For nodes created by document::setContent(), this function
    returns the column number in the XML document where the node was parsed.
    Otherwise, -1 is returned.

    \sa lineNumber(), document::setContent()
*/
int node::columnNumber() const
{
    return impl ? impl->columnNumber : -1;
}


/**************************************************************
 *
 * QDomNamedNodeMapPrivate
 *
 **************************************************************/

QDomNamedNodeMapPrivate::QDomNamedNodeMapPrivate(node::impl* n) : ref(1)
{
    readonly = false;
    parent = n;
    appendToParent = false;
}

QDomNamedNodeMapPrivate::~QDomNamedNodeMapPrivate()
{
    clearMap();
}

QDomNamedNodeMapPrivate* QDomNamedNodeMapPrivate::clone(node::impl* p)
{
    QScopedPointer<QDomNamedNodeMapPrivate> m(new QDomNamedNodeMapPrivate(p));
    m->readonly = readonly;
    m->appendToParent = appendToParent;

    QHash<pfs::string, node::impl*>::const_iterator it = map.constBegin();
    for (; it != map.constEnd(); ++it) {
        node::impl *new_node = (*it)->cloneNode();
        new_node->setParent(p);
        m->setNamedItem(new_node);
    }

    // we are no longer interested in ownership
    m->ref.deref();
    return m.take();
}

void QDomNamedNodeMapPrivate::clearMap()
{
    // Dereference all of our children if we took references
    if (!appendToParent) {
        QHash<pfs::string, node::impl *>::const_iterator it = map.constBegin();
        for (; it != map.constEnd(); ++it)
            if (!(*it)->ref.deref())
                delete *it;
    }
    map.clear();
}

node::impl* QDomNamedNodeMapPrivate::namedItem(const pfs::string& name) const
{
    node::impl* p = map[name];
    return p;
}

node::impl* QDomNamedNodeMapPrivate::namedItemNS(const pfs::string& nsURI, const pfs::string& localName) const
{
    QHash<pfs::string, node::impl *>::const_iterator it = map.constBegin();
    node::impl *n;
    for (; it != map.constEnd(); ++it) {
        n = *it;
        if (!n->prefix.isNull()) {
            // node has a namespace
            if (n->namespaceURI == nsURI && n->name == localName)
                return n;
        }
    }
    return 0;
}

node::impl* QDomNamedNodeMapPrivate::setNamedItem(node::impl* arg)
{
    if (readonly || !arg)
        return 0;

    if (appendToParent)
        return parent->appendChild(arg);

    node::impl *n = map.value(arg->nodeName());
    // We take a reference
    arg->ref.ref();
    map.insertMulti(arg->nodeName(), arg);
    return n;
}

node::impl* QDomNamedNodeMapPrivate::setNamedItemNS(node::impl* arg)
{
    if (readonly || !arg)
        return 0;

    if (appendToParent)
        return parent->appendChild(arg);

    if (!arg->prefix.isNull()) {
        // node has a namespace
        node::impl *n = namedItemNS(arg->namespaceURI, arg->name);
        // We take a reference
        arg->ref.ref();
        map.insertMulti(arg->nodeName(), arg);
        return n;
    } else {
        // ### check the following code if it is ok
        return setNamedItem(arg);
    }
}

node::impl* QDomNamedNodeMapPrivate::removeNamedItem(const pfs::string& name)
{
    if (readonly)
        return 0;

    node::impl* p = namedItem(name);
    if (p == 0)
        return 0;
    if (appendToParent)
        return parent->removeChild(p);

    map.remove(p->nodeName());
    // We took a reference, so we have to free one here
    p->ref.deref();
    return p;
}

node::impl* QDomNamedNodeMapPrivate::item(int index) const
{
    if (index >= length())
        return 0;
    return *(map.constBegin() + index);
}

int QDomNamedNodeMapPrivate::length() const
{
    return map.count();
}

bool QDomNamedNodeMapPrivate::contains(const pfs::string& name) const
{
    return map.value(name) != 0;
}

bool QDomNamedNodeMapPrivate::containsNS(const pfs::string& nsURI, const pfs::string & localName) const
{
    return namedItemNS(nsURI, localName) != 0;
}

/**************************************************************
 *
 * QDomNamedNodeMap
 *
 **************************************************************/

#define IMPL ((QDomNamedNodeMapPrivate*)impl)

/*!
    \class QDomNamedNodeMap
    \reentrant
    \brief The QDomNamedNodeMap class contains a collection of nodes
    that can be accessed by name.

    \inmodule QtXml
    \ingroup xml-tools

    Note that QDomNamedNodeMap does not inherit from QDomNodeList.
    QDomNamedNodeMaps do not provide any specific node ordering.
    Although nodes in a QDomNamedNodeMap may be accessed by an ordinal
    index, this is simply to allow a convenient enumeration of the
    contents of a QDomNamedNodeMap, and does not imply that the DOM
    specifies an ordering of the nodes.

    The QDomNamedNodeMap is used in three places:
    \list 1
    \li document_type::entities() returns a map of all entities
        described in the DTD.
    \li document_type::notations() returns a map of all notations
        described in the DTD.
    \li node::attributes() returns a map of all attributes of an
        element.
    \endlist

    Items in the map are identified by the name which node::name()
    returns. Nodes are retrieved using namedItem(), namedItemNS() or
    item(). New nodes are inserted with setNamedItem() or
    setNamedItemNS() and removed with removeNamedItem() or
    removeNamedItemNS(). Use contains() to see if an item with the
    given name is in the named node map. The number of items is
    returned by length().

    Terminology: in this class we use "item" and "node"
    interchangeably.
*/

/*!
    Constructs an empty named node map.
*/
QDomNamedNodeMap::QDomNamedNodeMap()
{
    impl = 0;
}

/*!
    Constructs a copy of \a n.
*/
QDomNamedNodeMap::QDomNamedNodeMap(const QDomNamedNodeMap &n)
{
    impl = n.impl;
    if (impl)
        impl->ref.ref();
}

QDomNamedNodeMap::QDomNamedNodeMap(QDomNamedNodeMapPrivate *n)
{
    impl = n;
    if (impl)
        impl->ref.ref();
}

/*!
    Assigns \a n to this named node map.
*/
QDomNamedNodeMap& QDomNamedNodeMap::operator=(const QDomNamedNodeMap &n)
{
    if (n.impl)
        n.impl->ref.ref();
    if (impl && !impl->ref.deref())
        delete impl;
    impl = n.impl;
    return *this;
}

/*!
    Returns true if \a n and this named node map are equal; otherwise
    returns false.
*/
bool QDomNamedNodeMap::operator== (const QDomNamedNodeMap& n) const
{
    return (impl == n.impl);
}

/*!
    Returns true if \a n and this named node map are not equal;
    otherwise returns false.
*/
bool QDomNamedNodeMap::operator!= (const QDomNamedNodeMap& n) const
{
    return (impl != n.impl);
}

/*!
    Destroys the object and frees its resources.
*/
QDomNamedNodeMap::~QDomNamedNodeMap()
{
    if (impl && !impl->ref.deref())
        delete impl;
}

/*!
    Returns the node called \a name.

    If the named node map does not contain such a node, a
    \l{node::isNull()}{null node} is returned. A node's name is
    the name returned by node::nodeName().

    \sa setNamedItem(), namedItemNS()
*/
node QDomNamedNodeMap::namedItem(const pfs::string& name) const
{
    if (!impl)
        return node();
    return node(IMPL->namedItem(name));
}

/*!
    Inserts the node \a newNode into the named node map. The name used
    by the map is the node name of \a newNode as returned by
    node::nodeName().

    If the new node replaces an existing node, i.e. the map contains a
    node with the same name, the replaced node is returned.

    \sa namedItem(), removeNamedItem(), setNamedItemNS()
*/
node QDomNamedNodeMap::setNamedItem(const node& newNode)
{
    if (!impl)
        return node();
    return node(IMPL->setNamedItem((node::impl*)newNode.impl));
}

/*!
    Removes the node called \a name from the map.

    The function returns the removed node or a
    \l{node::isNull()}{null node} if the map did not contain a
    node called \a name.

    \sa setNamedItem(), namedItem(), removeNamedItemNS()
*/
node QDomNamedNodeMap::removeNamedItem(const pfs::string& name)
{
    if (!impl)
        return node();
    return node(IMPL->removeNamedItem(name));
}

/*!
    Retrieves the node at position \a index.

    This can be used to iterate over the map. Note that the nodes in
    the map are ordered arbitrarily.

    \sa length()
*/
node QDomNamedNodeMap::item(int index) const
{
    if (!impl)
        return node();
    return node(IMPL->item(index));
}

/*!
    Returns the node associated with the local name \a localName and
    the namespace URI \a nsURI.

    If the map does not contain such a node,
    a \l{node::isNull()}{null node} is returned.

    \sa setNamedItemNS(), namedItem()
*/
node QDomNamedNodeMap::namedItemNS(const pfs::string& nsURI, const pfs::string& localName) const
{
    if (!impl)
        return node();
    return node(IMPL->namedItemNS(nsURI, localName));
}

/*!
    Inserts the node \a newNode in the map. If a node with the same
    namespace URI and the same local name already exists in the map,
    it is replaced by \a newNode. If the new node replaces an existing
    node, the replaced node is returned.

    \sa namedItemNS(), removeNamedItemNS(), setNamedItem()
*/
node QDomNamedNodeMap::setNamedItemNS(const node& newNode)
{
    if (!impl)
        return node();
    return node(IMPL->setNamedItemNS((node::impl*)newNode.impl));
}

/*!
    Removes the node with the local name \a localName and the
    namespace URI \a nsURI from the map.

    The function returns the removed node or a
    \l{node::isNull()}{null node} if the map did not contain a
    node with the local name \a localName and the namespace URI \a
    nsURI.

    \sa setNamedItemNS(), namedItemNS(), removeNamedItem()
*/
node QDomNamedNodeMap::removeNamedItemNS(const pfs::string& nsURI, const pfs::string& localName)
{
    if (!impl)
        return node();
    node::impl *n = IMPL->namedItemNS(nsURI, localName);
    if (!n)
        return node();
    return node(IMPL->removeNamedItem(n->name));
}

/*!
    Returns the number of nodes in the map.

    \sa item()
*/
int QDomNamedNodeMap::length() const
{
    if (!impl)
        return 0;
    return IMPL->length();
}

/*!
    \fn bool QDomNamedNodeMap::isEmpty() const

    Returns true if the map is empty; otherwise returns false. This function is
    provided for Qt API consistency.
*/

/*!
    \fn int QDomNamedNodeMap::count() const

    This function is provided for Qt API consistency. It is equivalent to length().
*/

/*!
    \fn int QDomNamedNodeMap::size() const

    This function is provided for Qt API consistency. It is equivalent to length().
*/

/*!
    Returns true if the map contains a node called \a name; otherwise
    returns false.

    \b{Note:} This function does not take the presence of namespaces into account.
    Use namedItemNS() to test whether the map contains a node with a specific namespace
    URI and name.
*/
bool QDomNamedNodeMap::contains(const pfs::string& name) const
{
    if (!impl)
        return false;
    return IMPL->contains(name);
}

#undef IMPL

/**************************************************************
 *
 * document_type::impl
 *
 **************************************************************/

document_type::impl::document_type::impl(document::impl* doc, node::impl* parent)
    : node::impl(doc, parent)
{
    init();
}

document_type::impl::document_type::impl(document_type::impl* n, bool deep)
    : node::impl(n, deep)
{
    init();
    // Refill the maps with our new children
    node::impl* p = first;
    while (p) {
        if (p->isEntity())
            // Don't use normal insert function since we would create infinite recursion
            entities->map.insertMulti(p->nodeName(), p);
        if (p->isNotation())
            // Don't use normal insert function since we would create infinite recursion
            notations->map.insertMulti(p->nodeName(), p);
        p = p->next;
    }
}

document_type::impl::~document_type::impl()
{
    if (!entities->ref.deref())
        delete entities;
    if (!notations->ref.deref())
        delete notations;
}

void document_type::impl::init()
{
    entities = new QDomNamedNodeMapPrivate(this);
    QT_TRY {
        notations = new QDomNamedNodeMapPrivate(this);
        publicId.clear();
        systemId.clear();
        internalSubset.clear();

        entities->setAppendToParent(true);
        notations->setAppendToParent(true);
    } QT_CATCH(...) {
        delete entities;
        QT_RETHROW;
    }
}

node::impl* document_type::impl::cloneNode(bool deep)
{
    node::impl* p = new document_type::impl(this, deep);
    // We are not interested in this node
    p->ref.deref();
    return p;
}

node::impl* document_type::impl::insertBefore(node::impl* newChild, node::impl* refChild)
{
    // Call the origianl implementation
    node::impl* p = node::impl::insertBefore(newChild, refChild);
    // Update the maps
    if (p && p->isEntity())
        entities->map.insertMulti(p->nodeName(), p);
    else if (p && p->isNotation())
        notations->map.insertMulti(p->nodeName(), p);

    return p;
}

node::impl* document_type::impl::insertAfter(node::impl* newChild, node::impl* refChild)
{
    // Call the origianl implementation
    node::impl* p = node::impl::insertAfter(newChild, refChild);
    // Update the maps
    if (p && p->isEntity())
        entities->map.insertMulti(p->nodeName(), p);
    else if (p && p->isNotation())
        notations->map.insertMulti(p->nodeName(), p);

    return p;
}

node::impl* document_type::impl::replaceChild(node::impl* newChild, node::impl* oldChild)
{
    // Call the origianl implementation
    node::impl* p = node::impl::replaceChild(newChild, oldChild);
    // Update the maps
    if (p) {
        if (oldChild && oldChild->isEntity())
            entities->map.remove(oldChild->nodeName());
        else if (oldChild && oldChild->isNotation())
            notations->map.remove(oldChild->nodeName());

        if (p->isEntity())
            entities->map.insertMulti(p->nodeName(), p);
        else if (p->isNotation())
            notations->map.insertMulti(p->nodeName(), p);
    }

    return p;
}

node::impl* document_type::impl::removeChild(node::impl* oldChild)
{
    // Call the origianl implementation
    node::impl* p = node::impl::removeChild( oldChild);
    // Update the maps
    if (p && p->isEntity())
        entities->map.remove(p->nodeName());
    else if (p && p->isNotation())
        notations->map.remove(p ->nodeName());

    return p;
}

node::impl* document_type::impl::appendChild(node::impl* newChild)
{
    return insertAfter(newChild, 0);
}

static pfs::string quotedValue(const pfs::string &data)
{
    QChar quote = data.indexOf(QLatin1Char('\'')) == -1
                    ? QLatin1Char('\'')
                    : QLatin1Char('"');
    return quote + data + quote;
}


/**************************************************************
 *
 * document_type
 *
 **************************************************************/

#define IMPL ((document_type::impl*)impl)

/*!
    \class document_type
    \reentrant
    \brief The document_type class is the representation of the DTD
    in the document tree.

    \inmodule QtXml
    \ingroup xml-tools

    The document_type class allows read-only access to some of the
    data structures in the DTD: it can return a map of all entities()
    and notations(). In addition the function name() returns the name
    of the document type as specified in the &lt;!DOCTYPE name&gt;
    tag. This class also provides the publicId(), systemId() and
    internalSubset() functions.

    \sa document
*/

/*!
    Creates an empty document_type object.
*/
document_type::document_type() : node()
{
}

/*!
    Constructs a copy of \a n.

    The data of the copy is shared (shallow copy): modifying one node
    will also change the other. If you want to make a deep copy, use
    cloneNode().
*/
document_type::document_type(const document_type& n)
    : node(n)
{
}

document_type::document_type(document_type::impl* n)
    : node(n)
{
}

/*!
    Assigns \a n to this document type.

    The data of the copy is shared (shallow copy): modifying one node
    will also change the other. If you want to make a deep copy, use
    cloneNode().
*/
document_type& document_type::operator= (const document_type& n)
{
    return (document_type&) node::operator=(n);
}

/*!
    Returns the name of the document type as specified in the
    &lt;!DOCTYPE name&gt; tag.

    \sa nodeName()
*/
pfs::string document_type::name() const
{
    if (!impl)
        return pfs::string();
    return IMPL->nodeName();
}

/*!
    Returns a map of all entities described in the DTD.
*/
QDomNamedNodeMap document_type::entities() const
{
    if (!impl)
        return QDomNamedNodeMap();
    return QDomNamedNodeMap(IMPL->entities);
}

/*!
    Returns a map of all notations described in the DTD.
*/
QDomNamedNodeMap document_type::notations() const
{
    if (!impl)
        return QDomNamedNodeMap();
    return QDomNamedNodeMap(IMPL->notations);
}

/*!
    Returns the public identifier of the external DTD subset or
    an empty string if there is no public identifier.

    \sa systemId(), internalSubset(), dom_implementation::createDocumentType()
*/
pfs::string document_type::publicId() const
{
    if (!impl)
        return pfs::string();
    return IMPL->publicId;
}

/*!
    Returns the system identifier of the external DTD subset or
    an empty string if there is no system identifier.

    \sa publicId(), internalSubset(), dom_implementation::createDocumentType()
*/
pfs::string document_type::systemId() const
{
    if (!impl)
        return pfs::string();
    return IMPL->systemId;
}

/*!
    Returns the internal subset of the document type or an empty
    string if there is no internal subset.

    \sa publicId(), systemId()
*/
pfs::string document_type::internalSubset() const
{
    if (!impl)
        return pfs::string();
    return IMPL->internalSubset;
}

/*
    Are these needed at all? The only difference when removing these
    two methods in all subclasses is that we'd get a different type
    for null nodes.
*/

/*!
    \fn node::type document_type::nodeType() const

    Returns \c DocumentTypeNode.

    \sa isDocumentType(), node::toDocumentType()
*/

#undef IMPL

/**************************************************************
 *
 * document_fragment::impl
 *
 **************************************************************/

document_fragment::impl::document_fragment::impl(document::impl* doc, node::impl* parent)
    : node::impl(doc, parent)
{
    name = QLatin1String("#document-fragment");
}

document_fragment::impl::document_fragment::impl(node::impl* n, bool deep)
    : node::impl(n, deep)
{
}

node::impl* document_fragment::impl::cloneNode(bool deep)
{
    node::impl* p = new document_fragment::impl(this, deep);
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

/*!
    Constructs an empty document fragment.
*/
document_fragment::document_fragment()
{
}

document_fragment::document_fragment(document_fragment::impl* n)
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
 * chardata::impl
 *
 **************************************************************/

chardata::impl::chardata::impl(document::impl* d, node::impl* p,
                                                      const pfs::string& data)
    : node::impl(d, p)
{
    value = data;
    name = QLatin1String("#character-data");
}

chardata::impl::chardata::impl(chardata::impl* n, bool deep)
    : node::impl(n, deep)
{
}

node::impl* chardata::impl::cloneNode(bool deep)
{
    node::impl* p = new chardata::impl(this, deep);
    // We are not interested in this node
    p->ref.deref();
    return p;
}

int chardata::impl::dataLength() const
{
    return value.length();
}

pfs::string chardata::impl::substringData(unsigned long offset, unsigned long n) const
{
    return value.mid(offset, n);
}

void chardata::impl::insertData(unsigned long offset, const pfs::string& arg)
{
    value.insert(offset, arg);
}

void chardata::impl::deleteData(unsigned long offset, unsigned long n)
{
    value.remove(offset, n);
}

void chardata::impl::replaceData(unsigned long offset, unsigned long n, const pfs::string& arg)
{
    value.replace(offset, n, arg);
}

void chardata::impl::appendData(const pfs::string& arg)
{
    value += arg;
}

/**************************************************************
 *
 * chardata
 *
 **************************************************************/

#define IMPL ((chardata::impl*)impl)

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

chardata::chardata(chardata::impl* n)
    : node(n)
{
}

/*!
    Assigns \a x to this character data.

    The data of the copy is shared (shallow copy): modifying one node
    will also change the other. If you want to make a deep copy, use
    cloneNode().
*/
chardata& chardata::operator= (const chardata& x)
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
 * attr::impl
 *
 **************************************************************/

attr::impl::attr::impl(document::impl* d, node::impl* parent, const pfs::string& name_)
    : node::impl(d, parent)
{
    name = name_;
    m_specified = false;
}

attr::impl::attr::impl(document::impl* d, node::impl* p, const pfs::string& nsURI, const pfs::string& qName)
    : node::impl(d, p)
{
    qt_split_namespace(prefix, name, qName, !nsURI.isNull());
    namespaceURI = nsURI;
    createdWithDom1Interface = false;
    m_specified = false;
}

attr::impl::attr::impl(attr::impl* n, bool deep)
    : node::impl(n, deep)
{
    m_specified = n->specified();
}

void attr::impl::setNodeValue(const pfs::string& v)
{
    value = v;
    text::impl *t = new text::impl(0, this, v);
    // keep the refcount balanced: appendChild() does a ref anyway.
    t->ref.deref();
    if (first) {
        delete removeChild(first);
    }
    appendChild(t);
}

node::impl* attr::impl::cloneNode(bool deep)
{
    node::impl* p = new attr::impl(this, deep);
    // We are not interested in this node
    p->ref.deref();
    return p;
}

bool attr::impl::specified() const
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

#define IMPL ((attr::impl*)impl)

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

attr::attr(attr::impl* n)
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
    return element((element::impl*)(impl->parent()));
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
 * element::impl
 *
 **************************************************************/

element::impl::element::impl(document::impl* d, node::impl* p,
                                          const pfs::string& tagname)
    : node::impl(d, p)
{
    name = tagname;
    m_attr = new QDomNamedNodeMapPrivate(this);
}

element::impl::element::impl(document::impl* d, node::impl* p,
        const pfs::string& nsURI, const pfs::string& qName)
    : node::impl(d, p)
{
    qt_split_namespace(prefix, name, qName, !nsURI.isNull());
    namespaceURI = nsURI;
    createdWithDom1Interface = false;
    m_attr = new QDomNamedNodeMapPrivate(this);
}

element::impl::element::impl(element::impl* n, bool deep) :
    node::impl(n, deep)
{
    m_attr = n->m_attr->clone(this);
    // Reference is down to 0, so we set it to 1 here.
    m_attr->ref.ref();
}

element::impl::~element::impl()
{
    if (!m_attr->ref.deref())
        delete m_attr;
}

node::impl* element::impl::cloneNode(bool deep)
{
    node::impl* p = new element::impl(this, deep);
    // We are not interested in this node
    p->ref.deref();
    return p;
}

pfs::string element::impl::attribute(const pfs::string& name_, const pfs::string& defValue) const
{
    node::impl* n = m_attr->namedItem(name_);
    if (!n)
        return defValue;

    return n->nodeValue();
}

pfs::string element::impl::attributeNS(const pfs::string& nsURI, const pfs::string& localName, const pfs::string& defValue) const
{
    node::impl* n = m_attr->namedItemNS(nsURI, localName);
    if (!n)
        return defValue;

    return n->nodeValue();
}

void element::impl::setAttribute(const pfs::string& aname, const pfs::string& newValue)
{
    node::impl* n = m_attr->namedItem(aname);
    if (!n) {
        n = new attr::impl(ownerDocument(), this, aname);
        n->setNodeValue(newValue);

        // Referencing is done by the map, so we set the reference counter back
        // to 0 here. This is ok since we created the attr::impl.
        n->ref.deref();
        m_attr->setNamedItem(n);
    } else {
        n->setNodeValue(newValue);
    }
}

void element::impl::setAttributeNS(const pfs::string& nsURI, const pfs::string& qName, const pfs::string& newValue)
{
    pfs::string prefix, localName;
    qt_split_namespace(prefix, localName, qName, true);
    node::impl* n = m_attr->namedItemNS(nsURI, localName);
    if (!n) {
        n = new attr::impl(ownerDocument(), this, nsURI, qName);
        n->setNodeValue(newValue);

        // Referencing is done by the map, so we set the reference counter back
        // to 0 here. This is ok since we created the attr::impl.
        n->ref.deref();
        m_attr->setNamedItem(n);
    } else {
        n->setNodeValue(newValue);
        n->prefix = prefix;
    }
}

void element::impl::removeAttribute(const pfs::string& aname)
{
    node::impl* p = m_attr->removeNamedItem(aname);
    if (p && p->ref.load() == 0)
        delete p;
}

attr::impl* element::impl::attributeNode(const pfs::string& aname)
{
    return (attr::impl*)m_attr->namedItem(aname);
}

attr::impl* element::impl::attributeNodeNS(const pfs::string& nsURI, const pfs::string& localName)
{
    return (attr::impl*)m_attr->namedItemNS(nsURI, localName);
}

attr::impl* element::impl::setAttributeNode(attr::impl* newAttr)
{
    node::impl* n = m_attr->namedItem(newAttr->nodeName());

    // Referencing is done by the maps
    m_attr->setNamedItem(newAttr);

    newAttr->setParent(this);

    return (attr::impl*)n;
}

attr::impl* element::impl::setAttributeNodeNS(attr::impl* newAttr)
{
    node::impl* n = 0;
    if (!newAttr->prefix.isNull())
        n = m_attr->namedItemNS(newAttr->namespaceURI, newAttr->name);

    // Referencing is done by the maps
    m_attr->setNamedItem(newAttr);

    return (attr::impl*)n;
}

attr::impl* element::impl::removeAttributeNode(attr::impl* oldAttr)
{
    return (attr::impl*)m_attr->removeNamedItem(oldAttr->nodeName());
}

bool element::impl::hasAttribute(const pfs::string& aname)
{
    return m_attr->contains(aname);
}

bool element::impl::hasAttributeNS(const pfs::string& nsURI, const pfs::string& localName)
{
    return m_attr->containsNS(nsURI, localName);
}

pfs::string element::impl::text()
{
    pfs::string t(QLatin1String(""));

    node::impl* p = first;
    while (p) {
        if (p->isText() || p->isCDATASection())
            t += p->nodeValue();
        else if (p->isElement())
            t += ((element::impl*)p)->text();
        p = p->next;
    }

    return t;
}

/**************************************************************
 *
 * element
 *
 **************************************************************/

#define IMPL ((element::impl*)impl)

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

element::element(element::impl* n)
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
    Returns a QDomNamedNodeMap containing all this element's attributes.

    \sa attribute(), setAttribute(), attributeNode(), setAttributeNode()
*/
QDomNamedNodeMap element::attributes() const
{
    if (!impl)
        return QDomNamedNodeMap();
    return QDomNamedNodeMap(IMPL->attributes());
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
    return attr(IMPL->setAttributeNode(((attr::impl*)newAttr.impl)));
}

/*!
    Removes the attribute \a oldAttr from the element and returns it.

    \sa attributeNode(), setAttributeNode()
*/
attr element::removeAttributeNode(const attr& oldAttr)
{
    if (!impl)
        return attr(); // ### should this return oldAttr?
    return attr(IMPL->removeAttributeNode(((attr::impl*)oldAttr.impl)));
}

/*!
  Returns a QDomNodeList containing all descendants of this element
  named \a tagname encountered during a preorder traversal of the
  element subtree with this element as its root. The order of the
  elements in the returned list is the order they are encountered
  during the preorder traversal.

  \sa elementsByTagNameNS(), document::elementsByTagName()
*/
QDomNodeList element::elementsByTagName(const pfs::string& tagname) const
{
    return QDomNodeList(new QDomNodeListPrivate(impl, tagname));
}

/*!
  Returns true if this element has an attribute called \a name;
  otherwise returns false.

  \b{Note:} This function does not take the presence of namespaces
  into account.  As a result, the specified name will be tested
  against fully-qualified attribute names that include any namespace
  prefixes that may be present.

  Use hasAttributeNS() to explicitly test for attributes with specific
  namespaces and names.
*/
bool element::hasAttribute(const pfs::string& name) const
{
    if (!impl)
        return false;
    return IMPL->hasAttribute(name);
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
    node::impl *n = IMPL->attributeNodeNS(nsURI, localName);
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
    return attr(IMPL->setAttributeNodeNS(((attr::impl*)newAttr.impl)));
}

/*!
  Returns a QDomNodeList containing all descendants of this element
  with local name \a localName and namespace URI \a nsURI encountered
  during a preorder traversal of the element subtree with this element
  as its root. The order of the elements in the returned list is the
  order they are encountered during the preorder traversal.

  \sa elementsByTagName(), document::elementsByTagNameNS()
*/
QDomNodeList element::elementsByTagNameNS(const pfs::string& nsURI, const pfs::string& localName) const
{
    return QDomNodeList(new QDomNodeListPrivate(impl, nsURI, localName));
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
 * text::impl
 *
 **************************************************************/

text::impl::text::impl(document::impl* d, node::impl* parent, const pfs::string& val)
    : chardata::impl(d, parent, val)
{
    name = QLatin1String("#text");
}

text::impl::text::impl(text::impl* n, bool deep)
    : chardata::impl(n, deep)
{
}

node::impl* text::impl::cloneNode(bool deep)
{
    node::impl* p = new text::impl(this, deep);
    // We are not interested in this node
    p->ref.deref();
    return p;
}

text::impl* text::impl::splitText(int offset)
{
    if (!parent()) {
        qWarning("text::splitText  The node has no parent. So I can not split");
        return 0;
    }

    text::impl* t = new text::impl(ownerDocument(), 0, value.mid(offset));
    value.truncate(offset);

    parent()->insertAfter(t, this);

    return t;
}


/**************************************************************
 *
 * text
 *
 **************************************************************/

#define IMPL ((text::impl*)impl)

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

text::text(text::impl* n)
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
 * comment::impl
 *
 **************************************************************/

comment::impl::comment::impl(document::impl* d, node::impl* parent, const pfs::string& val)
    : chardata::impl(d, parent, val)
{
    name = QLatin1String("#comment");
}

comment::impl::comment::impl(comment::impl* n, bool deep)
    : chardata::impl(n, deep)
{
}


node::impl* comment::impl::cloneNode(bool deep)
{
    node::impl* p = new comment::impl(this, deep);
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

comment::comment(comment::impl* n)
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
 * cdatasection::impl
 *
 **************************************************************/

cdatasection::impl::cdatasection::impl(document::impl* d, node::impl* parent,
                                                    const pfs::string& val)
    : text::impl(d, parent, val)
{
    name = QLatin1String("#cdata-section");
}

cdatasection::impl::cdatasection::impl(cdatasection::impl* n, bool deep)
    : text::impl(n, deep)
{
}

node::impl* cdatasection::impl::cloneNode(bool deep)
{
    node::impl* p = new cdatasection::impl(this, deep);
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

cdatasection::cdatasection(cdatasection::impl* n)
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
 * notation::impl
 *
 **************************************************************/

notation::impl::notation::impl(document::impl* d, node::impl* parent,
                                            const pfs::string& aname,
                                            const pfs::string& pub, const pfs::string& sys)
    : node::impl(d, parent)
{
    name = aname;
    m_pub = pub;
    m_sys = sys;
}

notation::impl::notation::impl(notation::impl* n, bool deep)
    : node::impl(n, deep)
{
    m_sys = n->m_sys;
    m_pub = n->m_pub;
}

node::impl* notation::impl::cloneNode(bool deep)
{
    node::impl* p = new notation::impl(this, deep);
    // We are not interested in this node
    p->ref.deref();
    return p;
}

/**************************************************************
 *
 * notation
 *
 **************************************************************/

#define IMPL ((notation::impl*)impl)

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

notation::notation(notation::impl* n)
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
 * entity::impl
 *
 **************************************************************/

entity::impl::entity::impl(document::impl* d, node::impl* parent,
                                        const pfs::string& aname,
                                        const pfs::string& pub, const pfs::string& sys, const pfs::string& notation)
    : node::impl(d, parent)
{
    name = aname;
    m_pub = pub;
    m_sys = sys;
    m_notationName = notation;
}

entity::impl::entity::impl(entity::impl* n, bool deep)
    : node::impl(n, deep)
{
    m_sys = n->m_sys;
    m_pub = n->m_pub;
    m_notationName = n->m_notationName;
}

node::impl* entity::impl::cloneNode(bool deep)
{
    node::impl* p = new entity::impl(this, deep);
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

#define IMPL ((entity::impl*)impl)

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

entity::entity(entity::impl* n)
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
 * entity_reference::impl
 *
 **************************************************************/

entity_reference::impl::entity_reference::impl(document::impl* d, node::impl* parent, const pfs::string& aname)
    : node::impl(d, parent)
{
    name = aname;
}

entity_reference::impl::entity_reference::impl(node::impl* n, bool deep)
    : node::impl(n, deep)
{
}

node::impl* entity_reference::impl::cloneNode(bool deep)
{
    node::impl* p = new entity_reference::impl(this, deep);
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

entity_reference::entity_reference(entity_reference::impl* n)
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
 * processing_instruction::impl
 *
 **************************************************************/

processing_instruction::impl::processing_instruction::impl(document::impl* d,
        node::impl* parent, const pfs::string& target, const pfs::string& data)
    : node::impl(d, parent)
{
    name = target;
    value = data;
}

processing_instruction::impl::processing_instruction::impl(processing_instruction::impl* n, bool deep)
    : node::impl(n, deep)
{
}


node::impl* processing_instruction::impl::cloneNode(bool deep)
{
    node::impl* p = new processing_instruction::impl(this, deep);
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

processing_instruction::processing_instruction(processing_instruction::impl* n)
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
 * document::impl
 *
 **************************************************************/

document::impl::document::impl()
    : node::impl(0),
      impl(new dom_implementation::impl),
      nodeListTime(1)
{
    type = new document_type::impl(this, this);
    type->ref.deref();

    name = QLatin1String("#document");
}

document::impl::document::impl(const pfs::string& aname)
    : node::impl(0),
      impl(new dom_implementation::impl),
      nodeListTime(1)
{
    type = new document_type::impl(this, this);
    type->ref.deref();
    type->name = aname;

    name = QLatin1String("#document");
}

document::impl::document::impl(document_type::impl* dt)
    : node::impl(0),
      impl(new dom_implementation::impl),
      nodeListTime(1)
{
    if (dt != 0) {
        type = dt;
    } else {
        type = new document_type::impl(this, this);
        type->ref.deref();
    }

    name = QLatin1String("#document");
}

document::impl::document::impl(document::impl* n, bool deep)
    : node::impl(n, deep),
      impl(n->impl->clone()),
      nodeListTime(1)
{
    type = static_cast<document_type::impl*>(n->type->cloneNode());
    type->setParent(this);
}

document::impl::~document::impl()
{
}

void document::impl::clear()
{
    impl.reset();
    type.reset();
    node::impl::clear();
}

static void initializeReader(QXmlSimpleReader &reader, bool namespaceProcessing)
{
    reader.setFeature(QLatin1String("http://xml.org/sax/features/namespaces"), namespaceProcessing);
    reader.setFeature(QLatin1String("http://xml.org/sax/features/namespace-prefixes"), !namespaceProcessing);
    reader.setFeature(QLatin1String("http://trolltech.com/xml/features/report-whitespace-only-CharData"), false); // Shouldn't change in Qt 4
}

bool document::impl::setContent(QXmlInputSource *source, bool namespaceProcessing, pfs::string *errorMsg, int *errorLine, int *errorColumn)
{
    QXmlSimpleReader reader;
    initializeReader(reader, namespaceProcessing);
    return setContent(source, &reader, errorMsg, errorLine, errorColumn);
}

bool document::impl::setContent(QXmlInputSource *source, QXmlReader *reader, pfs::string *errorMsg, int *errorLine, int *errorColumn)
{
    clear();
    impl = new dom_implementation::impl;
    type = new document_type::impl(this, this);
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

node::impl* document::impl::cloneNode(bool deep)
{
    node::impl *p = new document::impl(this, deep);
    // We are not interested in this node
    p->ref.deref();
    return p;
}

element::impl* document::impl::documentElement()
{
    node::impl *p = first;
    while (p && !p->isElement())
        p = p->next;

    return static_cast<element::impl *>(p);
}

element::impl* document::impl::createElement(const pfs::string &tagName)
{
    bool ok;
    pfs::string fixedName = fixedXmlName(tagName, &ok);
    if (!ok)
        return 0;

    element::impl *e = new element::impl(this, 0, fixedName);
    e->ref.deref();
    return e;
}

element::impl* document::impl::createElementNS(const pfs::string &nsURI, const pfs::string &qName)
{
    bool ok;
    pfs::string fixedName = fixedXmlName(qName, &ok, true);
    if (!ok)
        return 0;

    element::impl *e = new element::impl(this, 0, nsURI, fixedName);
    e->ref.deref();
    return e;
}

document_fragment::impl* document::impl::createDocumentFragment()
{
    document_fragment::impl *f = new document_fragment::impl(this, (node::impl*)0);
    f->ref.deref();
    return f;
}

text::impl* document::impl::createTextNode(const pfs::string &data)
{
    bool ok;
    pfs::string fixedData = fixedCharData(data, &ok);
    if (!ok)
        return 0;

    text::impl *t = new text::impl(this, 0, fixedData);
    t->ref.deref();
    return t;
}

comment::impl* document::impl::createComment(const pfs::string &data)
{
    bool ok;
    pfs::string fixedData = fixedComment(data, &ok);
    if (!ok)
        return 0;

    comment::impl *c = new comment::impl(this, 0, fixedData);
    c->ref.deref();
    return c;
}

cdatasection::impl* document::impl::createCDATASection(const pfs::string &data)
{
    bool ok;
    pfs::string fixedData = fixedCDataSection(data, &ok);
    if (!ok)
        return 0;

    cdatasection::impl *c = new cdatasection::impl(this, 0, fixedData);
    c->ref.deref();
    return c;
}

processing_instruction::impl* document::impl::createProcessingInstruction(const pfs::string &target,
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

    processing_instruction::impl *p = new processing_instruction::impl(this, 0, fixedTarget, fixedData);
    p->ref.deref();
    return p;
}
attr::impl* document::impl::createAttribute(const pfs::string &aname)
{
    bool ok;
    pfs::string fixedName = fixedXmlName(aname, &ok);
    if (!ok)
        return 0;

    attr::impl *a = new attr::impl(this, 0, fixedName);
    a->ref.deref();
    return a;
}

attr::impl* document::impl::createAttributeNS(const pfs::string &nsURI, const pfs::string &qName)
{
    bool ok;
    pfs::string fixedName = fixedXmlName(qName, &ok, true);
    if (!ok)
        return 0;

    attr::impl *a = new attr::impl(this, 0, nsURI, fixedName);
    a->ref.deref();
    return a;
}

entity_reference::impl* document::impl::createEntityReference(const pfs::string &aname)
{
    bool ok;
    pfs::string fixedName = fixedXmlName(aname, &ok);
    if (!ok)
        return 0;

    entity_reference::impl *e = new entity_reference::impl(this, 0, fixedName);
    e->ref.deref();
    return e;
}

node::impl* document::impl::importNode(const node::impl *importedNode, bool deep)
{
    node::impl *node = 0;
    switch (importedNode->nodeType()) {
        case node::AttributeNode:
            node = new attr::impl((attr::impl*)importedNode, true);
            break;
        case node::DocumentFragmentNode:
            node = new document_fragment::impl((document_fragment::impl*)importedNode, deep);
            break;
        case node::ElementNode:
            node = new element::impl((element::impl*)importedNode, deep);
            break;
        case node::EntityNode:
            node = new entity::impl((entity::impl*)importedNode, deep);
            break;
        case node::EntityReferenceNode:
            node = new entity_reference::impl((entity_reference::impl*)importedNode, false);
            break;
        case node::NotationNode:
            node = new notation::impl((notation::impl*)importedNode, deep);
            break;
        case node::ProcessingInstructionNode:
            node = new processing_instruction::impl((processing_instruction::impl*)importedNode, deep);
            break;
        case node::TextNode:
            node = new text::impl((text::impl*)importedNode, deep);
            break;
        case node::CDATASectionNode:
            node = new cdatasection::impl((cdatasection::impl*)importedNode, deep);
            break;
        case node::CommentNode:
            node = new comment::impl((comment::impl*)importedNode, deep);
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

#define IMPL ((document::impl*)impl)

/*!
    Constructs an empty document.
*/
document::document()
{
    impl = 0;
}

/*!
    Creates a document and sets the name of the document type to \a
    name.
*/
document::document(const pfs::string& name)
{
    // We take over ownership
    impl = new document::impl(name);
}

/*!
    Creates a document with the document type \a doctype.

    \sa dom_implementation::createDocumentType()
*/
document::document(const document_type& doctype)
{
    impl = new document::impl((document_type::impl*)(doctype.impl));
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

document::document(document::impl* x)
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
    Destroys the object and frees its resources.
*/
document::~document()
{
}

/*!
    \overload

    This function reads the XML document from the string \a text, returning
    true if the content was successfully parsed; otherwise returns false.
    Since \a text is already a Unicode string, no encoding detection
    is done.
*/
bool document::setContent(const pfs::string& text, bool namespaceProcessing, pfs::string *errorMsg, int *errorLine, int *errorColumn)
{
    if (!impl)
        impl = new document::impl();
    QXmlInputSource source;
    source.setData(text);
    return IMPL->setContent(&source, namespaceProcessing, errorMsg, errorLine, errorColumn);
}

/*!
    \nonreentrant

    This function parses the XML document from the byte array \a
    data and sets it as the content of the document. It tries to
    detect the encoding of the document as required by the XML
    specification.

    If \a namespaceProcessing is true, the parser recognizes
    namespaces in the XML file and sets the prefix name, local name
    and namespace URI to appropriate values. If \a namespaceProcessing
    is false, the parser does no namespace processing when it reads
    the XML file.

    If a parse error occurs, this function returns false and the error
    message is placed in \c{*}\a{errorMsg}, the line number in
    \c{*}\a{errorLine} and the column number in \c{*}\a{errorColumn}
    (unless the associated pointer is set to 0); otherwise this
    function returns true. The various error messages are described in
    the QXmlParseException class documentation. Note that, if you
    want to display these error messages to your application's users,
    they will be displayed in English unless they are explicitly
    translated.

    If \a namespaceProcessing is true, the function node::prefix()
    returns a string for all elements and attributes. It returns an
    empty string if the element or attribute has no prefix.

    Text nodes consisting only of whitespace are stripped and won't
    appear in the document. If this behavior is not desired,
    one can use the setContent() overload that allows a QXmlReader to be
    supplied.

    If \a namespaceProcessing is false, the functions
    node::prefix(), node::localName() and
    node::namespaceURI() return an empty string.

    Entity references are handled as follows:
    \list
    \li References to internal general entities and character entities occurring in the
        content are included. The result is a text node with the references replaced
        by their corresponding entity values.
    \li References to parameter entities occurring in the internal subset are included.
        The result is a document_type node which contains entity and notation declarations
        with the references replaced by their corresponding entity values.
    \li Any general parsed entity reference which is not defined in the internal subset and
        which occurs in the content is represented as a entity_reference node.
    \li Any parsed entity reference which is not defined in the internal subset and which
        occurs outside of the content is replaced with an empty string.
    \li Any unparsed entity reference is replaced with an empty string.
    \endlist

    \sa node::namespaceURI(), node::localName(),
        node::prefix(), pfs::string::isNull(), pfs::string::isEmpty()
*/
bool document::setContent(const QByteArray &data, bool namespaceProcessing, pfs::string *errorMsg, int *errorLine, int *errorColumn)
{
    if (!impl)
        impl = new document::impl();
    QBuffer buf;
    buf.setData(data);
    QXmlInputSource source(&buf);
    return IMPL->setContent(&source, namespaceProcessing, errorMsg, errorLine, errorColumn);
}

/*!
    \overload

    This function reads the XML document from the IO device \a dev, returning
    true if the content was successfully parsed; otherwise returns false.
*/
bool document::setContent(QIODevice* dev, bool namespaceProcessing, pfs::string *errorMsg, int *errorLine, int *errorColumn)
{
    if (!impl)
        impl = new document::impl();
    QXmlInputSource source(dev);
    return IMPL->setContent(&source, namespaceProcessing, errorMsg, errorLine, errorColumn);
}

/*!
    \overload
    \since 4.5

    This function reads the XML document from the QXmlInputSource \a source,
    returning true if the content was successfully parsed; otherwise returns false.

*/
bool document::setContent(QXmlInputSource *source, bool namespaceProcessing, pfs::string *errorMsg, int *errorLine, int *errorColumn )
{
    if (!impl)
        impl = new document::impl();
    QXmlSimpleReader reader;
    initializeReader(reader, namespaceProcessing);
    return IMPL->setContent(source, &reader, errorMsg, errorLine, errorColumn);
}

/*!
    \overload

    This function reads the XML document from the string \a text, returning
    true if the content was successfully parsed; otherwise returns false.
    Since \a text is already a Unicode string, no encoding detection
    is performed.

    No namespace processing is performed either.
*/
bool document::setContent(const pfs::string& text, pfs::string *errorMsg, int *errorLine, int *errorColumn)
{
    return setContent(text, false, errorMsg, errorLine, errorColumn);
}

/*!
    \overload

    This function reads the XML document from the byte array \a buffer,
    returning true if the content was successfully parsed; otherwise returns
    false.

    No namespace processing is performed.
*/
bool document::setContent(const QByteArray& buffer, pfs::string *errorMsg, int *errorLine, int *errorColumn )
{
    return setContent(buffer, false, errorMsg, errorLine, errorColumn);
}

/*!
    \overload

    This function reads the XML document from the IO device \a dev, returning
    true if the content was successfully parsed; otherwise returns false.

    No namespace processing is performed.
*/
bool document::setContent(QIODevice* dev, pfs::string *errorMsg, int *errorLine, int *errorColumn )
{
    return setContent(dev, false, errorMsg, errorLine, errorColumn);
}

/*!
    \overload

    This function reads the XML document from the QXmlInputSource \a source and
    parses it with the QXmlReader \a reader, returning true if the content was
    successfully parsed; otherwise returns false.

    This function doesn't change the features of the \a reader. If you want to
    use certain features for parsing you can use this function to set up the
    reader appropriately.

    \sa QXmlSimpleReader
*/
bool document::setContent(QXmlInputSource *source, QXmlReader *reader, pfs::string *errorMsg, int *errorLine, int *errorColumn )
{
    if (!impl)
        impl = new document::impl();
    return IMPL->setContent(source, reader, errorMsg, errorLine, errorColumn);
}

/*!
    Converts the parsed document back to its textual representation.

    This function uses \a indent as the amount of space to indent
    subelements.

    If \a indent is -1, no whitespace at all is added.
*/
pfs::string document::toString(int indent) const
{
    pfs::string str;
    QTextStream s(&str, QIODevice::WriteOnly);
    save(s, indent);
    return str;
}

/*!
    Converts the parsed document back to its textual representation
    and returns a QByteArray containing the data encoded as UTF-8.

    This function uses \a indent as the amount of space to indent
    subelements.

    \sa toString()
*/
QByteArray document::toByteArray(int indent) const
{
    // ### if there is an encoding specified in the xml declaration, this
    // encoding declaration should be changed to utf8
    return toString(indent).toUtf8();
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
dom_implementation document::implementation() const
{
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
        impl = new document::impl();
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
        impl = new document::impl();
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
        impl = new document::impl();
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
        impl = new document::impl();
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
        impl = new document::impl();
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
        impl = new document::impl();
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
        impl = new document::impl();
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
        impl = new document::impl();
    return entity_reference(IMPL->createEntityReference(name));
}

/*!
    Returns a QDomNodeList, that contains all the elements in the
    document with the name \a tagname. The order of the node list is
    the order they are encountered in a preorder traversal of the
    element tree.

    \sa elementsByTagNameNS(), element::elementsByTagName()
*/
QDomNodeList document::elementsByTagName(const pfs::string& tagname) const
{
    return QDomNodeList(new QDomNodeListPrivate(impl, tagname));
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
        impl = new document::impl();
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
        impl = new document::impl();
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
        impl = new document::impl();
    return attr(IMPL->createAttributeNS(nsURI, qName));
}

/*!
    Returns a QDomNodeList that contains all the elements in the
    document with the local name \a localName and a namespace URI of
    \a nsURI. The order of the node list is the order they are
    encountered in a preorder traversal of the element tree.

    \sa elementsByTagName(), element::elementsByTagNameNS()
*/
QDomNodeList document::elementsByTagNameNS(const pfs::string& nsURI, const pfs::string& localName)
{
    return QDomNodeList(new QDomNodeListPrivate(impl, nsURI, localName));
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

/**************************************************************
 *
 * Node casting functions
 *
 **************************************************************/

/*!
    Converts a node into a attr. If the node is not an
    attribute, the returned object will be \l{node::isNull()}{null}.

    \sa isAttr()
*/
attr node::toAttr() const
{
    if (impl && impl->isAttr())
        return attr(((attr::impl*)impl));
    return attr();
}

/*!
    Converts a node into a cdatasection. If the node is not a
    CDATA section, the returned object will be \l{node::isNull()}{null}.

    \sa isCDATASection()
*/
cdatasection node::toCDATASection() const
{
    if (impl && impl->isCDATASection())
        return cdatasection(((cdatasection::impl*)impl));
    return cdatasection();
}

/*!
    Converts a node into a document_fragment. If the node is
    not a document fragment the returned object will be \l{node::isNull()}{null}.

    \sa isDocumentFragment()
*/
document_fragment node::toDocumentFragment() const
{
    if (impl && impl->isDocumentFragment())
        return document_fragment(((document_fragment::impl*)impl));
    return document_fragment();
}

/*!
    Converts a node into a document. If the node is not a
    document the returned object will be \l{node::isNull()}{null}.

    \sa isDocument()
*/
document node::toDocument() const
{
    if (impl && impl->isDocument())
        return document(((document::impl*)impl));
    return document();
}

/*!
    Converts a node into a document_type. If the node is not a
    document type the returned object will be \l{node::isNull()}{null}.

    \sa isDocumentType()
*/
document_type node::toDocumentType() const
{
    if (impl && impl->isDocumentType())
        return document_type(((document_type::impl*)impl));
    return document_type();
}

/*!
    Converts a node into a element. If the node is not an
    element the returned object will be \l{node::isNull()}{null}.

    \sa isElement()
*/
element node::toElement() const
{
    if (impl && impl->isElement())
        return element(((element::impl*)impl));
    return element();
}

/*!
    Converts a node into a entity_reference. If the node is not
    an entity reference, the returned object will be \l{node::isNull()}{null}.

    \sa isEntityReference()
*/
entity_reference node::toEntityReference() const
{
    if (impl && impl->isEntityReference())
        return entity_reference(((entity_reference::impl*)impl));
    return entity_reference();
}

/*!
    Converts a node into a text. If the node is not a text,
    the returned object will be \l{node::isNull()}{null}.

    \sa isText()
*/
text node::toText() const
{
    if (impl && impl->isText())
        return text(((text::impl*)impl));
    return text();
}

/*!
    Converts a node into a entity. If the node is not an
    entity the returned object will be \l{node::isNull()}{null}.

    \sa isEntity()
*/
entity node::toEntity() const
{
    if (impl && impl->isEntity())
        return entity(((entity::impl*)impl));
    return entity();
}

/*!
    Converts a node into a notation. If the node is not a
    notation the returned object will be \l{node::isNull()}{null}.

    \sa isNotation()
*/
notation node::toNotation() const
{
    if (impl && impl->isNotation())
        return notation(((notation::impl*)impl));
    return notation();
}

/*!
    Converts a node into a processing_instruction. If the node
    is not a processing instruction the returned object will be \l{node::isNull()}{null}.

    \sa isProcessingInstruction()
*/
processing_instruction node::toProcessingInstruction() const
{
    if (impl && impl->isProcessingInstruction())
        return processing_instruction(((processing_instruction::impl*)impl));
    return processing_instruction();
}

/*!
    Converts a node into a chardata. If the node is not a
    character data node the returned object will be \l{node::isNull()}{null}.

    \sa isCharacterData()
*/
chardata node::toCharacterData() const
{
    if (impl && impl->isCharacterData())
        return chardata(((chardata::impl*)impl));
    return chardata();
}

/*!
    Converts a node into a comment. If the node is not a
    comment the returned object will be \l{node::isNull()}{null}.

    \sa isComment()
*/
comment node::toComment() const
{
    if (impl && impl->isComment())
        return comment(((comment::impl*)impl));
    return comment();
}


#endif
