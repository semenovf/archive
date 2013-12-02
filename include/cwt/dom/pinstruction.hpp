/**
 * @file pinstruction.hpp
 * @author wladt
 * @date Dec 2, 2013
 */

#ifndef __CWT_DOM_PINSTRUCTION_HPP__
#define __CWT_DOM_PINSTRUCTION_HPP__

#include <cwt/dom/node.hpp>

namespace cwt { namespace dom {

class pinstruction : public node
{
	pfs::string _target; // readonly
	pfs::string _data;

public:
	pinstruction () : node () {}

	const pfs::string & target () { return _target; }
	const pfs::string & data () const { return _data; }
	void setData (const pfs::string & data); // raises(DOMException) on setting

};

}} // cwt

#endif /* __CWT_DOM_PINSTRUCTION_HPP__ */
