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

class node;

class attr : node
{
public:
	pfs::string name () const { return _name; }
	const pfs::string & value () const { return _value; }
	void setValue (const pfs::string & v) { _value = v; }

private:
	pfs::string _name;      // ro
	bool        _specified; // ro
	pfs::string _value;     // raises(DOMException) on setting

  // Introduced in DOM Level 2:
  readonly attribute Element          ownerElement;
};

}} // cwt::dom

#endif /* __CWT_DOM_ATTR_HPP__ */
