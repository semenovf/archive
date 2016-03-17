#include "node_p.hpp"
#include "nodelist_p.hpp"
#include "namednodemap_p.hpp"
#include "attr_p.hpp"
#include "element_p.hpp"
#include "document_p.hpp"
#include "domimpl_p.hpp"
#include "chardata_p.hpp"
#include "text_p.hpp"

namespace pfs { namespace dom {

inline void node_impl::set_owner_document (document_impl * doc)
{
    _ownerNode = doc;
    _has_parent = false;
}

node_impl::node_impl(document_impl * d, node_impl * parent) : ref(1)
{
    if (parent)
        setParent (parent);
    else
        set_owner_document (d);

    _prev  = 0;
    _next  = 0;
    _first = 0;
    _last  = 0;
//    _createdWithDom1Interface = true;
}

node_impl::node_impl (node_impl * n, bool deep)
	: ref(1)
{
    set_owner_document(n->owner_document());
    _prev  = 0;
    _next  = 0;
    _first = 0;
    _last  = 0;

    _name = n->_name;
    _value = n->_value;
    _prefix = n->_prefix;
    _namespace_uri = n->_namespace_uri;
//    _createdWithDom1Interface = n->_createdWithDom1Interface;

    if (!deep)
        return;

    for (node_impl * x = n->_first; x; x = x->_next)
        append_child(x->clone_node(true));
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
            p->set_noparent();
        p = n;
    }
    _first = 0;
    _last = 0;
}

document_impl * node_impl::owner_document ()
{
    node_impl * p = this;
    while (p && !p->is_document()) {
        if (!p->_has_parent)
            return (document_impl *)p->_ownerNode;
        p = p->parent();
    }

    return static_cast<document_impl *>(p);
}

node_impl * node_impl::clone_node (bool deep)
{
    node_impl * p = new node_impl(this, deep);
    p->ref.deref();
    return p;
}

node_impl * node_impl::insert_after (node_impl * newChild, node_impl * refChild)
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
    document_impl * const doc = owner_document();
    if(doc)
        ++doc->_nodeListTime;

