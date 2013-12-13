/**
 * @file attr_p.hpp
 * @author wladt
 * @date Dec 12, 2013
 */

#ifndef __CWT_DOM_ATTR_P_HPP__
#define __CWT_DOM_ATTR_P_HPP__

#include "../include/cwt/dom/attr.hpp"

namespace cwt { namespace dom {

class node::impl;
class document::impl;

class attr::impl : public node::impl
{
public:
    impl(document::impl*, node::impl*, const pfs::string& name);
    impl(document::impl*, node::impl*, const pfs::string& nsURI, const pfs::string& qName);
    impl(impl* n, bool deep);

    bool specified() const;

    // Reimplemented from node::impl
    void setNodeValue(const pfs::string& v);
    node::impl* cloneNode(bool deep = true);
    node::type nodeType() const { return node::AttributeNode; }

    // Variables
    bool m_specified;
};


}} // cwt::dom

#endif /* __CWT_DOM_ATTR_P_HPP__ */
