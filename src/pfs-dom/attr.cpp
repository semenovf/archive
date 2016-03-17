/**
 * @file attr.cpp

 * @author wladt
 * @date Dec 12, 2013
 */

#include "node_p.hpp"
#include "nodelist_p.hpp"
#include "namednodemap_p.hpp"
#include "attr_p.hpp"
#include "element_p.hpp"
#include "chardata_p.hpp"
#include "text_p.hpp"
#include "document_p.hpp"
#include "utils.hpp"

namespace pfs { namespace dom {

attr_impl::attr_impl (document_impl * d, node_impl * parent, const pfs::string & nodeName)
    : node_impl(d, parent)
{
    _name = nodeName;
    _specified = false;
}

attr_impl::attr_impl (document_impl * d, node_impl * p, const pfs::string & nsURI, const pfs::string & qName)
    : node_impl(d, p)
{
	split_namespace(_prefix, _name, qName);
    _namespace_uri = nsURI;
//    _createdWithDom1Interface = false;
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

void attr_impl::set_node_value (const pfs::string & v)
{
    _value = v;
    text_impl *t = new text_impl(0, this, v);
    t->ref.deref();
    if (_first) {
        delete remove_child(_first);
    }
    append_child(t);
}

node_impl* attr_impl::clone_node(bool deep)
{
    node_impl* p = new attr_impl(this, deep);
    p->ref.deref();
    return p;
}


attr::attr() : node()
{}

attr::attr(const attr & other) : node(other)
{}

attr::attr (attr_impl * n) : node(n)
{}

attr & attr::operator = (const attr & other)
{
    return (attr &) node::operator = (other);
}

pfs::string attr::name () const
{
	return _d
			? _d->node_name()
			: pfs::string();
}

bool attr::specified () const
{
	return _d
			? dynamic_cast<attr_impl*>(_d)->specified()
			: false;
}

pfs::string attr::value () const
{
	return _d
			? _d->node_value()
			: pfs::string();
}

void attr::set_value (const pfs::string & v)
{
    if (!_d)
        return;
    _d->set_node_value(v);
    dynamic_cast<attr_impl*>(_d)->_specified = true;
}

element attr::owner_element() const
{
    PFS_ASSERT(_d->parent());

    if (!_d->parent()->is_element())
    	return element();
    return element(dynamic_cast<element_impl*>(_d->parent()));
}

}} // pfs::dom


