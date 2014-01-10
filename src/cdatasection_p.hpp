/**
 * @file cdatasection_p.hpp
 * @author wladt
 * @date Dec 13, 2013
 */

#ifndef __CWT_CDATASECTION_P_HPP__
#define __CWT_CDATASECTION_P_HPP__

#include "../include/cwt/dom/cdatasection.hpp"

namespace cwt { namespace dom {

class node_impl;
class text_impl;
class document_impl;

class cdatasection_impl : public text_impl
{
public:
	cdatasection_impl (document_impl *, node_impl * parent, const pfs::string & val);
	cdatasection_impl (cdatasection_impl* n, bool deep);

    virtual node_impl * cloneNode (bool deep = true);
    virtual node::type nodeType () const { return node::CDATASectionNode; }
};

}} // cwt::dom

#endif /* __CWT_CDATASECTION_P_HPP__ */
