/**
 * @file namednodemap.cpp
 * @author wladt
 * @date Dec 10, 2013
 */

#include "node_p.hpp"
#include "namednodemap_p.hpp"

namespace pfs { namespace dom {

void namednodemap_impl::clearMap()
{
    // Dereference all of our children if we took references
    if (!_appendToParent) {
        map_type::const_iterator it = _map.cbegin();

        for (; it != _map.cend(); ++it)
            if (!it->second->ref.deref())
                delete it->second;
    }
    _map.clear();
}

node_impl * namednodemap_impl::namedItem (const pfs::string & name) const
{
	node_impl * p = _map.value(name, nullptr);
	return p;
}

node_impl * namednodemap_impl::namedItemNS(const pfs::string & nsURI, const pfs::string & localName) const
{
    map_type::const_iterator it = _map.cbegin();
    node_impl * n;

    for (; it != _map.cend(); ++it) {
        n = it->second;

        if (!n->_prefix.isNull()) {
            // node has a namespace
            if (n->_namespaceURI == nsURI && n->_name == localName)
                return n;
        }
    }
    return 0;
}

node_impl * namednodemap_impl::setNamedItem(node_impl * arg)
{
    if (_readonly || !arg)
        return nullptr;

    if (_appendToParent)
        return _parent->appendChild(arg);

    node_impl *n = _map.value(arg->nodeName());
    arg->ref.ref();
    _map.insert(arg->nodeName(), arg);
    return n;
}

node_impl * namednodemap_impl::setNamedItemNS (node_impl * arg)
{
    if (_readonly || !arg)
        return nullptr;

    if (_appendToParent)
        return _parent->appendChild(arg);

    if (!arg->_prefix.isNull()) {
        // node has a namespace
        node_impl *n = namedItemNS(arg->_namespaceURI, arg->_name);
        // We take a reference
        arg->ref.ref();
        _map.insert(arg->nodeName(), arg);
        return n;
    } else {
        return setNamedItem(arg);
    }
}

node_impl * namednodemap_impl::item (size_t index) const
{
	if (index >= _map.size())
		return nullptr;

	namednodemap_impl::const_iterator it = _map.cbegin();
	while (index--) {
		++it;
	}
	return it->second;
}

node_impl * namednodemap_impl::removeNamedItem (const pfs::string & name)
{
    if (_readonly)
        return nullptr;

    node_impl * p = namedItem(name);

    if (p == nullptr)
        return nullptr;

    if (_appendToParent)
        return _parent->removeChild(p);

    _map.remove(p->nodeName());

    p->ref.deref();
    return p;
}

namednodemap_impl * namednodemap_impl::clone (node_impl * p)
{
    namednodemap_impl * m = new namednodemap_impl(p);
    m->_readonly = _readonly;
    m->_appendToParent = _appendToParent;

    pfs::map<pfs::string, node_impl*>::const_iterator it = _map.cbegin();

    for (; it != _map.cend(); ++it) {
        node_impl * new_node = it->second->cloneNode();
        new_node->setParent(p);
        m->setNamedItem(new_node);
    }

    m->_ref.deref();
    return m;
}

namednodemap::namednodemap(namednodemap_impl * impl)
{
    _pimpl = impl;
    if (_pimpl)
        _pimpl->_ref.ref();
}

namednodemap::namednodemap (const namednodemap & other)
{
    _pimpl = other._pimpl;
    if (_pimpl)
        _pimpl->_ref.ref();
}

namednodemap & namednodemap::operator = (const namednodemap & other)
{
    if (other._pimpl)
        other._pimpl->_ref.ref();

    if (_pimpl && !_pimpl->_ref.deref())
        delete _pimpl;

    _pimpl = other._pimpl;
    return *this;
}

/*!
    Destroys the object and frees its resources.
*/
namednodemap::~namednodemap()
{
    if (_pimpl && !_pimpl->_ref.deref())
        delete _pimpl;
}

node namednodemap::getNamedItem (const pfs::string & name) const
{
	return _pimpl
			? node(_pimpl->namedItem(name))
	        : node();
}

node namednodemap::getNamedItemNS (const pfs::string & namespaceURI, const pfs::string & localName) const
{
	return _pimpl
			? node(_pimpl->namedItemNS(namespaceURI, localName))
			: node();
}

/**
 * @details Removes a node specified by name.
 * 		When this map contains the attributes attached to an element,
 * 		if the removed attribute is known to have a default value,
 * 		an attribute immediately appears containing the default
 * 		value as well as the corresponding namespace URI,
 * 		local name, and prefix when applicable.
 *
 * @param name The node name of the node to remove.
 * @return The node removed from this map if a node with such a name exists.
 */
node namednodemap::removeNamedItem (const pfs::string & name)
{
	return _pimpl
			? node(_pimpl->removeNamedItem(name))
			: node();
}

node namednodemap::removeNamedItemNS (const pfs::string& nsURI, const pfs::string& localName)
{
    if (!_pimpl)
        return node();

    node_impl *n = _pimpl->namedItemNS(nsURI, localName);

    if (!n)
        return node();
    return node(_pimpl->removeNamedItem(n->_name));
}


node namednodemap::item (size_t index) const
{
	return _pimpl
			? node(_pimpl->item(index))
	        : node();
}


size_t namednodemap::length () const
{
	return _pimpl
			? _pimpl->length()
			: 0;
}

/**
 * @details Adds a node using its @c nodeName attribute.
 * 		If a node with that name is already present in this map,
 * 		it is replaced by the new one. As the nodeName attribute
 * 		is used to derive the name which the node must be stored under,
 * 		multiple nodes of certain types (those that have a "special" string value)
 * 		cannot be stored as the names would clash.
 * 		This is seen as preferable to allowing nodes to be aliased.
 *
 * @param arg A node to store in this map.
 * 		The node will later be accessible using the value of its @c nodeName attribute.
 * @return If the new Node replaces an existing node the replaced Node is returned,
 * 		otherwise null is returned.
 */
node namednodemap::setNamedItem (const node & newNode)
{
	return _pimpl
			? node(_pimpl->setNamedItem(newNode._pimpl))
			: node();
}

node namednodemap::setNamedItemNS (const node & newNode)
{
	return _pimpl
			? node(_pimpl->setNamedItemNS(newNode._pimpl))
			: node();
}

}} // pfs::dom

