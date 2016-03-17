/**
 * @file node_p.hpp
 * @author wladt
 * @date Dec 10, 2013
 */

#ifndef __PFS_DOM_NODE_P_HPP__
#define __PFS_DOM_NODE_P_HPP__

#include "pfs/dom/node.hpp"
#include <pfs/atomic.hpp>

namespace pfs { namespace dom {

class document_impl;
class nodelist_impl;

class DLL_API node_impl
{
	friend class nodelist_impl;

public:
    pfs::atomic_int ref;
    node_impl * _prev;
    node_impl * _next;
    node_impl * _ownerNode; // either the node's parent or the node's owner document
    node_impl * _first;
    node_impl * _last;

    pfs::string _name; // this is the local name if prefix != null
    pfs::string _value;
    pfs::string _prefix; // set this only for ElementNode and AttributeNode
    pfs::string _namespace_uri; // set this only for ElementNode and AttributeNode
//    bool _createdWithDom1Interface : 1;
    bool _has_parent : 1;

public:
    node_impl (document_impl *, node_impl * parent = 0);
    node_impl (node_impl * n, bool deep);
    virtual ~node_impl ();

    pfs::string node_name () const
    {
    	return _name;
    }

    pfs::string node_value () const
    {
    	return _value;
    }

    virtual void set_node_value (const pfs::string & v)
    {
    	_value = v;
    }

    document_impl * owner_document ();
    void set_owner_document (document_impl * doc);

    virtual node_impl * insert_before (node_impl * newChild, node_impl * refChild);
    virtual node_impl * insert_after  (node_impl * newChild, node_impl * refChild);
    virtual node_impl * replace_child (node_impl * newChild, node_impl * oldChild);
    virtual node_impl * remove_child  (node_impl * oldChild);
    virtual node_impl * append_child  (node_impl * newChild);

//    node_impl * named_item (const pfs::string & name);

    virtual node_impl * clone_node (bool deep = true);
    virtual void normalize ();
//    virtual void clear ();

    node_impl * parent () const
    {
    	return _has_parent ? _ownerNode : 0;
    }

    void setParent (node_impl * p)
    {
    	_ownerNode = p;
    	_has_parent = true;
    }

    void set_noparent ()
    {
        _ownerNode = _has_parent ? (node_impl *)owner_document() : 0;
        _has_parent = false;
    }

    bool is_attr() const
    {
    	return node_type() == node::attribute_node;
    }

    bool is_cdata_section() const
    {
    	return node_type() == node::cdata_section_node;
    }

    bool is_document_fragment() const
    {
    	return node_type() == node::document_fragment_node;
    }

    bool is_document() const
    {
    	return node_type() == node::document_node;
    }

    bool is_document_type() const
    {
    	return node_type() == node::document_type_node;
    }

    bool is_element() const
    {
    	return node_type() == node::element_node;
    }

    bool is_entity_reference() const
    {
    	return node_type() == node::entity_reference_node;
    }

    bool is_text() const
    {
    	const node::type nt = node_type();
    	return (nt == node::text_node)
    			|| (nt == node::cdata_section_node);
    }

    bool is_entity() const
    {
    	return node_type() == node::entity_node;
    }

    bool is_notation() const
    {
    	return node_type() == node::notation_node;
    }

    bool is_processing_instruction() const
    {
    	return node_type() == node::processing_instruction_node;
    }

    bool is_comment() const
    {
    	return node_type() == node::comment_node;
    }

    virtual node::type node_type() const
    {
    	return node::invalid_node;
    }
};

}} // pfs

#endif /* __PFS_DOM_NODE_P_HPP__ */
