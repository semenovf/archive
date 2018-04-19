/**
 * @file docfragment_p.hpp
 * @author wladt
 * @date Dec 13, 2013
 */

#ifndef __PFS_DOM_DOCFRAGMENT_P_HPP__
#define __PFS_DOM_DOCFRAGMENT_P_HPP__

#include "pfs/dom/docfragment.hpp"

namespace pfs { namespace dom {

class node_impl;
class document_impl;

class DLL_API document_fragment_impl : public node_impl
{
public:
	document_fragment_impl (document_impl * d, node_impl * p);
	document_fragment_impl (node_impl * n, bool deep);

    virtual node_impl* clone_node (bool deep = true);

    virtual node::type node_type() const
    {
    	return node::document_fragment_node;
    }
};

}} // pfs::dom

#endif /* __PFS_DOM_DOCFRAGMENT_P_HPP__ */
