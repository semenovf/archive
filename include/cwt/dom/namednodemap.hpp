/*
 * namednodemap.hpp
 *
 *  Created on: Nov 21, 2013
 *      Author: wladt
 */

#ifndef __CWT_DOM_NAMEDNODEMAP_HPP__
#define __CWT_DOM_NAMEDNODEMAP_HPP__

#include <pfs/string.hpp>

namespace cwt { namespace dom {

class node;
class namednodemap_impl;
class doctype;

class namednodemap
{
	friend class node;
	friend class doctype;

private:
	namednodemap_impl * _pimpl;

	namednodemap (namednodemap_impl *);

public:
	namednodemap () : _pimpl(nullptr) {}
	namednodemap (const namednodemap & other);
	namednodemap & operator = (const namednodemap & other);
	~namednodemap ();

	node namedItem (const pfs::string & name) const
		{ return getNamedItem (name); }
	node getNamedItem (const pfs::string & name) const;
	node namedItemNS (const pfs::string & namespaceURI, const pfs::string & localName) const
		{ return getNamedItemNS(namespaceURI, localName); }
	node getNamedItemNS (const pfs::string & namespaceURI, const pfs::string & localName) const;

	node setNamedItem (const node & arg);
	node setNamedItemNS (const node & arg);

	node removeNamedItem (const pfs::string & name);
	node removeNamedItemNS (const pfs::string & namespaceURI, const pfs::string & localName);

	node item (size_t index) const;
	size_t length () const;
};

}} // cwt::dom

#endif /* __CWT_DOM_NAMEDNODEMAP_HPP__ */
