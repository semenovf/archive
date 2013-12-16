#include "node_p.hpp"
#include "nodelist_p.hpp"
#include "namednodemap_p.hpp"
#include "element_p.hpp"
#include "document_p.hpp"
#include "domimpl_p.hpp"
#include "chardata_p.hpp"
#include "text_p.hpp"

namespace cwt { namespace dom {

inline void node_impl::setOwnerDocument (document_impl * doc)
{
    ownerNode = doc;
    hasParent = false;
}


node_impl::node_impl(node_impl *n, bool deep)
	: ref(1)
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

    if (!deep)
        return;

    for (node_impl* x = n->first; x; x = x->next)
        appendChild(x->cloneNode(true));
}

node_impl::~node_impl()
{
    node_impl* p = first;
    node_impl* n;

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

document_impl * node_impl::ownerDocument()
{
    node_impl * p = this;
    while (p && !p->isDocument()) {
        if (!p->hasParent)
            return (document_impl *)p->ownerNode;
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

node_impl * node_impl::insertAfter(node_impl * newChild, node_impl * refChild)
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
    document_impl *const doc = ownerDocument();
    if(doc)
        ++doc->nodeListTime;

    // Special handling for inserting a fragment. We just insert
    // all elements of the fragment instead of the fragment itself.
    if (newChild->isDocumentFragment()) {
        // Fragment is empty ?
        if (newChild->first == 0)
            return newChild;

        // New parent
        node_impl* n = newChild->first;
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

node_impl * node_impl::insertBefore(node_impl * newChild, node_impl * refChild)
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
        doc->nodeListTime++;

    // Special handling for inserting a fragment. We just insert
    // all elements of the fragment instead of the fragment itself.
    if (newChild->isDocumentFragment()) {
        // Fragment is empty ?
        if (newChild->first == 0)
            return newChild;

        // New parent
        node_impl * n = newChild->first;
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
        ++doc->nodeListTime;

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
        ++doc->nodeListTime;

    // Special handling for inserting a fragment. We just insert
    // all elements of the fragment instead of the fragment itself.
    if (newChild->isDocumentFragment()) {
        // Fragment is empty ?
        if (newChild->first == 0)
            return newChild;

        // New parent
        node_impl * n = newChild->first;

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


void node_impl::normalize()
{
    node_impl * p = this->first;
    text_impl * t = nullptr;

    while (p) {
        if (p->isText()) {
            if (t) {
                node_impl * tmp = p->next;
                t->appendData(p->nodeValue());
                this->removeChild(p);
                p = tmp;
            } else {
                t = (text_impl*)p;
                p = p->next;
            }
        } else {
            p = p->next;
            t = 0;
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

node::type node::nodeType () const
{
    return _pimpl
    		? _pimpl->nodeType()
    		: node::BaseNode;
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

    \sa lastChild(), childNodes()
*/
node node::firstChild () const
{
    return _pimpl
    		? node(_pimpl->first)
    		: node();
}

node node::lastChild () const
{
    return _pimpl
    		? node(_pimpl->last)
    		: node();
}


node node::previousSibling() const
{
    return _pimpl
    	? node(_pimpl->prev)
        : node();
}


node node::nextSibling() const
{
    return _pimpl
    	? node(_pimpl->next)
        : node();
}

pfs::string node::namespaceURI() const
{
    return _pimpl
    	? _pimpl->namespaceURI
        : pfs::string();
}

pfs::string node::prefix() const
{
    return _pimpl
        ? _pimpl->prefix
        : pfs::string();
}


pfs::string node::localName() const
{
    if (!_pimpl || _pimpl->createdWithDom1Interface)
        return pfs::string();
    return _pimpl->name;
}


bool node::isSupported(const pfs::string & feature, const pfs::string & version) const
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

node node::removeChild(const node& oldChild)
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


void node::setPrefix (const pfs::string & pre)
{
    if (!_pimpl || _pimpl->prefix.isNull())
        return;
    if (_pimpl->isAttr() || _pimpl->isElement())
        _pimpl->prefix = pre;
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
			? _pimpl->first != nullptr
			: false;
}

namednodemap node::attributes() const
{
    if (!_pimpl || !_pimpl->isElement())
        return namednodemap();

    return namednodemap(static_cast<element_impl *>(_pimpl)->attributes());
}


/*
node_impl_impl (const node_impl & n, bool deep)
	: _name(n._name)
	, _value(n._value)
	, _namespaceURI(n._namespaceURI)
	, _prefix(n._prefix)
{
    setParent(n._parent);

    if (!deep)
        return;

    for (node_impl * x = n._first.get(); x; x = x->_next.get())
        appendChild(x->cloneNode(true));

}

node_impl * node_impl::cloneNode (bool deep)
{
	return new node_impl(this, deep);
}
*/

#ifdef __COMMENT__
document_impl * node_impl::ownerDocument ()
{
	node_impl * p = this;
	while (p && !p->isDocument()) {
		p = p->_parent;
	}

	return static_cast<document_impl *>(p);
}

void node_impl::markDirty ()
{
	document_impl * const doc = ownerDocument();
	if (doc)
		++doc->_timestamp;
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
	return _pimpl
		? _pimpl->_prefix.isEmpty()
				? _pimpl->_name
				: _pimpl->_prefix + pfs::ucchar(':') + _pimpl->_name
		: pfs::string();
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

node node::previousSibling () const
{
	return _pimpl
			? node(_pimpl->_prev)
			: node();
}

node node::nextSibling () const
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

pfs::string node::prefix () const
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
	return _pimpl
			? _pimpl->_localName
			: pfs::string();
}

document node::ownerDocument () const
{
	if (!_pimpl)
		return document();

	pimpl_type r = _pimpl;
	while (r.get() && !r->isDocument()) {
		r = r->_parent;
	}

	return document(r);
}


bool node::isSupported (const pfs::string & feature, const pfs::string & version) const
{
    dom_implementation i;
    return i.hasFeature(feature, version);
}


bool node::hasChildNodes () const
{
	return _pimpl
			? _pimpl->_first.get() != nullptr
			: false;
}

bool node::hasAttributes() const
{
    if (!_pimpl || !_pimpl->isElement())
        return false;

    return dynamic_pointer_cast<element_impl>(_pimpl)->attrCount() > 0;
}

void node::setNodeValue (const pfs::string & value)
{
	if (_pimpl) {
		_pimpl->setNodeValue(value);
	}
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
void node::setPrefix (const pfs::string & p)  // raises(DOMException)
{
	if (_pimpl
			&& !_pimpl->_prefix.isEmpty()
			&& (_pimpl->isAttribute() || _pimpl->isElement())) {
		_pimpl->_prefix = p;
	}
}


nodelist node::childNodes () const
{
	return _pimpl
			? nodelist(new nodelist_impl(_pimpl))
			: nodelist();
}


namednodemap node::attributes () const
{
    if (!_pimpl || !_pimpl->isElement())
        return namednodemap();

    return dynamic_pointer_cast<element_impl>(_pimpl)->_attr;
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
node node::insertBefore (const node & newChild, const node & refChild) // raises(DOMException)
{
	if (!_pimpl)
		return node();

	if (!newChild._pimpl)
		return node();

	if (newChild == refChild)
		return node();

	if (refChild._pimpl && refChild.parentNode() != *this)
		return node();

	_pimpl->markDirty();

	// Special handling for inserting a fragment. We just insert
	// all elements of the fragment instead of the fragment itself.
	if (newChild._pimpl->isDocumentFragment()) {
		// Fragment is empty ?
		if (!newChild._pimpl->_first)
			return newChild;

		// New parent
		node::pimpl_type n = newChild._pimpl->_first;
		while (n)  {
			n->setParent(_pimpl);
			n = n->_next;
		}

		// Insert at the beginning ?
		if (!refChild._pimpl || !refChild._pimpl->_prev) {
			if (_pimpl->_first)
				_pimpl->_first->_prev = newChild._pimpl->_last;

			newChild._pimpl->_last->_next = _pimpl->_first;
			if (!_pimpl->_last)
				_pimpl->_last = newChild._pimpl->_last;
			_pimpl->_first = newChild._pimpl->_first;
		} else {
			// Insert in the middle
			newChild._pimpl->_last->_next = refChild._pimpl;
			newChild._pimpl->_first->_prev = refChild._pimpl->_prev;
			refChild._pimpl->_prev->_next = newChild._pimpl->_first;
			refChild._pimpl->_prev = newChild._pimpl->_last;
		}

		// Remove the nodes from the fragment
        node_impl::deref(newChild._pimpl->_first);
        node_impl::deref(newChild._pimpl->_last);

        return newChild;
	}

	if (newChild._pimpl->parent())
		newChild.parentNode().removeChild(newChild);

	newChild._pimpl->setParent(_pimpl);

	if (!refChild._pimpl) {
		if (_pimpl->_first)
			_pimpl->_first->_prev = newChild._pimpl;
		newChild._pimpl->_next = _pimpl->_first;
		if (!_pimpl->_last)
			_pimpl->_last = newChild._pimpl;
		_pimpl->_first = newChild._pimpl;
		return newChild;
	}

	if (!refChild._pimpl->_prev) {
		if (_pimpl->_first)
			_pimpl->_first->_prev = newChild._pimpl;
		newChild._pimpl->_next = _pimpl->_first;
		if (!_pimpl->_last)
			_pimpl->_last = newChild._pimpl;
		_pimpl->_first = newChild._pimpl;
		return newChild;
	}

	newChild._pimpl->_next = refChild._pimpl;
	newChild._pimpl->_prev = refChild._pimpl->_prev;
	refChild._pimpl->_prev->_next = newChild._pimpl;
	refChild._pimpl->_prev = newChild._pimpl;

	return newChild;
}

node node::insertAfter (const node & newChild, const node & refChild)
{
    if (!newChild._pimpl)
        return node();

    if (newChild == refChild)
        return node();

	if (refChild._pimpl && refChild.parentNode() != *this)
		return node();

	_pimpl->markDirty();

    // Special handling for inserting a fragment. We just insert
    // all elements of the fragment instead of the fragment itself.
    if (newChild._pimpl->isDocumentFragment()) {

        if (!newChild._pimpl->_first)
            return newChild;

		node::pimpl_type n = newChild._pimpl->_first;
		while (n)  {
			n->setParent(_pimpl);
			n = n->_next;
		}

        // Insert at the end
        if (!refChild._pimpl || !refChild._pimpl->_next) {
            if (_pimpl->_last)
                _pimpl->_last->_next = newChild._pimpl->_first;
            newChild._pimpl->_first->_prev = _pimpl->last;

            if (!_pimpl->_first)
                _pimpl->_first = newChild._pimpl->_first;
            _pimpl->_last = newChild._pimpl->_last;
        } else { // Insert in the middle
            newChild._pimpl->_first->_prev = refChild._pimpl;
            newChild._pimpl->_last->_next = refChild._pimpl->_next;
            refChild._pimpl->_next->_prev = newChild._pimpl->_last;
            refChild._pimpl->_next = newChild._pimpl->_first;
        }

        // Remove the nodes from the fragment
        node_impl::deref(newChild._pimpl->_first);
        node_impl::deref(newChild._pimpl->_last);

        return newChild;
    }

    // Release new node from its current parent
	if (newChild._pimpl->parent())
		newChild.parentNode().removeChild(newChild);

    // No more errors can occur now, so we take
    // ownership of the node
    //newChild->ref.ref(); FIXME ?

	newChild._pimpl->setParent(_pimpl);

    // Insert at the end
    if (!refChild._pimpl) {
        if (_pimpl->_last)
            _pimpl->_last->_next = newChild._pimpl;

        newChild._pimpl->_prev = _pimpl->_last;

        if (!_pimpl->_first)
            _pimpl->_first = newChild._pimpl;
        _pimpl->_last = newChild._pimpl;
        return newChild;
    }

    if (!refChild._pimpl->_next) {
        if (_pimpl->_last)
            _pimpl->_last->_next = newChild._pimpl;
        newChild._pimpl->_prev = _pimpl->_last;
        if (!_pimpl->_first)
            _pimpl->_first = newChild._pimpl;
        _pimpl->_last = newChild._pimpl;
        return newChild;
    }

    newChild._pimpl->_prev = refChild._pimpl;
    newChild._pimpl->_next = refChild._pimpl->_next;
    refChild._pimpl->_next->_prev = newChild._pimpl;
    refChild._pimpl->_next = newChild._pimpl;

    return newChild;
}

node node::replaceChild (const node & newChild, const node & oldChild)
{
    if (!_pimpl
    		|| !newChild._pimpl
    		|| !oldChild._pimpl)
        return node();

	if (oldChild.parentNode() != *this)
		return node();

    if (newChild == oldChild)
        return node;

    _pimpl->markDirty();

    // Special handling for inserting a fragment. We just insert
    // all elements of the fragment instead of the fragment itself.
    if (newChild._pimpl->isDocumentFragment()) {
        // Fragment is empty ?
        if (!newChild._pimpl->_first)
            return newChild;

		// New parent
		node::pimpl_type n = newChild._pimpl->_first;
		while (n)  {
			n->setParent(_pimpl);
			n = n->_next;
		}

        if (oldChild._pimpl->_next)
            oldChild._pimpl->_next->_prev = newChild._pimpl->_last;
        if (oldChild._pimpl->_prev)
            oldChild._pimpl->_prev->_next = newChild._pimpl->_first;

        newChild._pimpl->_last->_next = oldChild._pimpl->_next;
        newChild._pimpl->_first->_prev = oldChild._pimpl->_prev;

        if (_pimpl->_first == oldChild._pimpl)
            _pimpl->_first = newChild._pimpl->_first;
        if (_pimpl->_last == oldChild._pimpl)
            _pimpl->_last = newChild._pimpl->_last;

        oldChild._pimpl->clearParent();
        node_impl::deref(oldChild._pimpl->_next);
        node_impl::deref(oldChild._pimpl->_prev);

        // Remove the nodes from the fragment
        node_impl::deref(newChild._pimpl->_first);
        node_impl::deref(newChild._pimpl->_last);

        // We are no longer interested in the old node
//        if (oldChild) // FIXME
//            oldChild->ref.deref();

        return oldChild;
    }

    // No more errors can occur now, so we take
    // ownership of the node
    // newChild->ref.ref(); FIXME

    // Release new node from its current parent
	if (newChild._pimpl->parent())
		newChild.parentNode().removeChild(newChild);

	newChild._pimpl->setParent(_pimpl);

    if (oldChild._pimpl->_next)
        oldChild._pimpl->_next->_prev = newChild._pimpl;
    if (oldChild._pimpl->_prev)
        oldChild._pimpl->_prev->_next = newChild._pimpl;

    newChild._pimpl->_next = oldChild._pimpl->_next;
    newChild._pimpl->_prev = oldChild._pimpl->_prev;

    if (_pimpl->_first == oldChild._pimpl)
        _pimpl->_first = newChild._pimpl;
    if (_pimpl->_last == oldChild._pimpl)
        _pimpl->_last = newChild._pimpl;

    oldChild._pimpl->clearParent();
    node_impl::deref(oldChild._pimpl->_next);
    node_impl::deref(oldChild._pimpl->_prev);

    // We are no longer interested in the old node
//    if (oldChild)
//        oldChild->ref.deref(); // FIXME

    return oldChild;
}

/**
 * @details Removes the child node indicated by oldChild
 * 		from the list of children, and returns it.
 *
 * @param oldChild The node being removed.
 * @return The node removed.
 * @throw @a cwt::dom::exception
 */
node node::removeChild (const node & oldChild)
{
    if (!_pimpl)
        return node();

    if (!oldChild._pimpl)
        return node();

	if (oldChild.parentNode() != *this)
		return node();

	_pimpl->markDirty();

    // Perhaps oldChild was just created with "createElement" or that. In this case
    // its parent is QDomDocument but it is not part of the documents child list.
    if (!oldChild._pimpl->_next
    		&& !oldChild._pimpl->_prev
    		&& _pimpl->_first != oldChild._pimpl)
        return node();

    if (oldChild._pimpl->_next)
        oldChild._pimpl->_next->_prev = oldChild._pimpl->_prev;
    if (oldChild._pimpl->_prev)
        oldChild._pimpl->_prev->_next = oldChild._pimpl->_next;

    if (_pimpl->_last == oldChild._pimpl)
        _pimpl->_last = oldChild._pimpl->_prev;
    if (_pimpl->_first == oldChild._pimpl)
        _pimpl->_first = oldChild._pimpl->_next;

    oldChild._pimpl->clearParent();
    node_impl::deref(oldChild._pimpl->_next);
    node_impl::deref(oldChild._pimpl->_prev);

    return oldChild;
}

node node::appendChild (const node & newChild)
{
    if (!_pimpl)
        return node();

    return insertAfter(newChild, node());
}

node node::cloneNode (bool deep)
{
    if (!_pimpl)
        return node();

    return node(_pimpl->cloneNode(deep));
}
#endif

}} // cwt::dom
