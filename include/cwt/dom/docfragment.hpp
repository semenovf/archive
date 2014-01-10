/**
 * @file documentfragmant.hpp
 * @author wladt
 * @date Dec 2, 2013
 */

#ifndef __CWT_DOM_DOCFRAGMENT_HPP__
#define __CWT_DOM_DOCFRAGMENT_HPP__

#include <pfs.hpp>

namespace cwt { namespace dom {

class node;
class document;
class document_fragment_impl;

class DLL_API document_fragment : public node
{
	friend class document;

protected:
	document_fragment (document_fragment_impl *);

public:
	document_fragment ();
	document_fragment (const document_fragment & other);
	document_fragment & operator = (const document_fragment & other);
};

}} // cwt::dom

#endif /* __CWT_DOM_DOCFRAGMENT_HPP__ */
