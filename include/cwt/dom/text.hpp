/**
 * @file text.hpp
 * @author wladt
 * @date Dec 2, 2013
 */

#ifndef __CWT_DOM_TEXT_HPP__
#define __CWT_DOM_TEXT_HPP__

#include <cwt/dom/chardata.hpp>

namespace cwt { namespace dom {

class text_impl;

class text : public chardata
{
protected:
	text (text_impl *);

public:
	text ();
	text (const chardata & other);
	text & operator = (const text & other);

	text splitText (size_t offset);
};

}} // cwt

#endif /* __CWT_DOM_TEXT_HPP__ */
