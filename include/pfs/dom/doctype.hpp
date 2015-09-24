/**
 * @file documenttype.hpp
 * @author wladt
 * @date Dec 2, 2013
 */

#ifndef __PFS_DOM_DOCTYPE_HPP__
#define __PFS_DOM_DOCTYPE_HPP__

#include <pfs/string.hpp>

namespace pfs { namespace dom {

class node;
class namednodemap;
class document;
class dom_implementation;
class doctype_impl;

class DLL_API doctype : public node
{
	friend class document;
	friend class dom_implementation;

protected:
	doctype (doctype_impl * n);

public:
	doctype () : node () {}
	doctype (const doctype & other) : node (other) {}
	doctype & operator = (const doctype & other)
	{
	    return static_cast<doctype &>(node::operator = (other));
	}

	pfs::string name () const;
	namednodemap entities () const;
	namednodemap notations () const;
	pfs::string publicId () const;
	pfs::string systemId () const;
	pfs::string internalSubset () const;
};

}} // pfs::dom

#endif /* __PFS_DOM_DOCTYPE_HPP__ */
