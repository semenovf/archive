/**
 * @file pinstruction.hpp
 * @author wladt
 * @date Dec 2, 2013
 */

#ifndef __PFS_DOM_PINSTRUCTION_HPP__
#define __PFS_DOM_PINSTRUCTION_HPP__

#include <pfs/dom/node.hpp>

namespace pfs { namespace dom {

class pinstruction_impl;
class document;

class pinstruction : public node
{
	friend class document;

protected:
	pinstruction (pinstruction_impl *);

public:
	pinstruction ();
	pinstruction (const pinstruction & other);
	pinstruction & operator = (const pinstruction & other);

	pfs::string target () const;
	pfs::string data () const;
	void setData (const pfs::string & d);
};

}} // pfs

#endif /* __PFS_DOM_PINSTRUCTION_HPP__ */
