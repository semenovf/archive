/**
 * @file documentfragmant.hpp
 * @author wladt
 * @date Dec 2, 2013
 */

#ifndef __CWT_DOM_DOCFRAGMENT_HPP__
#define __CWT_DOM_DOCFRAGMENT_HPP__

#include <cwt/dom/node.hpp>

namespace cwt { namespace dom {

class DLL_API docfragment : public node
{
public:
	docfragment() : node() {}
};

}} // cwt::dom

#endif /* __CWT_DOM_DOCFRAGMENT_HPP__ */
