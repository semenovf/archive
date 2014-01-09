/**
 * @file comment.hpp
 * @author wladt
 * @date Dec 2, 2013
 */

#ifndef __CWT_DOM_COMMENT_HPP__
#define __CWT_DOM_COMMENT_HPP__

#include <pfs.hpp>

namespace cwt { namespace dom {

class comment_impl;
class node;
class chardata;

class DLL_API comment : public chardata
{
protected:
	comment (comment_impl *);

public:
	comment ();
	comment (const comment & other);
	comment & operator = (const comment & other);
};

}} // cwt

#endif /* __CWT_DOM_COMMENT_HPP__ */
