/*
 * nodelist.hpp
 *
 *  Created on: Nov 21, 2013
 *      Author: wladt
 */

#ifndef __CWT_DOM_NODELIST_HPP__
#define __CWT_DOM_NODELIST_HPP__

#include <cwt/dom/pimpl.hpp>

namespace cwt { namespace dom {

class node;

class DLL_API nodelist
{
	CWT_DOM_PIMPL_INLINE(nodelist)

public:
	node item (size_t index) const;
	size_t length () const;
	size_t size () const { return length(); }
};

}} // cwt::dom

#endif /* __CWT_DOM_NODELIST_HPP__ */
