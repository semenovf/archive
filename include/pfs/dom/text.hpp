/**
 * @file text.hpp
 * @author wladt
 * @date Dec 2, 2013
 */

#ifndef __PFS_DOM_TEXT_HPP__
#define __PFS_DOM_TEXT_HPP__

#include <pfs/dom/chardata.hpp>

namespace pfs { namespace dom {

class document;
class text_impl;

class text : public chardata
{
	friend class document;

protected:
	text (text_impl *);

public:
	text ();
	text (const text & other);
	text & operator = (const text & other);

	text splitText (size_t offset);
};

}} // pfs

#endif /* __PFS_DOM_TEXT_HPP__ */
