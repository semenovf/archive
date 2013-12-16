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
private:
	dom_implementation_impl * _pimpl;

	dom_implementation (dom_implementation_impl * p);

public:
	dom_implementation ();
	dom_implementation (const dom_implementation & x);
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

// TODO Refuse from this
public:
	bool isNull() { return (_pimpl == nullptr);	}

//    enum InvalidDataPolicy { AcceptInvalidChars = 0, DropInvalidChars, ReturnNullNode };
//    static InvalidDataPolicy invalidDataPolicy();
//    static void setInvalidDataPolicy (InvalidDataPolicy policy);

};

}} // cwt::dom

#endif /* __CWT_DOM_DOMIMPL_HPP__ */
