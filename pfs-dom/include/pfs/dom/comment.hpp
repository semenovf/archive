/**
 * @file comment.hpp
 * @author wladt
 * @date Dec 2, 2013
 */

#ifndef __PFS_DOM_COMMENT_HPP__
#define __PFS_DOM_COMMENT_HPP__

#include <pfs.hpp>

namespace pfs { namespace dom {

class document;
class chardata;
class comment_impl;

class DLL_API comment : public chardata
{
	friend class document;

protected:
	comment (comment_impl *);

public:
	comment ();
	comment (const comment & other);
	comment & operator = (const comment & other);
};

}} // pfs

#endif /* __PFS_DOM_COMMENT_HPP__ */
