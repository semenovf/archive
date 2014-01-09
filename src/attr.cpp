/**
 * @file attr.cpp
 * @author wladt
 * @date Dec 12, 2013
 */

#include "node_p.hpp"
#include "attr_p.hpp"
#include "document_p.hpp"
#include "element_p.hpp"

namespace cwt { namespace dom {

attr_impl::attr_impl (document_impl * d, node_impl * parent, const pfs::string & nodeName)
    : node_impl(d, parent)
{
    name = nodeName;
    _specified = false;
}

attr_impl::attr_impl (document_impl * d, node_impl * p, const pfs::string & nsURI, const pfs::string & qName)
    : node_impl(d, p)
{
    qt_split_namespace(prefix, name, qName, !nsURI.isNull()); // FIXME
    namespaceURI = nsURI;
    createdWithDom1Interface = false;
    _specified = false;
}

attr_impl::attr_impl (attr_impl * n, bool deep)
    : node_impl(n, deep)
{
    _specified = n->specified();
}

bool attr_impl::specified() const
{
    return _specified;
}

void attr_impl::setNodeValue (const pfs::string & v)
{
    value = v;
    text_impl *t = new text_impl(0, this, v);
    t->ref.deref();
    if (first) {
        delete removeChild(first);
    }
    appendChild(t);
}

node_impl* attr_impl::cloneNode(bool deep)
{
    node_impl* p = new attr_impl(this, deep);
    p->ref.deref();
    return p;
}


attr::attr()
{
}

attr::attr(const attr & other) : node(other)
{
}

attr::attr (attr_impl * n) : node(n)
{
}

attr & attr::operator = (const attr & other)
{
    return (attr &) node::operator = (other);
}

pfs::string attr::name () const
{
	return _pimpl
			? _pimpl->nodeName()
			: pfs::string();
}

bool attr::specified () const
{
	return _pimpl
			? dynamic_cast<attr_impl*>(_pimpl)->specified()
			: false;
}

pfs::string attr::value () const
{
	return _pimpl
			? _pimpl->nodeValue()
			: pfs::string();
}

void attr::setValue (const pfs::string & v)
{
    if (!_pimpl)
        return;
    _pimpl->setNodeValue(v);
    dynamic_cast<attr_impl*>(_pimpl)->_specified = true;
}

element attr::ownerElement() const
{
    PFS_ASSERT(_pimpl->parent());

    if (!_pimpl->parent()->isElement())
    	return element();
    return element(dynamic_cast<element_impl*>(_pimpl->parent()));
}

}} // cwt::dom


