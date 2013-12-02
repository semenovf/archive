/*
 * attr.hpp
 *
 *  Created on: Nov 20, 2013
 *      Author: wladt
 */

#ifndef __CWT_DOM_ATTR_HPP__
#define __CWT_DOM_ATTR_HPP__

#include <cwt/dom/node.hpp>

namespace cwt { namespace dom {

class element;

class attr : public node
{
	pfs::string _name;      // readonly
	bool        _specified; // readonly
	pfs::string _value;
	element *   _ownerElement; // readonly

public:
	const pfs::string & name () const { return _name; }
	bool specified () const { return _specified; }
	const pfs::string & value () const { return _value; }
	void setValue (const pfs::string & v) { _value = v; } // raises(DOMException)
	const element * ownerElement () const { return _ownerElement; }

};

}} // cwt::dom

#endif /* __CWT_DOM_ATTR_HPP__ */
