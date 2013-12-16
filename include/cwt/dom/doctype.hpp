/**
 * @file documenttype.hpp
 * @author wladt
 * @date Dec 2, 2013
 */

#ifndef __CWT_DOM_DOCTYPE_HPP__
#define __CWT_DOM_DOCTYPE_HPP__

#include <pfs/string.hpp>

namespace cwt { namespace dom {

class document_type_impl;
class node;
class namednodemap;
class dom_implementation;

class DLL_API document_type : public node
{
	friend class dom_implementation;

protected:
	document_type (document_type_impl * nimpl);

public:
	document_type () : node () {}
	document_type (const document_type & other) : node (other) {}
	document_type & operator= (const document_type & other)
	{
	    return static_cast<document_type &>(node::operator = (other));
	}

	pfs::string name () const;
	namednodemap entities () const;
	namednodemap notations () const;
	pfs::string publicId () const;
	pfs::string systemId () const;
	pfs::string internalSubset () const;
};

}} // cwt::dom

#endif /* __CWT_DOM_DOCTYPE_HPP__ */
