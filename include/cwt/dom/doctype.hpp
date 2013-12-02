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
	pfs::string _name;           // readonly
	pfs::shared_ptr<namednodemap>
				_entities;       // readonly
	pfs::shared_ptr<namednodemap>
				_notations;      // readonly
	pfs::string _publicId;       // readonly
	pfs::string _systemId;       // readonly
	pfs::string _internalSubset; // readonly

public:
	doctype ();

	const pfs::string & name ()       { return _name; }
	const namednodemap * entities ()  { return _entities; }
	const namednodemap * notations () { return _notations; }
	const pfs::string & publicId ()   { return _publicId; }
	const pfs::string & systemId ()   { return _systemId; }
	const pfs::string & internalSubset () { return _internalSubset; }
};

}} // cwt::dom

#endif /* __CWT_DOM_DOCTYPE_HPP__ */
