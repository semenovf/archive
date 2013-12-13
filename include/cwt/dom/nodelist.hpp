/*
 * nodelist.hpp
 *
 *  Created on: Nov 21, 2013
 *      Author: wladt
 */

#ifndef __CWT_DOM_NODELIST_HPP__
#define __CWT_DOM_NODELIST_HPP__

#include <pfs/shared_ptr.hpp>

namespace cwt { namespace dom {

class node;

class DLL_API nodelist
{
private:
	class impl;
	pfs::shared_ptr<impl> _pimpl;

public:
	node item (size_t index) const;
	size_t length () const;
	size_t size () const { return length(); }
};

}} // cwt::dom

#endif /* __CWT_DOM_NODELIST_HPP__ */
