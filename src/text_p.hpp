/**
 * @file text_p.hpp
 * @author wladt
 * @date Dec 13, 2013
 */

#ifndef __CWT_DOM_TEXT_P_HPP__
#define __CWT_DOM_TEXT_P_HPP__

#include "../include/cwt/dom/text.hpp"

namespace cwt { namespace dom {

class node::impl;
class chardata::impl;
class document::impl;

class text::impl : public chardata::impl
{
public:
    impl(QDomDocumentPrivate*, QDomNodePrivate* parent, const pfs::string& val);
    impl(impl* n, bool deep);

    impl* splitText(int offset);

    // Reimplemented from QDomNodePrivate
    node::impl * cloneNode(bool deep = true);
    node::type nodeType() const { return node::TextNode; }
};

}} // cwt::dom


#endif /* __CWT_DOM_TEXT_P_HPP__ */
