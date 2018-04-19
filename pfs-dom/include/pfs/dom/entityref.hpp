/**
 * @file entityref.hpp
 * @author wladt
 * @date Dec 2, 2013
 */

#ifndef __PFS_DOM_ENTITYREF_HPP__
#define __PFS_DOM_ENTITYREF_HPP__

#include <pfs/dom/node.hpp>

namespace pfs { namespace dom {

class document;
class entityref_impl;

class DLL_API entityref : public node
{
	friend class document;

protected:
	entityref (entityref_impl *);

public:
	entityref ();
	entityref (const entityref & other);
	entityref & operator = (const entityref & other);
};

}} // pfs::dom

#endif /* __PFS_DOM_ENTITYREF_HPP__ */
