#ifndef __CWT_XML_HPP__
#define __CWT_XML_HPP__

#include <pfs/string.hpp>
#include <cwt/dom.hpp>

namespace cwt { namespace xml {

//DLL_API cwt::dom::document createDocument (const pfs::string & namespaceURI
//		, const pfs::string & qualifiedName
//		, const cwt::dom::doctype & doctype);

DLL_API cwt::dom::document createDocument (const pfs::string & xml_source);

}} // cwt::xml

#endif /* __CWT_XML_HPP__ */
