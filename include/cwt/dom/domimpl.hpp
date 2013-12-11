/**
 * @file domimpl.hpp
 * @author wladt
 * @date Dec 2, 2013
 */

#ifndef __CWT_DOM_DOMIMPL_HPP__
#define __CWT_DOM_DOMIMPL_HPP__

#include <pfs/string.hpp>
#include <cwt/dom/pimpl.hpp>

namespace cwt { namespace dom {

class document;
class document_type;

class DLL_API dom_implementation
{
	CWT_DOM_PIMPL_INLINE(dom_implementation)

public:
	static bool hasFeature (const pfs::string & feature, const pfs::string & version);

//	static document_type createDocumentType (const pfs::string & qualifiedName
//			, const pfs::string & publicId
//			, const pfs::string & systemId); // raises(DOMException);
//
//	static document createDocument (const pfs::string & namespaceURI
//			, const pfs::string & qualifiedName
//			, const document_type & doctype); // raises(DOMException);
};

}} // cwt::dom

#endif /* __CWT_DOM_DOMIMPL_HPP__ */
