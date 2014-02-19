#include "node_p.hpp"
#include "nodelist_p.hpp"
#include "namednodemap_p.hpp"
#include "attr_p.hpp"
#include "element_p.hpp"
#include "document_p.hpp"
#include "domimpl_p.hpp"
#include "chardata_p.hpp"
#include "text_p.hpp"

namespace cwt { namespace dom {

inline void node_impl::setOwnerDocument (document_impl * doc)
{
    _ownerNode = doc;
    _hasParent = false;
}

node_impl::node_impl(document_impl * d, node_impl * parent) : ref(1)
{
    if (parent)
        setParent (parent);
    else
        setOwnerDocument (d);

    _prev  = nullptr;
    _next  = nullptr;
    _first = nullptr;
    _last  = nullptr;
    _createdWithDom1Interface = true;
}

node_impl::node_impl (node_impl * n, bool deep)
	: ref(1)
{
    setOwnerDocument(n->ownerDocument());
    _prev  = nullptr;
    _next  = nullptr;
    _first = nullptr;
    _last  = nullptr;

    _name = n->_name;
    _value = n->_value;
    _prefix = n->_prefix;
    _namespaceURI = n->_namespaceURI;
    _createdWithDom1Interface = n->_createdWithDom1Interface;

    if (!deep)
        return;

    for (node_impl * x = n->_first; x; x = x->_next)
        appendChild(x->cloneNode(true));
}

node_impl::~node_impl ()
{
    node_impl * p = _first;
    node_impl * n;

    while (p) {
        n = p->_next;
        if (!p->ref.deref())
            delete p;
        else
            p->setNoParent();
        p = n;
    }
    _first = 0;
    _last = 0;
}

document_impl * node_impl::ownerDocument ()
{
    node_impl * p = this;
    while (p && !p->isDocument()) {
        if (!p->_hasParent)
            return (document_impl *)p->_ownerNode;
        p = p->parent();
    }

    return static_cast<document_impl *>(p);
}

node_impl * node_impl::cloneNode (bool deep)
{
    node_impl * p = new node_impl(this, deep);
    p->ref.deref();
    return p;
}

node_impl * node_impl::insertAfter (node_impl * newChild, node_impl * refChild)
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
    document_impl * const doc = ownerDocument();
    if(doc)
        ++doc->_nodeListTime;

    // Special handling for inserting a fragment. We just insert
    // all elements of the fragment instead of the fragment itself.
    if (newChild->isDocumentFragment()) {
        // Fragment is empty ?
        if (newChild->_first == 0)
            return newChild;

        // New parent
        node_impl* n = newChild->_first;
        while (n) {
            n->setParent(this);
            n = n->_next;
        }

        // Insert at the end
        if (!refChild || refChild->_next == 0) {
            if (_last)
                _last->_next = newChild->_first;
            newChild->_first->_prev = _last;
            if (!_first)
                _first = newChild->_first;
            _last = newChild->_last;
        } else { // Insert in the middle
            newChild->_first->_prev = refChild;
            newChild->_last->_next = refChild->_next;
            refChild->_next->_prev = newChild->_last;
            refChild->_next = newChild->_first;
        }

        // No need to increase the reference since document_fragment
        // does not decrease the reference.

        // Remove the nodes from the fragment
        newChild->_first = 0;
        newChild->_last = 0;
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
        if (_last)
            _last->_next = newChild;
        newChild->_prev = _last;
        if (!_first)
            _first = newChild;
        _last = newChild;
        return newChild;
    }

    if (refChild->_next == 0) {
        if (_last)
            _last->_next = newChild;
        newChild->_prev = _last;
        if (!_first)
            _first = newChild;
        _last = newChild;
        return newChild;
    }

    newChild->_prev = refChild;
    newChild->_next = refChild->_next;
    refChild->_next->_prev = newChild;
    refChild->_next = newChild;

    return newChild;
}

node_impl * node_impl::insertBefore (node_impl * newChild, node_impl * refChild)
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
    document_impl * const doc = ownerDocument();
    if (doc)
        ++doc->_nodeListTime;

