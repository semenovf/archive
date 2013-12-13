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
class dom_implementation;

class document_type : public node
{
	friend class dom_implementation;

	class impl;
	pfs::shared_ptr<impl> _pimpl;

protected:
	document_type(); // deny default constructor
	explicit document_type (const pfs::string & qualifiedName
			, const pfs::string & publicId
			, const pfs::string & systemId);

public:
//
//	const pfs::string & name () const           { return _name; }
//	const namednodemap & entities () const      { return _entities; }
//	const namednodemap & notations () const     { return _notations; }
//	const pfs::string & publicId () const       { return _publicId; }
//	const pfs::string & systemId () const       { return _systemId; }
//	const pfs::string & internalSubset () const { return _internalSubset; }
};

}} // cwt::dom

#endif /* __CWT_DOM_DOCTYPE_HPP__ */
