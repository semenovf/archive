/*
 * node.hpp
 *
 *  Created on: Nov 20, 2013
 *      Author: wladt
 */

#ifndef __PFS_DOM_NODE_HPP__
#define __PFS_DOM_NODE_HPP__

#include <pfs/string.hpp>

namespace pfs { namespace dom {

class node_impl;
class document;
class nodelist;
class namednodemap;
class attr;
class element;

class DLL_API node
{
	friend class namednodemap;
	friend class nodelist;
	friend class document;

public:
	enum type {
		  null_node                    = 0
		, invalid_node                 = null_node
		, element_node                 = 1
		, attribute_node               = 2
		, text_node                    = 3
		, cdata_section_node           = 4
		, entity_reference_node        = 5
		, entity_node                  = 6
		, processing_instruction_node  = 7
		, comment_node                 = 8
		, document_node                = 9
		, document_type_node           = 10
		, document_fragment_node       = 11
		, notation_node                = 12
	};

protected:
	node_impl * _d;

	node (node_impl *);

public:
	node_impl * impl ()
	{
		return _d;
	}

	const node_impl * impl () const
	{
		return _d;
	}

public:
	node () : _d(0) {}

	node (const node & other);

	~node ();

	node & operator = (const node & other);

	pfs::string  node_name () const;
	pfs::string  node_value () const;
	node::type   node_type () const;
	node parent_node () const;
	node first_child () const;
	node last_child () const;
	node previous_sibling () const;
	node next_sibling () const;
	nodelist child_nodes () const;
	namednodemap attributes () const;
	pfs::string namespace_uri () const;
	pfs::string prefix () const;
	pfs::string localname () const;
	document owner_document () const;

	bool is_supported (const pfs::string & feature, const pfs::string & version) const;

	bool has_child_nodes () const;
	bool has_attributes () const;

	void normalize ();

	void set_node_value (const pfs::string & value);
	void set_prefix (const pfs::string & p);

	node insert_after (const node & newchild, const node & refchild);
	node insert_before (const node & newchild, const node & refchild);
	node replace_child (const node & newchild, const node & oldchild);
	node remove_child  (const node & oldchild);
	node append_child  (const node & newchild);

	node clone_node (bool deep = true) const;

	bool is_null () const
	{
		return _d == 0;
	}


	// Non-standard
	//

	attr to_attr () const;
	element to_element () const;

	void traverse (void (* on_start) (const pfs::dom::node & n, void * d)
			, void (* on_end) (const pfs::dom::node & n, void * d)
			, void * userdata) const;
};

}} // pfs::dom

#endif /* __PFS_DOM_NODE_HPP__ */
