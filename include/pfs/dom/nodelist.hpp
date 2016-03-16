/*
 * nodelist.hpp
 *
 *  Created on: Nov 21, 2013
 *      Author: wladt
 */

#ifndef __PFS_DOM_NODELIST_HPP__
#define __PFS_DOM_NODELIST_HPP__

#include <pfs.hpp>

namespace pfs { namespace dom {

class node;
class element;
class document;
class nodelist_impl;

class DLL_API nodelist
{
	friend class node;
	friend class element;
	friend class document;

private:
	nodelist_impl * _pimpl;

	nodelist (nodelist_impl * p) : _pimpl(p) {}

public:
	nodelist () : _pimpl(0) {}
	nodelist (const nodelist & other);
	nodelist & operator = (const nodelist & other);
	~nodelist ();

	node item (size_t index) const;
	size_t length () const;
	size_t size () const { return length(); }
};

}} // pfs::dom

#endif /* __PFS_DOM_NODELIST_HPP__ */
