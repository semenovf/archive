/**
 * @file docfragment.cpp
 * @author wladt
 * @date Jan 9, 2014
 */

#include "node_p.hpp"
#include "nodelist_p.hpp"
#include "namednodemap_p.hpp"
#include "attr_p.hpp"
#include "element_p.hpp"
#include "chardata_p.hpp"
#include "text_p.hpp"
#include "comment_p.hpp"
#include "document_p.hpp"
#include "docfragment_p.hpp"

namespace pfs { namespace dom {

document_fragment_impl::document_fragment_impl (document_impl * d, node_impl * p)
    : node_impl(d, p)
{
    _name = pfs::string("#document-fragment");
}

document_fragment_impl::document_fragment_impl (node_impl * n, bool deep)
    : node_impl(n, deep)
{
}

node_impl * document_fragment_impl::cloneNode (bool deep)
{
    node_impl* p = new document_fragment_impl(this, deep);
    p->ref.deref();
    return p;
}

document_fragment::document_fragment ()
{}

document_fragment::document_fragment (document_fragment_impl * n)
    : node(n)
{}

document_fragment::document_fragment(const document_fragment & other)
    : node(other)
{}

document_fragment& document_fragment::operator = (const document_fragment & other)
{
    return (document_fragment &) node::operator = (other);
}

}} // pfs::dom


