/*
 * nodelist.hpp
 *
 *  Created on: Nov 21, 2013
 *      Author: wladt
 */

#ifndef __CWT_DOM_NODELIST_HPP__
#define __CWT_DOM_NODELIST_HPP__

#include <pfs/list.hpp>
#include <pfs/shared_ptr.hpp>

namespace cwt { namespace dom {

class node;
typedef pfs::shared_ptr<node> node_ptr;

class DLL_API nodelist : public pfs::list<node_ptr>
{
	typedef pfs::list<node_ptr> base_class;

public:
	nodelist() : base_class () {}

	node_ptr item(size_t index);
	size_t length () const { return base_class::size(); }
};

}} // cwt::dom

#endif /* __CWT_DOM_NODELIST_HPP__ */
