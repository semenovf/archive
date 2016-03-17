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

namespace pfs { namespace dom {

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
        if (p->is_entity())
        	// FIXME
            // Don't use normal insert function since we would create infinite recursion
            _entities->_map.insert(p->node_name(), p); // _map.insertMulti(...)
        if (p->is_notation())
        	// FIXME
            // Don't use normal insert function since we would create infinite recursion
            _notations->_map.insert(p->node_name(), p); // was _map.insertMulti(...)
        p = p->_next;
    }
}


doctype_impl::~doctype_impl ()
{
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
	_public_id.clear();
	_system_id.clear();
	_internal_subset.clear();

	_entities->set_append_to_parent(true);
	_notations->set_append_to_parent(true);
//    } catch(...) {
//        delete entities;
//        throw ...;
//    }
}

node_impl * doctype_impl::clone_node (bool deep)
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
	return _d
			? _d->node_name()
			: pfs::string();
}

namednodemap doctype::entities () const
{
	return _d
			? namednodemap(reinterpret_cast<doctype_impl *>(_d)->_entities)
			: namednodemap();
}


namednodemap doctype::notations () const
{
	return _d
			? namednodemap(reinterpret_cast<doctype_impl *>(_d)->_notations)
			: namednodemap();
}

node_impl * doctype_impl::insert_before (node_impl * newChild, node_impl * refChild)
{
    // Call the original implementation
    node_impl * p = node_impl::insert_before(newChild, refChild);

    // Update the maps
    if (p && p->is_entity())
        _entities->_map.insert(p->node_name(), p); // FIXME was map.insertMulti
    else if (p && p->is_notation())
        _notations->_map.insert(p->node_name(), p); // FIXME was map.insertMulti

    return p;
}

node_impl * doctype_impl::insert_after (node_impl * newChild, node_impl * refChild)
{
    // Call the origianl implementation
    node_impl * p = node_impl::insert_after(newChild, refChild);

    // Update the maps
    if (p && p->is_entity())
        _entities->_map.insert(p->node_name(), p); // FIXME was map.insertMulti
    else if (p && p->is_notation())
        _notations->_map.insert(p->node_name(), p); // FIXME was map.insertMulti

    return p;
}

node_impl * doctype_impl::replace_child (node_impl * newChild, node_impl * oldChild)
{
    // Call the origianl implementation
    node_impl * p = node_impl::replace_child(newChild, oldChild);

    // Update the maps
    if (p) {
        if (oldChild && oldChild->is_entity())
            _entities->_map.remove(oldChild->node_name());
        else if (oldChild && oldChild->is_notation())
            _notations->_map.remove(oldChild->node_name());

        if (p->is_entity())
            _entities->_map.insert(p->node_name(), p); // FIXME was map.insertMulti
        else if (p->is_notation())
            _notations->_map.insert(p->node_name(), p); // FIXME was map.insertMulti
    }

    return p;
}

node_impl * doctype_impl::remove_child (node_impl * oldChild)
{
    // Call the origianl implementation
    node_impl* p = node_impl::remove_child( oldChild);

    // Update the maps
    if (p && p->is_entity())
        _entities->_map.remove(p->node_name());
    else if (p && p->is_notation())
        _notations->_map.remove(p ->node_name());

    return p;
}

node_impl * doctype_impl::append_child (node_impl * newChild)
{
    return insert_after(newChild, 0);
}


pfs::string doctype::public_id () const
{
	return _d
			? dynamic_cast<doctype_impl *>(_d)->_public_id
			: pfs::string();
}

pfs::string doctype::system_id () const
{
	return _d
			? dynamic_cast<doctype_impl *>(_d)->_system_id
			: pfs::string();
}


/*!
    Returns the internal subset of the document type or an empty
    string if there is no internal subset.

    \sa publicId(), systemId()
*/
pfs::string doctype::internal_subset () const
{
	return _d
			? dynamic_cast<doctype_impl *>(_d)->_internal_subset
			: pfs::string();
}


}} // pfs::dom
