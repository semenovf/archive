/**
 * @file nodelist.cpp
 * @author wladt
 * @date Dec 10, 2013
 */

#include "node_p.hpp"
#include "nodelist_p.hpp"
#include "namednodemap_p.hpp"
#include "attr_p.hpp"
#include "element_p.hpp"
#include "document_p.hpp"

namespace cwt { namespace dom {

nodelist_impl::nodelist_impl (node_impl * nimpl)
	: _ref(1)
{
    _node_impl = nimpl;

    if (_node_impl)
        _node_impl->ref.ref();
    _timestamp = 0;
}

nodelist_impl::nodelist_impl (node_impl * n_impl, const pfs::string & name)
	: _ref(1)
{
    _node_impl = n_impl;
    if (_node_impl)
        _node_impl->ref.ref();
    _tagname = name;
    _timestamp = 0;
}


nodelist_impl::nodelist_impl (node_impl * n_impl, const pfs::string & nsURI, const pfs::string & localName)
    : _ref(1)
{
    _node_impl = n_impl;
    if (_node_impl)
        _node_impl->ref.ref();
    _tagname   = localName;
    _nsURI     = nsURI;
    _timestamp = 0;
}

nodelist_impl::~nodelist_impl ()
{
    if (_node_impl && !_node_impl->ref.deref())
        delete _node_impl;
}

node_impl * nodelist_impl::item (size_t index)
{
    if (!_node_impl)
        return nullptr;

    const document_impl * const doc = _node_impl->ownerDocument();
    if (!doc || _timestamp != doc->_nodeListTime)
        createList();

    if (index >= _list.size())
        return 0;

    return _list.at(index);
}

size_t nodelist_impl::length ()
{
    if (!_node_impl)
        return 0;

    const document_impl * const doc = _node_impl->ownerDocument();

    if (!doc || _timestamp != doc->_nodeListTime) {
        nodelist_impl *that = const_cast<nodelist_impl *>(this);
        that->createList();
    }

    return _list.size();
}

void nodelist_impl::createList ()
{
    if (!_node_impl)
        return;

    const document_impl *const doc = _node_impl->ownerDocument();
    if (doc && _timestamp != doc->_nodeListTime)
        _timestamp = doc->_nodeListTime;

    node_impl * p = _node_impl->_first;

    _list.clear();

    if (_tagname.isNull()) {
        while (p) {
            _list.append(p);
            p = p->_next;
        }
    } else if (_nsURI.isNull()) {
        while (p && p != _node_impl) {
            if (p->isElement() && p->nodeName() == _tagname) {
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
            if (p->isElement() && p->_name == _tagname && p->_namespaceURI == _nsURI) {
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


nodelist::nodelist (const nodelist & other)
{
    _pimpl = other._pimpl;
    if (_pimpl)
        _pimpl->_ref.ref();
}

nodelist::~nodelist ()
{
    if (_pimpl && !_pimpl->_ref.deref()) {
        delete _pimpl;
        _pimpl = nullptr;
    }
}

nodelist & nodelist::operator = (const nodelist & other)
{
    if (other._pimpl)
        other._pimpl->_ref.ref();
    if (_pimpl && !_pimpl->_ref.deref())
        delete _pimpl;
    _pimpl = other._pimpl;
    return *this;
}

node nodelist::item (size_t index) const
{
	return _pimpl
			? node(_pimpl->item(index))
			: node();
}

/*!
    Returns the number of nodes in the list.
*/
size_t nodelist::length () const
{
	return _pimpl
			? _pimpl->length()
			: 0;
}

}} // cwt::dom


