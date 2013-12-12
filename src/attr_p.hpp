/**
 * @file attr_p.hpp
 * @author wladt
 * @date Dec 12, 2013
 */

#ifndef __CWT_DOM_ATTR_P_HPP__
#define __CWT_DOM_ATTR_P_HPP__

#include "../include/cwt/dom/attr.hpp"

namespace cwt { namespace dom {

class node::impl;
class element::impl;
class document;

class attr::impl : public node::impl
{
public:
	bool _specified;

public:
	virtual ~impl () {}
	impl (pfs::shared_ptr<node::impl> & parent, const pfs::string & name);
	impl (pfs::shared_ptr<node::impl> & parent, const pfs::string & namespaceURI, const pfs::string & qualifiedName);

	virtual node::type nodeType () const { return node::AttributeNode; }
};

}} // cwt::dom

#endif /* __CWT_DOM_ATTR_P_HPP__ */
