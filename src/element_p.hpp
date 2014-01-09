/**
 * @file element_p.hpp
 * @author wladt
 * @date Dec 12, 2013
 */

#ifndef __CWT_ELEMENT_P_HPP__
#define __CWT_ELEMENT_P_HPP__

#include "../include/cwt/dom/element.hpp"

namespace cwt { namespace dom {

class node_impl;
class attr_impl;
class document_impl;
class namednodemap_impl;

class element_impl : public node_impl
{
public:
    namednodemap_impl * _attr;

public:
    element_impl (document_impl * d, node_impl * parent, const pfs::string & name);
    element_impl (document_impl * d, node_impl * parent, const pfs::string & nsURI, const pfs::string & qName);
    element_impl (element_impl * n, bool deep);
    ~element_impl ();

    pfs::string attribute (const pfs::string & name, const pfs::string & defaultValue) const;
    pfs::string attributeNS (const pfs::string & nsURI, const pfs::string & localName, const pfs::string & defaultValue) const;
    void setAttribute (const pfs::string & name, const pfs::string & value);
    void setAttributeNS (const pfs::string & nsURI, const pfs::string & qName, const pfs::string & newValue);
    void removeAttribute (const pfs::string & name);
    attr_impl * attributeNode (const pfs::string & name);
    attr_impl * attributeNodeNS (const pfs::string & nsURI, const pfs::string & localName);
    attr_impl * setAttributeNode (attr_impl * newAttr);
    attr_impl * setAttributeNodeNS (attr_impl * newAttr);
    attr_impl * removeAttributeNode (attr_impl * oldAttr);
    bool hasAttribute (const pfs::string & name) const;
    bool hasAttributeNS (const pfs::string & nsURI, const pfs::string & localName) const;

    namednodemap_impl * attributes () { return _attr; }
    bool hasAttributes () const;

    virtual node::type nodeType() const { return node::ElementNode; }
    virtual node_impl * cloneNode (bool deep = true);
};

}} // cwt::dom

#endif /* __CWT_ELEMENT_P_HPP__ */
