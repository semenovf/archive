/**
 * @file namednodemap.cpp
 * @author wladt
 * @date Dec 10, 2013
 */

#include "namednodemap_p.hpp"

namespace cwt { namespace dom {

node namednodemap::getNamedItem (const pfs::string & name) const
{
	if (!_pimpl)
		return node();

	namednodemap::impl::const_iterator it = _pimpl->_map.find(name);
	return it != _pimpl->_map.cend()
			? node(it->second)
			: node();
}

node namednodemap::getNamedItemNS(const pfs::string & namespaceURI, const pfs::string & localName) const
{
	if (!_pimpl)
		return node();

	namednodemap::impl::const_iterator it = _pimpl->_map.cbegin();

    for (; it != _pimpl->_map.cend(); ++it) {
    	namednodemap::impl::item_type n = it->second;
        if (!n->_prefix.isEmpty()) { // node has a namespace
            if (n->_namespaceURI == namespaceURI && n->_name == localName)
                return node(n);
        }
    }

    return node();
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
node namednodemap::setNamedItem (const node & arg)
{
	if (!_pimpl)
		return node();

    if (arg.isNull())
        return node();

    namednodemap::impl::item_type n = _pimpl->_map.value(arg.nodeName()
    		, namednodemap::impl::item_type());

    _pimpl->_map.insert(arg.nodeName(), arg._pimpl);
    return node(n);
}

node namednodemap::setNamedItemNS(const node & arg)
{
	if (!_pimpl)
		return node();

    if (arg.isNull())
        return node();

    if (!arg._pimpl->_prefix.isEmpty()) { // node has a namespace
    	node n = getNamedItemNS(arg._pimpl->_namespaceURI, arg._pimpl->_name);
        _pimpl->_map.insert(arg.nodeName(), arg._pimpl);
        return n;
    }

    return setNamedItem(arg);
}


node namednodemap::item (size_t index) const
{
	if (!_pimpl)
		return node();

	if (index >= _pimpl->_map.size())
		return node();

	namednodemap::impl::const_iterator it = _pimpl->_map.cbegin();
	while(index--) {
		--it;
	}
	return node(it->second);
}

size_t namednodemap::length () const
{
	return _pimpl
			? _pimpl->_map.size()
			: 0;
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
	if (!_pimpl)
		return node();

	namednodemap::impl::iterator it = _pimpl->_map.find(name);
	if (it != _pimpl->_map.cend()) {
		node n(it->second);
		_pimpl->_map.remove(it);
		return n;
	}

	return node();
}

node namednodemap::removeNamedItemNS (const pfs::string & namespaceURI, const pfs::string & localName)
{
	if (!_pimpl)
		return node();

	node n = namedItemNS(namespaceURI, localName);
	if (n.isNull())
		return node();

	return removeNamedItem(n.nodeName());
}

}} // cwt::dom

