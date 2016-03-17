/**
 * @file doctype_p.hpp
 * @author wladt
 * @date Dec 10, 2013
 */

#ifndef __PFS_DOM_DOCTYPE_P_HPP__
#define __PFS_DOM_DOCTYPE_P_HPP__


#include "pfs/dom/doctype.hpp"

namespace pfs { namespace dom {

class node_impl;
class document_impl;
class namednodemap_impl;

class DLL_API doctype_impl : public node_impl
{
public:
    namednodemap_impl * _entities;
    namednodemap_impl * _notations;
    pfs::string _public_id;
    pfs::string _system_id;
    pfs::string _internal_subset;

public:
    doctype_impl (document_impl *, node_impl * parent = 0);
    doctype_impl (doctype_impl * other, bool deep);
    ~doctype_impl ();
    void init ();

    virtual node_impl * insert_before (node_impl * newchild, node_impl * refchild);
    virtual node_impl * insert_after  (node_impl * newchild, node_impl * refchild);
    virtual node_impl * replace_child (node_impl * newchild, node_impl * oldchild);
    virtual node_impl * remove_child  (node_impl * oldchild);
    virtual node_impl * append_child  (node_impl * newchild);

    virtual node_impl * clone_node (bool deep = true);

    virtual node::type node_type() const
    {
    	return node::document_type_node;
    }
};

}} // pfs::dom

#endif /* __PFS_DOM_DOCTYPE_P_HPP__ */
