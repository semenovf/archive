/**
 * @file doctype.cpp
 * @author wladt
 * @date Dec 10, 2013
 */

#include "node_p.hpp"
#include "doctype_p.hpp"
#include "document_p.hpp"
#include "namednodemap_p.hpp"

namespace cwt { namespace dom {

document_type_impl::document_type_impl (document_impl * doc, node_impl * parent)
    : node_impl(doc, parent)
{
    init();
}

document_type_impl::document_type_impl (document_type_impl * n, bool deep)
    : node_impl(n, deep)
{
    init();

    node_impl * p = first;

    while (p) {
        if (p->isEntity())
        	// FIXME
            // Don't use normal insert function since we would create infinite recursion
            _entities->_map.insert(p->nodeName(), p); // _map.inserMulti(...)
        if (p->isNotation())
        	// FIXME
            // Don't use normal insert function since we would create infinite recursion
            _notations->_map.insert(p->nodeName(), p); // was _map.inserMulti(...)
        p = p->next;
    }
}


document_type_impl::~document_type_impl()
{
    if (!_entities->_ref.deref())
        delete _entities;

    if (!_notations->_ref.deref())
        delete _notations;
}

void document_type_impl::init ()
{
    _entities = new namednodemap_impl(this);
//    QT_TRY {
	_notations = new namednodemap_impl(this);
	_publicId.clear();
	_systemId.clear();
	_internalSubset.clear();

	_entities->setAppendToParent(true);
	_notations->setAppendToParent(true);
//    } QT_CATCH(...) {
//        delete entities;
//        QT_RETHROW;
//    }
}

node_impl * document_type_impl::cloneNode (bool deep)
{
    node_impl * p = new document_type_impl(this, deep);
    p->ref.deref();
    return p;
}


document_type::document_type (document_type_impl * nimpl)
		: node(nimpl)
{}

pfs::string document_type::name () const
{
	return _pimpl
			? _pimpl->nodeName()
			: pfs::string();
}

namednodemap document_type::entities () const
{
	return _pimpl
			? namednodemap(static_cast<document_type_impl *>(_pimpl)->_entities)
			: namednodemap();
}


namednodemap document_type::notations () const
{
	return _pimpl
			? namednodemap(dynamic_cast<document_type_impl *>(_pimpl)->_notations)
			: namednodemap();
}

node_impl * document_type_impl::insertBefore (node_impl * newChild, node_impl * refChild)
{
    // Call the origianl implementation
    node_impl * p = node_impl::insertBefore(newChild, refChild);

    // Update the maps
    if (p && p->isEntity())
        _entities->_map.insert(p->nodeName(), p); // FIXME was map.insertMulti
    else if (p && p->isNotation())
        _notations->_map.insert(p->nodeName(), p); // FIXME was map.insertMulti

    return p;
}

node_impl * document_type_impl::insertAfter (node_impl * newChild, node_impl * refChild)
{
    // Call the origianl implementation
    node_impl * p = node_impl::insertAfter(newChild, refChild);

    // Update the maps
    if (p && p->isEntity())
        _entities->_map.insert(p->nodeName(), p); // FIXME was map.insertMulti
    else if (p && p->isNotation())
        _notations->_map.insert(p->nodeName(), p); // FIXME was map.insertMulti

    return p;
}

node_impl * document_type_impl::replaceChild (node_impl * newChild, node_impl * oldChild)
{
    // Call the origianl implementation
    node_impl* p = node_impl::replaceChild(newChild, oldChild);

    // Update the maps
    if (p) {
        if (oldChild && oldChild->isEntity())
            _entities->_map.remove(oldChild->nodeName());
        else if (oldChild && oldChild->isNotation())
            _notations->_map.remove(oldChild->nodeName());

        if (p->isEntity())
            _entities->_map.insert(p->nodeName(), p); // FIXME was map.insertMulti
        else if (p->isNotation())
            _notations->_map.insert(p->nodeName(), p); // FIXME was map.insertMulti
    }

    return p;
}

node_impl * document_type_impl::removeChild (node_impl * oldChild)
{
    // Call the origianl implementation
    node_impl* p = node_impl::removeChild( oldChild);

    // Update the maps
    if (p && p->isEntity())
        _entities->_map.remove(p->nodeName());
    else if (p && p->isNotation())
        _notations->_map.remove(p ->nodeName());

    return p;
}

node_impl * document_type_impl::appendChild (node_impl * newChild)
{
    return insertAfter(newChild, 0);
}


pfs::string document_type::publicId () const
{
	return _pimpl
			? dynamic_cast<document_type_impl *>(_pimpl)->_publicId
			: pfs::string();
}

pfs::string document_type::systemId () const
{
	return _pimpl
			? dynamic_cast<document_type_impl *>(_pimpl)->_systemId
			: pfs::string();
}


/*!
    Returns the internal subset of the document type or an empty
    string if there is no internal subset.

    \sa publicId(), systemId()
*/
pfs::string document_type::internalSubset () const
{
	return _pimpl
			? dynamic_cast<document_type_impl *>(_pimpl)->_internalSubset
			: pfs::string();
}


}} // cwt::dom
