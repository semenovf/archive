/**
 * @file domimpl_p.hpp
 * @author wladt
 * @date Dec 13, 2013
 */

#ifndef __PFS_DOM_DOMIMPL_P_HPP__
#define __PFS_DOM_DOMIMPL_P_HPP__

#include "pfs/dom/domimpl.hpp"
#include <pfs/atomic.hpp>

namespace pfs { namespace dom {

class DLL_API dom_implementation_impl
{
public:
	pfs::atomic_int ref;

public:
	dom_implementation_impl () {}
	virtual ~ dom_implementation_impl () {}
	virtual dom_implementation_impl * clone ();

	bool hasFeature (const pfs::string & feature, const pfs::string & version = pfs::string()) const;
	virtual pfs::string idName () const { return _l1("id"); }
};

}} // pfs::dom

#endif /* __PFS_DOM_DOMIMPL_P_HPP__ */
