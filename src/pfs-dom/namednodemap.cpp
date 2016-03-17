/**
 * @file namednodemap.cpp
 * @author wladt
 * @date Dec 10, 2013
 */

#include "node_p.hpp"
#include "namednodemap_p.hpp"

namespace pfs { namespace dom {

void namednodemap_impl::clear_map()
{
    // Dereference all of our children if we took references
	//
    if (!_append_to_parent) {
        map_type::const_iterator it = _map.cbegin();
        map_type::const_iterator itEnd = _map.cend();

        for (; it != itEnd; ++it) {
        	node_impl * n = it.value();

            if (!n->ref.deref())
                delete n;
        }
    }
    _map.clear();
}

node_impl * namednodemap_impl::named_item (const pfs::string & name) const
{
	node_impl * p = _map.valueAt(name, 0);
	return p;
}

node_impl * namednodemap_impl::named_item_ns(const pfs::string & nsURI, const pfs::string & localName) const
{
    map_type::const_iterator it = _map.cbegin();
    node_impl * n;

    for (; it != _map.cend(); ++it) {
        n = it.value();

        if (!n->_prefix.isNull()) {
            // node has a namespace
            if (n->_namespace_uri == nsURI && n->_name == localName)
                return n;
        }
    }
    return 0;
}

node_impl * namednodemap_impl::set_named_item(node_impl * arg)
{
    if (_readonly || !arg)
        return 0;

    if (_append_to_parent)
        return _parent->append_child(arg);

    node_impl * n = _map.valueAt(arg->node_name());
    arg->ref.ref();
    _map.insert(arg->node_name(), arg);
    return n;
}

node_impl * namednodemap_impl::set_named_item_ns (node_impl * arg)
{
    if (_readonly || !arg)
        return 0;

    if (_append_to_parent)
        return _parent->append_child(arg);

    if (!arg->_prefix.isNull()) {
        // node has a namespace
        node_impl * n = named_item_ns(arg->_namespace_uri, arg->_name);
        // We take a reference
        arg->ref.ref();
        _map.insert(arg->node_name(), arg);
        return n;
    } else {
        return set_named_item(arg);
    }
}

node_impl * namednodemap_impl::item (size_t index) const
{
	if (index >= _map.size())
		return 0;

	namednodemap_impl::const_iterator it = _map.cbegin();
	while (index--) {
		++it;
	}

	return it.value();
}

node_impl * namednodemap_impl::remove_named_item (const pfs::string & name)
{
    if (_readonly)
        return 0;

    node_impl * p = named_item(name);

    if (p == 0)
        return 0;

    if (_append_to_parent)
        return _parent->remove_child(p);

    _map.remove(p->node_name());

    p->ref.deref();
    return p;
}

namednodemap_impl * namednodemap_impl::clone (node_impl * p)
{
    namednodemap_impl * m = new namednodemap_impl(p);
    m->_readonly = _readonly;
    m->_append_to_parent = _append_to_parent;

    pfs::map<pfs::string, node_impl*>::const_iterator it = _map.cbegin();

    for (; it != _map.cend(); ++it) {
        node_impl * new_node = it.value()->cloneNode();
        new_node->setParent(p);
        m->set_named_item(new_node);
    }

    m->_ref.deref();
    return m;
}

namednodemap::namednodemap(namednodemap_impl * impl)
{
    _d = impl;
    if (_d)
        _d->_ref.ref();
}

namednodemap::namednodemap (const namednodemap & other)
{
    _d = other._d;
    if (_d)
        _d->_ref.ref();
}

namednodemap & namednodemap::operator = (const namednodemap & other)
{
    if (other._d)
        other._d->_ref.ref();

    if (_d && !_d->_ref.deref())
        delete _d;

    _d = other._d;
    return *this;
}

/*!
    Destroys the object and frees its resources.
*/
namednodemap::~namednodemap()
{
    if (_d && !_d->_ref.deref())
        delete _d;
}

node namednodemap::get_namedItem (const pfs::string & name) const
{
	return _d
			? node(_d->named_item(name))
	        : node();
}

node namednodemap::get_named_item_ns (const pfs::string & namespaceURI, const pfs::string & localName) const
{
	return _d
			? node(_d->named_item_ns(namespaceURI, localName))
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
node namednodemap::remove_named_item (const pfs::string & name)
{
	return _d
			? node(_d->remove_named_item(name))
			: node();
}

node namednodemap::remove_named_item_ns (const pfs::string& nsURI, const pfs::string& localName)
{
    if (!_d)
        return node();

    node_impl *n = _d->named_item_ns(nsURI, localName);

    if (!n)
        return node();
    return node(_d->remove_named_item(n->_name));
}


node namednodemap::item (size_t index) const
{
	return _d
			? node(_d->item(index))
	        : node();
}


size_t namednodemap::length () const
{
	return _d
			? _d->length()
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
node namednodemap::set_named_item (const node & newNode)
{
	return _d
			? node(_d->set_named_item(newNode._d))
			: node();
}

node namednodemap::set_named_item_ns (const node & newNode)
{
	return _d
			? node(_d->set_named_item_ns(newNode._d))
			: node();
}

}} // pfs::dom

