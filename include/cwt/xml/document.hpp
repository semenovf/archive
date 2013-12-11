/**
 * @file document.hpp
 * @author wladt
 * @date Dec 10, 2013
 */

#ifndef __CWT_XML_DOCUMENT_HPP__
#define __CWT_XML_DOCUMENT_HPP__

#include <cwt/xml.hpp>
#include <cwt/dom/document.hpp>
#include <cwt/dom/doctype.hpp>
#include <cwt/dom/exception.hpp>

namespace cwt { namespace xml {

class DLL_API document_builder : public handlers
{
public:
	document_builder () : handlers() {}

	virtual ~document_builder () {}

	virtual void startElement (const pfs::string &, const pfs::map<pfs::string, pfs::string> &);
	virtual void endElement (const pfs::string &);
	virtual void characterData (const pfs::string &);
	virtual void processingInstruction (const pfs::string & , const pfs::string & );
	virtual void comment (const pfs::string &);
	virtual void startCdataSection ();
	virtual void endCdataSection ();
	virtual void defaultData (const pfs::string &);

	static cwt::dom::document * createDocument (const pfs::string & namespaceURI
			, const pfs::string & qualifiedName
			, const cwt::dom::doctype & doctype); // raises(DOMException);

};

} // cwt::xml

#endif /* __XML_CWT_DOCUMENT_HPP__ */
