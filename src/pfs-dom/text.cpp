/**
 * @file text.cpp
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
#include "document_p.hpp"


namespace pfs { namespace dom {

text_impl::text_impl (document_impl * d, node_impl * parent, const pfs::string & val)
    : chardata_impl(d, parent, val)
{
    _name = pfs::string("#text");
}

text_impl::text_impl(text_impl * n, bool deep)
    : chardata_impl(n, deep)
{
}

text_impl * text_impl::splitText (size_t offset)
{
    if (!parent()) {
        return 0;
    }

    text_impl * t = new text_impl(ownerDocument(), 0, _value.substr(offset));
    _value = _value.left(offset);

    parent()->insertAfter(t, this);

    return t;
}

node_impl * text_impl::cloneNode(bool deep)
{
    node_impl * p = new text_impl(this, deep);
    p->ref.deref();
    return p;
}

text::text() : chardata()
{}

text::text (const text & other) : chardata(other)
{}

text::text (text_impl * n) : chardata(n)
{}

text & text::operator = (const text & other)
{
    return (text &) node::operator = (other);
}


text text::splitText (size_t offset)
{
	return _pimpl
			? text(dynamic_cast<text_impl*>(_pimpl)->splitText(offset))
			: text();
}


}} // pfs::dom

