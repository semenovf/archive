/**
 * @file entity.hpp
 * @author wladt
 * @date Dec 2, 2013
 */

#ifndef __CWT_DOM_ENTITY_HPP__
#define __CWT_DOM_ENTITY_HPP__

#include <pfs.hpp>

namespace cwt { namespace dom {

class node;
class entity_impl;

class DLL_API entity : public node
{
//	pfs::string _publicId;     // readonly
//	pfs::string _systemId;     // readonly
//	pfs::string _notationName; // readonly
//
//public:
//	entity () : node () {}
//
//	const pfs::string & publicId () const { return _publicId; }
//	const pfs::string & systemId () const { return _systemId; }
//	const pfs::string & notationName () const { return _notationName; }
};

}} // cwt::dom

#endif /* __CWT_DOM_ENTITY_HPP__ */