    // Special handling for inserting a fragment. We just insert
    // all elements of the fragment instead of the fragment itself.
    if (newChild->isDocumentFragment()) {
        // Fragment is empty ?
        if (newChild->_first == 0)
            return newChild;

        // New parent
        node_impl * n = newChild->_first;
        while (n)  {
            n->setParent(this);
            n = n->_next;
        }

        // Insert at the beginning ?
        if (!refChild || refChild->_prev == 0) {
            if (_first)
                _first->_prev = newChild->_last;
            newChild->_last->_next = _first;
            if (!_last)
                _last = newChild->_last;
            _first = newChild->_first;
        } else {
            // Insert in the middle
            newChild->_last->_next = refChild;
            newChild->_first->_prev = refChild->_prev;
            refChild->_prev->_next = newChild->_first;
            refChild->_prev = newChild->_last;
        }

        // No need to increase the reference since document_fragment
        // does not decrease the reference.

        // Remove the nodes from the fragment
        newChild->_first = 0;
        newChild->_last = 0;
        return newChild;
    }

    // No more errors can occur now, so we take
    // ownership of the node.
    newChild->ref.ref();

    if (newChild->parent())
        newChild->parent()->removeChild(newChild);

    newChild->setParent(this);

    if (!refChild) {
        if (_first)
            _first->_prev = newChild;

        newChild->_next = _first;

        if (!_last)
            _last = newChild;

        _first = newChild;
        return newChild;
    }

    if (refChild->_prev == 0) {
        if (_first)
            _first->_prev = newChild;

        newChild->_next = _first;

        if (!_last)
            _last = newChild;

        _first = newChild;
        return newChild;
    }

    newChild->_next = refChild;
    newChild->_prev = refChild->_prev;
    refChild->_prev->_next = newChild;
    refChild->_prev = newChild;

    return newChild;
}

node_impl * node_impl::appendChild (node_impl * newChild)
{
    return insertAfter(newChild, 0);
}

node_impl * node_impl::removeChild (node_impl * oldChild)
{
    // Error check
    if (oldChild->parent() != this)
        return 0;

    // "mark lists as dirty"
    document_impl * const doc = ownerDocument();

    if(doc)
        ++doc->_nodeListTime;

    // Perhaps oldChild was just created with "createElement" or that. In this case
    // its parent is document but it is not part of the documents child list.
    if (oldChild->_next == 0 && oldChild->_prev == 0 && _first != oldChild)
        return 0;

    if (oldChild->_next)
        oldChild->_next->_prev = oldChild->_prev;

    if (oldChild->_prev)
        oldChild->_prev->_next = oldChild->_next;

    if (_last == oldChild)
        _last = oldChild->_prev;

    if (_first == oldChild)
        _first = oldChild->_next;

    oldChild->setNoParent();
    oldChild->_next = 0;
    oldChild->_prev = 0;

    // We are no longer interested in the old node
    oldChild->ref.deref();

    return oldChild;
}

