/**
 * @file element_p.hpp
 * @author wladt
 * @date Dec 12, 2013
 */

#ifndef __CWT_ELEMENT_P_HPP__
#define __CWT_ELEMENT_P_HPP__

#include "../include/cwt/dom/element.hpp"

namespace cwt { namespace dom {

class namednodemap;

class element::impl : public node::impl
{
public:
	namednodemap _attr;

public:
	virtual ~impl () {}

	virtual node::type nodeType () const { return node::ElementNode; }
	size_t attrCount () const { return _attr._pimpl->_map.size(); }
};

}} // cwt::dom

#endif /* __CWT_ELEMENT_P_HPP__ */
