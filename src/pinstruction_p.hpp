/**
 * @file pinstruction_p.hpp
 * @author wladt
 * @date Dec 13, 2013
 */

#ifndef __CWT_DOM_PINSTRUCTION_P_HPP__
#define __CWT_DOM_PINSTRUCTION_P_HPP__

#include "../include/cwt/dom/pinstruction.hpp"

namespace cwt { namespace dom {

class node_impl;
class document_impl;

class DLL_API pinstruction_impl : public node_impl
{
public:
    pinstruction_impl (document_impl *, node_impl * parent, const pfs::string & target, const pfs::string & data);
    pinstruction_impl (pinstruction_impl * n, bool deep);

    virtual node_impl * cloneNode (bool deep = true);
    virtual node::type nodeType () const { return node::ProcessingInstructionNode; }
};

}} // cwt::dom

#endif /* __CWT_DOM_PINSTRUCTION_P_HPP__ */
