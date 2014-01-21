/**
 * @file doctype.cpp
 * @author wladt
 * @date Dec 10, 2013
 */


#include "node_p.hpp"
#include "nodelist_p.hpp"
#include "namednodemap_p.hpp"
#include "attr_p.hpp"
#include "element_p.hpp"
#include "chardata_p.hpp"
#include "text_p.hpp"
#include "comment_p.hpp"
#include "doctype_p.hpp"
#include "document_p.hpp"
#include "namednodemap_p.hpp"

namespace cwt { namespace dom {

doctype_impl::doctype_impl (document_impl * doc, node_impl * parent)
    : node_impl(doc, parent)
{
    init();
}

doctype_impl::doctype_impl (doctype_impl * n, bool deep)
    : node_impl(n, deep)
{
    init();

    node_impl * p = _first;

    while (p) {
        if (p->isEntity())
        	// FIXME
            // Don't use normal insert function since we would create infinite recursion
            _entities->_map.insert(p->nodeName(), p); // _map.inserMulti(...)
        if (p->isNotation())
        	// FIXME
            // Don't use normal insert function since we would create infinite recursion
            _notations->_map.insert(p->nodeName(), p); // was _map.inserMulti(...)
        p = p->_next;
    }
}


doctype_impl::~doctype_impl ()
{
	PFS_TRACE("~doctype_impl()");

    if (!_entities->_ref.deref())
        delete _entities;

    if (!_notations->_ref.deref())
        delete _notations;
}

void doctype_impl::init ()
{
    _entities = new namednodemap_impl(this);
//    try {
	_notations = new namednodemap_impl(this);
	_publicId.clear();
	_systemId.clear();
	_internalSubset.clear();

	_entities->setAppendToParent(true);
	_notations->setAppendToParent(true);
//    } catch(...) {
//        delete entities;
//        throw ...;
//    }
}

node_impl * doctype_impl::cloneNode (bool deep)
{
    node_impl * p = new doctype_impl(this, deep);
    p->ref.deref();
    return p;
}


doctype::doctype (doctype_impl * nimpl)
		: node(nimpl)
{}

pfs::string doctype::name () const
{
	return _pimpl
			? _pimpl->nodeName()
			: pfs::string();
}

namednodemap doctype::entities () const
{
	return _pimpl
			? namednodemap(reinterpret_cast<doctype_impl *>(_pimpl)->_entities)
			: namednodemap();
}


namednodemap doctype::notations () const
{
	return _pimpl
			? namednodemap(reinterpret_cast<doctype_impl *>(_pimpl)->_notations)
			: namednodemap();
}

node_impl * doctype_impl::insertBefore (node_impl * newChild, node_impl * refChild)
{
    // Call the original implementation
    node_impl * p = node_impl::insertBefore(newChild, refChild);

    // Update the maps
    if (p && p->isEntity())
        _entities->_map.insert(p->nodeName(), p); // FIXME was map.insertMulti
    else if (p && p->isNotation())
        _notations->_map.insert(p->nodeName(), p); // FIXME was map.insertMulti

    return p;
}

node_impl * doctype_impl::insertAfter (node_impl * newChild, node_impl * refChild)
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

node_impl * doctype_impl::replaceChild (node_impl * newChild, node_impl * oldChild)
{
    // Call the origianl implementation
    node_impl * p = node_impl::replaceChild(newChild, oldChild);

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

node_impl * doctype_impl::removeChild (node_impl * oldChild)
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

node_impl * doctype_impl::appendChild (node_impl * newChild)
{
    return insertAfter(newChild, 0);
}


pfs::string doctype::publicId () const
{
	return _pimpl
			? dynamic_cast<doctype_impl *>(_pimpl)->_publicId
			: pfs::string();
}

pfs::string doctype::systemId () const
{
	return _pimpl
			? dynamic_cast<doctype_impl *>(_pimpl)->_systemId
			: pfs::string();
}


/*!
    Returns the internal subset of the document type or an empty
    string if there is no internal subset.

    \sa publicId(), systemId()
*/
pfs::string doctype::internalSubset () const
{
	return _pimpl
			? dynamic_cast<doctype_impl *>(_pimpl)->_internalSubset
			: pfs::string();
}


}} // cwt::dom
