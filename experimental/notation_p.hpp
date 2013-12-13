/**
 * @file notation_p.hpp
 * @author wladt
 * @date Dec 13, 2013
 */

#ifndef __CWT_DOM_NOTATION_P_HPP__
#define __CWT_DOM_NOTATION_P_HPP__

#include "../include/cwt/dom/notation.hpp"

namespace cwt { namespace dom {

class node::impl;

class notation::impl : public node::impl
{
public:
	pfs::string _publicId; // readonly
	pfs::string _systemId; // readonly

public:
	impl (const pfs::string & publicId, const pfs::string & systemId)
		: _publicId(publicId)
		, _systemId(systemId)
	{ ;	}

	impl (const impl & other, bool deep = true)
		: node::impl(other, deep)
		, _publicId(other._publicId)
		, _systemId(other._systemId)
	{ ;	}

	virtual ~impl () {}

	virtual node::type nodeType () const { return node::NotationNode; }
};

}} // cwt::dom

#endif /* __CWT_DOM_NOTATION_P_HPP__ */
