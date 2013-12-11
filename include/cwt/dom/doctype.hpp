/**
 * @file documenttype.hpp
 * @author wladt
 * @date Dec 2, 2013
 */

#ifndef __CWT_DOM_DOCTYPE_HPP__
#define __CWT_DOM_DOCTYPE_HPP__

#include <cwt/dom/node.hpp>
#include <pfs/shared_ptr.hpp>

namespace cwt { namespace dom {

class namednodemap;

class doctype : public node
{
	class impl;
	impl * _pimpl;
public:
	doctype ();

	const pfs::string & name () const           { return _name; }
	const namednodemap & entities () const      { return _entities; }
	const namednodemap & notations () const     { return _notations; }
	const pfs::string & publicId () const       { return _publicId; }
	const pfs::string & systemId () const       { return _systemId; }
	const pfs::string & internalSubset () const { return _internalSubset; }
};

}} // cwt::dom

#endif /* __CWT_DOM_DOCTYPE_HPP__ */
