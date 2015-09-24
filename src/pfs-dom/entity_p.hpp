/**
 * @file entity_p.hpp
 * @author wladt
 * @date Dec 13, 2013
 */

#ifndef __PFS_DOM_ENTITY_P_HPP__
#define __PFS_DOM_ENTITY_P_HPP__

#include "pfs/dom/entity.hpp"

namespace pfs { namespace dom {

class node_impl;
class document_impl;

class DLL_API entity_impl : public node_impl
{
public:
    pfs::string _sys;
    pfs::string _pub;
    pfs::string _notationName;

public:
    entity_impl (document_impl *, node_impl * parent, const pfs::string & name
    		, const pfs::string & pub, const pfs::string & sys, const pfs::string & notation);
    entity_impl (entity_impl* n, bool deep);

    virtual node_impl * cloneNode (bool deep = true);
    virtual node::type nodeType () const { return node::EntityNode; }
};

}} // pfs::dom

#endif /* __DOM_CWT_ENTITY_P_HPP__ */
