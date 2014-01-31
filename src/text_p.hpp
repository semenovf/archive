/**
 * @file text_p.hpp
 * @author wladt
 * @date Dec 13, 2013
 */

#ifndef __CWT_DOM_TEXT_P_HPP__
#define __CWT_DOM_TEXT_P_HPP__

#include "../include/cwt/dom/text.hpp"

namespace cwt { namespace dom {

class node_impl;
class chardata_impl;
class document_impl;

class DLL_API text_impl : public chardata_impl
{
public:
	text_impl (document_impl *, node_impl * parent, const pfs::string & val);
	text_impl (text_impl * n, bool deep);

	text_impl * splitText (size_t offset);

    virtual node_impl * cloneNode (bool deep = true);
    virtual node::type nodeType () const { return node::TextNode; }
};

}} // cwt::dom


#endif /* __CWT_DOM_TEXT_P_HPP__ */
