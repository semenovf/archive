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
class document;

class DLL_API attr : public node
{
	friend class node;
	friend class element;
	friend class document;

protected:
	attr (attr_impl *);

public:
	attr ();
	attr (const attr & other);
	attr & operator = (const attr & other);

	pfs::string name () const;
	bool specified () const;
	pfs::string value () const;
	void setValue (const pfs::string & v);
	element ownerElement () const;
};

}} // cwt::dom

#endif /* __CWT_DOM_ATTR_HPP__ */
