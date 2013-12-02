/**
 * @file text.hpp
 * @author wladt
 * @date Dec 2, 2013
 */

#ifndef __CWT_DOM_TEXT_HPP__
#define __CWT_DOM_TEXT_HPP__

#include <cwt/dom/chardata.hpp>

namespace cwt { namespace dom {

class text : public chardata
{
public:
	text () : chardata () {}
	text * splitText (size_t offset);  // raises(DOMException);
};

}} // cwt

#endif /* __CWT_DOM_TEXT_HPP__ */
