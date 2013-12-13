/**
 * @file element_p.hpp
 * @author wladt
 * @date Dec 12, 2013
 */

#ifndef __CWT_ELEMENT_P_HPP__
#define __CWT_ELEMENT_P_HPP__

#include "../include/cwt/dom/element.hpp"

namespace cwt { namespace dom {

class node::impl;
class attr::impl;
class document::impl;
class namednodemap::impl;

class element::impl : public node::impl
{
public:
    impl(document::impl *, node::impl* parent, const pfs::string& name);
    impl(document::impl *, node::impl* parent, const pfs::string& nsURI, const pfs::string& qName);
    impl(impl* n, bool deep);
    ~impl();

    pfs::string attribute(const pfs::string& name, const pfs::string& defValue) const;
    pfs::string attributeNS(const pfs::string& nsURI, const pfs::string& localName, const pfs::string& defValue) const;
    void setAttribute(const pfs::string& name, const pfs::string& value);
    void setAttributeNS(const pfs::string& nsURI, const pfs::string& qName, const pfs::string& newValue);
    void removeAttribute(const pfs::string& name);
    attr::impl* attributeNode(const pfs::string& name);
    attr::impl* attributeNodeNS(const pfs::string& nsURI, const pfs::string& localName);
    attr::impl* setAttributeNode(attr::impl* newAttr);
    attr::impl* setAttributeNodeNS(attr::impl* newAttr);
    attr::impl* removeAttributeNode(attr::impl* oldAttr);
    bool hasAttribute(const pfs::string& name);
    bool hasAttributeNS(const pfs::string& nsURI, const pfs::string& localName);

    pfs::string text();

    // Reimplemented from node::impl
    namednodemap::impl * attributes() { return m_attr; }
    bool hasAttributes() { return (m_attr->length() > 0); }
    node::type nodeType() const { return node::ElementNode; }
    node::impl* cloneNode(bool deep = true);

    // Variables
    namednodemap::impl * m_attr;
};


}} // cwt::dom

#endif /* __CWT_ELEMENT_P_HPP__ */
