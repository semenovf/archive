/**
 * @file nodelist.cpp
 * @author wladt
 * @date Dec 10, 2013
 */

#include "nodelist_p.hpp"
#include "node_p.hpp"
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


node_impl * nodelist_impl::item (size_t index)
{
    if (!_node_impl)
        return nullptr;

    const document_impl * const doc = _node_impl->ownerDocument();
    if (!doc || _timestamp != doc->nodeListTime)
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

    if (!doc || _timestamp != doc->nodeListTime) {
        nodelist_impl *that = const_cast<nodelist_impl *>(this);
        that->createList();
    }

    return _list.size();
}

void nodelist_impl::createList()
{
    if (!_node_impl)
        return;

    const document_impl *const doc = _node_impl->ownerDocument();
    if (doc && _timestamp != doc->nodeListTime)
        _timestamp = doc->nodeListTime;

    node_impl * p = _node_impl->first;

    _list.clear();
    if (_tagname.isNull()) {
        while (p) {
            _list.append(p);
            p = p->next;
        }
    } else if (_nsURI.isNull()) {
        while (p && p != _node_impl) {
            if (p->isElement() && p->nodeName() == _tagname) {
                _list.append(p);
            }
            if (p->first)
                p = p->first;
            else if (p->next)
                p = p->next;
            else {
                p = p->parent();
                while (p && p != _node_impl && !p->next)
                    p = p->parent();
                if (p && p != _node_impl)
                    p = p->next;
            }
        }
    } else {
        while (p && p != _node_impl) {
            if (p->isElement() && p->name == _tagname && p->namespaceURI == _nsURI) {
                _list.append(p);
            }
            if (p->first)
                p = p->first;
            else if (p->next)
                p = p->next;
            else {
                p = p->parent();
                while (p && p != _node_impl && !p->next)
                    p = p->parent();
                if (p && p != _node_impl)
                    p = p->next;
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


#ifdef __COMMENT__

void nodelist::impl::populate ()
{
    if (!_node_impl_ptr)
        return;

    const document::impl * const doc = _node_impl->ownerDocument();
    if (doc && _timestamp != doc->_timestamp)
        _timestamp = doc->_timestamp;

    item_type p = _node_impl->_first;

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

//size_t nodelist::impl::size ()
//{
//    const document::impl * const doc = _pimpl->_node_impl->ownerDocument();
//    if (!doc || _pimpl->_timestamp != doc->_timestamp)
//    	_pimpl->populate();
//
//    return _pimpl->_list.size();
//}
//
//node nodelist::item (size_t index) const
//{
//	if (!_pimpl)
//		return node();
//
//    return index < _pimpl->size() // populated on demand
//    		? node(_pimpl->_list.at(index))
//    		: node();
//}
//
//size_t nodelist::length () const
//{
//	return _pimpl
//			? _pimpl->size() // populated on demand
//			: 0;
//}
#endif

}} // cwt::dom


