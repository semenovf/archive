/**
 * @file nodelist.cpp
 * @author wladt
 * @date Dec 10, 2013
 */

#include "nodelist_p.hpp"
#include "node_p.hpp"
#include "document_p.hpp"

namespace cwt { namespace dom {

void nodelist::impl::populate ()
{
    if (!_node_impl.get())
        return;

    const document::impl * const doc = _node_impl->ownerDocument();
    if (doc && _timestamp != doc->_timestamp)
        _timestamp = doc->_timestamp;

    element_type p = _node_impl->_first;

    _list.clear();

    if (_tagname.isEmpty()) {
        while (p) {
            _list.append(p);
            p = p->_next;
        }
    } else if (_namespaceURI.isEmpty()) {
        while (p && p != _node_impl) {
            if (p->isElement() && p->_name() == _tagname) {
                _list.append(p);
            }

            if (p->_first)
                p = p->_first;
            else if (p->_next)
                p = p->_next;
            else {
                p = p->parent();

                while (p && p != _node_impl && !p->_next)
                    p = p->parent();
                if (p && p != _node_impl)
                    p = p->_next;
            }
        }
    } else {
        while (p && p != _node_impl) {
            if (p->isElement() && p->_name == _tagname && p->_namespaceURI == _namespaceURI) {
                _list.append(p);
            }

            if (p->_first)
                p = p->_first;
            else if (p->_next)
                p = p->_next;
            else {
                p = p->parent();
                while (p && p != _node_impl && !p->_next)
                    p = p->parent();
                if (p && p != _node_impl)
                    p = p->_next;
            }
        }
    }
}

size_t nodelist::impl::size ()
{
    const document::impl * const doc = _pimpl->_node_impl->ownerDocument();
    if (!doc || _pimpl->_timestamp != doc->_timestamp)
    	_pimpl->populate();

    return _pimpl->_list.size();
}

node nodelist::item (size_t index) const
{
	if (!_pimpl)
		return node();

    return index < _pimpl->size() // populated on demand
    		? node(_pimpl->_list.at(index))
    		: node();
}

size_t nodelist::length () const
{
	return _pimpl
			? _pimpl->size() // populated on demand
			: 0;
}

}} // cwt::dom


