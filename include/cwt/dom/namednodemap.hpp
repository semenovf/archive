/*
 * namednodemap.hpp
 *
 *  Created on: Nov 21, 2013
 *      Author: wladt
 */

#ifndef __CWT_DOM_NAMEDNODEMAP_HPP__
#define __CWT_DOM_NAMEDNODEMAP_HPP__

#include <pfs/string.hpp>
#include <cwt/dom/pimpl.hpp>

namespace cwt { namespace dom {

class node;

class namednodemap
{
	CWT_DOM_PIMPL_INLINE(namednodemap)

public:
	node namedItem (const pfs::string & name) const
		{ return getNamedItem (name); }
//	node getNamedItem (const pfs::string & name) const;
//	node setNamedItem (node arg); // raises(DOMException)
//	node removeNamedItem (const pfs::string & name); // raises(DOMException)
//	node item (size_t index) const;
//
//	size_t length () const;

	node namedItemNS(const pfs::string & namespaceURI, const pfs::string & localName) const
		{ return getNamedItemNS(namespaceURI, localName); }
//	node getNamedItemNS(const pfs::string & namespaceURI, const pfs::string & localName);
//	node setNamedItemNS(const node & arg); //  raises(DOMException)
//	node removeNamedItemNS(const pfs::string & namespaceURI, const pfs::string & localName); // raises(DOMException);
};

}} // cwt::dom

#endif /* __CWT_DOM_NAMEDNODEMAP_HPP__ */
