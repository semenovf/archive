/**
 * @file documentfragmant.hpp
 * @author wladt
 * @date Dec 2, 2013
 */

#ifndef __PFS_DOM_DOCFRAGMENT_HPP__
#define __PFS_DOM_DOCFRAGMENT_HPP__

#include <pfs.hpp>

namespace pfs { namespace dom {

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

}} // pfs::dom

#endif /* __PFS_DOM_DOCFRAGMENT_HPP__ */
