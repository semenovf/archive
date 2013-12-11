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

	namednodemap::impl::const_iterator it = _pimpl->find(name);
	return it != _pimpl->cend()
			? it->second
			: node();
}

node namednodemap::setNamedItem (node arg) // raises(DOMException)
{
	if (!_pimpl)
		return node();

	if (arg.isNull())
		return node();

	namednodemap::impl::iterator it = _pimpl->find(arg.nodeName());
	if (it != _pimpl->end()) {
		node n = it->second;
		it->second = arg;
		return n;
	}

	_pimpl->insert(arg.nodeName(), arg);
	return node();
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
node namednodemap::removeNamedItem (const pfs::string & name) // raises(DOMException)
{
	if (!_pimpl)
		return node();

	namednodemap::impl::iterator it = _pimpl->find(name);

	if (it != _pimpl->cend()) {
		node n = it->second;
		_pimpl->remove(it);
	}

	return node();
}

node namednodemap::item (size_t index) const
{
	if (!_pimpl)
		return node();

/*
	return index < _pimpl->size()
			? _pimpl->cbegin() + index // TODO may be does not work
			: node();
*/

	if (index < _pimpl->size()) {
		namednodemap::impl::const_iterator it = _pimpl->cbegin();

		while(index--) {
			--it;
		}
		return it->second;
	}

	return node;
}

size_t namednodemap::length () const
{
	return _pimpl
			? _pimpl->size()
			: 0;
}

}} // cwt::dom

