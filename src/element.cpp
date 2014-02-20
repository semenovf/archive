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

namespace cwt { namespace dom {

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
    _namespaceURI = nsURI;
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
    node_impl * n = _attr->namedItem(nodeName);
    return n ? n->nodeValue()
    		: defaultValue;
}

pfs::string element_impl::attributeNS (const pfs::string & nsURI, const pfs::string & localName, const pfs::string & defaltValue) const
{
    node_impl * n = _attr->namedItemNS(nsURI, localName);
    return n ? n->nodeValue()
    		 : defaltValue;
}


void element_impl::setAttribute (const pfs::string & attname, const pfs::string & newValue)
{
    node_impl * n = _attr->namedItem(attname);

    if (!n) {
        n = new attr_impl(ownerDocument(), this, attname);
        n->setNodeValue(newValue);
        n->ref.deref();
        _attr->setNamedItem(n);
    } else {
        n->setNodeValue(newValue);
    }
}

void element_impl::setAttributeNS (const pfs::string & nsURI, const pfs::string & qName, const pfs::string & newValue)
{
    pfs::string prefix, localName;

    split_namespace(prefix, localName, qName);

    node_impl * n = _attr->namedItemNS(nsURI, localName);

    if (!n) {
        n = new attr_impl(ownerDocument(), this, nsURI, qName);
        n->setNodeValue(newValue);
        n->ref.deref();
        _attr->setNamedItem(n);
    } else {
        n->setNodeValue(newValue);
        n->_prefix = prefix;
    }
}

void element_impl::removeAttribute (const pfs::string & attname)
{
    node_impl* p = _attr->removeNamedItem(attname);
    if (p && p->ref.load() == 0)
        delete p;
}

attr_impl * element_impl::attributeNode (const pfs::string & attname)
{
    return dynamic_cast<attr_impl*>(_attr->namedItem(attname));
}

attr_impl* element_impl::attributeNodeNS(const pfs::string& nsURI, const pfs::string& localName)
{
    return dynamic_cast<attr_impl*>(_attr->namedItemNS(nsURI, localName));
}

attr_impl * element_impl::setAttributeNode (attr_impl * newAttr)
{
    node_impl * n = _attr->namedItem(newAttr->nodeName());

    _attr->setNamedItem(newAttr);
    newAttr->setParent(this);
    return dynamic_cast<attr_impl *>(n);
}

attr_impl* element_impl::setAttributeNodeNS(attr_impl* newAttr)
{
    node_impl * n = nullptr;

    if (!newAttr->_prefix.isNull())
        n = _attr->namedItemNS(newAttr->_namespaceURI, newAttr->_name);

    _attr->setNamedItem(newAttr);
    return dynamic_cast<attr_impl *>(n);
}

attr_impl * element_impl::removeAttributeNode (attr_impl * oldAttr)
{
    return dynamic_cast<attr_impl *>(_attr->removeNamedItem(oldAttr->nodeName()));
}

bool element_impl::hasAttribute (const pfs::string & name) const
{
    return _attr->contains(name);
}

bool element_impl::hasAttributeNS (const pfs::string & nsURI, const pfs::string & localName) const
{
    return _attr->containsNS(nsURI, localName);
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

node_impl * element_impl::cloneNode(bool deep)
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

pfs::string element::tagName () const
{
	return _pimpl
			? _pimpl->nodeName()
			: pfs::string();
}

pfs::string element::getAttribute (const pfs::string & name, const pfs::string & defaultValue) const
{
	return _pimpl
			? dynamic_cast<element_impl *>(_pimpl)->attribute(name, defaultValue)
			: defaultValue;
}

void element::setAttribute (const pfs::string & name, const pfs::string & value)
{
    if (_pimpl) {
    	dynamic_cast<element_impl *>(_pimpl)->setAttribute(name, value);
    }
}

void element::removeAttribute (const pfs::string& name)
{
    if (_pimpl) {
    	dynamic_cast<element_impl *>(_pimpl)->removeAttribute(name);
    }
}

attr element::getAttributeNode (const pfs::string & name)
{
    return _pimpl
    		? attr(dynamic_cast<element_impl *>(_pimpl)->attributeNode(name))
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
attr element::setAttributeNode(const attr & newAttr)
{
	return _pimpl
			? attr(dynamic_cast<element_impl *>(_pimpl)->setAttributeNode((dynamic_cast<attr_impl*>(newAttr._pimpl))))
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
attr element::removeAttributeNode(const attr & oldAttr)
{
	return _pimpl
			? attr(dynamic_cast<element_impl *>(_pimpl)->removeAttributeNode((dynamic_cast<attr_impl*>(oldAttr._pimpl))))
			: attr();
}

nodelist element::getElementsByTagName (const pfs::string & tagname) const
{
    return nodelist(new nodelist_impl(_pimpl, tagname));
}

pfs::string element::getAttributeNS (const pfs::string & nsURI, const pfs::string & localName, const pfs::string & defaultValue) const
{
	return _pimpl
			? dynamic_cast<element_impl *>(_pimpl)->attributeNS(nsURI, localName, defaultValue)
			: defaultValue;
}


void element::setAttributeNS (const pfs::string & nsURI, const pfs::string & qName, const pfs::string & value)
{
	if (_pimpl) {
		dynamic_cast<element_impl *>(_pimpl)->setAttributeNS(nsURI, qName, value);
	}
}

void element::removeAttributeNS (const pfs::string & nsURI, const pfs::string & localName)
{
	if (_pimpl) {
		node_impl * n = dynamic_cast<element_impl *>(_pimpl)->attributeNodeNS(nsURI, localName);
	    if (n)
	    	dynamic_cast<element_impl *>(_pimpl)->removeAttribute(n->nodeName());
	}
}

attr element::getAttributeNodeNS (const pfs::string & nsURI, const pfs::string & localName) const
{
	return _pimpl
			? attr(dynamic_cast<element_impl *>(_pimpl)->attributeNodeNS(nsURI, localName))
			: attr();
}

attr element::setAttributeNodeNS (const attr & newAttr)
{
	return _pimpl
			? attr(dynamic_cast<element_impl *>(_pimpl)->setAttributeNodeNS((dynamic_cast<attr_impl *>(newAttr._pimpl))))
			: attr();
}

nodelist element::getElementsByTagNameNS (const pfs::string& nsURI, const pfs::string& localName) const
{
    return nodelist(new nodelist_impl(_pimpl, nsURI, localName));
}

bool element::hasAttribute (const pfs::string & name) const
{
    return _pimpl
    		? dynamic_cast<element_impl *>(_pimpl)->hasAttribute(name)
    		: false;
}

bool element::hasAttributeNS (const pfs::string & nsURI, const pfs::string & localName) const
{
    return _pimpl
    		? dynamic_cast<element_impl *>(_pimpl)->hasAttributeNS(nsURI, localName)
    		: false;
}

}} // cwt::dom

