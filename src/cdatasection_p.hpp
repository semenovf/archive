/**
 * @file cdatasection_p.hpp
 * @author wladt
 * @date Dec 13, 2013
 */

#ifndef __CWT_CDATASECTION_P_HPP__
#define __CWT_CDATASECTION_P_HPP__

#include "../include/cwt/dom/cdatasection.hpp"

namespace cwt { namespace dom {

class node::impl;
class document::impl;

class cdatasection::impl : public text::impl
{
public:
    impl (document::impl*, node::impl* parent, const pfs::string& val);
    impl (cdatasection::impl* n, bool deep);

    // Reimplemented from node::impl
    node::impl* cloneNode(bool deep = true);
    node::type nodeType() const { return node::CDATASectionNode; }
};

}} // cwt::dom

#endif /* __CWT_CDATASECTION_P_HPP__ */
