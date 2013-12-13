/**
 * @file entity_p.hpp
 * @author wladt
 * @date Dec 13, 2013
 */

#ifndef __CWT_DOM_ENTITY_P_HPP__
#define __CWT_DOM_ENTITY_P_HPP__

#include "../include/cwt/dom/entity.hpp"

namespace cwt { namespace dom {

class entity::impl : public node::impl
{
public:
    entity::impl(document::impl*, node::impl* parent, const pfs::string& name,
                        const pfs::string& pub, const pfs::string& sys, const pfs::string& notation);
    entity::impl(entity::impl* n, bool deep);

    // Reimplemented from node::impl
    node::impl* cloneNode(bool deep = true);
    node::type nodeType() const { return node::EntityNode; }

    // Variables
    pfs::string m_sys;
    pfs::string m_pub;
    pfs::string m_notationName;
};

}} // cwt::dom

#endif /* __DOM_CWT_ENTITY_P_HPP__ */
