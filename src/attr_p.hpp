/**
 * @file attr_p.hpp
 * @author wladt
 * @date Dec 12, 2013
 */

#ifndef __CWT_DOM_ATTR_P_HPP__
#define __CWT_DOM_ATTR_P_HPP__

#include "../include/cwt/dom/attr.hpp"

namespace cwt { namespace dom {

class node_impl;
class document_impl;

class attr_impl : public node_impl
{
public:
	attr_impl (document_impl *, node_impl *, const pfs::string & name);
	attr_impl (document_impl *, node_impl *, const pfs::string & nsURI, const pfs::string & qName);
	attr_impl (attr_impl * n, bool deep);

    bool specified () const;

    // Reimplemented from node_impl
    void setNodeValue (const pfs::string & v);
    node_impl * cloneNode (bool deep = true);
    node::type nodeType () const { return node::AttributeNode; }

    // Variables
    bool _specified;
};


}} // cwt::dom

#endif /* __CWT_DOM_ATTR_P_HPP__ */
