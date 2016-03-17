/*
 * namednodemap.hpp
 *
 *  Created on: Nov 21, 2013
 *      Author: wladt
 */

#ifndef __PFS_DOM_NAMEDNODEMAP_HPP__
#define __PFS_DOM_NAMEDNODEMAP_HPP__

#include <pfs/string.hpp>

namespace pfs { namespace dom {

class node;
class namednodemap_impl;
class doctype;

class namednodemap
{
	friend class node;
	friend class doctype;

private:
	namednodemap_impl * _d;

	namednodemap (namednodemap_impl *);

public:
	namednodemap ()
		: _d(0)
	{ }

	namednodemap (const namednodemap & other);

	~namednodemap ();

	namednodemap & operator = (const namednodemap & other);

	node named_item (const pfs::string & name) const
	{
		return get_named_item (name);
	}

	node get_named_item (const pfs::string & name) const;

	node named_item_ns (const pfs::string & namespace_uri, const pfs::string & localname) const
	{
		return get_named_item_ns(namespace_uri, localname);
	}

	node get_named_item_ns (const pfs::string & namespace_uri, const pfs::string & localname) const;

	node set_named_item (const node & arg);
	node set_named_item_ns (const node & arg);

	node remove_named_item (const pfs::string & name);
	node remove_named_item_ns (const pfs::string & namespace_uri, const pfs::string & localname);

	node item (size_t index) const;
	size_t length () const;
};

}} // pfs::dom

#endif /* __PFS_DOM_NAMEDNODEMAP_HPP__ */
