/**
 * @file pinstruction.cpp
 * @author wladt
 * @date Jan 10, 2014
 */

#include "node_p.hpp"
#include "nodelist_p.hpp"
#include "namednodemap_p.hpp"
#include "attr_p.hpp"
#include "element_p.hpp"
#include "pinstruction_p.hpp"
#include "document_p.hpp"

namespace cwt { namespace dom {

pinstruction_impl::pinstruction_impl (document_impl * d
		, node_impl * parent
		, const pfs::string & target
		, const pfs::string & data)
    : node_impl(d, parent)
{
    _name  = target;
    _value = data;
}

pinstruction_impl::pinstruction_impl (pinstruction_impl * n, bool deep) : node_impl(n, deep)
{}


node_impl * pinstruction_impl::cloneNode (bool deep)
{
    node_impl * p = new pinstruction_impl(this, deep);
    p->ref.deref();
    return p;
}


pinstruction::pinstruction () : node()
{}

pinstruction::pinstruction (const pinstruction & other) : node(other)
{}

pinstruction::pinstruction (pinstruction_impl * n) : node(n)
{}

pinstruction & pinstruction::operator = (const pinstruction & other)
{
    return (pinstruction &) node::operator = (other);
}

pfs::string pinstruction::target() const
{
	return _pimpl
			? _pimpl->nodeName()
			: pfs::string();
}

pfs::string pinstruction::data() const
{
	return _pimpl
			? _pimpl->nodeValue()
			: pfs::string();
}

void pinstruction::setData (const pfs::string & d)
{
	if (_pimpl) {
		_pimpl->setNodeValue(d);
	}
}

}} // cwt::dom