    // Special handling for inserting a fragment. We just insert
    // all elements of the fragment instead of the fragment itself.
    if (newChild->is_document_fragment()) {
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
        newChild->parent()->remove_child(newChild);

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

node_impl * node_impl::insert_before (node_impl * newChild, node_impl * refChild)
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
    document_impl * const doc = owner_document();
    if (doc)
        ++doc->_nodeListTime;

    // Special handling for inserting a fragment. We just insert
    // all elements of the fragment instead of the fragment itself.
    if (newChild->is_document_fragment()) {
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
        newChild->parent()->remove_child(newChild);

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

node_impl * node_impl::append_child (node_impl * newChild)
{
    return insert_after(newChild, 0);
}

node_impl * node_impl::remove_child (node_impl * oldChild)
{
    // Error check
    if (oldChild->parent() != this)
        return 0;

    // "mark lists as dirty"
    document_impl * const doc = owner_document();

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

    oldChild->set_noparent();
    oldChild->_next = 0;
    oldChild->_prev = 0;

    // We are no longer interested in the old node
    oldChild->ref.deref();

    return oldChild;
}

node_impl * node_impl::replace_child (node_impl * newChild, node_impl * oldChild)
{
    if (!newChild || !oldChild)
        return 0;

    if (oldChild->parent() != this)
        return 0;

    if (newChild == oldChild)
        return 0;

    // mark lists as dirty
    document_impl * const doc = owner_document();
    if(doc)
        ++doc->_nodeListTime;

    // Special handling for inserting a fragment. We just insert
    // all elements of the fragment instead of the fragment itself.
    if (newChild->is_document_fragment()) {
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

        oldChild->set_noparent();
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
        newChild->parent()->remove_child(newChild);

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

    oldChild->set_noparent();
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
    text_impl * t = 0;

    while (p) {
        if (p->is_text()) {
            if (t) {
                node_impl * tmp = p->_next;
                t->append_data(p->node_value());
                this->remove_child(p);
                p = tmp;
            } else {
                t = (text_impl*)p;
                p = p->_next;
            }
        } else {
            p = p->_next;
            t = 0;
        }
    }
}


node::node (const node & n)
{
    _d = n._d;
    if (_d)
        _d->ref.ref();
}

node::node (node_impl * n)
{
    _d = n;
    if (_d)
        _d->ref.ref();
}


node & node::operator = (const node & other)
{
	if (other._d)
		other._d->ref.ref();

	if (_d && !_d->ref.deref())
		delete _d;

	_d = other._d;
	return *this;
}

node::~node ()
{
    if (_d && !_d->ref.deref()) {
    	delete _d;
        _d = 0;
    }
}

node node::clone_node (bool deep) const
{
    if (!_d)
        return node();
    return node(_d->clone_node(deep));
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
pfs::string node::node_name () const
{
    if (!_d)
    	return pfs::string();

    if (!_d->_prefix.isEmpty())
    	return _d->_prefix + pfs::ucchar(':') + _d->_name;

    return _d->_name;
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
pfs::string node::node_value () const
{
    return _d
    		? _d->_value
    		: pfs::string();
}

node::type node::node_type () const
{
    return _d
    		? _d->node_type()
    		: node::invalid_node;
}

document node::owner_document () const
{
    return _d
    		? document(_d->owner_document())
    		: document();
}

/*!
    Returns the first child of the node. If there is no child node, a
    \l{isNull()}{null node} is returned. Changing the
    returned node will also change the node in the document tree.
V
    \sa lastChild(), childNodes()
*/
node node::first_child () const
{
    return _d
    		? node(_d->_first)
    		: node();
}

node node::last_child () const
{
    return _d
    		? node(_d->_last)
    		: node();
}


node node::previous_sibling() const
{
    return _d
    	? node(_d->_prev)
        : node();
}


node node::next_sibling() const
{
    return _d
    	? node(_d->_next)
        : node();
}

pfs::string node::namespace_uri() const
{
    return _d
    	? _d->_namespace_uri
        : pfs::string();
}

pfs::string node::prefix() const
{
    return _d
        ? _d->_prefix
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
pfs::string node::localname () const
{
/*
    if (!_pimpl || _pimpl->_createdWithDom1Interface)
        return pfs::string();
    return _pimpl->_name;
*/
    return _d
    		? _d->_name
    		: pfs::string();
}


bool node::is_supported (const pfs::string & feature, const pfs::string & version) const
{
    dom_implementation i;
    return i.has_feature(feature, version);
}


node node::insert_after (const node & newChild, const node & refChild)
{
    if (!_d)
        return node();
    return node(_d->insert_after(newChild._d, refChild._d));
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
 * @throw @a pfs::dom::exception
 */
node node::insert_before (const node & newChild, const node & refChild)
{
    if (!_d)
        return node();
    return node(_d->insert_before(newChild._d, refChild._d));
}

node node::append_child (const node & newChild)
{
    if (!_d) {
        return node();
    }
    return node(_d->append_child(newChild._d));
}

/**
 * @details Removes the child node indicated by oldChild
 * 		from the list of children, and returns it.
 *
 * @param oldChild The node being removed.
 * @return The node removed.
 * @throw @a pfs::dom::exception
 */
node node::remove_child (const node& oldChild)
{
    if (!_d)
        return node();

    if (oldChild.is_null())
        return node();

    return node(_d->remove_child(oldChild._d));
}

node node::replace_child (const node & newChild, const node & oldChild)
{
    if (!_d || !newChild._d || !oldChild._d)
        return node();
    return node(_d->replace_child(newChild._d, oldChild._d));
}

node node::parent_node () const
{
    return _d
    		? node(_d->parent())
    		: node();
}

void node::normalize()
{
    if (_d)
        _d->normalize();
}

/*!
    Sets the node's value to \a v.

    \sa nodeValue()
*/
void node::set_node_value (const pfs::string & v)
{
    if (_d)
    	_d->set_node_value(v);
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
void node::set_prefix (const pfs::string & pre)
{
    if (!_d || _d->_prefix.is_null())
        return;
    if (_d->is_attr() || _d->is_element())
        _d->_prefix = pre;
}

nodelist node::child_nodes() const
{
	return _d
			? nodelist(new nodelist_impl(_d))
			: nodelist();
}


bool node::has_attributes() const
{
    if (!_d || !_d->is_element())
        return false;
    return static_cast<element_impl *>(_d)->has_attributes();
}


bool node::has_child_nodes () const
{
	return _d
			? _d->_first != 0
			: false;
}

namednodemap node::attributes() const
{
    if (!_d || !_d->is_element())
        return namednodemap();

    return namednodemap(static_cast<element_impl *>(_d)->attributes());
}


attr node::to_attr () const
{
	return (_d && _d->is_attr())
		? attr(static_cast<attr_impl *>(_d))
		: attr();
}

element node::to_element () const
{
	return (_d && _d->is_element())
		? element(static_cast<element_impl *>(_d))
		: element();
}

void node::traverse (void (* onStart) (const pfs::dom::node & n, void * d)
		, void (* onEnd) (const pfs::dom::node & n, void * d)
		, void * userData) const
{
	pfs::dom::nodelist children = child_nodes();

	// No children
	if (!children.size())
		return;

	for (size_t i = 0; i < children.size(); ++i) {
		if (onStart)
			onStart(children.item(i), userData);

		children.item(i).traverse(onStart, onEnd, userData);

		if (onEnd)
			onEnd(children.item(i), userData);
	}
}

}} // pfs::dom
