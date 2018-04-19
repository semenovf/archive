/**
 * @file entity.hpp
 * @author wladt
 * @date Dec 2, 2013
 */

#ifndef __PFS_DOM_ENTITY_HPP__
#define __PFS_DOM_ENTITY_HPP__

#include <pfs/dom/node.hpp>

namespace pfs { namespace dom {

class entity_impl;

class DLL_API entity : public node
{

protected:
	entity (entity_impl *);

public:
	entity ();
	entity (const entity & other);
	entity & operator = (const entity & other);

	pfs::string public_id () const;
	pfs::string system_id () const;
	pfs::string notation_name () const;
};

}} // pfs::dom

#endif /* __PFS_DOM_ENTITY_HPP__ */
