/**
 * @file cdatasection_p.hpp
 * @author wladt
 * @date Dec 13, 2013
 */

#ifndef __PFS_CDATASECTION_P_HPP__
#define __PFS_CDATASECTION_P_HPP__

#include "pfs/dom/cdatasection.hpp"

namespace pfs { namespace dom {

class node_impl;
class text_impl;
class document_impl;

class DLL_API cdatasection_impl : public text_impl
{
public:
	cdatasection_impl (document_impl *, node_impl * parent, const pfs::string & val);
	cdatasection_impl (cdatasection_impl* n, bool deep);

    virtual node_impl * clone_node (bool deep = true);

    virtual node::type node_type () const
    {
    	return node::cdata_section_node;
    }
};

}} // pfs::dom

#endif /* __PFS_CDATASECTION_P_HPP__ */
