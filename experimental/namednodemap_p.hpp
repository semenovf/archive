/**
 * @file namednodemap_p.hpp
 * @author wladt
 * @date Dec 10, 2013
 */

#ifndef __CWT_DOM_NAMEDNODEMAP_P_HPP__
#define __CWT_DOM_NAMEDNODEMAP_P_HPP__

#include "../include/cwt/dom/namednodemap.hpp"
#include <pfs/map.hpp>

namespace cwt { namespace dom {

class node;

class namednodemap::impl
{
public:
	typedef pfs::shared_ptr<node::impl> item_type;
	typedef pfs::map<pfs::string, item_type> map_type;

	typedef map_type::iterator iterator;
	typedef map_type::const_iterator const_iterator;

    map_type _map;

public:
};

}} // cwt::dom

#endif /* __CWT_DOM_NAMEDNODEMAP_P_HPP__ */
