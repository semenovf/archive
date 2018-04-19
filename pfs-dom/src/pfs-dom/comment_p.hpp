/**
 * @file comment_p.hpp
 * @author wladt
 * @date Dec 13, 2013
 */

#ifndef __PFS_DOM_COMMENT_P_HPP__
#define __PFS_DOM_COMMENT_P_HPP__

#include "pfs/dom/comment.hpp"

namespace pfs { namespace dom {

class node_impl;
class chardata_impl;
class document_impl;

class DLL_API comment_impl : public chardata_impl
{
public:
	comment_impl (document_impl * d, node_impl * parent, const pfs::string & val);
	comment_impl (comment_impl * n, bool deep);

	virtual node_impl * clone_node(bool deep = true);

    virtual node::type node_type() const
    {
    	return node::comment_node;
    }
};

}} // pfs::dom

#endif /* __DOM_CWT_COMMENT_P_HPP__ */
