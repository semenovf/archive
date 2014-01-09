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

protected:
	entity (entity_impl *);

public:
	entity ();
	entity (const entity & other);
	entity & operator = (const entity & other);

	pfs::string publicId () const;
	pfs::string systemId () const;
	pfs::string notationName () const;
};

}} // cwt::dom

#endif /* __CWT_DOM_ENTITY_HPP__ */
