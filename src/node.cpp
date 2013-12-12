#include "node_p.hpp"
#include "nodelist_p.hpp"
#include "namednodemap_p.hpp"
#include "element_p.hpp"
#include "document_p.hpp"

namespace cwt { namespace dom {

document::impl * node::impl::ownerDocument ()
{
	node::impl * p = this;
	while (p && !p->isDocument()) {
		p = p->parent();
	}

	return static_cast<document::impl *>(p);
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

node node::parentNode () const
{
	return _pimpl
			? node(_pimpl->parent())
			: node();
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

    return dynamic_pointer_cast<element::impl>(_pimpl)->attrCount() > 0;
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
			? nodelist(new nodelist::impl(_pimpl))
			: nodelist();
}


namednodemap node::attributes () const
{
    if (!_pimpl || !_pimpl->isElement())
        return namednodemap();

    return dynamic_pointer_cast<element::impl>(_pimpl)->_attr;
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
	if (_pimpl)
		return node();

	if (!newChild._pimpl)
		return node();

	if (newChild == refChild)
		return node();

	if (refChild._pimpl && refChild.parentNode() != *this)
		return node();

	// "mark lists as dirty"
	document::impl * const doc = _pimpl->ownerDocument();
	if (doc)
		++doc->_timestamp;

	// Special handling for inserting a fragment. We just insert
	// all elements of the fragment instead of the fragment itself.
	if (newChild._pimpl->isDocumentFragment()) {
		// Fragment is empty ?
		if (!newChild._pimpl->_first)
			return newChild;

		// New parent
		node::pimpl_type n = newChild._pimpl->_first;
		while (n)  {
			n->setParent(_pimpl); // FIXME
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
		node::pimpl_type null1;
		node::pimpl_type null2;
		newChild._pimpl->_first.swap(null1);
		newChild._pimpl->_last.swap(null2);
		return newChild;
	}

	if (newChild._pimpl->parent())
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

/**
 * @details Removes the child node indicated by oldChild
 * 		from the list of children, and returns it.
 *
 * @param oldChild The node being removed.
 * @return The node removed.
 * @throw @a cwt::dom::exception
 */
//node node::removeChild (const node & oldChild)
//{
//	if (!_pimpl || oldChild.isNull())
//		return node();
//
//	if (oldChild.parentNode() != *this)
//		return node();
//
//	// Just created and does not added to any container
//	if (oldChild.nextSibling().isNull()
//			&& oldChild.previousSibling().isNull()
//			&& oldChild != this->firstChild())
//		return node();
//
//	if (!oldChild._pimpl->_nextSibling.isNull())
//		oldChild._pimpl->_nextSibling._pimpl->_previousSibling = oldChild._pimpl->_previousSibling;
//
//	if (!oldChild._pimpl->_previousSibling.isNull())
//		oldChild._pimpl->_previousSibling._pimpl->_nextSibling = oldChild._pimpl->_nextSibling;
//}

}} // cwt::dom
