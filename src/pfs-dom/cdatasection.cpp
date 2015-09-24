/**
 * @file cdatasection.cpp
 * @author wladt
 * @date Jan 10, 2014
 */

#include "node_p.hpp"
#include "nodelist_p.hpp"
#include "namednodemap_p.hpp"
#include "attr_p.hpp"
#include "element_p.hpp"
#include "chardata_p.hpp"
#include "text_p.hpp"
#include "cdatasection_p.hpp"
#include "document_p.hpp"
#include "utils.hpp"

namespace pfs { namespace dom {

cdatasection_impl::cdatasection_impl (document_impl * d, node_impl * parent, const pfs::string & val)
    : text_impl(d, parent, val)
{
    _name = pfs::string("#cdata-section");
}

cdatasection_impl::cdatasection_impl(cdatasection_impl * n, bool deep)
    : text_impl(n, deep)
{}

node_impl * cdatasection_impl::cloneNode (bool deep)
{
    node_impl * p = new cdatasection_impl(this, deep);
    p->ref.deref();
    return p;
}


cdatasection::cdatasection () : text()
{}

cdatasection::cdatasection (const cdatasection & other) : text(other)
{}

cdatasection::cdatasection (cdatasection_impl * n) : text(n)
{}

cdatasection & cdatasection::operator = (const cdatasection & other)
{
    return (cdatasection &) node::operator = (other);
}

}} // pfs::dom


