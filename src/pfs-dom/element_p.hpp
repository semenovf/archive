/**
 * @file element_p.hpp
 * @author wladt
 * @date Dec 12, 2013
 */

#ifndef __PFS_ELEMENT_P_HPP__
#define __PFS_ELEMENT_P_HPP__

#include "pfs/dom/element.hpp"

namespace pfs { namespace dom {

class node_impl;
class attr_impl;
class document_impl;
class namednodemap_impl;

class DLL_API element_impl : public node_impl
{
public:
    namednodemap_impl * _attr;

public:
    element_impl (document_impl * d, node_impl * parent, const pfs::string & name);
    element_impl (document_impl * d, node_impl * parent, const pfs::string & nsURI, const pfs::string & qName);
    element_impl (element_impl * n, bool deep);
    ~element_impl ();

    pfs::string attribute (const pfs::string & name, const pfs::string & defaultvalue) const;
    pfs::string attribute_ns (const pfs::string & ns_uri, const pfs::string & localname, const pfs::string & defaultvalue) const;

    void set_attribute (const pfs::string & name, const pfs::string & value);
    void set_attribute_ns (const pfs::string & ns_uri, const pfs::string & qname, const pfs::string & newvalue);
    void remove_attribute (const pfs::string & name);
    attr_impl * attribute_node (const pfs::string & name);
    attr_impl * attribute_node_ns (const pfs::string & nsURI, const pfs::string & localname);
    attr_impl * set_attribute_node (attr_impl * newattr);
    attr_impl * set_attribute_node_ns (attr_impl * newattr);
    attr_impl * remove_attribute_node (attr_impl * oldattr);
    bool has_attribute (const pfs::string & name) const;
    bool has_attribute_ns (const pfs::string & ns_uri, const pfs::string & localname) const;

    namednodemap_impl * attributes ()
    {
    	return _attr;
    }

    bool has_attributes () const
    {
    	return _attr->length() > 0;
    }

    virtual node::type node_type() const
    {
    	return node::element_node;
    }

    virtual node_impl * clone_node (bool deep = true);
};

}} // pfs::dom

#endif /* __PFS_ELEMENT_P_HPP__ */
