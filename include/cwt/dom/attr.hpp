/*
 * attr.hpp
 *
 *  Created on: Nov 20, 2013
 *      Author: wladt
 */

#ifndef __CWT_DOM_ATTR_HPP__
#define __CWT_DOM_ATTR_HPP__

#include <pfs/string.hpp>

namespace cwt { namespace dom {

class attr_impl;
class node;
class element;

class DLL_API attr : public node
{
public:
//	pfs::string name () const { return node::nodeName (); }
//	bool specified () const;
//	pfs::string value () const { return node::nodeValue (); }
//	void setValue (const pfs::string & v); // raises(DOMException)
//	element ownerElement () const;
};

}} // cwt::dom

#endif /* __CWT_DOM_ATTR_HPP__ */
