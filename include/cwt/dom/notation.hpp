/**
 * @file notation.hpp
 * @author wladt
 * @date Dec 2, 2013
 */

#ifndef __CWT_DOM_NOTATION_HPP__
#define __CWT_DOM_NOTATION_HPP__

#include <cwt/dom/node.hpp>

namespace cwt { namespace dom {

class notation : public node
{
	pfs::string _publicId; // readonly
	pfs::string _systemId; // readonly

public:
	notation () : node () {}
	const pfs::string & publicId () const { return _publicId; }
	const pfs::string & systemId () const { return _systemId; }
};

}} // cwt::dom

#endif /* __CWT_DOM_NOTATION_HPP__ */
