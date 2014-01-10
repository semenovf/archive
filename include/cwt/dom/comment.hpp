/**
 * @file comment.hpp
 * @author wladt
 * @date Dec 2, 2013
 */

#ifndef __CWT_DOM_COMMENT_HPP__
#define __CWT_DOM_COMMENT_HPP__

#include <pfs.hpp>

namespace cwt { namespace dom {

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

}} // cwt

#endif /* __CWT_DOM_COMMENT_HPP__ */
