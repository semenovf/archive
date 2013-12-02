/**
 * @file entityref.hpp
 * @author wladt
 * @date Dec 2, 2013
 */

#ifndef __CWT_DOM_ENTITYREF_HPP__
#define __CWT_DOM_ENTITYREF_HPP__

#include <cwt/dom/node.hpp>

namespace cwt { namespace dom {

class entityref : public node {
public:
	entityref () : node () {}
};

}} // cwt::dom

#endif /* __CWT_DOM_ENTITYREF_HPP__ */
