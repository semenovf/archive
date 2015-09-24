/**
 * @file notation.cpp
 * @author wladt
 * @date Jan 10, 2014
 */

#include "node_p.hpp"
#include "nodelist_p.hpp"
#include "namednodemap_p.hpp"
#include "attr_p.hpp"
#include "element_p.hpp"
#include "notation_p.hpp"
#include "document_p.hpp"

namespace pfs { namespace dom {

notation_impl::notation_impl (document_impl * d
		, node_impl * parent
		, const pfs::string & name
		, const pfs::string & pub
		, const pfs::string & sys)
    : node_impl(d, parent)
{
    _name = name;
    _pub  = pub;
    _sys  = sys;
}

notation_impl::notation_impl (notation_impl * n, bool deep)
    : node_impl(n, deep)
{
    _sys = n->_sys;
    _pub = n->_pub;
}

node_impl * notation_impl::cloneNode (bool deep)
{
    node_impl * p = new notation_impl(this, deep);
    p->ref.deref();
    return p;
}


notation::notation() : node()
{}

notation::notation(const notation & other) : node(other)
{}

notation::notation(notation_impl * n) : node(n)
{}

notation & notation::operator = (const notation & other)
{
    return (notation &) node::operator = (other);
}

pfs::string notation::publicId () const
{
	return _pimpl
			? dynamic_cast<notation_impl *>(_pimpl)->_pub
			: pfs::string();
}

pfs::string notation::systemId () const
{
	return _pimpl
			? dynamic_cast<notation_impl *>(_pimpl)->_sys
			: pfs::string();
}

}} // pfs::dom
