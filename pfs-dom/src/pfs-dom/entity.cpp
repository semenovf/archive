/**
 * @file entity.cpp
 * @author wladt
 * @date Jan 9, 2014
 */

#include "node_p.hpp"
#include "nodelist_p.hpp"
#include "namednodemap_p.hpp"
#include "attr_p.hpp"
#include "element_p.hpp"
#include "entity_p.hpp"
#include "document_p.hpp"

namespace pfs { namespace dom {


entity_impl::entity_impl (document_impl * d
		, node_impl * parent
        , const pfs::string & name
        , const pfs::string & pub
        , const pfs::string & sys
        , const pfs::string & notation)
    : node_impl(d, parent)
{
    _name = name;
    _pub  = pub;
    _sys  = sys;
    _notationName = notation;
}

entity_impl::entity_impl (entity_impl * n, bool deep)
    : node_impl(n, deep)
{
    _sys = n->_sys;
    _pub = n->_pub;
    _notationName = n->_notationName;
}

node_impl * entity_impl::clone_node (bool deep)
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

pfs::string entity::public_id () const
{
	return _d
			? dynamic_cast<entity_impl *>(_d)->_pub
			: pfs::string();
}

pfs::string entity::system_id () const
{
	return _d
			? dynamic_cast<entity_impl *>(_d)->_sys
			: pfs::string();
}

pfs::string entity::notation_name() const
{
	return _d
			? dynamic_cast<entity_impl *>(_d)->_notationName
			: pfs::string();
}

}} // pfs::dom


