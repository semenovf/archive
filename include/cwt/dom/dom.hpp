/**
 * @file dom.hpp
 * @author wladt
 * @date Dec 2, 2013
 */

#ifndef __CWT_DOM_DOM_HPP__
#define __CWT_DOM_DOM_HPP__

#include <pfs/string.hpp>
//#include <cwt/dom/documenttype.hpp>

namespace cwt { namespace dom {

class document;
class documenttype;

class DLL_API dom
{
public:
	static bool hasFeature(const pfs::string & feature, const pfs::string & version);

	static documenttype * createDocumentType(const pfs::string & qualifiedName,
			const pfs::string & publicId,
			const pfs::string & systemId); // raises(DOMException);

	static document * createDocument(const pfs::string & namespaceURI,
			const pfs::string & qualifiedName,
			const documenttype & doctype); // raises(DOMException);
};


}} // cwt

#endif /* __CWT_DOM_DOM_HPP__ */
