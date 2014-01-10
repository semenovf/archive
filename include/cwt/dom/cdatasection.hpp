/**
 * @file cdatasection.hpp
 * @author wladt
 * @date Dec 2, 2013
 */

#ifndef __CWT_DOM_CDATASECTION_HPP__
#define __CWT_DOM_CDATASECTION_HPP__

#include <cwt/dom/text.hpp>

namespace cwt { namespace dom {

class text;
class document;
class cdatasection_impl;

class DLL_API cdatasection : public text
{
	friend class document;

protected:
	cdatasection (cdatasection_impl *);

public:
	cdatasection ();
	cdatasection (const cdatasection & other);
	cdatasection & operator = (const cdatasection & other);
};

}} // cwt::dom

#endif /* __CWT_DOM_CDATASECTION_HPP__ */
