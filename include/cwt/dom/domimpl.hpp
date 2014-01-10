/**
 * @file domimpl.hpp
 * @author wladt
 * @date Dec 2, 2013
 */

#ifndef __CWT_DOM_DOMIMPL_HPP__
#define __CWT_DOM_DOMIMPL_HPP__

#include <pfs/string.hpp>

namespace cwt { namespace dom {

class document;
class document_type;
class dom_implementation_impl;

class DLL_API dom_implementation
{
	friend class document;

private:
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
			, const document_type & doctype);   // raises(DOMException)

	document_type createDocumentType (const pfs::string & qualifiedName
			, const pfs::string & publicId
			, const pfs::string & systemId); // raises(DOMException)
};

}} // cwt::dom

#endif /* __CWT_DOM_DOMIMPL_HPP__ */
