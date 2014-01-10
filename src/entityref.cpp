/**
 * @file entityref.cpp
 * @author wladt
 * @date Jan 10, 2014
 */

#include "node_p.hpp"
#include "nodelist_p.hpp"
#include "namednodemap_p.hpp"
#include "attr_p.hpp"
#include "element_p.hpp"
#include "entityref_p.hpp"
#include "document_p.hpp"

namespace cwt { namespace dom {

entityref_impl::entityref_impl (document_impl * d, node_impl * parent, const pfs::string & name)
    : node_impl(d, parent)
{
    _name = name;
}

entityref_impl::entityref_impl (node_impl * n, bool deep)
    : node_impl(n, deep)
{
}

node_impl * entityref_impl::cloneNode (bool deep)
{
    node_impl * p = new entityref_impl(this, deep);
    p->ref.deref();
    return p;
}

entityref::entityref () : node()
{}

entityref::entityref (const entityref & other) : node(other)
{}

entityref::entityref (entityref_impl * n) : node(n)
{}

entityref & entityref::operator = (const entityref & other)
{
    return (entityref &) node::operator = (other);
}

}} // cwt::dom

