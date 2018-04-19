/**
 * @file element.cpp
 * @author wladt
 * @date Dec 12, 2013
 */

#include "node_p.hpp"
#include "attr_p.hpp"
#include "nodelist_p.hpp"
#include "namednodemap_p.hpp"
#include "element_p.hpp"
#include "utils.hpp"

namespace pfs { namespace dom {

element_impl::element_impl (document_impl * d, node_impl * p, const pfs::string & tagname)
    : node_impl(d, p)
{
    _name = tagname;
    _attr = new namednodemap_impl(this);
}

element_impl::element_impl (document_impl * d, node_impl * p, const pfs::string & nsURI, const pfs::string & qName)
    : node_impl(d, p)
{
    split_namespace(_prefix, _name, qName);
    _namespace_uri = nsURI;
//    _createdWithDom1Interface = false;
    _attr = new namednodemap_impl(this);
}

element_impl::element_impl (element_impl * n, bool deep)
	: node_impl(n, deep)
{
    _attr = n->_attr->clone(this);
    _attr->_ref.ref();
}

element_impl::~element_impl()
{
    if (!_attr->_ref.deref())
        delete _attr;
}

pfs::string element_impl::attribute (const pfs::string & nodeName, const pfs::string & defaultValue) const
{
    node_impl * n = _attr->named_item(nodeName);
    return n ? n->node_value()
    		: defaultValue;
}

pfs::string element_impl::attribute_ns (const pfs::string & nsURI, const pfs::string & localName, const pfs::string & defaltValue) const
{
    node_impl * n = _attr->named_item_ns(nsURI, localName);
    return n ? n->node_value()
    		 : defaltValue;
}


void element_impl::set_attribute (const pfs::string & attname, const pfs::string & newValue)
{
    node_impl * n = _attr->named_item(attname);

    if (!n) {
        n = new attr_impl(owner_document(), this, attname);
        n->set_node_value(newValue);
        n->ref.deref();
        _attr->set_named_item(n);
    } else {
        n->set_node_value(newValue);
    }
}

void element_impl::set_attribute_ns (const pfs::string & nsURI, const pfs::string & qName, const pfs::string & newValue)
{
    pfs::string prefix, localName;

    split_namespace(prefix, localName, qName);

    node_impl * n = _attr->named_item_ns(nsURI, localName);

    if (!n) {
        n = new attr_impl(owner_document(), this, nsURI, qName);
        n->set_node_value(newValue);
        n->ref.deref();
        _attr->set_named_item(n);
    } else {
        n->set_node_value(newValue);
        n->_prefix = prefix;
    }
}

void element_impl::remove_attribute (const pfs::string & attname)
{
    node_impl * p = _attr->remove_named_item(attname);
    if (p && p->ref.load() == 0)
        delete p;
}

attr_impl * element_impl::attribute_node (const pfs::string & attname)
{
    return dynamic_cast<attr_impl*>(_attr->named_item(attname));
}

attr_impl * element_impl::attribute_node_ns(const pfs::string & ns_uri, const pfs::string & localname)
{
    return dynamic_cast<attr_impl*>(_attr->named_item_ns(ns_uri, localname));
}

attr_impl * element_impl::set_attribute_node (attr_impl * newattr)
{
    node_impl * n = _attr->named_item(newattr->node_name());

    _attr->set_named_item(newattr);
    newattr->setParent(this);
    return dynamic_cast<attr_impl *>(n);
}

attr_impl* element_impl::set_attribute_node_ns(attr_impl* newattr)
{
    node_impl * n = 0;

    if (!newattr->_prefix.empty())
        n = _attr->named_item_ns(newattr->_namespace_uri, newattr->_name);

    _attr->set_named_item(newattr);
    return dynamic_cast<attr_impl *>(n);
}

attr_impl * element_impl::remove_attribute_node (attr_impl * oldattr)
{
    return dynamic_cast<attr_impl *>(_attr->remove_named_item(oldattr->node_name()));
}

bool element_impl::has_attribute (const pfs::string & name) const
{
    return _attr->contains(name);
}

bool element_impl::has_attribute_ns (const pfs::string & nsURI, const pfs::string & localName) const
{
    return _attr->contains_ns(nsURI, localName);
}


/*

pfs::string element_impl::text()
{
    pfs::string t(QLatin1String(""));

    node_impl* p = first;
    while (p) {
        if (p->isText() || p->isCDATASection())
            t += p->nodeValue();
        else if (p->isElement())
            t += ((element_impl*)p)->text();
        p = p->next;
    }

    return t;
}
*/

node_impl * element_impl::clone_node(bool deep)
{
    node_impl * p = new element_impl(this, deep);
    p->ref.deref();
    return p;
}


element::element () : node()
{
}

element::element (const element & other) : node(other)
{
}

element::element (element_impl * n) : node(n)
{
}

element & element::operator = (const element & other)
{
    return (element &) node::operator = (other);
}

pfs::string element::tagname () const
{
	return _d
			? _d->node_name()
			: pfs::string();
}

pfs::string element::get_attribute (const pfs::string & name, const pfs::string & defaultValue) const
{
	return _d
			? dynamic_cast<element_impl *>(_d)->attribute(name, defaultValue)
			: defaultValue;
}

void element::set_attribute (const pfs::string & name, const pfs::string & value)
{
    if (_d) {
    	dynamic_cast<element_impl *>(_d)->set_attribute(name, value);
    }
}

void element::remove_attribute (const pfs::string& name)
{
    if (_d) {
    	dynamic_cast<element_impl *>(_d)->remove_attribute(name);
    }
}

attr element::get_attribute_node (const pfs::string & name)
{
    return _d
    		? attr(dynamic_cast<element_impl *>(_d)->attribute_node(name))
    		: attr();
}

/**
 * @details Adds a new attribute node.
 * 		If an attribute with that name (nodeName)
 * 		is already present in the element,
 * 		it is replaced by the new one.
 * 		To add a new attribute node with a qualified name and
 * 		namespace URI, use the @c setAttributeNodeNS method.
 *
 * @param newAttr The @c attr node to add to the attribute list.
 *
 * @return If the newAttr attribute replaces an existing attribute,
 * 		the replaced @c attr node is returned, otherwise null is returned.
 */
attr element::set_attribute_node(const attr & newattr)
{
	return _d
			? attr(dynamic_cast<element_impl *>(_d)->set_attribute_node((dynamic_cast<attr_impl*>(newattr._d))))
			: attr();
}

/**
 * @details Removes the specified attribute node.
 * 		If the removed @c attr has a default value
 * 		it is immediately replaced.
 * 		The replacing attribute has the same namespace
 * 		URI and local name, as well as the original prefix,
 * 		when applicable.
 *
 * @param oldAttr The @c attr node to remove from the attribute list.
 *
 * @return The @c attr node that was removed.
 */
attr element::remove_attribute_node(const attr & oldattr)
{
	return _d
			? attr(dynamic_cast<element_impl *>(_d)->remove_attribute_node((dynamic_cast<attr_impl*>(oldattr._d))))
			: attr();
}

nodelist element::get_elements_by_tagname (const pfs::string & tagname) const
{
    return nodelist(new nodelist_impl(_d, tagname));
}

pfs::string element::get_attribute_ns (const pfs::string & ns_uri, const pfs::string & localName, const pfs::string & defaultValue) const
{
	return _d
			? dynamic_cast<element_impl *>(_d)->attribute_ns(ns_uri, localName, defaultValue)
			: defaultValue;
}


void element::set_attribute_ns (const pfs::string & ns_uri, const pfs::string & qname, const pfs::string & value)
{
	if (_d) {
		dynamic_cast<element_impl *>(_d)->set_attribute_ns(ns_uri, qname, value);
	}
}

void element::remove_attribute_ns (const pfs::string & nsURI, const pfs::string & localName)
{
	if (_d) {
		node_impl * n = dynamic_cast<element_impl *>(_d)->attribute_node_ns(nsURI, localName);
	    if (n)
	    	dynamic_cast<element_impl *>(_d)->remove_attribute(n->node_name());
	}
}

attr element::get_attribute_node_ns (const pfs::string & nsURI, const pfs::string & localName) const
{
	return _d
			? attr(dynamic_cast<element_impl *>(_d)->attribute_node_ns(nsURI, localName))
			: attr();
}

attr element::set_attribute_node_ns (const attr & newAttr)
{
	return _d
			? attr(dynamic_cast<element_impl *>(_d)->set_attribute_node_ns((dynamic_cast<attr_impl *>(newAttr._d))))
			: attr();
}

nodelist element::get_elements_by_tagname_ns (const pfs::string& nsURI, const pfs::string& localName) const
{
    return nodelist(new nodelist_impl(_d, nsURI, localName));
}

bool element::has_attribute (const pfs::string & name) const
{
    return _d
    		? dynamic_cast<element_impl *>(_d)->has_attribute(name)
    		: false;
}

bool element::has_attribute_ns (const pfs::string & nsURI, const pfs::string & localName) const
{
    return _d
    		? dynamic_cast<element_impl *>(_d)->has_attribute_ns(nsURI, localName)
    		: false;
}

}} // pfs::dom

