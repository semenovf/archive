/**
 * @file domimpl.hpp
 * @author wladt
 * @date Dec 2, 2013
 */

#ifndef __PFS_DOM_DOMIMPL_HPP__
#define __PFS_DOM_DOMIMPL_HPP__

#include <pfs/string.hpp>

namespace pfs { namespace dom {

class document;
class doctype;
class dom_implementation_impl;

class DLL_API dom_implementation
{
	friend class document;

protected:
	dom_implementation_impl * _pimpl;
	dom_implementation (dom_implementation_impl * p);

public:
	dom_implementation ();
	dom_implementation (const dom_implementation & other);
	dom_implementation & operator = (const dom_implementation & other);
	~dom_implementation ();

	bool hasFeature (const pfs::string & feature, const pfs::string & version = pfs::string()) const;
	bool hasFeature (const char * feature, const char * version = nullptr) const
		{ return hasFeature(pfs::string(feature), pfs::string(version)); }

	document createDocument (const pfs::string & namespaceURI
			, const pfs::string & qualifiedName
			, const doctype & doctype);   // raises(DOMException)

	doctype createDocumentType (const pfs::string & qualifiedName
			, const pfs::string & publicId
			, const pfs::string & systemId); // raises(DOMException)

	// non-standard
	pfs::string idName () const; // used by node::getElementById() e.g.
};

}} // pfs::dom

#endif /* __PFS_DOM_DOMIMPL_HPP__ */