node_impl * node_impl::replaceChild (node_impl * newChild, node_impl * oldChild)
{
    if (!newChild || !oldChild)
        return 0;

    if (oldChild->parent() != this)
        return 0;

    if (newChild == oldChild)
        return 0;

    // mark lists as dirty
    document_impl * const doc = ownerDocument();
    if(doc)
        ++doc->_nodeListTime;

    // Special handling for inserting a fragment. We just insert
    // all elements of the fragment instead of the fragment itself.
    if (newChild->isDocumentFragment()) {
        // Fragment is empty ?
        if (newChild->_first == 0)
            return newChild;

        // New parent
        node_impl * n = newChild->_first;

        while (n) {
            n->setParent(this);
            n = n->_next;
        }

        if (oldChild->_next)
            oldChild->_next->_prev = newChild->_last;

        if (oldChild->_prev)
            oldChild->_prev->_next = newChild->_first;

        newChild->_last->_next = oldChild->_next;
        newChild->_first->_prev = oldChild->_prev;

        if (_first == oldChild)
            _first = newChild->_first;

        if (_last == oldChild)
            _last = newChild->_last;

        oldChild->setNoParent();
        oldChild->_next = 0;
        oldChild->_prev = 0;

        // No need to increase the reference since document_fragment
        // does not decrease the reference.

        // Remove the nodes from the fragment
        newChild->_first = 0;
        newChild->_last = 0;

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

    if (oldChild->_next)
        oldChild->_next->_prev = newChild;
    if (oldChild->_prev)
        oldChild->_prev->_next = newChild;

    newChild->_next = oldChild->_next;
    newChild->_prev = oldChild->_prev;

    if (_first == oldChild)
        _first = newChild;
    if (_last == oldChild)
        _last = newChild;

    oldChild->setNoParent();
    oldChild->_next = 0;
    oldChild->_prev = 0;

    // We are no longer interested in the old node
    if (oldChild)
        oldChild->ref.deref();

    return oldChild;
}


void node_impl::normalize()
{
    node_impl * p = this->_first;
    text_impl * t = nullptr;

    while (p) {
        if (p->isText()) {
            if (t) {
                node_impl * tmp = p->_next;
                t->appendData(p->nodeValue());
                this->removeChild(p);
                p = tmp;
            } else {
                t = (text_impl*)p;
                p = p->_next;
            }
        } else {
            p = p->_next;
            t = nullptr;
        }
    }
}


node::node (const node & n)
{
    _pimpl = n._pimpl;
    if (_pimpl)
        _pimpl->ref.ref();
}

node::node (node_impl * n)
{
    _pimpl = n;
    if (_pimpl)
        _pimpl->ref.ref();
}


node & node::operator = (const node & other)
{
	if (other._pimpl)
		other._pimpl->ref.ref();

	if (_pimpl && !_pimpl->ref.deref())
		delete _pimpl;

	_pimpl = other._pimpl;
	return *this;
}

node::~node ()
{
    if (_pimpl && !_pimpl->ref.deref()) {
    	delete _pimpl;
        _pimpl = nullptr;
    }
}

node node::cloneNode (bool deep) const
{
    if (!_pimpl)
        return node();
    return node(_pimpl->cloneNode(deep));
}


/**
 * @brief Return the name of this node, depending on its type.
 *
 * @return The name of this node:
 * 		@arg Attr                  - name of attribute
 * 		@arg CDATASection          - #cdata-section
 * 		@arg Comment               - #comment
 * 		@arg Document              - #document
 * 		@arg DocumentFragment      - #document-fragment
 * 		@arg DocumentType          - document type name
 * 		@arg Element               - tag name
 * 		@arg Entity                - entity name
 * 		@arg EntityReference       - name of entity referenced
 * 		@arg Notation              - notation name
 * 		@arg ProcessingInstruction - target
 * 		@arg Text                  - #text
 */
pfs::string node::nodeName () const
{
    if (!_pimpl)
    	return pfs::string();

    if (!_pimpl->_prefix.isEmpty())
    	return _pimpl->_prefix + pfs::ucchar(':') + _pimpl->_name;

    return _pimpl->_name;
}

/**
 * @brief Return the value of this node, depending on its type
 *
 * @return The value of this node:
 * 		@arg Attr                  - value of attribute
 * 		@arg CDATASection          - content of the CDATA Section
 * 		@arg Comment               - content of the comment
 * 		@arg Document              - null
 * 		@arg DocumentFragment      - null
 * 		@arg DocumentType          - null
 * 		@arg Element               - null
 * 		@arg Entity                - entity name
 * 		@arg EntityReference       - null
 * 		@arg Notation              - null
 * 		@arg ProcessingInstruction - entire content excluding the target
 * 		@arg Text - content of the text node
 */
pfs::string node::nodeValue () const
{
    return _pimpl
    		? _pimpl->_value
    		: pfs::string();
}

node::type node::nodeType () const
{
    return _pimpl
    		? _pimpl->nodeType()
    		: node::InvalidNode;
}

document node::ownerDocument () const
{
    return _pimpl
    		? document(_pimpl->ownerDocument())
    		: document();
}

/*!
    Returns the first child of the node. If there is no child node, a
    \l{isNull()}{null node} is returned. Changing the
    returned node will also change the node in the document tree.
V
    \sa lastChild(), childNodes()
*/
node node::firstChild () const
{
    return _pimpl
    		? node(_pimpl->_first)
    		: node();
}

node node::lastChild () const
{
    return _pimpl
    		? node(_pimpl->_last)
    		: node();
}


node node::previousSibling() const
{
    return _pimpl
    	? node(_pimpl->_prev)
        : node();
}


node node::nextSibling() const
{
    return _pimpl
    	? node(_pimpl->_next)
        : node();
}

pfs::string node::namespaceURI() const
{
    return _pimpl
    	? _pimpl->_namespaceURI
        : pfs::string();
}

pfs::string node::prefix() const
{
    return _pimpl
        ? _pimpl->_prefix
        : pfs::string();
}


/**
 * @details Returns the local part of the qualified name of this node.
 * 		For nodes of any type other than @c ElementNode and @c AttributeNode
 * 		and nodes created with a DOM Level 1 method, such as @c createElement
 * 		from the Document interface, this is always null.
 *
 * @return Returns the local part of the qualified name of this node.
 */
pfs::string node::localName () const
{
    if (!_pimpl || _pimpl->_createdWithDom1Interface)
        return pfs::string();
    return _pimpl->_name;
}


bool node::isSupported (const pfs::string & feature, const pfs::string & version) const
{
    dom_implementation i;
    return i.hasFeature(feature, version);
}


node node::insertAfter (const node & newChild, const node & refChild)
{
    if (!_pimpl)
        return node();
    return node(_pimpl->insertAfter(newChild._pimpl, refChild._pimpl));
}

/**
 * @details Inserts the node newChild before the existing child node refChild.
 * 		If refChild is null, insert newChild at the end of the list of children.
 * 		If newChild is a DocumentFragment object, all of its children are inserted,
 * 		in the same order, before refChild.
 * 		If the newChild is already in the tree, it is first removed.
 *
 * @param newChild The node to insert.
 * @param refChild The reference node, i.e., the node before which the new node must be inserted.
 * @return The node being inserted.
 * @throw @a cwt::dom::exception
 */
node node::insertBefore (const node & newChild, const node & refChild)
{
    if (!_pimpl)
        return node();
    return node(_pimpl->insertBefore(newChild._pimpl, refChild._pimpl));
}

node node::appendChild (const node & newChild)
{
    if (!_pimpl) {
        return node();
    }
    return node(_pimpl->appendChild(newChild._pimpl));
}

/**
 * @details Removes the child node indicated by oldChild
 * 		from the list of children, and returns it.
 *
 * @param oldChild The node being removed.
 * @return The node removed.
 * @throw @a cwt::dom::exception
 */
node node::removeChild (const node& oldChild)
{
    if (!_pimpl)
        return node();

    if (oldChild.isNull())
        return node();

    return node(_pimpl->removeChild(oldChild._pimpl));
}

node node::replaceChild (const node & newChild, const node & oldChild)
{
    if (!_pimpl || !newChild._pimpl || !oldChild._pimpl)
        return node();
    return node(_pimpl->replaceChild(newChild._pimpl, oldChild._pimpl));
}

node node::parentNode () const
{
    return _pimpl
    		? node(_pimpl->parent())
    		: node();
}

void node::normalize()
{
    if (_pimpl)
        _pimpl->normalize();
}

/*!
    Sets the node's value to \a v.

    \sa nodeValue()
*/
void node::setNodeValue (const pfs::string & v)
{
    if (_pimpl)
    	_pimpl->setNodeValue(v);
}


/**
 * @details For nodes of any type other than @c ElementNode and @c AttributeNode
 * 		and nodes created with a DOM Level 1 method, such as createElement
 * 		from the Document interface, this is always null.
 * 		A namespace prefix must have be specified at creation time;
 * 		it is not possible to add a namespace prefix afterwards.
 *
 * @param p New prefix name.
 */
void node::setPrefix (const pfs::string & pre)
{
    if (!_pimpl || _pimpl->_prefix.isNull())
        return;
    if (_pimpl->isAttr() || _pimpl->isElement())
        _pimpl->_prefix = pre;
}

nodelist node::childNodes() const
{
	return _pimpl
			? nodelist(new nodelist_impl(_pimpl))
			: nodelist();
}


bool node::hasAttributes() const
{
    if (!_pimpl || !_pimpl->isElement())
        return false;
    return static_cast<element_impl *>(_pimpl)->hasAttributes();
}


bool node::hasChildNodes () const
{
	return _pimpl
			? _pimpl->_first != nullptr
			: false;
}

namednodemap node::attributes() const
{
    if (!_pimpl || !_pimpl->isElement())
        return namednodemap();

    return namednodemap(static_cast<element_impl *>(_pimpl)->attributes());
}

}} // cwt::dom
