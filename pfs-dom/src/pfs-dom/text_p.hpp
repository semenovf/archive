/**
 * @file text_p.hpp
 * @author wladt
 * @date Dec 13, 2013
 */

#ifndef __PFS_DOM_TEXT_P_HPP__
#define __PFS_DOM_TEXT_P_HPP__

#include "pfs/dom/text.hpp"

namespace pfs { namespace dom {

class node_impl;
class chardata_impl;
class document_impl;

class DLL_API text_impl : public chardata_impl
{
public:
	text_impl (document_impl *, node_impl * parent, const pfs::string & val);
	text_impl (text_impl * n, bool deep);

	text_impl * split_text (size_t offset);

    virtual node_impl * clone_node (bool deep = true);

    virtual node::type node_type () const
    {
    	return node::text_node;
    }
};

}} // pfs::dom


#endif /* __PFS_DOM_TEXT_P_HPP__ */
