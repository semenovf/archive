/**
 * @file nodelist_p.hpp
 * @author wladt
 * @date Dec 10, 2013
 */

#ifndef __CWT_DOM_NODELIST_P_HPP__
#define __CWT_DOM_NODELIST_P_HPP__

#include <pfs/vector.hpp>
#include "../include/cwt/dom/nodelist.hpp"

namespace cwt { namespace dom {

class node::impl;

// TODO Must be 'live' (http://www.w3.org/TR/2000/REC-DOM-Level-2-Core-20001113/core.html#td-live)

class nodelist::impl
{
public:
	typedef pfs::shared_ptr<node::impl> item_type;
	typedef pfs::vector<item_type>   list_type;

	item_type    _node_impl;
    pfs::string  _tagname;
    pfs::string  _namespaceURI;
	list_type    _list;
    long_t       _timestamp;

public:
    impl (const item_type & node_impl)
    	: _node_impl(node_impl)
    	, _timestamp(0)
    {}

    impl (const item_type & node_impl, const pfs::string & tagname)
		: _node_impl(node_impl)
    	, _tagname(tagname)
		, _timestamp(0)
    {}

    impl (const item_type & node_impl, const pfs::string & namespaceUri, const pfs::string & localName)
		: _node_impl(node_impl)
		, _tagname(localName)
    	, _namespaceURI(namespaceUri)
		, _timestamp(0)
    {}


    bool operator == (const impl & other) const
	{
    	return (_node_impl == other._node_impl) && (_tagname == other._tagname);
	}

    bool operator != (const impl & other) const
	{
    	return (_node_impl != other._node_impl) || (_tagname != other._tagname);
	}

	void populate ();
	size_t size (); // with population on demand
};

}} // cwt::dom

#endif /* __CWT_DOM_NODELIST_P_HPP__ */
