/**
 * @file attr_p.hpp
 * @author wladt
 * @date Dec 12, 2013
 */

#ifndef __PFS_DOM_ATTR_P_HPP__
#define __PFS_DOM_ATTR_P_HPP__

#include "pfs/dom/attr.hpp"

namespace pfs { namespace dom {

class node_impl;
class document_impl;

class DLL_API attr_impl : public node_impl
{
public:
    bool _specified;

public:
	attr_impl (document_impl *, node_impl *, const pfs::string & name);
	attr_impl (document_impl *, node_impl *, const pfs::string & nsURI, const pfs::string & qName);
	attr_impl (attr_impl * n, bool deep);

    bool specified () const;

    void set_node_value (const pfs::string & v);
    virtual node_impl * clone_node (bool deep = true);

    virtual node::type node_type () const
    {
    	return node::attribute_node;
    }
};

}} // pfs::dom

#endif /* __PFS_DOM_ATTR_P_HPP__ */
