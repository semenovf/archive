/**
 * @file pinstruction_p.hpp
 * @author wladt
 * @date Dec 13, 2013
 */

#ifndef __CWT_DOM_PINSTRUCTION_P_HPP__
#define __CWT_DOM_PINSTRUCTION_P_HPP__

#include "../include/cwt/dom/pinstruction.hpp"

namespace cwt { namespace dom {

class processing_instruction::impl : public node::impl
{
public:
    processing_instruction::impl(document::impl*, node::impl* parent, const pfs::string& target,
                                       const pfs::string& data);
    processing_instruction::impl(processing_instruction::impl* n, bool deep);

    // Reimplemented from node::impl
    node::impl * cloneNode(bool deep = true);
    node::type nodeType() const { return node::ProcessingInstructionNode; }
};

}} // cwt::dom

#endif /* __CWT_DOM_PINSTRUCTION_P_HPP__ */
