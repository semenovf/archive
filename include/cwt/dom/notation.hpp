/**
 * @file notation.hpp
 * @author wladt
 * @date Dec 2, 2013
 */

#ifndef __CWT_DOM_NOTATION_HPP__
#define __CWT_DOM_NOTATION_HPP__

#include <cwt/dom/node.hpp>

namespace cwt { namespace dom {

class notation_impl;

class DLL_API notation : public node
{
protected:
	notation (notation_impl *);

public:
	notation ();
	notation (const notation & other);
	notation & operator = (const notation & other);

	pfs::string publicId () const;
	pfs::string systemId () const;
};

}} // cwt::dom

#endif /* __CWT_DOM_NOTATION_HPP__ */
