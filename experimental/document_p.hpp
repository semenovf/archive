/**
 * @file document_p.hpp
 * @author wladt
 * @date Dec 10, 2013
 */

#ifndef __CWT_DOM_DOCUMENT_P_HPP__
#define __CWT_DOM_DOCUMENT_P_HPP__

#include "../include/cwt/dom/document.hpp"

namespace cwt { namespace dom {

class node::impl;
class dom_implementation;
class document_type::impl;

class document::impl : public node::impl
{
public:
    pfs::shared_ptr<dom_implementation> _domImpl;
    pfs::shared_ptr<document_type::impl> _docType;
	long_t _timestamp;

public:
	virtual ~impl () {}
//	impl ()
//	    : node::impl()
//	    , impl(new QDomImplementationPrivate),
//	    _timestamp(1)
//	{
//	    type = new QDomDocumentTypePrivate(this, this);
//	    type->ref.deref();
//
//	    name = QLatin1String("#document");
//	}


	virtual node::type nodeType () const { return node::DocumentNode; }
};

}} // cwt

#endif /* __CWT_DOM_DOCUMENT_P_HPP__ */
