/**
 * @file pinstruction.hpp
 * @author wladt
 * @date Dec 2, 2013
 */

#ifndef __PFS_DOM_PROCESSING_INSTRUCTION_HPP__
#define __PFS_DOM_PROCESSING_INSTRUCTION_HPP__

#include <pfs/dom/node.hpp>

namespace pfs { namespace dom {

class processing_instruction_impl;
class document;

class processing_instruction : public node
{
	friend class document;

protected:
	processing_instruction (processing_instruction_impl *);

public:
	processing_instruction ();
	processing_instruction (const processing_instruction & other);
	processing_instruction & operator = (const processing_instruction & other);

	pfs::string target () const;
	pfs::string data () const;
	void set_data (const pfs::string & d);
};

}} // pfs

#endif /* __PFS_DOM_PROCESSING_INSTRUCTION_HPP__ */
