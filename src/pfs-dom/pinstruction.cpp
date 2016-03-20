/**
 * @file pinstruction.cpp
 * @author wladt
 * @date Jan 10, 2014
 */

#include "node_p.hpp"
#include "nodelist_p.hpp"
#include "namednodemap_p.hpp"
#include "attr_p.hpp"
#include "element_p.hpp"
#include "pinstruction_p.hpp"
#include "document_p.hpp"

namespace pfs { namespace dom {

processing_instruction_impl::processing_instruction_impl (document_impl * d
		, node_impl * parent
		, const pfs::string & target
		, const pfs::string & data)
    : node_impl(d, parent)
{
    _name  = target;
    _value = data;
}

processing_instruction_impl::processing_instruction_impl (processing_instruction_impl * n, bool deep) : node_impl(n, deep)
{}


node_impl * processing_instruction_impl::clone_node (bool deep)
{
    node_impl * p = new processing_instruction_impl(this, deep);
    p->ref.deref();
    return p;
}


processing_instruction::processing_instruction () : node()
{}

processing_instruction::processing_instruction (const processing_instruction & other) : node(other)
{}

processing_instruction::processing_instruction (processing_instruction_impl * n) : node(n)
{}

processing_instruction & processing_instruction::operator = (const processing_instruction & other)
{
    return (processing_instruction &) node::operator = (other);
}

pfs::string processing_instruction::target() const
{
	return _d
			? _d->node_name()
			: pfs::string();
}

pfs::string processing_instruction::data() const
{
	return _d
			? _d->node_value()
			: pfs::string();
}

void processing_instruction::set_data (const pfs::string & d)
{
	if (_d) {
		_d->set_node_value(d);
	}
}

}} // pfs::dom


