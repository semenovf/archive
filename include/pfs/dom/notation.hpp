/**
 * @file notation.hpp
 * @author wladt
 * @date Dec 2, 2013
 */

#ifndef __PFS_DOM_NOTATION_HPP__
#define __PFS_DOM_NOTATION_HPP__

#include <pfs/dom/node.hpp>

namespace pfs { namespace dom {

class notation_impl;

class DLL_API notation : public node
{
protected:
	notation (notation_impl *);

public:
	notation ();
	notation (const notation & other);
	notation & operator = (const notation & other);

	pfs::string public_id () const;
	pfs::string system_id () const;
};

}} // pfs::dom

#endif /* __PFS_DOM_NOTATION_HPP__ */
