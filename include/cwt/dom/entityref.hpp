/**
 * @file entityref.hpp
 * @author wladt
 * @date Dec 2, 2013
 */

#ifndef __CWT_DOM_ENTITYREF_HPP__
#define __CWT_DOM_ENTITYREF_HPP__

#include <cwt/dom/node.hpp>

namespace cwt { namespace dom {

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

}} // cwt::dom

#endif /* __CWT_DOM_ENTITYREF_HPP__ */
