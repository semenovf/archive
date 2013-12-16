/**
 * @file element.cpp
 * @author wladt
 * @date Dec 12, 2013
 */

//#include <pfs/vector.hpp>
#include "node_p.hpp"
#include "attr_p.hpp"
#include "element_p.hpp"
#include "nodelist_p.hpp"
#include "namednodemap_p.hpp"

namespace cwt { namespace dom {

bool element_impl::hasAttributes () const
{
	return _attr->length() > 0;
}

bool element_impl::hasAttribute (const pfs::string & name) const
{
    return _attr->contains(name);
}

bool element::hasAttribute (const pfs::string & name) const
{
	return _pimpl
			? static_cast<element_impl *>(_pimpl)->hasAttribute(name)
			: false;
}

#ifdef __COMMENT_
pfs::string element::getAttribute (const pfs::string & name, const pfs::string & defaultValue) const
{
    if (!_pimpl)
        return defaultValue;

    node n = dynamic_pointer_cast<element::impl>(_pimpl)->_attr.namedItem(name);

    if (n.isNull())
        return defaultValue;

    return n.nodeValue();
}

void element::setAttribute (const pfs::string & name, const pfs::string & value)
{
    if (!_pimpl)
        return;

    node n = dynamic_pointer_cast<element::impl>(_pimpl)->_attr.namedItem(name);

    if (n.isNull()) {
        n = attr(new attr::impl(_pimpl, name)); // _pimpl is parent (i.e. this instance)
        n.setNodeValue(value);
        node n = dynamic_pointer_cast<element::impl>(_pimpl)->_attr.setNamedItem(n);
    } else {
        n.setNodeValue(value);
    }
}

void element::removeAttribute (const pfs::string & name)
{
    if (!_pimpl)
        return;

    node n = dynamic_pointer_cast<element::impl>(_pimpl)->_attr.removeNamedItem(name);
    if (!n.isNull()) {
    	pfs::shared_ptr<node::impl> null;
    	n._pimpl.swap(null); // TODO Test this, n must be destroyed (if no more references)
    }
}

attr element::getAttributeNode (const pfs::string & name)
{
	return _pimpl
			? dynamic_pointer_cast<element::impl>(_pimpl)->_attr.getNamedItem(name)
			: attr();
}

attr element::getAttributeNodeNS (const pfs::string & namespaceURI
					, const pfs::string & localName) const
{
	return _pimpl
			? dynamic_pointer_cast<element::impl>(_pimpl)->_attr.getNamedItemNS(namespaceURI, localName)
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
attr element::setAttributeNode (const attr & newAttr)
{
    if (!_pimpl)
        return attr();

    node n = dynamic_pointer_cast<element::impl>(_pimpl)->_attr.getNamedItem(newAttr.nodeName());

    attr a (newAttr);
    a._pimpl->_parent = _pimpl;
    dynamic_pointer_cast<element::impl>(_pimpl)->_attr.setNamedItem(a);

    return attr(dynamic_pointer_cast<attr::impl>(n._pimpl));
}

attr element::setAttributeNodeNS (const attr & newAttr)
{
    if (!_pimpl)
        return attr();

    node n;

    if (!newAttr.prefix().isEmpty())
    	n = dynamic_pointer_cast<element::impl>(_pimpl)->_attr.getNamedItemNS(newAttr.namespaceURI(), newAttr.nodeName());

    attr a (newAttr);
    a._pimpl->_parent = _pimpl;
    dynamic_pointer_cast<element::impl>(_pimpl)->_attr.setNamedItem(newAttr);

    return attr(dynamic_pointer_cast<attr::impl>(n._pimpl));
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
attr element::removeAttributeNode (const attr & oldAttr)
{
    if (!_pimpl)
        return attr();

    node n = dynamic_pointer_cast<element::impl>(_pimpl)->_attr.removeNamedItem(oldAttr.nodeName());
    return attr(dynamic_pointer_cast<attr::impl>(n._pimpl));
}


nodelist element::getElementsByTagName (const pfs::string & name) const
{
	return nodelist(new nodelist::impl(_pimpl, name));
}

nodelist element::getElementsByTagNameNS (const pfs::string & namespaceURI
			    	, const pfs::string & localName) const
{
	return nodelist(new nodelist::impl(_pimpl, namespaceURI, localName));
}

pfs::string element::getAttributeNS (const pfs::string & namespaceURI
		, const pfs::string & localName
		, const pfs::string & defaultValue) const
{

    if (!_pimpl)
        return defaultValue;

    node n = dynamic_pointer_cast<element::impl>(_pimpl)->_attr.namedItemNS(namespaceURI, localName);

    if (n.isNull())
        return defaultValue;

    return n.nodeValue();
}


void element::setAttributeNS (const pfs::string & namespaceURI
					, const pfs::string & qualifiedName
					, const pfs::string & value)
{
    if (!_pimpl)
        return;

    pfs::string prefix, localName;
    pfs::vector<pfs::string> parts = qualifiedName.split(pfs::ucchar(':'), true);
    if (parts.size() > 1) {
    	prefix = parts[0];
    	localName = parts[1];
    } else {
    	localName = qualifiedName;
    }

    node n = dynamic_pointer_cast<element::impl>(_pimpl)->_attr.namedItemNS(namespaceURI, localName);

    if (n.isNull()) {
        n = attr(new attr::impl(_pimpl, namespaceURI, qualifiedName)); // _pimpl is parent (i.e. this instance)
        n.setNodeValue(value);
        node n = dynamic_pointer_cast<element::impl>(_pimpl)->_attr.setNamedItem(n);
    } else {
        n.setNodeValue(value);
        n._pimpl->_prefix = prefix;
    }
}

void element::removeAttributeNS (const pfs::string & namespaceURI
					, const pfs::string & localName)
{
    if (!_pimpl)
        return;

    node n = dynamic_pointer_cast<element::impl>(_pimpl)->_attr.removeNamedItemNS(namespaceURI, localName);
    if (!n.isNull()) {
    	pfs::shared_ptr<node::impl> null;
    	n._pimpl.swap(null); // TODO Test this, n must be destroyed (if no more references)
    }
}


bool element::hasAttribute (const pfs::string & name) const
{
    if (!_pimpl)
        return false;

    return !dynamic_pointer_cast<element::impl>(_pimpl)->_attr.namedItem(name).isNull();
}

bool element::hasAttributeNS (const pfs::string & namespaceURI
		, const pfs::string & localName) const
{
    if (!_pimpl)
        return false;

    return !dynamic_pointer_cast<element::impl>(_pimpl)->_attr.namedItemNS(namespaceURI, localName).isNull();
}

#endif

}} // cwt::dom

