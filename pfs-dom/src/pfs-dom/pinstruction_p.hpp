/**
 * @file pinstruction_p.hpp
 * @author wladt
 * @date Dec 13, 2013
 */

#ifndef __PFS_DOM_PINSTRUCTION_P_HPP__
#define __PFS_DOM_PINSTRUCTION_P_HPP__

#include "pfs/dom/pinstruction.hpp"

namespace pfs { namespace dom {

class node_impl;
class document_impl;

class DLL_API processing_instruction_impl : public node_impl
{
public:
    processing_instruction_impl (document_impl *, node_impl * parent, const pfs::string & target, const pfs::string & data);
    processing_instruction_impl (processing_instruction_impl * n, bool deep);

    virtual node_impl * clone_node (bool deep = true);

    virtual node::type node_type () const
    {
    	return node::processing_instruction_node;
    }
};

}} // pfs::dom

#endif /* __PFS_DOM_PINSTRUCTION_P_HPP__ */
