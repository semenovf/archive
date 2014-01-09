/**
 * @file entity.cpp
 * @author wladt
 * @date Jan 9, 2014
 */

#include "node_p.hpp"
#include "entity_p.hpp"
#include "document_p.hpp"

namespace cwt { namespace dom {


entity_impl::entity_impl (document_impl * d
		, node_impl * parent
        , const pfs::string & aname
        , const pfs::string & pub
        , const pfs::string & sys
        , const pfs::string & notation)
    : node_impl(d, parent)
{
    name  = aname;
    _pub = pub;
    _sys = sys;
    _notationName = notation;
}

entity_impl::entity_impl (entity_impl * n, bool deep)
    : node_impl(n, deep)
{
    _sys = n->_sys;
    _pub = n->_pub;
    _notationName = n->_notationName;
}

node_impl * entity_impl::cloneNode (bool deep)
{
    node_impl * p = new entity_impl(this, deep);
    p->ref.deref();
    return p;
}


entity::entity() : node()
{
}

entity::entity (const entity & other) : node(other)
{}

entity::entity (entity_impl * n) : node(n)
{}

entity & entity::operator = (const entity & other)
{
    return (entity &) node::operator = (other);
}

pfs::string entity::publicId () const
{
	return _pimpl
			? dynamic_cast<entity_impl *>(_pimpl)->_pub
			: pfs::string();
}

pfs::string entity::systemId () const
{
	return _pimpl
			? dynamic_cast<entity_impl *>(_pimpl)->_sys
			: pfs::string();
}

pfs::string entity::notationName() const
{
	return _pimpl
			? dynamic_cast<entity_impl *>(_pimpl)->_notationName
			: pfs::string();
}

}} // cwt::dom


