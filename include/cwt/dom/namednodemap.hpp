/*
 * namednodemap.hpp
 *
 *  Created on: Nov 21, 2013
 *      Author: wladt
 */

#ifndef __CWT_DOM_NAMEDNODEMAP_HPP__
#define __CWT_DOM_NAMEDNODEMAP_HPP__

#include <pfs/map.hpp>
#include <pfs/string.hpp>

namespace cwt { namespace dom {

class node;

class named_node_map : public cwt::map<pfs::string, node *>
{
	typedef cwt::map<pfs::string, node *> base_class;
public:
	named_node_map() : base_class() {}

	node * getNamedItem (const pfs::string & name);
	node * setNamedItem (node * arg); // raises(DOMException)
	node * removeNamedItem (const pfs::string & name); // raises(DOMException)
	node * item (size_t index);

	size_t length () const { return base_class::size(); }

	// node * getNamedItemNS(const cwt::string & namespaceURI, const cwt::string & localName);
	// node * setNamedItemNS(node * arg); //  raises(DOMException)
	// node * removeNamedItemNS(const cwt::string & namespaceURI,
	//		 	 	 const cwt::string & localName); // raises(DOMException);
}

}} // cwt::dom

#endif /* __CWT_DOM_NAMEDNODEMAP_HPP__ */
