/*
 * attr.hpp
 *
 *  Created on: Nov 20, 2013
 *      Author: wladt
 */

#ifndef __CWT_DOM_ATTR_HPP__
#define __CWT_DOM_ATTR_HPP__

namespace cwt { namespace dom {

class Attr : Node {
  readonly attribute DOMString        name;
  readonly attribute boolean          specified;
           attribute DOMString        value;
                                        // raises(DOMException) on setting

  // Introduced in DOM Level 2:
  readonly attribute Element          ownerElement;
};

}} // cwt::dom

#endif /* __CWT_DOM_ATTR_HPP__ */
