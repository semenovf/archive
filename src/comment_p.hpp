/**
 * @file comment_p.hpp
 * @author wladt
 * @date Dec 13, 2013
 */

#ifndef __CWT_DOM_COMMENT_P_HPP__
#define __CWT_DOM_COMMENT_P_HPP__

#include "../include/cwt/dom/comment.hpp"

namespace cwt { namespace dom {

class node::impl;
class document::impl;

class comment::impl : public QDomCharacterDataPrivate
{
public:
    impl(document::impl *, node::impl * parent, const pfs::string & val);
    impl(impl* n, bool deep);

    // Reimplemented from QDomNodePrivate
    node::impl * cloneNode(bool deep = true);
    node::type nodeType() const { return node::CommentNode; }
};

}} // cwt::dom

#endif /* __DOM_CWT_COMMENT_P_HPP__ */
