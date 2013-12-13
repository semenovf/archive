/**
 * @file notation.hpp
 * @author wladt
 * @date Dec 2, 2013
 */

#ifndef __CWT_DOM_NOTATION_HPP__
#define __CWT_DOM_NOTATION_HPP__

#include <pfs/shared_ptr.hpp>
#include <cwt/dom/node.hpp>

namespace cwt { namespace dom {

class notation : public node
{
	class impl;
	pfs::shared_ptr<impl> _pimpl;

protected:
	notation (); // deny

public:
	const pfs::string & publicId () const;
	const pfs::string & systemId () const;
};

}} // cwt::dom

#endif /* __CWT_DOM_NOTATION_HPP__ */
