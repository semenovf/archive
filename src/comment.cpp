/**
 * @file comment.cpp
 * @author wladt
 * @date Jan 9, 2014
 */

#include "node_p.hpp"
#include "nodelist_p.hpp"
#include "namednodemap_p.hpp"
#include "attr_p.hpp"
#include "element_p.hpp"
#include "chardata_p.hpp"
//#include "text_p.hpp"
#include "comment_p.hpp"
#include "document_p.hpp"

namespace cwt { namespace dom {

comment_impl::comment_impl (document_impl * d, node_impl * parent, const pfs::string & val)
    : chardata_impl(d, parent, val)
{
    _name = pfs::string("#comment");
}

comment_impl::comment_impl (comment_impl * n, bool deep)
    : chardata_impl(n, deep)
{
}

node_impl * comment_impl::cloneNode (bool deep)
{
    node_impl * p = new comment_impl(this, deep);
    p->ref.deref();
    return p;
}

comment::comment() : chardata()
{}

comment::comment (const comment & other) : chardata(other)
{}

comment::comment (comment_impl * n) : chardata(n)
{}

comment & comment::operator = (const comment & other)
{
    return (comment &) node::operator = (other);
}


}} // cwt::dom

