/**
 * @file doctype_p.hpp
 * @author wladt
 * @date Dec 10, 2013
 */

#ifndef __CWT_DOM_DOCTYPE_P_HPP__
#define __CWT_DOM_DOCTYPE_P_HPP__

#include "../include/cwt/dom/doctype.hpp"

namespace cwt { namespace dom {

class node::impl

class doctype::impl : public node::impl
{
public:
	pfs::shared_ptr<namednodemap> _entities;
	pfs::shared_ptr<namednodemap> _notations;
	pfs::string    _publicId;
	pfs::string    _systemId;
	pfs::string    _internalSubset;

public:
	impl ();
	virtual ~impl () {}

	virtual node::type nodeType () const { return node::DocumentTypeNode; }
}

}} // cwt::dom

#endif /* __CWT_DOM_DOCTYPE_P_HPP__ */
