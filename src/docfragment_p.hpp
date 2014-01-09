/**
 * @file docfragment_p.hpp
 * @author wladt
 * @date Dec 13, 2013
 */

#ifndef __CWT_DOM_DOCFRAGMENT_P_HPP__
#define __CWT_DOM_DOCFRAGMENT_P_HPP__

#include "../include/cwt/dom/docfragment.hpp"

namespace cwt { namespace dom {

class node_impl;
class document_impl;

class document_fragment_impl : public node_impl
{
public:
	document_fragment_impl (document_impl * d, node_impl * p);
	document_fragment_impl (node_impl * n, bool deep);

    virtual node_impl* cloneNode (bool deep = true);
    virtual node::type nodeType() const { return node::DocumentFragmentNode; }
};

}} // cwt::dom

#endif /* __CWT_DOM_DOCFRAGMENT_P_HPP__ */
