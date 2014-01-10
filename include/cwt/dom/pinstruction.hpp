/**
 * @file pinstruction.hpp
 * @author wladt
 * @date Dec 2, 2013
 */

#ifndef __CWT_DOM_PINSTRUCTION_HPP__
#define __CWT_DOM_PINSTRUCTION_HPP__

#include <cwt/dom/node.hpp>

namespace cwt { namespace dom {

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

}} // cwt

#endif /* __CWT_DOM_PINSTRUCTION_HPP__ */
