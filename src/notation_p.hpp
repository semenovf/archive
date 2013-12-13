/**
 * @file notation_p.hpp
 * @author wladt
 * @date Dec 13, 2013
 */

#ifndef __CWT_DOM_NOTATION_P_HPP__
#define __CWT_DOM_NOTATION_P_HPP__

#include "../include/cwt/dom/notation.hpp"

namespace cwt { namespace dom {

class node::impl;
class document::impl;


class notation::impl : public node::impl
{
public:
    notation::impl(document::impl*, node::impl* parent, const pfs::string& name,
                          const pfs::string& pub, const pfs::string& sys);
    notation::impl(notation::impl* n, bool deep);

    // Reimplemented from node::impl
    node::impl* cloneNode(bool deep = true);
    node::type nodeType() const { return node::NotationNode; }

    // Variables
    pfs::string m_sys;
    pfs::string m_pub;
};

}} // cwt::dom

#endif /* __CWT_DOM_NOTATION_P_HPP__ */
